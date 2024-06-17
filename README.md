# Harpsichord Jack Tracking

This repository collects together all materials (CAD Drawings, diagrams, firmware source code) related to Harpsichord Jack Tracking Project at the NEMUS project.

| ![ ](img/side_w-o_sensors.jpeg)                             |
| ----------------------------------------------------------- |
| Side View of the model harpsichord used during development. |

- [Harpsichord Jack Tracking](#harpsichord-jack-tracking)
  - [Measurements](#measurements)
    - [Model Keyboard](#model-keyboard)
    - [Jack Pitch](#jack-pitch)
    - [Full Scale Keyboard](#full-scale-keyboard)
    - [Jack Pitch](#jack-pitch-1)
    - [Diagram](#diagram)
  - [QRE1113 Notes](#qre1113-notes)
    - [Functionality](#functionality)
    - [Data sheets](#data-sheets)
  - [MIDI Specifications](#midi-specifications)
    - [Jack Differentiation](#jack-differentiation)
    - [Aftertouch](#aftertouch)


## Measurements

### Model Keyboard

###  Jack Pitch

| Jack | Centre distance from backboard (mm) |
| ---- | ----------------------------------- |
| 1    | `10.25`                             |
| 2    | `25.1`                              |
| 3    | `38.5`                              |

Gives a rough jack pitch of `14.4875 mm` with an offset of `10.25 mm`.

Pitch of the back row of jacks was determined by measuring from the open edge of the model to the front edge of the jack. Each jack had its width measured providing the values in the table below.

| Jack | Front edge dist (mm) | Width (mm) | Centre dist (mm) |
| ---- | -------------------- | ---------- | ---------------- |
| 4    | `59.85`              | `4.25`     | `61.975`         |
| 5    | `45.25`              | `4.30`     | `47.400`         |
| 6    | `31.20`              | `4.60`     | `33.500`         |

Give a rough pitch of `14.2375 mm` with an offset of  `33.5 mm` from the front edge

### Full Scale Keyboard 

###  Jack Pitch

### Diagram

| ![ ](img/cross_section_invert.png)                |
| ------------------------------------------------- |
| Cross-section of full sized harpsichord interface |


## QRE1113 Notes

| ![ ](img/top_down_w_nano.jpg)                               |
| ----------------------------------------------------------- |
| Model keyboard and QRE1113s attached to Arduino Nano 33 IoT |


Displacement of the jacks is measure using QRE1113 IR LED / Transistor pair. The section covers some general observations and helpful information about these sensors.

### Functionality

QRE1113s are used with alongside adhesive strips with a black and white gradient printed on it, which are attached to each jack. 

The optimal distance between the strip and the sensor is around 6mm with a voltage reference of [`1.65V` provided by Arduino SAMD boards](https://www.arduino.cc/reference/en/language/functions/analog-io/analogreference/)

| ![ ](img/jacks_w_strip_sensor_internal.jpg) |
| ------------------------------------------- |
| Jacks with gradient strip attached          |


### Data sheets

- [QRE1113 Datasheet](https://www.mouser.in/datasheet/2/308/QRE1113-1121523.pdf)

<img width="496" alt="image" src="https://github.com/mhamilt/harpsichord-model-data/assets/33174176/ae605c5e-47d4-40e9-a4b1-86a8f60ec120">
<img width="394" alt="image" src="https://github.com/mhamilt/harpsichord-model-data/assets/33174176/14ffb907-1013-4fd9-bb91-761ac52bf176">


## MIDI Specifications

Below are additional MIDI specifications retrieve the most information from the controller. 

Currently this is being written and subject to change

### Jack Differentiation

There is one channel per key as per [Standard MPE specifications](https://midi.org/mpe-midi-polyphonic-expression#:~:text=The%20MPE%20specification%20defines%20a,products%2C%20using%20MIDI%201.0%20messages.).


There are two note ons with associated velocities and note offs

A `note on` message `channel`, `pitch value`, `velocity`.

Velocity demarcates wether the note on message is related to the first jack or the second jack.

Given the signal obtained from the jack tracking does not provide the resolution in velocity to utilize the full 7 bit set of velocity values.

The First jack uses the velocity values from `0` to `63` and the second jack from `64` to `127`.

Note off messages use the same specification.

Therefore the physical model can relate the note off message to it's respective note on message.

example:

|            | Jack1 | Jack2  |
| ---------- | ----- | ------ |
| `channel`  | 1     | 1      |
| `pitch`    | 56    | 56     |
| `velocity` | 0-63  | 64-127 |

**NOTE:** There is a secondary use case where the controller is used with a digital instrument that expects a standard MPE signal. In that case there _may_ be problem. However, it is not guaranteed to not work. Behavior in this case is undefined.

The same is true of the plug-in, which will be written to expect the above format of note messages.
As a result, the plug-in will need two modes of operation. A standard MPE for using with a standard MPE keyboard requiring a single note on/note off per key. The second mode designed specifically for the harpsichord keyboard interface. This gives control of the excitation of both of the strings.


### Aftertouch

Channel Pressure (MIDI Status event nybble `0xD`) is sent as one stream, taken from the front jack tracking signal.

