# MotoButtons Lite
A low cost, waterproof, Bluetooth (BLE) controller for [Drivemode Dashboard](https://www.drivemodedashboard.com/) motorcycle navigation with joystick and mouse mode.
**At this time, you must [install the DMD2 beta version](https://youtu.be/bJglvH23oGI?feature=shared&t=128) from the Google Play Store by joining the DMD2 beta program at the button of the DMD2 app page in the Play Store**

## Why a DIY controller?
As of October of 2023, [all available controllers](https://www.drivemodedashboard.com/certified-remote-controllers/) for Drivemode Dashboard are rather expensive, especially if shipped outside of Europe, or do not offer the ideal functionality in a small size. The [least expensive controller](https://www.thorkracing.com/produto/arn-remote-pro/) that is in stock and can be shipped to the USA costs $215 USD with shipping. Since many riders have two, three or even four motorcycles, the total cost could be as high as $683, just for a set of basic controllers! However, even this controller only offers a two-way switch, so functionality is limited. 

On the other hand, an ideal controller would offer a low price, small size, light weight and a navigation switch (joystick). The joystick could be used either as a "mouse" (up/down/left/right/click) or to manipulate Drivemode Dashboard functions. 

MotoButtons Lite is for the DIY enthusiast who wants to construct a low cost controller. The trade off is that the Lite version requires careful drilling and gluing to construct the enclosure and waterproof the joystick.

## Cost and Parts Availability
As a DIY controller, the MotoButtons Lite has been designed with the highest quality, IP68 (waterproof) parts, while maintaining a low cost. In fact, the necessary parts can be ordered from Mouser.com for only $27.82 plus $7.99 UPS shipping. The only exception is that a low cost, IP68 rated 5-way navigation switch can not be purchased at this time ([this](https://www.digikey.com/en/products/detail/ruffy-controls-inc/HS1-6/17868372) is the least expensive I found, at $76 each). Thus, you will waterproof a $2.50 switch, which is surprisingly rugged and user friendly. In addition, you will need some epoxy glue, basic hole drilling tools and some other basic supplies, which most DIY enthusiasts will have in their garage.

## Microcontroller and Programming Experience Not Needed
No programming knowledge or skill is needed to program the Bluetooth (BLE) Arduino microcontroller. Source code, as well as a simple installer are available on this site. However, if you would like to modify the code to change or add functionality, or just want to learn Arduino programming, you can [download the programming environment](https://www.arduino.cc/en/software) (IDE) and [follow the guide](https://wiki.seeedstudio.com/XIAO_BLE/) for configuring the environment for the [Seeed XIAO nRF52840](https://www.seeedstudio.com/Seeed-XIAO-BLE-nRF52840-p-5201.html) board.

## Recommended Tools and Supplies
To build your own Motobuttons Lite, I suggest the following tools and supplies, which can be obtained at Amazon and [Harbor Freight](https://www.harborfreight.com/), for example. However, as a DIY enthusiast, you will hopefully already have many suitable tools.

- [C-POXY 30 by CECCORP](https://www.amazon.com/CECCORP-C-Poxy-30/dp/B0918TGK53)
  - Do not use a quick setting epoxy (e.g. 1 or 5 minute), as it lacks strength and sets too quickly for proper construction
- [Step drill with 35/64" diameter](https://www.harborfreight.com/2-piece-titanium-nitride-coated-high-speed-steel-step-drills-96275.html)
- [Wire size drill bit set for #56 and #60 drill bits](https://www.amazon.com/Drill-Speed-Steel-Numbered-Metal/dp/B07SW836NQ/ref=sr_1_9?crid=2E4TQN5R710MU)
  - Only needed if you don't already have #56 and #60 drill bits, such as in a 115 piece set
- [Automatic center punch](https://www.harborfreight.com/spring-loaded-center-punch-621.html)
  - For precise alignment of drilled holes to the schematic drawing template
- [Needle file set](https://www.harborfreight.com/needle-file-set-12-piece-56743.html) (Optional)
  - Needed only if you want to have external USB-C connectivity and you are potting with epoxy
- Small piece of sandpaper, such as 150 grit or similar
  - For roughening the case surface prior to gluing the navigation joystick. Can be obtained at any auto parts or home improvement store.
- Silicone sealant for waterproofing joystick
  - Black color is recommended, such as [Permatex 81158](https://www.amazon.com/Permatex-81158-Silicone-Adhesive-Sealant/dp/B000AL6WLA). Can also be found at most auto parts stores for a higher price. If you already have clear silicone, there's no need to buy additional sealant.
- Alcohol wipes or rubbing alcohol
  - For cleaning the case prior to gluing the navigation joystick. You can obtain these at any grocery or drug store, or [on the internet](https://www.amazon.com/Curad-Alcohol-Prep-Thick-Swabs/dp/B00KOSP454).
- Small gauge insulated wire
  - You'll need small gauge (e.g. 26 AWG) wire for soldering the microcontroller to the joystick, buttons and LEDs. If you have an old USB or serial port cable lying around, you can use the wire from there. Otherwise, you can [order wire on the net](https://www.amazon.com/Fermerry-Stranded-Colors-Flexible-electrical/dp/B089CZFDX2).
- [Soldering iron, solder and related tools](https://www.amazon.com/Soldering-Kit-Temperature-Desoldering-Electronics/dp/B07GTGGLXN)
  - You'll need an iron with a small, pointed tip and basic tools such as needle nose plyers and tweezers.

## Parts to Order
In addition to the tools and supplies mentioned above, you will need to order the parts required to construct Motobuttons Lite. [This link](https://www.mouser.com/ProjectManager/ProjectDetail.aspx?AccessID=e90304f243) will allow you to order the necessary parts from Mouser.com. Also, you can view the [formal Bill of Materials](https://github.com/joncox123/MotoButtons/tree/main/Parts) and data sheets, if interested.
-	$10.38 [Seeed Studio XIAO nRF52840 - Bluetooth 5.0](https://www.mouser.com/ProductDetail/713-102010448)
-	$5.52 [Hammond Polycarbonate Black IP68 Enclosure 1551WHBK](https://www.mouser.com/ProductDetail/546-1551WHBK)
-	$4.38 [Blue IP68 button with white LED](https://www.mouser.com/ProductDetail/655-2-2329387-1)
-	$5.09 [Green IP68 button with white LED](https://www.mouser.com/ProductDetail/655-1-2329387-6)
-	$1.95 [5-way Navigation Joystick Switch](https://www.mouser.com/ProductDetail/485-504)
-	$0.50 [Rubber boot for joystick](https://www.mouser.com/ProductDetail/485-4697)

## Instructions
Complete [step-by-step instructions](https://github.com/joncox123/MotoButtons/tree/main/ConstructionGuide) for building your own MotoButtons Lite are available. If you have questions, concerns or suggestions, [please create a new issue](https://github.com/joncox123/MotoButtons/issues) in this GitHub repository.
