/*!
 * @brief Encapsulation of an RGB LED attached to Arduino PWM output pins
 */

#include <Arduino.h>
#include "RGBLED.h"

const RGBColour RGBLED::Off = {0, 0, 0};
const RGBColour RGBLED::Red  = {255, 0, 0};
const RGBColour RGBLED::Green  = {0, 255, 0};
const RGBColour RGBLED::Blue  = {0, 0, 255};
const RGBColour RGBLED::Amber  = {192, 192, 0};
const RGBColour RGBLED::White = {160, 160, 160};

RGBLED::RGBLED(uint8_t redPin, uint8_t greenPin, uint8_t bluePin) {
    _redPin = redPin;
    _greenPin = greenPin;
    _bluePin = bluePin;
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
    display(Off);
    _savedColour = Off;
}

// RGBLED::RGBLED() = delete;

void RGBLED::display(RGBColour colour) {
    analogWrite(_redPin, colour.red);
    analogWrite(_greenPin, colour.green);
    analogWrite(_bluePin, colour.blue);
    _currentColour = colour;
}

void RGBLED::flash(RGBColour colour, uint8_t times, unsigned long offTimeMs, unsigned long onTimeMs, unsigned long pauseBeforeMs) {
    if (pauseBeforeMs > 0) {
        // turn off LED before starting to flash
        display(Off);
        delay(pauseBeforeMs);
    }
    // flash the LED
    for (uint8_t i = 1; i <= times; i++) {
        display(colour);
        delay(onTimeMs);
        display(Off);
        delay(offTimeMs);
    }
}

void RGBLED::blink(RGBColour colour, unsigned long offTimeMs, unsigned long onTimeMs, bool restorePrevColour) {
    RGBColour save = _currentColour;
    if (_currentColour != Off) {
        display(Off);
        delay(offTimeMs);
    }
    display(colour);
    delay(onTimeMs);
    display(Off);
    delay(offTimeMs);
    if (restorePrevColour) {
        display(save);
    }

}

RGBColour RGBLED::saveCurrentColour() {
    RGBColour previous = _savedColour;
    _savedColour = _currentColour;
    return previous;
}

RGBColour RGBLED::restoreColour() {
    display(_savedColour);
    return _currentColour;
}

RGBColour RGBLED::getCurrentColour() const {
    return _currentColour;
}
