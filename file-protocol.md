# File protocol for Laser Projector

Each file contains bytes. Every three bytes correspond to an instruction for the projector.

Exept for the first byte. A 11111111(255) tells the arduino that they are reciving a new image.

The first byte specifies the instruction. The latter two bytes specify the arguments.

A three byte 0 is used as EOF.

1 x y: is a moveto command. It moves the laser to point x y. With the laser off.

2 x y is a draw line command. It movees the laser to point x y. With the laser turned on.
