#include <Modulino.h>

ModulinoThermo thermo;
ModulinoButtons buttons;
ModulinoBuzzer buzzer;

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_C 2

const float tempThreshold = 30.0;

enum TempUnit { UNIT_C, UNIT_F, UNIT_K };
TempUnit tempUnit = UNIT_C;

void setup() {
  Modulino.begin();
  thermo.begin();
  buttons.begin();
  buzzer.begin();
  Serial.begin(9600);
  for (int i = 0; i < 30; i++) {
  Serial.println();
  }

  Serial.print("Starting Temperature Sensor...\n");
}

void loop() {
  float tempC = thermo.getTemperature();  // Read raw °C
  float displayTemp = tempC;
  String label = "°C";
  buttons.update();  // 🔑 This line is essential!

  // Button handling
  if (buttons.isPressed(BUTTON_A)) {
    tempUnit = UNIT_C;
    buzzer.tone(880, 100);
    delay(200);
  } else if (buttons.isPressed(BUTTON_B)) {
    tempUnit = UNIT_F;
    buzzer.tone(1000, 100);
    delay(200);
  } else if (buttons.isPressed(BUTTON_C)) {
    tempUnit = UNIT_K;
    buzzer.tone(1200, 100);
    delay(200);
  }

  switch (tempUnit) {
  case UNIT_C:
    displayTemp = tempC;
    label = "°C";
    break;
  case UNIT_F:
    displayTemp = tempC * 9.0 / 5.0 + 32;
    label = "°F";
    break;
  case UNIT_K:
    displayTemp = tempC + 273.15;
    label = "K";
    break;
}


  // Output result
  Serial.print("Temp: ");
  Serial.print(displayTemp, 2);
  Serial.println(" " + label);

  if (tempC >= tempThreshold) {
  Serial.print("TOO HOT: TEMPERATURE IS OR HIGHER THAN 30.00 °C\n");
  buzzer.tone(1500, 1000);
} 
 else if (tempC =< 0.00)
 {
  Serial.print("TOO COLD: TEMPERATURE IS OR LOWER THAN 0.00 °C\n");
  buzzer.tone(1500, 1000);
 }
else {
  buzzer.noTone();
}

  delay(1500);
}
