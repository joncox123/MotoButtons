/*********************************************************************
author: joncox@alum.mit.edu, copyright 2023.
*********************************************************************/
#include <bluefruit.h>

BLEDis bledis;
BLEHidAdafruit blehid;

// Key codes to send for button presses
// https://www.drivemodedashboard.com/controller-implementation-guide/
// https://arduino.stackexchange.com/questions/65513/how-do-i-send-non-ascii-keys-over-the-ble-hid-connection-using-an-adafruit-nrf52
// https://github.com/adafruit/Adafruit_nRF52_Arduino/blob/200b3aaefb3256ac26df82ebc9b5b58923d9c37c/cores/nRF5/Adafruit_TinyUSB_Core/tinyusb/src/class/hid/hid.h#L188
// https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/785
/* DMD HID android key events
KEYCODE_ENTER
KEYCODE_DPAD_LEFT
KEYCODE_DPAD_RIGHT
KEYCODE_DPAD_UP
KEYCODE_DPAD_DOWN
KEYCODE_F6
KEYCODE_F7
*/
const uint8_t HIDKEY_UP     = HID_KEY_ARROW_UP;
const uint8_t HIDKEY_DOWN   = HID_KEY_ARROW_DOWN;
const uint8_t HIDKEY_LEFT   = HID_KEY_ARROW_LEFT;
const uint8_t HIDKEY_RIGHT  = HID_KEY_ARROW_RIGHT;
const uint8_t HIDKEY_CENTER = HID_KEY_RETURN;
const uint8_t HIDKEY_A      = HID_KEY_F6;
const uint8_t HIDKEY_B      = HID_KEY_F7;
#define N_KEY_REPORT 6
uint8_t keyReport[N_KEY_REPORT] = {HID_KEY_NONE, HID_KEY_NONE , HID_KEY_NONE , HID_KEY_NONE , HID_KEY_NONE , HID_KEY_NONE};
bool keyReportChanged = false;

const char BLE_DEVICE_NAME[] = "DMD2 CTL 7K";
const char BLE_DEVICE_MODEL[] = "Pad_v1";
const char BLE_MANUFACTURER[] = "joncox";
#define BLE_TX_POWER 8

//Mouse
#define MOUSE_RATE_SLOW 10
#define MOUSE_RATE_FAST 40
#define MOUSE_RATE_DELAY 1000
#define MODE_TOGGLE_MS 1000
bool mouse_mode = false;

// Digital IO pin mapping
#define BUTTON_UP      0
#define BUTTON_DOWN    1
#define BUTTON_LEFT    2
#define BUTTON_RIGHT   3
#define BUTTON_CENTER  4
#define BUTTON_A       5
#define BUTTON_B       6
#define LED_BUTTON_A   7
#define LED_BUTTON_B   8

#define DEBOUNCE_TIME_MS 20

// state of buttons
bool button_up_state     = false;
bool button_down_state   = false;
bool button_left_state   = false;
bool button_right_state  = false;
bool button_center_state = false;
bool button_A_state      = false;
bool button_B_state      = false;

// prior state of button reading for debouncing purposes
bool button_up_state_prior     = false;
bool button_down_state_prior   = false;
bool button_left_state_prior   = false;
bool button_right_state_prior  = false;
bool button_center_state_prior = false;
bool button_A_state_prior      = false;
bool button_B_state_prior      = false;

// Record state change of buttons to be used to monitor when event is transmitted
bool button_up_flipped     = false;
bool button_down_flipped   = false;
bool button_left_flipped   = false;
bool button_right_flipped  = false;
bool button_center_flipped = false;
bool button_A_flipped      = false;
bool button_B_flipped      = false;

// last time that button transitioned from low to high
unsigned long button_up_time     = 0;
unsigned long button_down_time   = 0;
unsigned long button_left_time   = 0;
unsigned long button_right_time  = 0;
unsigned long button_center_time = 0;
unsigned long button_A_time      = 0;
unsigned long button_B_time      = 0;

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

      Serial.print(buttonName);
      if (*state)
        Serial.println(" button pressed.");
      else
        Serial.println(" button released.");
    }

    *priorState = reading;

    return stateChanged;
}

void updateButtons() {
  bool stateChanged = false;

  stateChanged |= debounceButton(BUTTON_UP, &button_up_state, &button_up_state_prior, &button_up_flipped, &button_up_time, "UP");
  stateChanged |= debounceButton(BUTTON_DOWN, &button_down_state, &button_down_state_prior, &button_down_flipped, &button_down_time, "DOWN");
  stateChanged |= debounceButton(BUTTON_LEFT, &button_left_state, &button_left_state_prior, &button_left_flipped, &button_left_time, "LEFT");
  stateChanged |= debounceButton(BUTTON_RIGHT, &button_right_state, &button_right_state_prior, &button_right_flipped, &button_right_time, "RIGHT");
  stateChanged |= debounceButton(BUTTON_CENTER, &button_center_state, &button_center_state_prior, &button_center_flipped, &button_center_time, "CENTER");
  stateChanged |= debounceButton(BUTTON_A, &button_A_state, &button_A_state_prior, &button_A_flipped, &button_A_time, "A");
  stateChanged |= debounceButton(BUTTON_B, &button_B_state, &button_B_state_prior, &button_B_flipped, &button_B_time, "B");

  // Check whether mouse mode should be toggled
  if (button_A_state && button_B_state && (millis() - button_A_time > MODE_TOGGLE_MS) && (millis() - button_B_time > MODE_TOGGLE_MS)) {
    mouse_mode != mouse_mode;
    digitalWrite(LED_BUTTON_B, mouse_mode);
    blehid.mouseButtonRelease();
    if (mouse_mode)
      Serial.println("Mouse mode activated.");
    else
      Serial.println("Mouse mode deactivated.");
  }

  if (stateChanged) {
    keyReportChanged = stateChanged;

    // Update the HID keyboard report with all pressed keys
    unsigned int i = 0;
    if (button_up_state && !mouse_mode) {
      keyReport[i] = HIDKEY_UP;
      ++i;
    }
    if (button_down_state && !mouse_mode) {
      keyReport[i] = HIDKEY_DOWN;
      ++i;
    }
    if (button_left_state && !mouse_mode) {
      keyReport[i] = HIDKEY_LEFT;
      ++i;
    }
    if (button_right_state && !mouse_mode) {
      keyReport[i] = HIDKEY_RIGHT;
      ++i;
    }
    if (button_center_state && !mouse_mode) {
      keyReport[i] = HIDKEY_CENTER;
      ++i;
    }
    if (button_A_state) {
      keyReport[i] = HIDKEY_A;
      ++i;
    }
    if (button_B_state && (i < N_KEY_REPORT)) {
      keyReport[i] = HIDKEY_B;
      ++i;
    }

    for (int j = i; i < N_KEY_REPORT; j++) {
      keyReport[j] = HID_KEY_NONE;
    }
  }
}

void setupDigitalIO() {
  pinMode(BUTTON_UP,      INPUT);
  pinMode(BUTTON_DOWN,    INPUT);
  pinMode(BUTTON_LEFT,    INPUT);
  pinMode(BUTTON_RIGHT,   INPUT);
  pinMode(BUTTON_CENTER,  INPUT);
  pinMode(BUTTON_A,       INPUT);
  pinMode(BUTTON_B,       INPUT);

  pinMode(LED_BUTTON_A,   OUTPUT);
  pinMode(LED_BUTTON_B,   OUTPUT);
}

void setup() 
{
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println("Drivemode Dashboard BLE Controller");
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

  setupDigitalIO();

  Serial.println("Setup complete.");
}

void startAdv(void)
{  
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_MOUSE);
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

  digitalWrite(LED_BUTTON_A, HIGH);
}

void loop() 
{
  updateButtons();

  if (keyReportChanged) {
    blehid.keyboardReport(0, keyReport);

    if (mouse_mode) {
      // Handle click and release
      if (button_center_state && button_center_flipped) {
        Serial.println("Mouse left click.");
        blehid.mouseButtonPress(MOUSE_BUTTON_LEFT);
        button_center_flipped = false;
      }
      else if (!button_center_state && button_center_flipped) {
        Serial.println("Mouse left release.");
        blehid.mouseButtonRelease();
        button_center_flipped = false;
      }

      // Move pointer
      int rate = MOUSE_RATE_SLOW;
      unsigned long currTimeMs = millis();
      if ((currTimeMs - button_up_time > MOUSE_RATE_DELAY) ||
          (currTimeMs - button_down_time > MOUSE_RATE_DELAY) ||
          (currTimeMs - button_left_time > MOUSE_RATE_DELAY) ||
          (currTimeMs - button_right_time > MOUSE_RATE_DELAY))
        rate = MOUSE_RATE_FAST;

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
}

