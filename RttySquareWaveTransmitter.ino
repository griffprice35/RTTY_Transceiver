void setup() {
  pinMode(9, OUTPUT);  // Pin 9 used due to being able to output a PWM signal
  Serial.begin(9600);  // 9600 baudrate for printing to the serial terminal for testing

  // Using Timer1 with fast PWM
  TCCR1A = (1 << COM1A1) | (1 << WGM11);               // Sets the correct bits in TCCR1A to create a PWM signal on pin 9 and use fast PWM with ICR1 as the TOP value
  TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10);  // Sets the correct bits in TCCR1B to have no prescaling and use fast PWM with ICR1 as the TOP value

  setFrequency(2295);  // Initial state is at stop bit so receiver can detect the high-to-low transition
}

void loop() {
  String str = "Hello World!";
  String binaryStr = "";
  for (int x = 0; x < str.length(); x++) {
    char currentChar = str.charAt(x);
    for (int y = 7; y >= 0; y--) {
      if (currentChar & (1 << y)) {
        binaryStr += '1';
      } else {
        binaryStr += '0';
      }
    }
  }

  int charIndex = 0;
  for (int x = 0; x < str.length(); x++) {
    Serial.print('0');
    setFrequency(2125);  // Start bit
    delay(22);          // 22 ms delay
    for (int y = 0; y < 8; y++) {
      if (binaryStr.charAt(charIndex) == '0') {
        Serial.print('0');
        setFrequency(2125);  // Outputs a space frequency (data bit = 0)
      } else {
        Serial.print('1');
        setFrequency(2295);  // Outputs a mark frequency (data bit = 1)
      }
      delay(22);        // 45.45 ms delay between bits
      charIndex++;
    }
    Serial.print('1');
    setFrequency(2295);  // Stop Bit
    delay(100);         // 0.1 second delay so receiver doesn't fall behind in transmission
    Serial.println("");
  }
}

void setFrequency(uint16_t frequency) {
  // Calculates the TOP value for either frequency
  uint16_t TOP = (16000000 / frequency) - 1;  // 16000000 because of the 16MHz clock speed

  // Sets the correct values into the input and output capture registers
  ICR1 = TOP;
  OCR1A = TOP / 2;  // divided by 2 to get 50% duty cycle
}
