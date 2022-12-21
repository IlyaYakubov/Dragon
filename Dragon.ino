/*
   Данный скетч предназначен для Arduino Uno
   Дракончик
   @author Ilya
*/

#include <Wire.h>                   // библиотека для протокола IIC
#include <LiquidCrystal_I2C.h>      // подключаем библиотеку LCD IIC
LiquidCrystal_I2C LCD(0x27, 20, 2); // присваиваем имя lcd для дисплея

const byte button = 2;  // константа кнопки
const byte sound = 10;  // константа динамика

int level = 1;          // переменная для отсчета уровня
int pauseNumber = 400;  // переменная для задержки
byte jump = 0;          // переменная для времени прыжка

// Создаем массивы дракончика, дерева, камня и птицы
byte dracon[8] = {
  0b01110, 0b11011, 0b11111, 0b11100, 0b11111, 0b01100, 0b10010, 0b11011
};
byte derevo[8] = {
  0b00000, 0b00000, 0b00000, 0b11011, 0b11011, 0b11011, 0b01100, 0b01100
};
byte kamen[8] = {
  0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b01110, 0b11111
};
byte ptica[8] = {
  0b00100, 0b00101, 0b01111, 0b11111, 0b10100, 0b00100, 0b00000, 0b00000
};

void setup() {
  pinMode (sound, OUTPUT);          // подключаем пьезодинамик
  pinMode (button, INPUT_PULLUP);   // подключаем кнопку

  LCD.init();       // инициализация LCD дисплея
  LCD.backlight();  // включение подсветки дисплея

  // Создаем символы дракончика, дерева, камня и птицы
  LCD.createChar(0, dracon);
  LCD.createChar(1, derevo);
  LCD.createChar(2, kamen);
  LCD.createChar(3, ptica);

  // Начинаем игру: выводим надпись GO!
  LCD.setCursor(7, 0);
  LCD.print("GO!");
  delay(400);
  tone(sound, 600);
  delay(100);
  noTone(sound);
  LCD.clear();
}

void loop() {
  // Первоначальное положение дракончика и препятствия
  byte dragonPosition = 1;
  byte x = 15;
  byte y = 1;

  // Выбираем препятствие, которое появится, рандомно
  byte i = random (1, 4);
  if (i == 3) {
    y = 0;
  } else {
    y = 1;
  }

  while (x > 0) {
    // Очищаем экран и выводим номер уровня
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print(level);

    // Считываем данные с кнопки и учитываем количество циклов в прыжке
    // Если дракончик находится в прыжке долго, возвращаем его вниз
    if (digitalRead(button) == LOW) {
      dragonPosition = 0;
    }
    if (digitalRead(button) == HIGH) {
      dragonPosition = 1;
    }

    if (jump > 3) {
      dragonPosition = 1;
    }

    // Выводим дракончика в нужной строке
    LCD.setCursor(4, dragonPosition);
    LCD.print(char(0));

    // Выводим препятствие
    LCD.setCursor(x, y);
    tone(sound, 50);
    LCD.print(char(i));
    noTone(sound);

    // Если дракончик наткнулся на препятствие выводим надпись GAME OVER!
    if (x == 4 && y == dragonPosition) {
      delay(400);
      tone(sound, 50);
      delay(100);
      noTone(sound);
      delay(100);
      tone(sound, 20);
      delay(300);
      noTone(sound);
      LCD.clear();
      delay(200);
      LCD.setCursor(3, 0);
      LCD.print("GAME OVER!");
      delay(600);
      LCD.clear();
      delay(400);
      LCD.setCursor(3, 0);
      LCD.print("GAME OVER!");
      delay(600);
      LCD.clear();
      LCD.setCursor(3, 1);
      LCD.print("LEVEL: ");
      LCD.print(level);
      delay(400);
      LCD.setCursor(3, 0);
      LCD.print("GAME OVER!");
      delay(7000);
      LCD.clear();

      // Начинаем игру заново, обнулив уровень игры
      LCD.setCursor(7, 0);
      LCD.print("GO!");
      delay(400);
      tone(sound, 600);
      delay(100);
      noTone(sound);
      LCD.clear();

      level = 0;
      pauseNumber = 400;
      jump = 0;
      y = 1;
      x = 0;
      break;
    }

    // Если дракончик прыгнул, издаем звук
    if (dragonPosition == 0) {
      tone(sound, 200);
      delay(100);
      noTone(sound);
    } else {
      delay(100);
    }

    // Если дракончик не столкнулся, то меняем положение препятствия
    //  начинаем считать сколько циклов дракончик находится в прыжке
    delay(pauseNumber);
    x = x - 1;
    jump = jump + 1;
    if (jump > 4) {
      jump = 0;
    }
  }

  // Переходим на следующий уровень и сокращаем время задержки
  tone(sound, 800);
  delay(20);
  level = level + 1;
  pauseNumber = pauseNumber - 20;

  if (pauseNumber < 0) {
    pauseNumber = 0;
  }
}
