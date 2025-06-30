#include <Modulino.h>

// Modulino modules
ModulinoThermo thermo;
ModulinoButtons buttons;
ModulinoBuzzer buzzer;
ModulinoPixels pixels;

// Button IDs (based on physical labels)
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
  pixels.begin();

  pixels.clear();   // Clear on startup
  pixels.show();

  Serial.begin(9600);
  while (!Serial);  // Wait for Serial to open (optional)
  for (int i = 0; i < 30; i++) Serial.println();

  Serial.println(F("Starting Temperature Sensor..."));
}

void loop() {
  float tempC = thermo.getTemperature();  // Always base in °C
  if (isnan(tempC)) {
  Serial.println("ERROR: tempC is NaN!");
}
  float displayTemp = tempC;
  String label = "°C";

  buttons.update();  // MUST be before reading!

  // Handle button-based unit switching
  if (buttons.isPressed(BUTTON_A)) {
    Serial.println("SWITCHING TO °C");
    tempUnit = UNIT_C;
    buzzer.tone(880, 100);
    delay(200);
  } else if (buttons.isPressed(BUTTON_B)) {
    Serial.println("SWITCHING TO °F");
    tempUnit = UNIT_F;
    buzzer.tone(1000, 100);
    delay(200);
  } else if (buttons.isPressed(BUTTON_C)) {
    Serial.println("SWITCHING TO K");
    tempUnit = UNIT_K;
    buzzer.tone(1200, 100);
    delay(200);
  }

  // Convert to selected unit
  switch (tempUnit) {
    case UNIT_C:
      displayTemp = tempC;
      label = "°C";
      break;
    case UNIT_F:
      displayTemp = tempC * 9.0 / 5.0 + 32.0;
      label = "°F";
      break;
    case UNIT_K:
      displayTemp = tempC + 273.15;
      label = "K";
      break;
  }

  // Output current temperature
  Serial.print("Temp: ");
  Serial.print(displayTemp, 2);
  Serial.println(" " + label);

  // Alert conditions
  if (tempC >= tempThreshold) {
    Serial.println("TOO HOT: TEMPERATURE IS OR HIGHER THAN 30.00 °C OR 86 °F OR 303.15 K");
    buzzer.tone(1500, 1000);
    for (int i = 0; i < 8; i++) {
      pixels.set(i, 255, 0, 0, 10);  // 🔴 Red
    }
    pixels.show();
  } else if (tempC <= 0.00) {
    Serial.println("TOO COLD: TEMPERATURE IS OR LOWER THAN 0.00 °C OR 32 °F OR 273.15 K");
    buzzer.tone(1500, 1000);
    for (int i = 0; i < 8; i++) {
      pixels.set(i, 0, 0, 255, 10);  // 🔵 Blue
    }
    pixels.show();
  } else {
    buzzer.noTone();
    pixels.clear();
    pixels.show();
  }

  delay(1500);  // Polling delay
}
