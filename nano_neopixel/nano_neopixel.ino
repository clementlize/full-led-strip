/** --- FULL LED STRIP ---
 * This program generates nice patterns, animations and controls for your Neopixel LED Strip
 * Created by Clément Lizé
 * See: https://clementlize.com
 * Github repository: https://github.com/clementlize/full-led-strip
 * v0.1 - Nov 2020
 */

// ------ MSGEQ7 ------

  #define EQ_OUT    0   // Read from multiplexer using analog input 0
  #define EQ_STROBE 2   // Strobe is attached to digital pin 2
  #define EQ_RESET  3   // Reset is attached to digital pin 3
  int spectrumValue[7]; // To hold msgeq7 band values
  int spectrumBassValue; // To hold msgeq7 band values

// ------ END: MSGEQ7 ------


// ------ SENSORS ------

  #define POT_PIN 2     // Potentiometer is attached to analog 2
  #define LED_PIN 6     // Led is attached to digital 6
  #define POT_MAX_ANALOG 1023

// ------ END: SENSORS ------


// ------ GLOBAL VARIABLES ------

  bool isBassOn = false;    // Main boolean which indicates if a bass signal is detected
  int thresold = 0;         // The thresold for the detection. Will be set by the potentiometer value
  int MIN_THRESOLD = 50; 
  int MAX_THRESOLD = 400;
  int pot_value = 0;        // Value returned by the potentiometer

// ------ END: GLOBAL VARIABLES ------

void setup(){
  
  Serial.begin(9600);

  // MSGEQ7
  pinMode(EQ_OUT, INPUT);
  pinMode(EQ_STROBE, OUTPUT);
  pinMode(EQ_RESET, OUTPUT);

  // LED
  pinMode(LED_PIN, OUTPUT);
  
  //analogReference(DEFAULT);
  
  digitalWrite(EQ_RESET, LOW);
  digitalWrite(EQ_STROBE, HIGH);
}

void loop(){

  //MSGEQ7
  digitalWrite(EQ_RESET, HIGH);
  digitalWrite(EQ_RESET, LOW);

  pot_value = analogRead(POT_PIN);
  thresold = (pot_value / (float)POT_MAX_ANALOG) * (MAX_THRESOLD-MIN_THRESOLD) + MIN_THRESOLD;

  digitalWrite(EQ_STROBE, LOW);
  delayMicroseconds(30); // to allow the output to settle
  spectrumBassValue = analogRead(EQ_OUT);

  if(spectrumBassValue > thresold){
      
    digitalWrite(LED_PIN, HIGH);
    isBassOn = true;
    delay(70);
    
  } else {
    
    digitalWrite(LED_PIN, LOW);
    isBassOn = false;
  }
  
  digitalWrite(EQ_STROBE, HIGH);

  for (int i=0; i<6; i++) {
    digitalWrite(EQ_STROBE, LOW);
    delayMicroseconds(30); // to allow the output to settle
    digitalWrite(EQ_STROBE, HIGH);
  }

  /*for (int i = 0; i < 7; i++){
    
    digitalWrite(EQ_STROBE, LOW);
    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[i] = analogRead(EQ_OUT);

    if(i == 0 && spectrumValue[i] > thresold){
      
      digitalWrite(LED_PIN, HIGH);
      isBassOn = true;
      
    } else if (i==0) {
      
      digitalWrite(LED_PIN, LOW);
      isBassOn = false;
    }
  
    digitalWrite(EQ_STROBE, HIGH);
  }*/

  Serial.print("Pot: ");
  Serial.print(pot_value);
  Serial.print("   |   Thresold: ");
  Serial.print(thresold);
  Serial.print("   |   Bass: ");
  Serial.println(spectrumValue[0]);
}
