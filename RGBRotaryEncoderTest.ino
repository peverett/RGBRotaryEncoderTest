// Arduino Nano
//
// Sparkfun Rotary Encoder Breakout - Illuminated (RG/RGB) 3.99 GBP from Ebay
// https://www.proto-pic.co.uk/rotary-encoder-breakout-illuminated-rgrgb.html
// Rotary Encoder - Iluminated (RGB) - 4.59 from Ebay
//
// Encoder A - connected to A0
// Encoder B - connected to A1
// Encoder C - connected to GND
//
// Encoder R - connected to D11
// Encoder G - connected to D10
// Encoder B - connected to D09
//
// Encoder SW - connected to A2, with 10Kr resister to ground (pulldown)
// Encoder + - Connected to +5V


#define DEBUG_PRINT
#ifdef DEBUG_PRINT
#define PRINT_INIT(X) Serial.begin((X))
#define PRINT(X) Serial.print(X)
#define PRINTLN(X) Serial.println(X)
#else 
#define PRINT_INIT(X)
#define PRINT(X)
#define PRINTLN(X)
#endif


const byte pin_a = 14;   // A0
const byte pin_b = 15;   // A1
const byte pin_sw = 16;  // A2

unsigned long current_time;
unsigned long loop_time;

byte enc_a;
byte enc_b;
byte prev_enc_a=0;

enum SelectLed { red, green, blue, last };
SelectLed selected = red;
byte ledVal[3] = { 0, 0, 0 };         // All start at 0, which is full ON.
const byte ledPin[3] = { 11, 10, 9 }; // D11, D10, D9

void setup() {
  PRINT_INIT( 9600 );
  PRINTLN("Setup started.");

  pinMode(pin_a, INPUT_PULLUP);
  pinMode(pin_b, INPUT_PULLUP);
  pinMode(pin_sw, INPUT);

  pinMode(ledPin[red], OUTPUT);
  pinMode(ledPin[green], OUTPUT);
  pinMode(ledPin[blue], OUTPUT);
  
  digitalWrite(ledPin[red], ledVal[red]);
  digitalWrite(ledPin[green], ledVal[green]);
  digitalWrite(ledPin[blue], ledVal[blue]);

  PRINTLN("Setup completed.");
}

int debounceRead(const byte pin) {
  int res = LOW;
  if (digitalRead(pin)) {
    res = HIGH;
    while(digitalRead(pin));
  }
  return res;
}


void loop() {
  current_time = millis();

  if (current_time >= (loop_time+5)) {
    enc_a = digitalRead(pin_a);
    enc_b = digitalRead(pin_b);

    if (debounceRead(pin_sw)) {
      switch (selected) {
        case red:
          selected = green;
          break;
        case green:
          selected = blue;
          break;
        case blue:
          selected = red;
          break;
      }
    }
  
    if (!enc_a && prev_enc_a) {
      if (enc_b) {
        ledVal[selected] = (ledVal[selected]==255) ? 0 : ledVal[selected] + 15; 
      } 
      else {
        ledVal[selected] = (ledVal[selected]==0) ? 255 : ledVal[selected] - 15;
      }
      
      PRINT("R:");
      PRINT(ledVal[red]);
      PRINT(", G:");
      PRINT(ledVal[green]);
      PRINT(", B:");
      PRINTLN(ledVal[blue]);

      analogWrite(ledPin[selected], ledVal[selected]);
    }
    prev_enc_a = enc_a;
    loop_time = current_time;
  }
}
