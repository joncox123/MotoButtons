# User Guide
## Pairing with your phone
When MotoButtons is first powered on, both button LEDs will flash rapidly until the device is paired with your phone. If pairing is ever lost during use, they will start flashing again.

To pair with your phone, power up MotoButtons and ask your phone to pair with the Bluetooth device named "DMD2 CTL 8K" by navigating to Bluetooth pairing in the phone's Settings. This name is chosen for compatibility with the Drivemode Dashboard beta app. Once paired with your phone, it should quickly auto-connect when powered on again. 

## Cycling Through Modes
MotoButtons supports multiple modes of operation, including:
1. Mouse cursor mode
2. Drivemode Dashboard 2 mode
3. MyRoute App mode

MotoButtons can be cycled through these modes, in that order (Mouse->DMD2->MRA) by holding down both A and B buttons for 1 second. When advancing the mode, both LEDs will flash rapidly for 2 seconds. Then LED A will flash to indicate the mode number (one flash for mouse, two flashes for DMD2, three flashes for MyRoute App). 

The mode state will be saved so that the last mode prior to power down will be activated at the next power up. In other words, if you keep your MotoButtons set to MyRoute App mode, it will remember that when you use it next. Upon boot, the mode will be indicated by flashing the mode number.

## Button Mapping
This section describes the button mapping for each mode.

### Mouse Mode
Mouse mode is simple. The joystick moves the mouse cursor. Pressing the center of the joystick will simulate a left click. In addition, pressing Button B will also simuilate left click. Pressing Button A will simulate hitting the enter key on a keyboard. 

### Drivemode Dashboard 2 Mode
The joystick controls up/down/left/right, accordingly. A button activated F5, B button F6 and long pressing the joystick center activates F7.

### MyRoute App Mode
First, you must set MyRoute App to use the "BarButtons" controller. This provides the necessary functionality and MyRoute App will interpret the button presses correctly.

When in MyRoute App mode, the joystick will pan the map view. Buttons A and B will zoom the map view in and out. Tapping the joystick center button will center the map. Long pressing the joystick center will activate the menu.

## Rotating the Orientation
Since MotoButtons may be installed on the motorcycle in any orientation, it is sometimes necessary to rotate the orientation of the joystick. This can be done by holding down the joystick in the direction you want to be the new "up" direction, while powering on. In other words, let's say you want to mount the device upside down. Then you would press "down" on the joystick (it will now be upward when mounted upside down), then while holding this direction, apply power to the device. The change should be stored within a couple of seconds. 

At subsequent power cycles, the settings, including orientation, will be remembered and loaded.

## Updating the Software
To update the software on the microcontroller with a new release of MotoButtons, you may need to reset the controller to put it into Device Firmware Update (DFU) mode. This can be done by holding down buttons A and B for 10 seconds. This is the same procedure for changing the mode, except you must hold them down together for 10 seconds instead of only for one second.

There is a bug in the microcontroller's bootloader firmware that can sometimes prevent new MotoButtons software releases from being uploaded via the Arduino IDE, unless a special reset is performed. If this happens, the IDE will fail to upload new "sketches" (software code) by either timing out or printing a DFU (Device Firmware Update) error message. This could be especially problematic if you filled the case with epoxy (potting) or cannot access the tiny reset button on the microcontroller. Otherwise, it is necessary to quickly double tap the tiny reset button in order to manually enter DFU (software update) mode again.
