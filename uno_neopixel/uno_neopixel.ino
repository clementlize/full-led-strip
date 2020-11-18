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
  typedef enum {
    RED, GREEN, BLUE
    } mainColor;

  /** Color offset
   * Defines the offset of the color (more or less red for example)
   * Constraints: -5 <= value <= 5
   */
  typedef int colorOffset;

  /** LED Brightness
   * Constraints: 1 <= value <= 5
   */
  typedef int ledBrightness;

  /** Interface for the Led class
   */
  class Led {

    public:

      // Constructor
      Led (int id);  

      // Methods
      askDisplay ();
      setColor (mainColor color, colorOffset offset);
      setBrightness (ledBrightness brightness);
      setState (bool state);

    private:

      int _red;  // Red intensity
      int _green;  // Green intensity
      int _blue;  // Blue intensity

      int _id;  // LED id
      bool _state;  // LED state: on (true) or off (false)
      mainColor _color;  // LED color
      colorOffset _colorOffset;  // LED color offset
      ledBrightness _brightness;  // LED brightness
  };

  /** Constructor
   * Initialising a Led with its id
   */
  Led::Led (int id) {

    _id = id;
    _color = RED;  // Default color
    _colorOffset = 0; // No offset
    _brightness = 3;  // Default brightness
    _state = true;  // The Led is turned on
  };

  /** Sends an order to display a LED with its current parameters.
   * WARNING: this is a synchronous operation: the order is cached and will be executed
   * when the function pixels.show() is called (at the end of void loop())
   */
  Led::askDisplay () {

    Serial.println("Asking to display led " + _id);

    int red, green, blue;

    // Checking if the LED is turned on
    if (_state) {

      // Setting basic color with its offset
      switch (_color) {

        case RED:
          red = 255;
          green = 0;
          blue = 0;
          break;
      };

      // Applying brightness
      red = 120;
      green = green;
      blue = blue;

    }
    // LED is turned off
    else {

      red = 0;
      green = 0;
      blue = 0;
    }

    // Applying the color
    pixels.setPixelColor (_id, pixels.Color(red, green, blue));
  };

// ------ END: CLASS LED ------

// Array of LEDs
Led* leds = (Led*) malloc(sizeof(Led) * NUMPIXELS);

void setup() {

  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.

  // Creating the led objects with their ids
  for (int i=0; i<NUMPIXELS; i++) {

    leds[i] = Led (i);
  }
}

void loop() {

  //pixels.setPixelColor(73, pixels.Color(120, 120, 120));

  //led_1.askDisplay();

  // Asking to display each LED of the strip
  for (int i=0; i<NUMPIXELS; i++) {

    leds[i].askDisplay();
  }

  //THIS CALL MUST BE LOCATED AT THE END OF THE LOOP FUNCTION
  pixels.show();
}
