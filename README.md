# Mortarboard Design

TODO

## Hardware Requirements and Wiring

* Rapberry Pi Pico with RP2040 (it should also run on the newer RP2350, but I have not yet tested it).
* Since for simplicity we omit an amplifier and drive the speaker solely from a GPIO pin, current through
  protective resistor and speaker must be limited to 12mA at 3.3V to avoid damage to the Pico.  Therefore,
  the Speaker should have 100Ω or higher (otherwise, most of the power will be lost at the protective
  resistor).
* 220Ω protective resistor in series with the speaker.
* A push-button for turning on power and starting the sound.
* A 9V battery.

No D/A converter is needed, since we use PWM on a single GPIO to create wave forms with 8 bit resolution.

## Software Environment

This code compiles on a Raspberry Pi 4 with Pico SDK 1.5 or later installed.
It should also compile on any other platforms that support developing code for the Pico.

## Compiling

Assuming a Raspberry Pi as development environment, ensure you have installed the C/C++ development environment as provided by the Raspberry foundation.  The software of this project expects the <code>pico-sdk</code> and <code>pico-examples</code> directories to be found in the parent directory of this project's root directory.  It should be ok to use symlinks that point to the installed development environment, if it is not acceptable to place this project side-by-side with the environment installation.

From within the root directory of this project, just invoke

```$ build.sh```

to compile the project.  On success, continue with deploying the compiled code to your Pico radio clock.

## Deploying

Make sure your Pico is ready to receive a new <code>.uf2</code> flash image file.  Your Pico should be visible as a mounted file system, and for now, the script assumes that this file system appears in the home directory of user <code>pi</code>, called <code>RPI-RP2</code>.  If this is not the case, just copy the <code>.uf2</code>> flash image from the <code>build</code> directory onto the Pico as described in the Pico docs.

Note that any other previously existing code will be overwritten.  If you have previously uploaded files using the Python developing environment, the complete environment on the Pico, including all uploaded files, will be overwritten when uploading the DCF77 Radio Clock flash image.

To upload and run the radio clock flash image, simply run the <code>deploy.sh</code> script from within the root directory of the project:

```$ deploy.sh```

## TODOs / Ideas

* Multiple push-buttons for selecting different waves.
