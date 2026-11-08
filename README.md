# Pico Mortarboard

This Pico project provides a small toy that has been included in a
mortarboard hat to mark the completion of a doctorate.  It features
two sounds, a sirene and a _wee-woo, wee-woo_ sound that may be played
by pressing two push buttons.  Two blue LEDs will flash, following the
rhythm of the sounds.

## Usage

The application features two push buttons.  Each of them serves both,
powering up the Pico as well as selecting one of two sounds (a sirene
and a _wee-woo, wee-woo_ sound) to play.  If both buttons are pushed
at the same time, both sounds will play simultaneously, but with
reduced volume to avoid sample clipping.

## Hardware Requirements

* **Rapberry Pi Pico** with RP2040 (it should also run on the newer
  RP2350, but I have not yet tested it).

* **A speaker.** Since for simplicity we omit an amplifier and drive
  the speaker solely from a `GPIO` pin.  The current that the speaker
  draws must be limited to at most 12mA at 3.3V (absolute limit at
  15mA) to avoid damage to the Pico.  Therefore, we a use protective
  resistor in series with the speaker.  The sum of the speaker's and
  the protective resistor's must be (3.3V / 12mA) = 275Ω or higher
  (but not too high to get maximum volume).  The speaker's
  contribution to the total resistor should be as high as possible;
  otherwise we also loose volume caused by the voltage drop on the
  protective resistor.  I used a speaker with roughly 100Ω and a
  protective resistor with 220Ω, resulting in a total value of roughly
  320Ω.

* **A protective resistor** in series with the speaker, as discussed
  above.  In my case with a 100Ω speaker, I decided for a 220Ω
  resistor.

* **Two push-buttons** for turning on power and starting the sound.

* **Two rectifying diodes**, such as `1N4148`.  The Pico operates at
  1.8V to 5.5V on `VSYS`.  Given to AA batteries and a voltage drop of
  around 0.7V of the `1N4148`, the Pico will stop to work correctly
  when the average voltage of the batteries has dropped to around
  (1.8V + 0.7V) / 2 = 1.25V.  Maybe a better choose would be a diode
  with lower voltage drop like the Schottky diode `1N5817`, such the
  batteries will work until their average voltage drops to about (1.8V
  \+ 0.2V) / 2 = 1.0V.  Note that using three batteries in series is
  not an option, since the input voltage on the `GPIO` pins must not
  exceed 3.3V for proper operation.  A voltage above 3.6V is likely to
  destroy the `GPIO` pin.

* **Two blue LEDs.** There a blue LEDs at different wave lengths and
  different brightness.  For visual reasons, be sure to choose a deep
  blue LED.  Still, the voltage drop may vary between roughly 3.0V and
  3.6V.  Since the `GPIO` high ouput level is roughly 3.3V, try to
  choose a deep blue LED with as low voltage drop as possible;
  otherwise the brightness of the LEDs will be poor.  Also, put a
  resistor in series to limit the current, even if the nominal voltage
  drop on the resistor is expected to be very small.

* **Two resistors** for limiting the current through the LEDs, as
  mentioned above. To be on the safe side, I have chosen a 150Ω
  resistor for each LED.

* **Two 1.5V alkaline batteries** (type `AA` or `AAA`).  The two
  batteries are connected in series, one after the other.

* **A battery holder** for the two batteries.

* **Stranded wire**, 0.14mm² or more for soldering all components
  together.

No D/A converter is needed, since we use PWM on a single `GPIO` to
create wave forms with 8 bit resolution.

## Wiring

* The minus pole of the two batteries in series is connected with Pico
  `GND` (pin 38 or any other `GND` pin).

* The plus pole of the two batteries in series splits into two
  branches, each branch wired with one side of one switch, again
  branching into a wire connected to `GPIO 2` (pin 4) and `GPIO 3`
  (pin 5) respectively, and the other branches each connecting to the
  `VSYS` (pin 39) via the rectifying diode.  That is, when pressing
  one of the two buttons, the plus will be directly connected to `GPIO
  2` and also (via one of the two diodes) with `VSYS`.  When pressing
  the other button, the plus will be directly connected to `GPIO 3`
  and also (via the other diode) with `VSYS`.

* On side of the series of speaker and protecting resistor is
  connected to `GPIO 1` (pin 2), and the other side to `GND` (pin 3 or
  any other `GND` pin).

* One of the LEDs is (in series with the current limiting resistor)
  connected to `GPIO 5` (pin 7) and `GND` (pin 8 or any other `GND`
  pin).

* The other LEDs is (in series with the current limiting resistor)
  connected to `GPIO 6` (pin 9) and `GND` (pin 8 or any other `GND`
  pin).

## Software Environment

This code compiles on a Raspberry Pi 4 with Pico SDK 1.5 or later
installed.  It should also compile on any other platforms that support
developing code for the Pico.

## Compiling

Assuming a Raspberry Pi as development environment, ensure you have
installed the C/C++ development environment as provided by the
Raspberry foundation.  The software of this project expects the
`pico-sdk` and `pico-examples` directories to be found in the parent
directory of this project's root directory.  It should be ok to use
symlinks that point to the installed development environment, if it is
not acceptable to place this project side-by-side with the environment
installation.

From within the root directory of this project, just invoke

```bash
$ build.sh
```

to compile the project.  On success, continue with deploying the
compiled code to your Pico.

## Deploying

Make sure your Pico is ready to receive a new `.uf2` flash image file.
Your Pico should be visible as a mounted file system, and for now, the
script assumes that this file system appears in the home directory of
user `pi`, called `RPI-RP2`.  If this is not the case, just copy the
`.uf2` flash image from the `build` directory onto the Pico as
described in the Pico docs.

Note that any other previously existing code will be overwritten.  If
you have previously uploaded files using the Python developing
environment, the complete environment on the Pico, including all
uploaded files, will be overwritten when uploading the flash image.

To upload and run the application, simply run the `deploy.sh` script
from within the root directory of the project:

```bash
$ deploy.sh
```
