/** --- FULL LED STRIP ---
 * This program generates nice patterns, animations and controls for your Neopixel LED Strip
 * Created by Clément Lizé
 * See: https://clementlize.com
 * Github repository: https://github.com/clementlize/full-led-strip
 * v0.1 - Nov 2020
 */


// ------ NEOPIXEL ------

  #define LED_PIN   2
  #define NUMPIXELS 300

  #include <Adafruit_NeoPixel.h>
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ------ END: NEOPIXEL ------


// ------ SENSORS ------

  #define KNOB_PIN 0
  #define BTN_PIN 10

// ------ END: SENSORS ------


// ------ CONSTANTS ------

  #define D_1   0
  #define D_2b  73
  #define D_2h  74
  #define D_3b  111
  #define D_3h  112
  #define D_4   118
  #define HD    119
  #define HM_d  150
  #define HM_g  151
  #define HG    182
  #define G_4   183
  #define G_3h  190
  #define G_3b  191
  #define G_2h  227
  #define G_2b  228
  #define G_1   299

// ------ END: CONSTANTS ------

// ------ TYPES ------

  typedef byte ledBrightness;  // Brightness value. Can be 1, 2, 3 or 4
  typedef byte colorOffset;  // Offset value. Must be between -3 and 3.

  /** The colors available for the LED strip.
   * Color palette from https://flatuicolors.com/palette/defo
   * Converted to HSV from https://www.peko-step.com/fr/tool/hsvrgb.html
   * Converted to Adafruit Neopixel HSV from https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
   */
  typedef enum {
    RED, GREEN, BLUE,
    TURQUOISE, EMERALD, PETER_RIVER, AMETHYST, WET_ASPHALT,
    SUN_FLOWER, CARROT, ALIZARIN, CLOUDS, CONCRETE,
    COLORS_COUNT
  } mainColor;

  // The directions available for the movments
  typedef enum {
    FULL_NORM, FULL_REV, HALF_DOWN, HALF_UP
  } direction;

// ------ END: TYPES ------


// ------ GLOBAL VARIABLES ------

  bool isOn = true;  // LED strip global on-off variable

  ledBrightness stripBrightness = 2;  // The main brightness of the strip
  byte stripBrightnessOffset = 0;

  direction stripDirection = FULL_NORM;  // The direction for the movments

  int stripDelay = 60;
  
  mainColor stripColor1 = ALIZARIN;
  mainColor stripColor2 = TURQUOISE;

// ------ END: GLOBAL VARIABLES ------


// ------ COLOR CONTROL FUNTIONS ------

  /*void knobChangeBrightness (int value) {

    if (value >= 0 && value < 138) { stripBrightness = 1; }
    else if (value >= 138 && value < 276) { stripBrightness = 2; }
    else if (value >= 276 && value < 414) { stripBrightness = 3; }
    else if (value >= 414 && value < 552) { stripBrightness = 4; }
    else { stripBrightness = 5; }
  }*/

  /** Returns the value to put in the setHsvColor function, depending on the global brightness value.
   * @param brightness the 
   * @returns the brightness value (0-255)
   */
  byte getHsvBrightness (ledBrightness brightness) {

    switch (brightness) {

      case 1:
        return 30;
        break;

      case 2:
        return 60;
        break;

      case 4:
        return 150;
        break;

      case 3:
      default:
        return 100;
        break;
    }
  }

  /** Sets the color of a LED
   * @param id the id of the LED
   * @param color the color to set (see type mainColor for info)
   * @param offset the color offset (see type colorOffset for info)
   */
  void setLed (int id, mainColor color, colorOffset offset) {

    uint32_t hsv;

    if (isOn) {

      byte hsvBrightness = getHsvBrightness(stripBrightness) + stripBrightnessOffset;

      // Setting basic color with its offset
      switch (color) {

        case RED:
          hsv = pixels.ColorHSV(0, 255, hsvBrightness);
          break;

        case GREEN:
          hsv = pixels.ColorHSV(21845, 255, hsvBrightness);
          break;

        case TURQUOISE:
          hsv = pixels.ColorHSV(30583, 219, hsvBrightness);
          break;

        case EMERALD:
          hsv = pixels.ColorHSV(26396, 197, hsvBrightness);
          break;

        case PETER_RIVER:
          hsv = pixels.ColorHSV(37137, 194, hsvBrightness);
          break;

        case AMETHYST:
          hsv = pixels.ColorHSV(51336, 130, hsvBrightness);
          break;

        case WET_ASPHALT:
          hsv = pixels.ColorHSV(38229, 113, hsvBrightness);
          break;

        case SUN_FLOWER:
          hsv = pixels.ColorHSV(8738, 239, hsvBrightness);
          break;

        case CARROT:
          hsv = pixels.ColorHSV(5097, 217, hsvBrightness);
          break;

        case ALIZARIN:
          hsv = pixels.ColorHSV(910, 188, hsvBrightness);
          break;

        case CLOUDS:
          hsv = pixels.ColorHSV(34952, 5, hsvBrightness);
          break;

        case CONCRETE:
          hsv = pixels.ColorHSV(33314, 26, hsvBrightness);
          break;

        case BLUE:
        default:
          hsv = pixels.ColorHSV(43690, 255, hsvBrightness);
          break;
      }

      // Applying the color
      pixels.setPixelColor(id, pixels.gamma32(hsv));

    }
    else {

      pixels.setPixelColor(id, pixels.Color(0,0,0));
    }
  };

// ------ END: COLOR CONTROL FUNTIONS ------


// ------ STRIP FUNTIONS ------

  int directionCounterRange = 20000;
  int directionCounter = 0;
  int randomDirectionRange = 300;
  int randomDirection = (rand() % randomDirectionRange) - randomDirectionRange/2;

  void changeDirection () {

    if (directionCounter == directionCounterRange/4 + randomDirection) {

      stripDirection = HALF_DOWN;
    }
    else if (directionCounter == directionCounterRange/2 + randomDirection) {

      stripDirection = FULL_REV;
    }
    else if (directionCounter == 3*directionCounterRange/4 + randomDirection) {

      stripDirection = HALF_UP;
    }
    else if (directionCounter == directionCounterRange + randomDirection) {
    
      stripDirection = HALF_DOWN;
      directionCounter = 0;
    }

    directionCounter++;
  }

  bool isReactingTriggered = false;
  bool isReacting = false;
  byte regenerating = 31;
  byte reactingCycle = 5;

  void musicReact() {

    if (isReactingTriggered) {
      isReacting = true;
      isReactingTriggered = false;
    }

    if (isReacting) {

      switch (reactingCycle) {

        case 5:
          stripDelay = 0;
          stripBrightnessOffset = 10;
          reactingCycle--;
          break;

        case 4:
          stripDelay = 5;
          reactingCycle--;
          break;

        case 3:
          stripDelay = 10;
          reactingCycle--;
          break;

        case 2:
          stripDelay = 20;
          stripBrightnessOffset = 5;
          reactingCycle--;
          break;

        case 1:
          stripDelay = 40;
          stripBrightnessOffset = 2;
          reactingCycle--;
          break;

        default:
          stripDelay = 60;
          reactingCycle = 5;
          stripBrightnessOffset = 0;
          isReacting = false;
          regenerating = 30;
      }
    }
  }

// ------ END: STRIP FUNTIONS ------


// ------ STRIP MOVMENTS ------

  int mvt_5_full_deplacement [8] = {0, NUMPIXELS*1/8, NUMPIXELS*2/8, NUMPIXELS*3/8, NUMPIXELS*4/8, NUMPIXELS*5/8, NUMPIXELS*6/8, NUMPIXELS*7/8};
  int mvt_5_half_deplacement [5] = {0, NUMPIXELS/2*1/5, NUMPIXELS/2*2/5, NUMPIXELS/2*3/5, NUMPIXELS/2*4/5}; 

  void mvt_5 (mainColor color1, mainColor color2, direction dir) {

    byte dash_radius = 3;

    bool isColored;  // Decides if the LED will get color 2
    bool isNormOrUp = (dir == FULL_NORM ||dir == HALF_UP);  // For the direction
    bool isHalf = (dir == HALF_UP || dir == HALF_DOWN);
    
    // FULL
    if (!isHalf) {

      for (int i=0; i<NUMPIXELS; i++) {

        isColored = false;
        for (int j=0; j<8; j++) {

          // Global checking
          if (
            (i >= (mvt_5_full_deplacement[j]-dash_radius) && i <= (mvt_5_full_deplacement[j]+dash_radius)) // "normal" case
            || (i < dash_radius && mvt_5_full_deplacement[j] >= NUMPIXELS - dash_radius + i) // start of strip case
            || (i >= NUMPIXELS - dash_radius && mvt_5_full_deplacement[j] < dash_radius - (NUMPIXELS-i))  // end of strip case
            ) {

            isColored = true;
          }
        }

        if (isColored) {
          setLed(i, color2, 0);
        }
        else {
          setLed(i, color1, 0);
        }
      }

      for (int i=0; i<8; i++) {

        if ((isNormOrUp && mvt_5_full_deplacement[i] != NUMPIXELS-1) || (!isNormOrUp && mvt_5_full_deplacement[i] != 0)) {
          (isNormOrUp) ? mvt_5_full_deplacement[i]++ : mvt_5_full_deplacement[i]--;
        }
        else {
          (isNormOrUp) ? mvt_5_full_deplacement[i] = 0 : mvt_5_full_deplacement[i] = NUMPIXELS-1;
        }
      }

    } // HALF
    else {

      int led_begin, led_end;  // begin and end of the part of the strip we're looking at
      int i;

      for (int k=0; k<2; k++) {

        int mvt_5_half_transposed[5];

        for (int m=0; m<5; m++) {
          mvt_5_half_transposed[m] = NUMPIXELS -1 - mvt_5_half_deplacement[m];
        }

        for (
          (k==0) ? i=0 : i=NUMPIXELS/2;
          (k==0) ? i<NUMPIXELS/2 : i<NUMPIXELS;
          i++
        ) {

          isColored = false;
          for (int j=0; j<5; j++) {

            if (
              (i >= (mvt_5_half_deplacement[j]-dash_radius) && i <= (mvt_5_half_deplacement[j]+dash_radius) && i <= NUMPIXELS/2 -1)
              ||
              (k == 0 && i <= NUMPIXELS/2 -1 && i >= NUMPIXELS/2 - dash_radius + mvt_5_half_deplacement[j])
              ||
              (k == 0 && i <= dash_radius - (NUMPIXELS/2 - mvt_5_half_deplacement[j]))
              ||
              (i >= (mvt_5_half_transposed[j]-dash_radius) && i <= (mvt_5_half_transposed[j]+dash_radius) && i >= NUMPIXELS/2)
              ||
              (k == 1 && i >= NUMPIXELS/2 && i <= NUMPIXELS/2 + dash_radius -1 - (NUMPIXELS - mvt_5_half_transposed[j] -1))
              ||
              (k == 1 && i >= NUMPIXELS - (NUMPIXELS/2 + dash_radius - mvt_5_half_transposed[j]))
              ) {

              isColored = true;
            }
          }

          if (isColored) {
            setLed(i, color2, 0);
          }
          else {
            setLed(i, color1, 0);
          }
        }
      }

      for (int i=0; i<5; i++) {

        if ((isNormOrUp && mvt_5_half_deplacement[i] != NUMPIXELS/2-1) || (!isNormOrUp && mvt_5_half_deplacement[i] != 0)) {
          (isNormOrUp) ? mvt_5_half_deplacement[i]++ : mvt_5_half_deplacement[i]--;
        }
        else {
          (isNormOrUp) ? mvt_5_half_deplacement[i] = 0 : mvt_5_half_deplacement[i] = NUMPIXELS/2-1;
        }
      }
    }
  }

// ------ END: STRIP MOVMENTS ------

void setup() {

  //Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(BTN_PIN, INPUT);

}

void loop() {

  /*int pot_value = analogRead(KNOB_PIN);
  knobChangeBrightness(pot_value);

  Serial.println(stripBrightness);*/

  /*if (digitalRead(BTN_PIN) == HIGH) {
    switch (stripDirection) {
      case FULL_NORM:
        stripDirection = FULL_REV;
        break;
      case FULL_REV:
        stripDirection = HALF_DOWN;
        break;
      case HALF_DOWN:
        stripDirection = HALF_UP;
        break;
      default:
        stripDirection = FULL_NORM;
    }
    delay(200);
    //Serial.println("Btn on");
  }
  else {
    //Serial.println("Btn off");
  }*/

  if (reactingCycle == 5 && digitalRead(BTN_PIN) == HIGH) {

    isReactingTriggered = true;
    //stripColor1 = (mainColor)(rand() % COLORS_COUNT);
    //stripColor2 = (mainColor)(rand() % COLORS_COUNT);
  }

  musicReact();

  changeDirection ();
  mvt_5 (stripColor1, stripColor2, stripDirection);

  pixels.show();
  delay(stripDelay);
  //delay(1000);
}
