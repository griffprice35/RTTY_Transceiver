String binaryString = "";

void setup() {
  pinMode(2, INPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(2) == LOW) {                     // Start bit is received
    delay(33);                                     // Delays into the middle of the next bit
    for (int x = 0; x <= 7; x++) {
      if (digitalRead(2) == HIGH) {                // 1 is detected
        binaryString += '1';                       // Adds a 1 to the data packet
        delay(22);
      } 
      else if (digitalRead(2) == LOW) {            // 0 is detected
        binaryString += '0';                       // Adds a 0 to the data packet
        delay(22);  
      }
    }

    delay(11);                                                     // Delays into the middle of the stop bit

    if (binaryString.length() == 8 && (digitalRead(2) == HIGH)) {  // Checks for a complete byte (data packet)
      char asciiChar = 0;
      for (int x = 0; x <= 7; x++) {
        if (binaryString.charAt(x) == '1') {
          asciiChar |= (1 << (7 - x));                              // Sets the corresponding bit
        }
      }
      Serial.print(asciiChar);                                      // Prints the final character
      binaryString = "";
    }
  }
}