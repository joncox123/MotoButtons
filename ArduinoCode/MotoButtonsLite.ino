/*********************************************************************
author: joncox@alum.mit.edu
License: GNU GENERAL PUBLIC LICENSE; Version 3, 29 June 2007
Version: 1.1 with support for the following modes: DMD2, mouse cursor, MyRoute App
*********************************************************************/
#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <stdlib.h>

using namespace Adafruit_LittleFS_Namespace;

/*----- Persistent Storage Filesystem -----*/
// This is used to store settings, such as the last mode
#define FILENAME    "/MotoButtons.set"
File file(InternalFS);

// BLE classes
BLEDis bledis;
BLEHidAdafruit blehid;

// BLE configuration
#define BLE_TX_POWER 8
const char BLE_DEVICE_NAME[] = "DMD2 CTL 8K";
const char BLE_DEVICE_MODEL[] = "MotoButtons Lite 1.1";
const char BLE_MANUFACTURER[] = "Me";
bool BLE_connected = false;

/* 
 * --------------------- MODE CONFIGURATION ----------------------------
 * 	DMD2: up/down/left/right arrows, enter, F6 and F7
 * 	Mouse: mouse up/down/left/right, left click, left click, ?
 * 	MRA: up/down/left/right arrows, 'c', -, +, longpress center for 'n'
 */
// Selected Mode: indicates the currently selected operating mode
#define MODE_TOGGLE_MS 1000
#define N_MODES 3
enum Mode {
	mouse = 0,
  DMD2 = 1,
	MRA = 2
};
enum Mode currentMode;

bool modeButtonsReleased = true;

/* DMD2 Mode Configuration */
// https://www.drivemodedashboard.com/controller-implementation-guide/
// https://arduino.stackexchange.com/questions/65513/how-do-i-send-non-ascii-keys-over-the-ble-hid-connection-using-an-adafruit-nrf52
// https://github.com/adafruit/Adafruit_nRF52_Arduino/blob/200b3aaefb3256ac26df82ebc9b5b58923d9c37c/cores/nRF5/Adafruit_TinyUSB_Core/tinyusb/src/class/hid/hid.h#L188
// https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/785
// Key codes to send for button presses:
const uint8_t DMD_KEY_UP      = HID_KEY_ARROW_UP;
const uint8_t DMD_KEY_DOWN    = HID_KEY_ARROW_DOWN;
const uint8_t DMD_KEY_LEFT    = HID_KEY_ARROW_LEFT;
const uint8_t DMD_KEY_RIGHT   = HID_KEY_ARROW_RIGHT;
const uint8_t DMD_KEY_CENTER  = HID_KEY_ENTER;
const uint8_t DMD_KEY_A       = HID_KEY_F5;
const uint8_t DMD_KEY_B       = HID_KEY_F6;
const uint8_t DMD_KEY_VIRTUAL = HID_KEY_F7;

/* Mouse Mode Configuration */
#define MOUSE_RATE_SLOW 10
#define MOUSE_RATE_FAST 40
#define MOUSE_RATE_DELAY 500
bool mouse_left_button_pressed = false;
const uint8_t MOUSE_KEY_A     = HID_KEY_ENTER;

/* MRA Mode Configuration */
const uint8_t MRA_KEY_UP      = HID_KEY_ARROW_UP;
const uint8_t MRA_KEY_DOWN    = HID_KEY_ARROW_DOWN;
const uint8_t MRA_KEY_LEFT    = HID_KEY_ARROW_LEFT;
const uint8_t MRA_KEY_RIGHT   = HID_KEY_ARROW_RIGHT;
const uint8_t MRA_KEY_CENTER  = HID_KEY_C;
const uint8_t MRA_KEY_A       = HID_KEY_KEYPAD_ADD;
const uint8_t MRA_KEY_B       = HID_KEY_MINUS;
const uint8_t MRA_KEY_VIRTUAL = HID_KEY_N;
/*---------------------- END MODE CONFIGURATION ----------------------*/

/*----------------- BUTTON CONFIGURATION AND LOGIC -------------------*/
/* BLE key report */
#define N_KEY_REPORT 6
uint8_t keyReport[N_KEY_REPORT] = {HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE};
bool keyReportChanged = false;

// Digital IO pin mapping
#define BUTTON_UP      2
#define BUTTON_DOWN    4
#define BUTTON_LEFT    3
#define BUTTON_RIGHT   0
#define BUTTON_CENTER  1
#define BUTTON_A       6
#define BUTTON_B       5
#define LED_BUTTON_A   8
#define LED_BUTTON_B   7

#define DEBOUNCE_TIME_MS 20

// state of buttons
bool button_up_state      = false;
bool button_down_state    = false;
bool button_left_state    = false;
bool button_right_state   = false;
bool button_center_state  = false;
bool button_A_state       = false;
bool button_B_state       = false;
bool button_virtual_state = false; // 8th virtual button activated via long press

// prior state of button reading for debouncing purposes
bool button_up_state_prior      = false;
bool button_down_state_prior    = false;
bool button_left_state_prior    = false;
bool button_right_state_prior   = false;
bool button_center_state_prior  = false;
bool button_A_state_prior       = false;
bool button_B_state_prior       = false;

// Record state change of buttons to be used to monitor when event is transmitted
bool button_up_flipped      = false;
bool button_down_flipped    = false;
bool button_left_flipped    = false;
bool button_right_flipped   = false;
bool button_center_flipped  = false;
bool button_A_flipped       = false;
bool button_B_flipped       = false;
bool button_virtual_flipped = false;

// last time that button transitioned from low to high
unsigned long button_up_time      = 0;
unsigned long button_down_time    = 0;
unsigned long button_left_time    = 0;
unsigned long button_right_time   = 0;
unsigned long button_center_time  = 0;
unsigned long button_A_time       = 0;
unsigned long button_B_time       = 0;
unsigned long button_virtual_time = 0;
/*------------------- END BUTTON CONFIG & LOGIC-----------------------*/

//if  This function should be called rapidly in a loop to update the debounce filter and key state
// https://docs.arduino.cc/built-in-examples/digital/Debounce
bool debounceButton(unsigned int button, bool *state, bool *priorState, bool *buttonFlipped,
                    unsigned long *debounceTime, const char *buttonName) {
  bool reading;
  unsigned long readTime;
  bool stateChanged = false;

  reading = digitalRead(button);
  readTime = millis();

  // If reading has changed, switch has not settled yet
  if (reading != *priorState)
    *debounceTime = readTime;
  // State has been stable for time exceeding debounce filter delay, so update state
  else if ((readTime - *debounceTime) > DEBOUNCE_TIME_MS)
    // If state changed after debounce, new state has not yet been transmitted
    if (reading != *state) {
      stateChanged = true;
      *state = reading;
      *buttonFlipped = true;
    }
    *priorState = reading;

    return stateChanged;
}

void releaseAllKeys() {
  uint8_t keyReportRelease[N_KEY_REPORT] = {HID_KEY_NONE, HID_KEY_NONE , HID_KEY_NONE , HID_KEY_NONE , HID_KEY_NONE , HID_KEY_NONE};
  blehid.keyboardReport(0, keyReportRelease);
}

void indicateMode() {
    // signal the new mode number
    for (unsigned int i = 0; i < 2*((int)currentMode+1); i++) {
      digitalToggle(LED_BUTTON_A);
      delay(500);
    }
    digitalWrite(LED_BUTTON_A, LOW);
}

void updateButtons() {
  bool stateChanged = false;
  
  // Read the state of all physical buttons
  stateChanged |= debounceButton(BUTTON_UP, &button_up_state, &button_up_state_prior, &button_up_flipped, &button_up_time, "UP");
  stateChanged |= debounceButton(BUTTON_DOWN, &button_down_state, &button_down_state_prior, &button_down_flipped, &button_down_time, "DOWN");
  stateChanged |= debounceButton(BUTTON_LEFT, &button_left_state, &button_left_state_prior, &button_left_flipped, &button_left_time, "LEFT");
  stateChanged |= debounceButton(BUTTON_RIGHT, &button_right_state, &button_right_state_prior, &button_right_flipped, &button_right_time, "RIGHT");
  stateChanged |= debounceButton(BUTTON_CENTER, &button_center_state, &button_center_state_prior, &button_center_flipped, &button_center_time, "CENTER");
  stateChanged |= debounceButton(BUTTON_A, &button_A_state, &button_A_state_prior, &button_A_flipped, &button_A_time, "A");
  stateChanged |= debounceButton(BUTTON_B, &button_B_state, &button_B_state_prior, &button_B_flipped, &button_B_time, "B");
  
  /* ----------- Detect state of virtual buttons ---------------------*/
  if (button_center_state && (millis() - button_center_time > MODE_TOGGLE_MS)) {
    // virtual button is active
    if (!button_virtual_state) {
      Serial.println("Virtual button activated.");
      button_virtual_time = millis();
      button_virtual_flipped = true;
      stateChanged |= true;

      // indicate virtual button
      digitalWrite(LED_BUTTON_A, HIGH);
    }
    button_virtual_state = true;
  }
  else {
    // virtual button is not active
    if (button_virtual_state) {
      Serial.println("Virtual button deactivated.");
      digitalWrite(LED_BUTTON_A, LOW);
      stateChanged |= true;
    }
    button_virtual_state = false;
  }
  /*------------------------------------------------------------------*/

  // Indicate whether any buttons changed state
  keyReportChanged = stateChanged;

  /*------------------- Handle mode cycling --------------------------*/
  // Check for release of mode cycle button combination
  if (!button_A_state || !button_B_state) {
    modeButtonsReleased = true;
  }
 
  if (button_A_state && button_B_state && (millis() - button_A_time > MODE_TOGGLE_MS) && (millis() - button_B_time > MODE_TOGGLE_MS) && modeButtonsReleased) {
    // Buttons A and B were both long-pressed, which means we should advance the mode
    currentMode = (Mode)(((int)currentMode + 1) % N_MODES);
    Serial.print("Mode advanced to ");
    Serial.println(currentMode);
    
    // Since mode has been changed, inactivate any ongoing key or mouse presses
    releaseAllKeys();
    blehid.mouseButtonRelease();
    modeButtonsReleased = false;

    // store new mode to flash memory
    writeSettings();

	  // Flash the LEDs indicating a change of mode occured
    for (unsigned int i = 0; i < 30; i++) {
      digitalToggle(LED_BUTTON_A);
      digitalToggle(LED_BUTTON_B);
      delay(100);
    }
    digitalWrite(LED_BUTTON_A, LOW);
    digitalWrite(LED_BUTTON_B, LOW);
    delay(1000);
    indicateMode();
  }
  /*------------------------------------------------------------------*/
}

void mapButtonsToKeyReport() {
	unsigned int i = 0;

	switch (currentMode) {
		case DMD2:
			if (button_up_state && !button_center_state) {
			  keyReport[i] = DMD_KEY_UP;
			  ++i;
			}
			if (button_down_state && !button_center_state) {
			  keyReport[i] = DMD_KEY_DOWN;
			  ++i;
			}
			if (button_left_state && !button_center_state) {
			  keyReport[i] = DMD_KEY_LEFT;
			  ++i;
			}
			if (button_right_state && !button_center_state) {
			  keyReport[i] = DMD_KEY_RIGHT;
			  ++i;
			}
			if (button_center_state) {
			  keyReport[i] = DMD_KEY_CENTER;
			  ++i;
			}
			if (button_A_state) {
			  keyReport[i] = DMD_KEY_A;
			  ++i;
			}
			if (button_B_state && (i < N_KEY_REPORT)) {
			  keyReport[i] = DMD_KEY_B;
			  ++i;
			}
			if (button_virtual_state && (i < N_KEY_REPORT)) {
			  keyReport[i] = DMD_KEY_VIRTUAL;
			  ++i;
			}
			break;
		case mouse:
			if (button_A_state && button_A_flipped) {
			  keyReport[0] = MOUSE_KEY_A;
			  ++i;
			  button_A_flipped = false;
			}
			break;
		case MRA:
			if (button_up_state && !button_center_state) {
			  keyReport[i] = MRA_KEY_UP;
			  ++i;
			}
			if (button_down_state && !button_center_state) {
			  keyReport[i] = MRA_KEY_DOWN;
			  ++i;
			}
			if (button_left_state && !button_center_state) {
			  keyReport[i] = MRA_KEY_LEFT;
			  ++i;
			}
			if (button_right_state && !button_center_state) {
			  keyReport[i] = MRA_KEY_RIGHT;
			  ++i;
			}
			if (button_center_state) {
			  keyReport[i] = MRA_KEY_CENTER;
			  ++i;
			}
			if (button_A_state) {
			  keyReport[i] = MRA_KEY_A;
			  ++i;
			}
			if (button_B_state && (i < N_KEY_REPORT)) {
			  keyReport[i] = MRA_KEY_B;
			  ++i;
			}
			if (button_virtual_state && (i < N_KEY_REPORT)) {
			  keyReport[i] = MRA_KEY_VIRTUAL;
			  ++i;
			}
			break;
		default:
			break;
	}
	
	for (unsigned int j = i; j < N_KEY_REPORT; j++) {
		keyReport[j] = HID_KEY_NONE;
	}
}

void handleMouse() {
    // handle mouse click release
    if ((!button_center_state && button_center_flipped) || (!button_B_state && button_B_flipped) && mouse_left_button_pressed) {
      Serial.println("Mouse left release.");
      blehid.mouseButtonRelease();
      button_center_flipped = false;
      button_B_flipped = false;
      mouse_left_button_pressed = false;
    }

      // Handle click
      if ((button_center_state && button_center_flipped) || (button_B_state && button_B_flipped) && !mouse_left_button_pressed) {
        Serial.println("Mouse left click.");
        blehid.mouseButtonPress(MOUSE_BUTTON_LEFT);
        button_center_flipped = false;
        button_B_flipped = false;
        mouse_left_button_pressed = true;
      }

      // Move pointer
      if (!button_center_state) {
        int rate = MOUSE_RATE_SLOW;
        unsigned long currTimeMs = millis();
        if (((currTimeMs - button_up_time > MOUSE_RATE_DELAY) && button_up_state) ||
            ((currTimeMs - button_down_time > MOUSE_RATE_DELAY) && button_down_state) ||
            ((currTimeMs - button_left_time > MOUSE_RATE_DELAY) && button_left_state) ||
            ((currTimeMs - button_right_time > MOUSE_RATE_DELAY) && button_right_state))
          rate = MOUSE_RATE_FAST;
        else
          rate = MOUSE_RATE_SLOW;

        if (button_up_state)
          blehid.mouseMove(0, -rate);
        if (button_down_state)
          blehid.mouseMove(0, rate);
        if (button_left_state)
          blehid.mouseMove(-rate, 0);
        if (button_right_state)
          blehid.mouseMove(rate, 0);
      }
}

void setupDigitalIO() {
  pinMode(BUTTON_UP,      INPUT_PULLDOWN);
  pinMode(BUTTON_DOWN,    INPUT_PULLDOWN);
  pinMode(BUTTON_LEFT,    INPUT_PULLDOWN);
  pinMode(BUTTON_RIGHT,   INPUT_PULLDOWN);
  pinMode(BUTTON_CENTER,  INPUT_PULLDOWN);
  pinMode(BUTTON_A,       INPUT_PULLDOWN);
  pinMode(BUTTON_B,       INPUT_PULLDOWN);

  pinMode(LED_BUTTON_A,   OUTPUT);
  pinMode(LED_BUTTON_B,   OUTPUT);

  digitalWrite(LED_BUTTON_A, LOW);
  digitalWrite(LED_BUTTON_B, LOW);
}

bool writeSettings() {
    char modeStr[8];

    // Settings file does not exist, we need to create it
    Serial.print("Creating " FILENAME " to store settings...");

    if (file.open(FILENAME, FILE_O_WRITE)) {
      Serial.println("File opened successfully.");
      // file is opened in append mode by default
      file.truncate(0);
      file.seek(0);

      itoa((int)currentMode, modeStr, 10);
      file.write(modeStr, strlen(modeStr));
      file.close();

      return true;
    }
    else {
      Serial.println("Error writing settings file!");

      return false;
    }
}

bool readSettings() {
  file.open(FILENAME, FILE_O_READ);
  // Does settings file already exist?
  if (file) {
    // File already exists
    Serial.println(FILENAME " settings file exists, reading...");
    
    uint32_t readlen;
    char buffer[8] = { 0 };
    readlen = file.read(buffer, sizeof(buffer));

    buffer[readlen] = 0;
    Serial.print("Mode: ");
    Serial.println(buffer);
    file.close();

    // Store the current mode
    int n = atoi(buffer);
    if (n < 0 || (n > N_MODES-1)) {
      Serial.println("Invalid mode value, setting to zero.");
      n = 0;
    }
    currentMode = (Mode)n;

    return true;
  }
  else {
    Serial.println(FILENAME " settings file not found.");
    
    return false;
  }
}

void setup() 
{
  currentMode = mouse;
  
  setupDigitalIO();

  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println("MotoButtons BLE Controller");
  Serial.println("-----------------------------\n");
  Serial.println();

  Bluefruit.begin();
  // HID Device can have a min connection interval of 9*1.25 = 11.25 ms
  Bluefruit.Periph.setConnInterval(9, 16); // min = 9*1.25=11.25 ms, max = 16*1.25=20ms
  Bluefruit.setTxPower(BLE_TX_POWER);    // Check bluefruit.h for supported values
  Bluefruit.setName(BLE_DEVICE_NAME);

  // Configure and Start Device Information Service
  bledis.setManufacturer(BLE_MANUFACTURER);
  bledis.setModel(BLE_DEVICE_MODEL);
  bledis.begin();

  // BLE HID
  blehid.begin();

  // Set up and start advertising
  startAdv();

  Serial.println("Setup complete.");
  digitalWrite(LED_BUTTON_A, HIGH);
  digitalWrite(LED_BUTTON_B, HIGH);

  // Initialize Internal File System
  InternalFS.begin();

  // Restore settings
  bool success = false;
  success = readSettings();
  if (!success)
    writeSettings();

  digitalWrite(LED_BUTTON_A, LOW);
  digitalWrite(LED_BUTTON_B, LOW);
  indicateMode();
  delay(1000);
}

void startAdv(void)
{  
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  //Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_MOUSE);
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  
  // Include BLE HID service
  Bluefruit.Advertising.addService(blehid);

  // There is enough room for 'Name' in the advertising packet
  Bluefruit.Advertising.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void loop() 
{
  // Indicate whether the device is connected and running
  if (!BLE_connected && (Bluefruit.connected() > 0)) {
    Serial.println("BLE connected to host.");
    digitalWrite(LED_BUTTON_A, LOW);
    digitalWrite(LED_BUTTON_B, LOW);
    BLE_connected = true;
  }
  else if (Bluefruit.connected() == 0) {
    digitalToggle(LED_BUTTON_A);
    digitalToggle(LED_BUTTON_B);
    delay(200);
    BLE_connected = false;
  }

  // Read current state of buttons
  updateButtons();
  
  if (BLE_connected) {
	  // Compile the BLE HID key report
	  if (keyReportChanged) {
		Serial.println("Sent key report.");
		mapButtonsToKeyReport();
		blehid.keyboardReport(0, keyReport);
	  }
	  
	  if (currentMode == mouse)
		  handleMouse();
  }
}
