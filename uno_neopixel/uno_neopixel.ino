/** --- FULL LED STRIP ---
 * This program generates nice patterns, animations and controls for your Neopixel LED Strip
 * Created by Clément Lizé
 * See: https://clementlize.com
 * Github repository: https://github.com/clementlize/full-led-strip
 * v0.1 - Nov 2020
 */

// ------ NEOPIXEL ------

  #define LED_PIN        2    // PIN where the Neopixel is plugged in
  #define NUMPIXELS      300  // Number of LEDs available on the LED Strip

  // Adafruit Neopixel Library
  #include <Adafruit_NeoPixel.h>
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ------ END: NEOPIXEL ------

// ------ CLASS LED ------

  /** All the colors available for this project
   * Based on Bootstrap colors
   * See the public method setColor() for implementation information
   */
  typedef byte mainColor;
  #define MC_RED    1;
  #define MC_GREEN  2;
  #define MC_BLUE   3;

  byte ledBrightness;

  /** Represents a LED
   */
  class Led {

    private:

      int _id;  // LED id
      mainColor _color;  // LED color

    public:

      /** CONSTRUCTOR
       * Initialising a Led.
       * WARNING: use setId() after creating the object!
       */
      Led () {

        _id = 0;  // WARNING: use setId() after creating the object!
        _color = MC_RED;  // Default color
      };

      /** Setting a LED id
       * @param id the LED id
       */
      void setId (int id) {

        _id = id;
      };

      /** Sets a color to the LED.
       * @param color   The color to set. See the declaration of mainColor for info
       */
      void setColor (mainColor color) {

        _color = color;
      };

      /** Sends an order to display a LED with its current parameters.
       * WARNING: this is a synchronous operation: the order is cached and will be executed
       * when the function pixels.show() is called (at the end of void loop())
       */
      void askDisplay (byte brightness) {

        uint32_t hsv;
        byte hsvBrightness;

        switch (brightness)
        {
        case 0:
          hsvBrightness = 0;
          break;

        case 1:
          hsvBrightness = 30;
          break;

        case 2:
          hsvBrightness = 60;
          break;

        case 4:
          hsvBrightness = 150;
          break;

        case 5:
          hsvBrightness = 250;
          break;
        
        case 3:
        default:
          hsvBrightness = 100;
          break;
        }

        // Setting basic color with its offset
        switch (_color) {

          case 1:
            hsv = pixels.ColorHSV(0, 255, hsvBrightness);
            break;

          case 2:
            hsv = pixels.ColorHSV(21845, 255, hsvBrightness);
            break;

          case 3:   
          default:
            hsv = pixels.ColorHSV(43690, 255, hsvBrightness);
            break;
        };

        // Applying the color
        pixels.setPixelColor(_id, pixels.gamma32(hsv));
      };
  };

// ------ END: CLASS LED ------

// Array of LEDs
Led leds[NUMPIXELS];

void setup() {

  //Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
  ledBrightness = 1;

  // Creating the led objects with their ids
  for (int i=0; i<NUMPIXELS; i++) { leds[i].setId(i); }
}

void loop() {

  byte cpt_color = 0;
  for (int i=0; i<NUMPIXELS; i++) {

    switch (cpt_color)
    {
    case 0:
      leds[i].setColor(1);
      cpt_color++;
      break;

    case 1:
      leds[i].setColor(2);
      cpt_color++;
      break;

    case 2:
    default:
      leds[i].setColor(3);
      cpt_color = 0;
      break;
    }
  }

  // Asking to display each LED of the strip
  for (int i=0; i<NUMPIXELS; i++) { leds[i].askDisplay(ledBrightness); }

  //THIS CALL MUST BE LOCATED AT THE END OF THE LOOP FUNCTION
  pixels.show();

  delay(500);
}
