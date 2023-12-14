# Laser-projector

This program allows an arduino to read image data over serial and send data to servos over SPI. Is also accepts configuration data over serial.

# Concept

This takes instructions over Serial and starts drawing them. The instructions tell where to move the laser, and if the laser should be on. This does not actually move the motors by itself. It only sends instructions over SPI. Currently it can only draw straigh lins from one point to another. Vertical and horizontal lines are trivial. Other lines are drawn using Bresenham's algorithm.

The servos have 255 positions each to be in. Therefore we can think of the result as a 255x255 screen with 'pixels'.

# Usage

## Commands:

### New images:

sending the char `d` makes the arduino ready to recieve a new image. The images are given as described in file-protocol.md. Three bytes of null (0x00) will end the image and the controller will then start drawing the image

### Settings:

`gwt <NUMBER>` changes GOTO_WAIT_TIME to number

`mwt <NUMBER>` changes MOVING_WAIT_MULTIPLIER to number

`mxe <NUMBER>` if number is 0 then x motor is disabled. Otherwise it is enabled

`mye <NUMBER>` if number is 0 then y motor is disabled. Otherwise it is enabled
