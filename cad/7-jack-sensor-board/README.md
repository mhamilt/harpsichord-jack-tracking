# 7 Jack Sensor Board

This word was an intermediate to try out the new spacing for the full scale model

Pins for an Arduino Nano are included directly on the board.

## Wiring

|   Connection   | Pin |
| -------------- | --- |
| Jack 0 QRE1113 | A0  |
| Jack 1 QRE1113 | A1  |
| Jack 2 QRE1113 | A2  |
| Jack 3 QRE1113 | A3  |
| Jack 4 QRE1113 | A4  |
| Jack 5 QRE1113 | A6  |
| Jack 6 QRE1113 | A7  |
| Jack 0 LED     | D3  |
| Jack 1 LED     | D4  |
| Jack 2 LED     | D5  |
| Jack 3 LED     | D6  |
| Jack 4 LED     | D7  |
| Jack 5 LED     | D8  |
| Jack 6 LED     | D9  |
| Rotary CLK     | D12 |
| Rotary DATA    | D11 |
| Rotary Switch  | D10 |

## PCB

### Eagle Libraries

- https://github.com/cyberlink1/Arduino-Eagle-Cad-Library

### Logo

[Imagemagick](https://imagemagick.org) was use to quantise the nemus logo to a 1-bit bitmap for the silkscreen

```sh
magick logo.png logo.bmp
magick logo.bmp -resize 8x8% logo.bmp
magick logo.bmp -monochrome logo.bmp
magick -flop -flip logo.bmp logo_b.bmp # for the bottom layer, the image is reversed
```