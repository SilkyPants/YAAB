int led = 13;                                        // the pin where you will put the LED
int sensorpin = A3;                           // the analog pin where you put your sensorLED
int ledPin = 5;

const long interval = 500;
int ledState = LOW;             // ledState used to set the LED

void setup() {
  pinMode(led, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  Serial.begin(9600);   
}


void loop() { 

    int test = 1024;
    int sens = 0;
                                             
    for(int i = 0; i < 20; i++) {// remember the lowest value out of many readings
        sens = analogRead(sensorpin);
        delay(1);                                   // the sensor needs a delay here to catch its breath
        if (sens < test) {
            test = sens;
        }
    }
    
    if ( test == 0) { // Beam is broken
        digitalWrite(led, HIGH);
    }
    else {
        digitalWrite(led, LOW);
    }
    
    Serial.print(test);Serial.print("  ");Serial.println(sens); //just for debugging
}
