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
  #define BTN_PIN 8

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

  // The colors available for the LED strip.
  typedef enum {
    RED, GREEN, BLUE
  } mainColor;

  // The directions available for the movments
  typedef enum {
    FULL_NORM, FULL_REV, HALF_DOWN, HALF_UP
  } direction;

// ------ END: TYPES ------


// ------ GLOBAL VARIABLES ------

  bool isOn = true;  // LED strip global on-off variable
  ledBrightness stripBrightness = 2;  // The main brightness of the strip
  direction stripDirection = HALF_UP;  // The direction for the movments

// ------ END: GLOBAL VARIABLES ------


// ------ COLOR CONTROL FUNTIONS ------

  void knobChangeBrightness (int value) {

    if (value >= 0 && value < 138) { stripBrightness = 1; }
    else if (value >= 138 && value < 276) { stripBrightness = 2; }
    else if (value >= 276 && value < 414) { stripBrightness = 3; }
    else if (value >= 414 && value < 552) { stripBrightness = 4; }
    else { stripBrightness = 5; }
  }

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

      // Setting basic color with its offset
      switch (color) {

        case RED:
          hsv = pixels.ColorHSV(0, 255, getHsvBrightness(stripBrightness));
          break;

        case GREEN:
          hsv = pixels.ColorHSV(21845, 255, getHsvBrightness(stripBrightness));
          break;

        case BLUE:   
        default:
          hsv = pixels.ColorHSV(43690, 255, getHsvBrightness(stripBrightness));
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

  int mvt_5_full_deplacement [5] = {0, NUMPIXELS*1/5, NUMPIXELS*2/5, NUMPIXELS*3/5, NUMPIXELS*4/5};
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
        for (int j=0; j<5; j++) {

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

      for (int i=0; i<5; i++) {

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
            /*if (
              (k == 0 && 
                (i >= (mvt_5_half_deplacement[j]-dash_radius) && i <= (mvt_5_half_deplacement[j]+dash_radius))
              )
              ||
              (k == 1 && 
                (i >= (mvt_5_half_deplacement[j]-dash_radius) && i <= (mvt_5_half_deplacement[j]+dash_radius))
              )
            ) {*/

            if (
              (i >= (mvt_5_half_deplacement[j]-dash_radius) && i <= (mvt_5_half_deplacement[j]+dash_radius))
              ||
              (i >= (mvt_5_half_transposed[j]-dash_radius) && i <= (mvt_5_half_transposed[j]+dash_radius))
              ||
              (k == 0 && i < dash_radius && mvt_5_half_deplacement[j] >= NUMPIXELS/2 - dash_radius + i) // start of strip case
              ||
              (k == 1 && i >= NUMPIXELS/2 && i < NUMPIXELS/2 + dash_radius && mvt_5_half_transposed[j] >= NUMPIXELS - dash_radius + i) // start of strip case
              ||
              (k == 0 && i >= NUMPIXELS/2 - dash_radius && mvt_5_half_deplacement[j] < dash_radius - (NUMPIXELS/2-i))  // end of strip case
              ||
              (k == 1 && i >= NUMPIXELS - dash_radius && mvt_5_half_deplacement[j] >= NUMPIXELS/2 && mvt_5_half_deplacement[j] < NUMPIXELS/2 + dash_radius - (NUMPIXELS/2-i))  // end of strip case
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

// ------ END: STRIP FUNTIONS ------

void setup() {

  //Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
  //pinMode(BTN_PIN, INPUT);

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

  changeDirection ();
  mvt_5 (RED, BLUE, stripDirection);

  pixels.show();
  delay(50);
  //delay(1000);
}
