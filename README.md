# Harpsichord Jack Tracking

All material related to Harpsichord Jack Tracking Project

## Measurementes

###  Jack spacing from back plate

```
jack 1:  10.25 mm
jack 2:  25.1  mm
jack 3:  38.5  mm
```

Gives a rough jack pitch of `14.4875 mm` with an offset of `10.25 mm`.

Rounded to the nearest `0.05mm` with a positive offset, picthes would be

```
jack 1:  10.25 mm
jack 2:  24.8  mm
jack 3:  39.3  mm
```

## MIDI Specifications


One channel per key as per Standard MPE spec.

Channel Aftertouch should be one stream, taken from the front jack tracking signal.

There are two note ons with associated velocities and note offs

A `note on` message `channel`, `pitch value`, `velocity`.

Velocity demarcates wether the note on message is related to the first jack or the second jack.

Given the signal obtained from the jack tracking does not provide the resolution in velocity to utilize the full 7 bit set of velocity values.

The First jack uses the velocity values from `0` to `63` and the second jack from `64` to `127`.

Note off messages use the same specification.

Therefore the physcial model can relate the note off message to it's respective note on message.

example:

|            | Jack1 | Jack2  |
| ---------- | ----- | ------ |
| `channel`  | 1     | 1      |
| `pitch`    | 56    | 56     |
| `velocity` | 0-63  | 64-127 |

**NOTE:** There is a secodary use case where the controlleris used with a digital instrymentr that expects a standard MPE signal. In that case there _may_ be problem. However, it is not guarenteed to not work. Behaviour is undefinded.

The same is true of the plug-in, which will be written to expect the above format of note messages.
As a result, the plug-in will need two modes of operation. A standard MPE for using with a standard MPE keyboard requiring a single note on/note off per key. The econd mode designed spewcifially for the harpsichord keyboard interface. This gives control of the excitation of both of the strings.