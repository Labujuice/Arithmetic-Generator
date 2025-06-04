#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define Number_Limit 20

#define TEST_PLUS 1
#define TEST_MINUS 1
#define TEST_MULTIPLY 1
#define TEST_DIVIDE 1

#define BUTTON_PIN 2  // D2

LiquidCrystal_I2C lcd(0x27, 16, 2);

void wait_button_press() {
  bool buttonPressed = false;

  while (true) {
    if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
      buttonPressed = true;
      break;
    }
    if (digitalRead(BUTTON_PIN) == HIGH) {
      buttonPressed = false;
    }
  }
}

void generate_Arithmetic(const char* op, uint32_t a, uint32_t b, uint32_t answer, const char* type) {
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

void generate_Plus()      { uint32_t a = random(Number_Limit), b = random(Number_Limit); generate_Arithmetic("+", a, b, a + b, "Addition"); }
void generate_Minus()     { uint32_t a = random(Number_Limit), b = random(a+1); generate_Arithmetic("-", a, b, a - b, "Subtraction"); }
void generate_Multiply()  { uint32_t a = random(10), b = random(10); generate_Arithmetic("x", a, b, a * b, "Multiplication"); }
void generate_Division()  { uint32_t a = random(10), b = random(10); generate_Arithmetic("/", a*b, b, a, "Division"); }

void setup() {
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(0));

  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // 建立可用測試項目的陣列
  uint8_t testOptions[4] = {TEST_PLUS, TEST_MINUS, TEST_MULTIPLY, TEST_DIVIDE};
  uint8_t availableTests[4];
  uint8_t count = 0;

  // 收集啟用的測試項目
  for (uint8_t i = 0; i < 4; i++) {
    if (testOptions[i]) {
      availableTests[count++] = i;
    }
  }

  if (count > 0) {
    // 隨機選擇一種啟用的測試項目
    uint8_t selectedTest = availableTests[random(count)];

    switch (selectedTest) {
      case 0:
        generate_Plus();
        break;
      case 1:
        generate_Minus();
        break;
      case 2:
        generate_Multiply();
        break;
      case 3:
        generate_Division();
        break;
    }
  } else {
    // 如果沒有測試被啟用
    lcd.setCursor(2, 0);
    lcd.print("No test enabled");
  }
}

void loop() {}