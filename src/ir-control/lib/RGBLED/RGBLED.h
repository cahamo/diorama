/*!
 * @brief Encapsulation of an RGB LED attached to Arduino PWM output pins
 */

#ifndef RGBLED_h
#define RGBLED_h

#include <Arduino.h>

/*!
 * @brief Structure that encapsultes an RGB colour.
*/
struct RGBColour {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    inline bool operator==(RGBColour a) const {
        return (red == a.red) && (green == a.green) && (blue == a.blue);
    }
    inline bool operator!=(RGBColour a) const {
        return (red != a.red) || (green != a.green) || (blue != a.blue);
    }
};

/*!
 * @brief Class that manages an RGB LED. It can display or flash the LED in a given colour.
 *
 * @note Requires an RGB LED attached to 3 GPIO pins that support PWM.
 */
class RGBLED {
    public:
        static const RGBColour Off;
        static const RGBColour Red;
        static const RGBColour Green;
        static const RGBColour Blue;
        static const RGBColour Amber;
        static const RGBColour White;
    public:
        /*!
         * @brief Instantiate the RGBLED class to control an RGB LED using the specified GPIO pins.
         *
         * @note GPIO pins must support PWM. All GPIO pins are set to OUTPUT in this constructor.
         *
         * @param redPin GPIO pin controlling the red segment of the RGB LED.
         * @param greenPin GPIO pin controlling the green segment of the RGB LED.
         * @param bluePin GPIO pin controlling the blue segment of the RGB LED.
         */
        RGBLED(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);

        // No default or copy contructors
        RGBLED() = delete;
        RGBLED(const RGBLED &) = delete;

        /*!
         * @brief Display a given colour in the RGB LED.
         *
         * @param colour Colour to be displayed. Use RGBLED::Off to turn of the LED.
         */
        void display(RGBColour colour);

        /*!
         * @brief Flash the RGB LED.
         *
         * @param colour Colour to be displayed when the LED is on.
         * @param times Number of times to flash the LED.
         * @param offTimeMs Time the LED is off while being flashed (in milliseconds).
         * @param onTimeMs Time the LED is on while being flashed (in milliseconds).
         * @param pauseBeforeMs Optional. Time to turn off the LED for before flashing (in milliseconds). Set to 0 (default) if no pause required.
         */
        void flash(RGBColour colour, uint8_t times, unsigned long offTimeMs, unsigned long onTimeMs, unsigned long pauseBeforeMs = 0);

        /*!
         * @brief Blink the LED once.
         *
         * @param colour Colour to be displayed when the LED is on.
         * @param offTimeMs Time the LED is off while being flashed (in milliseconds).
         * @param onTimeMs Time the LED is on while being flashed (in milliseconds).
         * @param restorePrevColour Flag indicating whether current colour should be restored after blinking.
         */
        void blink(RGBColour colour, unsigned long offTimeMs, unsigned long onTimeMs, bool restorePrevColour);

        /*!
         * @brief Saves a copy of the current colour. Each call overwrites the saved colour.
         *
         * @return Returns the previously saved colour, or @ref RGBLED:Off is no colour has been saved.
         */
        RGBColour saveCurrentColour();

        /*!
         * @brief Displays the last saved colour. If no colour was saved using @ref saveCurrentColour() then the LED is switched off.
         *
         * @return Returns the last saved colour, or @ref RGBLED::Off if no colour has been saved.
         */
        RGBColour restoreColour();

        /*!
         * @brief Returns the currently dislayed colour.
         *
         * @return The current colour of the LED.
         */
        RGBColour getCurrentColour() const;
    private:
        uint8_t _redPin;
        uint8_t _greenPin;
        uint8_t _bluePin;
        RGBColour _currentColour;
        RGBColour _savedColour;
};

#endif