#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define Number_Limit 20

#define TEST_PLUS 1
#define TEST_MINUS 1
#define TEST_MULTIPLY 1
#define TEST_DIVIDE 1

#define BUTTON_PIN 2  // D2

LiquidCrystal_I2C lcd(0x27, 16, 2);

typedef void (*TestFunc)();

TestFunc tests[4];
uint8_t count = 0;

void wait_button_press() {
  // Debunce
  while (digitalRead(BUTTON_PIN) == LOW);
  delay(100);
  while (digitalRead(BUTTON_PIN) == HIGH);
  delay(100);
}

void generate_Arithmetic(const char* op, uint32_t a, uint32_t b,
                         uint32_t answer, const char* type) {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print(type);
  lcd.setCursor(2, 1);
  lcd.print(String(a) + op + String(b) + "=?");
  wait_button_press();
  lcd.setCursor(2, 1);
  lcd.print(String(a) + op + String(b) + "=" + String(answer));
  wait_button_press();
}

void generate_Plus() {
  uint32_t a = random(Number_Limit), b = random(Number_Limit);
  generate_Arithmetic("+", a, b, a + b, "Addition");
}
void generate_Minus() {
  uint32_t a = random(Number_Limit), b = random(a + 1);
  generate_Arithmetic("-", a, b, a - b, "Subtraction");
}
void generate_Multiply() {
  uint32_t a = random(10), b = random(10);
  generate_Arithmetic("x", a, b, a * b, "Multiplication");
}
void generate_Division() {
  uint32_t a = random(1,10), b = random(1,10);
  generate_Arithmetic("/", a * b, b, a, "Division");
}

void setup() {
  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Initialize random seed
  randomSeed(analogRead(0));

  // Initialize button pin
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Add tests based on defined macros
  if (TEST_PLUS) tests[count++] = generate_Plus;
  if (TEST_MINUS) tests[count++] = generate_Minus;
  if (TEST_MULTIPLY) tests[count++] = generate_Multiply;
  if (TEST_DIVIDE) tests[count++] = generate_Division;
}

void loop() {
  // Generate a random arithmetic test
  if (count > 0) {
    tests[random(count)]();
  } else {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("No test enabled");
  }
}