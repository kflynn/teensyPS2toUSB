# teensyPS2toUSB

I have a couple of very old Kinesis Contour keyboards. These are ancient PC/AT keyboards which I've been using with a Macintosh via the hack of connecting the keyboard to a PC/AT to PS/2 adapter, then connecting that to a PS/2 to USB adapter.

Problem is, the adapters are dying and becoming hard to find... but hey, Teensy-LC is cheap, and it's small enough to fit into the Kinesis case. Disconnecting the Kinesis' PC/AT cable from its controller board is easy, and it even uses a connector that a standard 0.1" header will fit, so getting the Kinesis to send its PS/2 protocol directly to the Teensy is easy too. This project is the code to interpret the PS/2 keyboard codes and turn them into USB HID messages.

To keep the code simple, the lion's share of the parsing is done with a finite state machine, generated by `scanCodesToFSM.py` from `scanCodeMap` -- or, if you're like me and using the Kinesis in Dvorak, from `scanCodeMap2`. The PS/2 protocol is so hideous that it uses _16_ states rather than the one or two I was expecting -- since the Teensy-LC doesn't have that much RAM, I collapsed the _12_ degenerate states into a `switch` statement in the code. (Honestly, I should probably modify `scanCodesToFSM.py` to output code rather than data tables -- the Teensy-LC has a lot more code space than RAM.)

Once the scan code FSM is built, compiling the sketch should be very straightforward with Teensyduino. Just make sure you pick the USB Keyboard, and make sure you use a level shifter on the keyboard data lines if you're using a Teensy-LC (they are NOT 5V tolerant, and the Kinesis runs on 5V).

Note well: several folks out there have hacked these keyboards by completely replacing the controller inside the Kinesis with a Teensy. That's definitely The Right Way To Do This™ -- on the other hand, I need to get the keyboard back into service now, without the time to be wrangling that level of hardware mod. This way was _much_ faster to get working, and it'll work fine until the Kinesis' internal controller dies (in fact, I'm typing on the modded keyboard right now).

Enjoy! (More notes on the hardware shortly.)
