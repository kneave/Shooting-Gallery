# NERF Gun Shooting Gallery
First off, this has nothing to do with Hasbro or NERF themselves, I'm just a big fan of the toys and this seemed a fun game to make.

This repository contains all the files needed to build a NERF based shooting gallery.  It was built while mostly sleep deprived so it isn't without its quirks...  It is based around an Arduino Mega controlling a bunch of servos, WS2811 RGB LEDs and IR sensors.  The game board was laser cut and the rest of the parts were 3d printed.  **Note**,
I made a bunch of rookie errors on this and though the resulting product works, use it as a base rather than assuming it'll work out of the box.

# Overview
The idea behind this game is that it would be easy for the games team to run at the Reading Beer Festival, namely that it keeps track of its score and the targets reset.  To that end I designed a target hinge that contained a servo to reset the target once hit along with a sensor to detect a hit in the first place.

## Electronics
The electronic heart of this project is an Arduino Mega with an Adafruit PWM Servo shield.  Connected to it are:-
 - [Six WS2811 RGB LEDs](https://coolcomponents.co.uk/products/digital-rgb-addressable-led-5mm-through-hole-10-pack)
 - Six 9G hobby servos
 - [Five QTR-1A IR sensors ](https://www.pololu.com/product/958)
 - Four momentary buttons, one illuminated (optional)

The pins used are as follows:
- [Analog] 8, 9, 10, 11, 12: IR sensors
- 32: WS2811 LED pin
- 37: Reset button LED (with limiting resistor)
- 41: Reset button
- 42: IR sensor emitter pin
- 43: Set button
- 45: Up button 
- 47: Down button

The servos are connected to connections 0-5 with 0 being the points gauge. 
 
## Backboard
The backboard is was lasercut at rlab.org.uk out of transparent acrylic.  When I cut the board I was inexperienced with the laser cutter software and the result is slightly smaller than the SVG.  As a result the 3D printed parts are scaled to fit what was cut, not what was designed.  You could either scale the file to match the 3D printed components or tweak them to fit the original design.  Or just do your own thing which is what I'd recommend!

## 3D Printed/Lasercut Components

The majority of components in this game are 3D printed, as mentioned above they are scaled for the size my board ended up being rather than designed so triple check that everything will match and learn from my mistake!  

- 45deg_LED_holders.stl, rendered file for the 45 degree LED holders
- LED_holders.scad, source file for the above
- button_bracket.stl, rendered file for the button holder bracket
- button_bracket.scad, source for the above
- counter arm.stl, an arm for the point gauge but also used as the arm to reset the targets
- parametric arm.scad, source for the above
- points gauge.stl, a simple 0..100 analogue gauge
- points gauge.scad, source for the above
- Target.dxf, DXF file of the backboard
- Target.svg, SVG of the above
- target_arm_10p.stl, an arm that holds the 10 point target
- target_arm_25p.stl, an arm that holds the 25 point target
- target_arm_5p.stl, an arm that holds the 5 point target
- target_hinge.stl, a rendered file for the hinge that holds the target arm, servo and sensor
- target_hinge.scad, source for the above but also, confusingly, the target arms...
- target_scores.stl, rendered file that contains an embossed marker to go on each target
- target_scores.scad, source for the above

## QTR-1A IR Sensors
I had a bunch of these sensors and I though that they'd be a great fit for this game, the target would never physically hit the sensor so should improve reliability.  Fantastic idea in theory, in practice it worked at home but not when I took it to the beer festival.  The fest is held in a large marquee outdoors and the fiery sun god was angry that weekend.  The IR from the sun swamped the sensors and rendered them useless until I cobbled together an enclosure for it using a cardboard box, gaffer tape and a few black bin bags.
In the code there are two sets of sensitivity levels for the sensors, day and night, use them accordingly.  If you're only going to use this indoors you should be fine.
## Servos
The servos I used and the servos you use may have very different min and max values to mine so you'll want to double check these values in a simple servo test sketch first.  Adafruit include one that is ideal with the servos shield library.

## WS2811 LEDs
The LEDs I used behaved oddly to start with, the CRGB colour values didn't map correctly.  Turned out that the FastLED library has a colour order variable and I needed to change it from the default to RGB for mine.  If your LEDs misbehave, this could be why.
