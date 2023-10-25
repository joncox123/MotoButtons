# Construction (building your MotoButtons Lite)
Building your own MotoButtons Lite requires no programming experience, only relatively basic hand tools and DIY skills. However, if you are not moderately competent at soldering, working with glue and sealants and drilling precision holes, you might want to ask a friend for help. 

## Be Prepared
### Disclaimer
No warranty or implied fitness for any use is provided and the authors assume no liability for any failures or accidents that occur. You assume all risk of building or operating MotoButtons. The controller should never be used while the vehicle is in motion to reduce the risk of a collision or accident. Operating a motorcycle, working with tools and chemicals is an inherently dangerous activity and should only be attempted by qualified and experienced people. By proceeding, you agree to assume all risk associated with construction and operation of MotoButtons.

### Gather Supplies and Tools
First, you'll need to acquire the necessary components, tools and supplies, as mentioned on the [main page](https://github.com/joncox123/MotoButtons/tree/main#recommended-tools-and-supplies). Most DIY enthusiasts will have many of these tools and supplies, except for the parts from Mouser, the slow curing epoxy, and the necessary drill bits.

## Let's Get Started
### Machine the Enclosure
First, you need to make the necessary modifications to the Hammond IP68 rated enclosure. While I used a milling machine to do this, it's readily possible with the proper hand tools, technique and attention to detail. At the end of the day, it is not necessary for everything to be perfect. The sealant, joystick boot and buttons will cover up any minor issues.
Take a look at the [3D model (3D PDF)](../Enclosure/MotoButtons%20Lite%20Enclosure%201551WH%20Bottom%203D.pdf) to get an idea of what you will be doing.

<img src="../Enclosure/enclosure.png" alt="3D model of modified case" width="600"/>

If you happen to have a milling machine, you can clamp the enclosure (with lid screwed on) in a vise with three layers of duct tape above the parallels.

<img src="Photos/Milling_Machine_Setup.jpg" alt="case in milling machine" width="600"/>

First, print out the [schematic drawing](../Enclosure/MotoButtons%20Lite%20Enclosure%201551WH%20Bottom.pdf) of the enclosure, making sure to print as "actual size", so that the print is scaled exactly as indicated. You may want to check the dimensions with a caliper or ruler to verify that you printed it out true to scale. 

<a href="../Enclosure/MotoButtons%20Lite%20Enclosure%201551WH%20Bottom.pdf">
  <img src="Photos/schematic.png" alt="diagram of modified case" width="600"/>
</a>

Cut out the top left diagram titled "Top View (1:1 Scale)" and carefully tape it over the top of the enclosure. Do your best to center it on the case and avoid any wrinkles in the paper. Use an automatic center punch to very precisely create indentations (divots) on the center of each of the 8 holes. This can be done by placing the tip of the center punch directly over the center markings of each hole and pressing down. You might want to practice this step on a piece of plastic or wood as it is easy to misjudge the center of the punch. 

After you have created the eight marks, carefully clamp the case in a vise [recommend soft vise jaws](https://www.amazon.com/HORUSDY-Universal-Magnetic-Retention-Urethane/dp/B078MVLSZS) and drill the two large button holes (on the mark centers) with a small pilot drill about 1/8" diameter. When clamping the case, keep in mind that it is strong, but polycarbonate can suddenly crack if too much force is applied. Tighten the vise just enough to hold the case securely when moved by hand. After drilling the pilot holes, drill both holes out to 36/64" using a step drill. Set your hand drill or drill press to a low speed for this step to avoid chatter. 

Finally, you will drill the small holes for the electrical pins of the 5-way navigation switch (joystick). This step is challenging to do with a hand drill because the small drill bits tend to break easily with the slightest deflection--so be careful and wear safety glasses. The upper and lower holes will use the #56 bit, and the two "middle" holes will use the smaller #60 bit. **Notice that the two "middle" holes are not exactly in the middle. The orientation of the switch matters!**  When you are done, your case should look as follows:

<img src="Photos/Drill_Holes_in_Case.jpg" alt="Top of case with holes drilled" width="600"/>

Before proceeding, test fit the buttons and the navigation switch. **Caution: the switch must be inserted in the six holes with the correct orientation! The two middle holes are closer to one side.**

<img src="Photos/Test_Fit_Parts.jpg" alt="Microcontroller in USB-C slot" width="600"/>

<img src="Photos/Test_Fit_Navigation_Switch.jpg" alt="Microcontroller in USB-C slot" width="600"/>

### Cut Off the Circuit Board Posts (Optional)
If you ordered the buttons with screw thread mounting, as opposed to the snap on ones, you'll need to cut off two of the PCB mounting posts on the inside of the case. Otherwise, you won't be able to properly tighten the nut to secure the top button. It's possible to omit this step by ordering the snap in buttons, but the waterproofing won't be as good unless you are also potting the case with epoxy. 

**Nevertheless, if you are going to pot with epoxy, you could simply tighten the nut the best you can without cutting off the posts, and skip this step.**

Use a dremel with a small cut off wheel to cut off the upper posts. Then sand the area flat using a sanding wheel. *I used a milling machine and a 7/32" end mill to do this step, so it might be a good idea to omit this step unless you are confident in your abilities and tools.*

## Power Entry
This section describes two options for power the device. By the way, this microcontroller has integrated functionality for charging a lithium battery. While this guide does not describe such a version, it could be easily built by soldering a small battery to the proper terminals.

### Drill Hole for Power Wires (Option 1, Recommended)
The easiest option to power the device, which does not require machining a slot for the USB-C connector, is to drill a hole on the end for the power wires. This option is also the easiest to seal, as after you run the wires through the end of the case, you only need to apply a bit of sealant on the inside and outside of the wires. 

Since the microcontroller requires 5 Volt power, the easiest way to power it is via an old USB cable, assuming you also have a USB port on your bike. Thus, you could take an old cable, cut the end off, and solder the ground and power wire to the GND and 5V pads on the microcontroller, as described near the end of this guide.

### Machine the USB-C Connector Slot (Option 2, Not Recommended at this Time)
If you are planning to epoxy fill (pot) the enclosure, for the ultimate in durability and waterproofing, you may wish to cut a slot for the USB-C connector on the microcontroller. This will allow you to update the firmware in the future, should additional functionality become available (or if you wish to modify the source code yourself). **However, I do not recommend potting with epoxy just yet because of a bug in the microcontroller firmware that requires hitting the reset button twice to upload new firmware. Update: a solution to this issue has been implemented, which requires holding down buttons A and B for 10 seconds to trigger a software reset into DFU mode. However, please wait until further testing is completed before potting with epoxy.**

The slot for a female USB-C connector is approximately 1/8" (3.18 mm) tall and 9 mm wide (end to end). Therefore, using the center punch, mark two horizontally spaced holes that are 5.83 mm apart by printing out the template and using a center punch or using a caliper. Drill both holes with a 1/8" drill bit. Using a needle file set, carefully file out the slot. You will need to test fit the slot with the microcontroller, as shown below. This will require some trial and error, as it is necessary to file the ends using the round file and the straight edges with a flat file until the connector fits snuggly. I used a milling machine with a 1/8" endmill to do this, although I still had to do manual filing to get the connector to fit.

<img src="Photos/Cut_USB-C_Connector_Slot.jpg" alt="USB-C slot in enclosure" width="600"/>

<img src="Photos/Test_Fit_Microcontroller.jpg" alt="Microcontroller in USB-C slot" width="600"/>

## Glue 5-Way Navigation Switch to Case
Congratulations, you've successfully machined your enclosure! The next step is to glue the joystick to the outside of the case using epoxy glue, and then seal the rubber boot over it with silicone sealant. Begin by assembling your gluing and sealing supplies:

<img src="Photos/Assemble_Gluing_Supplies.jpg" alt="gluing supplies" width="600"/>

First, lightly sand the area on the case where the switch will sit with some 150 grit or similar sand paper. This will help the epoxy stick to the case. Then *very throroughly clean* the case with an alcohol wipe or rubbing alcohol. Blow off any lint that remains. You may also want to wipe down the bottom and sides of the switch with alcohol, especially if you have been handling it. Preparation is key to achieving a durable and waterproof bond. 

<img src="Photos/Scuff_Switch_Gluing_Area.jpg" alt="sanding case" width="600"/>

Mix up a small amount of C-POXY 30 two-part epoxy. This epoxy has an approximately 10 minute working time and takes about two hours to cure (full cure in 24 hours). Using a small stick or applicator, carefully spread a thin layer between the holes on the case, being **careful not to apply epoxy over the holes.** Also apply a **thin layer** of epoxy to the underside of the switch. *Note: the amount applied in the photos is more than you need, just use a thin layer*. 

<img src="Photos/Apply_Epoxy_To_Case.jpg" alt="epoxy on case" width="600"/>

<img src="Photos/Apply_Glue_to_Switch_Bottom.jpg" alt="epoxy on switch" width="600"/>

Insert the switch to the holes of the case, making sure not to get any epoxy on the electrical pins. **Observe the proper orientation of the switch, as it is not symmetric top to bottom. The two holes in the "middle" are closer to the bottom side (away from buttons), so line them up before inserting**. A little epoxy should squish out the sides, and you can wipe additional epoxy along the sides of the switch, being careful not to get any on the upper portion (and ruining the mechanism). The idea is to wipe epoxy along the sides and the case surface so that it holds along the perimeter.  

<img src="Photos/Insert_switch_with_Glue.jpg" alt="epoxy squishing out of switch and case" width="600"/>

<img src="Photos/Apply_glue_around_switch.jpg" alt="wipe epoxy around switch" width="600"/>

When you are finished applying epoxy, it should look as follows. **You will want to proceed to install the boot, seal the boot and then cure the enclosure upside down fairly quickly, to avoid having epoxy run down the pin holes and cover the electrical contacts.**

<img src="Photos/Epoxy_Fully_Applied.jpg" alt="epoxy completely applied" width="600"/>

### Install Rubber Boot over 5-way Switch
Before the epoxy begins to cure (within a couple of minutes), you will want to install the rubber boot over the joystick. To do so, simply press the cap over the switch with a bit of force. Also press around the perimeter of the cap, near the base, so seat it over the base. It requires a little bit of work to get it fully seated. 

<img src="Photos/Press_Cap_onto_Switch.jpg" alt="cap inserted onto switch" width="600"/>

After seating the switch, take your silicone sealant and use another small applicator stick to spread sealant around the edge of the boot and the case surface. However, be careful not to get it too close to where you will install the button. Look carefully around the boot to make sure there are no bubbles and there is a generous amount of sealant around the entirety of the base of the boot, extending to the case.

<img src="Photos/Apply_Sealant_Around_Boot.jpg" alt="sealing boot with silicone" width="600"/>

When you are satisfied, you can gently wipe all four sides with your fingers to smooth out the appearance, but this is not necessary (for asthetic purposes only). Be carefull not to press too hard because you want a generous amount of sealant to remain. 

<img src="Photos/Smooth_Sealant_With_Finger.jpg" alt="smooth sealant" width="600"/>

When everything is completed, it should look as follows. Quickly proceed to the next step to cure it upside down.

<img src="Photos/Sealed_Switch.jpg" alt="smooth sealant" width="600"/>

**Turn the case upside down so that it is supported by the switch boot.** Prop up the other side with some material. You can place objects or the epoxy bottles on either side of the case to keep it from tipping over. Place some sort of weight on the case to provide a bit of pressure to cure the switch flush to the case. While the epoxy is mostly cured after about 2 hours, the silicone will take 24 hours, so let the entire arrangement rest undisturbed for 24 hours before proceeding.

You can see in the following photo that I applied a bit too much epoxy, and didn't turn upside down soon enough. As a result, some epoxy began to run down the pins, although there is still some exposed metal that I can solder to.

<img src="Photos/Cure_Epoxy_Upside_Down.jpg" alt="cure upside down" width="600"/>

## Installing Buttons and Soldering Wires
First, install the two buttons. Place the O-ring in the groove, then insert the buttons into the case. Place the lock washer and the nut over the back of the buttons and tighten gently (but firmly) with a 14 mm socket. **Make sure to align the buttons such that the two *shorter* wire leads face inward. These are the LED negative (-) terminals.**

### Soldering Wires to 5-Way Switch
With some color coded small gauge wire (approximately 26 AWG), "tin" each of the six pins and six wire segments of about 4 cm each. Solder the wires to each of the six pins. The switch pinout is shown in the photo below. Looking from the inside of the case, with the two buttons oriented up, the pinout clockwise from the upper left pin is: center, up, left, down, common, right. If viewed from the outside front of the case, the "left" pin will be on the left side and the "up" pin will face up, assuming the two buttons are on the top (direction of arrow). 

<img src="Photos/SolderWires5WaySwitch.jpg" alt="soldering wires to 5-way switch and pinout" width="600"/>

### Solder Wires to Buttons and Button LEDs
Make sure the buttons are rotated so that the two shorter leads face inward. These are the LED negative (-) pins that you will connect to ground. Solder wires to each of the button leads, except **bridge the two short LED leads together by soldering both with a single ground wire (see photos).** 

Also, you may want to put a piece of heat shrink tubing over the bottom most LED wire for Button B. This can prevent it from shorting out on either the microcontroller or the other pins on the button. If you don't have a small diameter piece of heat shrink, you can put a bit of electrical tape around it.

<img src="Photos/SolderButtonWires.jpg" alt="all wires soldered to buttons and joystick" width="600"/>

## Solder All Wires to Microcontroller
Study the [wiring diagram](CaseConnections.pdf) shown below:

<img src="CaseConnections.png" alt="wiring diagram"/>

You will solder the wires to the pins, which are labeled on the back of the microcontroller, as shown in the diagram. The pinout of the microcontroller, as viewed from the top, is shown in the upper right of the diagram. However, twist the three 3V3 (power) wires together and solder them to the back of the 3V3 pad. The rest of the wires are individually soldered to the indicated pad on the microcontroller.

### Solder 5 Volt Power Cable (Option 1)
If you decided to solder an old USB cable to the microcontroller for power, instead of making a slot for the USB-C connector, you should do this now. As a guide, [see this page](https://www.electroschematics.com/usb-how-things-work/) for the pinout of a USB cable. To identify ground and power, you can plug it in and use a multimeter to see which pin is grounded and which has +5V.

<img src="Photos/usb-wiring-connection.jpg" alt="USB cable pinout" width="600"/>

## Install in Case
After soldering all wires, gently bend over the remaining button LED leads such that they do not short to any other pins. Wrap the wires carefully around the buttons in the case as shown below. Press them into the case so that they won't get caught in the case lip when you screw the lid on (otherwise you will pinch your wires and there will be a gap in the lid's waterproof seal).

<img src="Photos/MicrocontrollerInCase.jpg" alt="wires wrapped into case" width="600"/>

## Upload the Microcontroller Software
The easiest way to do this would be with a custom installer. However, that work is not yet done, so you will need to [download the Arduino IDE](https://www.arduino.cc/en/software), install the support package for the microcontroller, and then upload via a USB cable. The installation steps are [described here.](https://wiki.seeedstudio.com/XIAO_BLE/#getting-started)

First, open the Arduino IDE and then open Preferences in the File menu. Paste the URL (https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json) to the microcontroller board support package into the "Additional board managers URLs" text field. Click OK. 

<img src="Photos/AddBSP.PNG" alt="add Seeed BSP" width="600"/>

Second, install the Seeed nRF Boards package by clicking on the side button that is second from the top, to open the Board Manager. Type "nRF" into the search field at the top and install the "Seeed nRF52 Boards" package. 

<img src="Photos/InstallBSP.PNG" alt="install Seeed BSP" width="600"/>

Third, [download the source code file](../ArduinoCode/MotoButtonsLite.ino) and open in the Arduino IDE. Attach the microcontroller via a USB-C cable. Select the board that you just attached with the drop down box in the upper toolbar. It should say "Seeed XIAO nRF52840".

Finally, click the upload button!

<img src="Photos/upload.png" alt="install Seeed BSP" width="600"/>

## Sealing the Case
At this point, you are ready to close the case. However, you have a couple of options to consider before proceeding. Since MotoButtons is currently under active development, you may not want to epoxy fill the case just yet.

Therefore, if you milled a slot for the USB-C connector, I recommend using some hot glue to temporarily seal and fix the microcontroller in the case. If you opted to simply drill a small hole for two power wires (connected to GND and 5V), then you can apply sealant to the power wire holes to make it waterproof, then screw the lid on. 

## Mounting to the Motorcycle
Now that you've completed your build, and hopefully tested it successfully with the DMD2 beta app, you are ready to mount it on the motorcycle. This is the perfect time to be creative and figure out new and better ways to mount the unit. One simple option is to use waterproof, double sided mounting tape to adhere it to the switchgear on the right hand side, for example. You could also adhere it to any other relatively flat surface, including one attached to a RAM ball mount. If you are ambitious, you could attach it to a standard handlebar clamp mount with a screw and bolt or inserting a stud into the case and filling with epoxy. The options are endless.

<img src="Photos/MotoButtonsLiteMounted.jpg" alt="motobuttons lite installed on handlebar" width="800"/>

## Updating the Software for Future Releases
To update the software again, you need to open the case and connect the USB-C port to your computer. Launch the Arduino IDE and open the new source code file. However, there is a bug in the microcontroller's DFU (Device Firmware Update) bootloader that sometimes prevents new software from being uploaded unless a special reset procedure is followed. 

If you try to upload new software and you either get an error stating the DFU upload failed, or it simply times out after about 60 seconds, you probably need to perform the special reset. There are two options to perform this reset. The first option is to hold down buttons A and B for 10 seconds, as if you were changing the mode. This should reset the controller into DFU mode again. The second option is to rapidly double tap the tiny reset button that is next to the USB-C connector. However, this is hard to do because it is so small, and it requires opening the case, which could be impossible if it is filled with epoxy and you can't get to the button. 

After resetting into DFU mode, keep in mind that the COM port (serial port) number of the microcontroller will have changed. So you will need to select it again in the drop down menu in the toolbar at the top of the Arduino IDE. Then you can proceed to upload the new code.

