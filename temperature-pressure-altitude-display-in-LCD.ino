#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>

#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)

Adafruit_BMP280 bmp;

int led1 = 4;

LiquidCrystal_I2C lcd(0x27, 20, 4);

byte degree_symbol[7] = {
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte cloud[7] = {
  0b01100,
  0b11110,
  0b11111,
  0b01110,
  0b00000,
  0b00000,
  0b00000
};

byte cloud2[7] = {
  0b11010,
  0b11111,
  0b11111,
  0b01110,
  0b00000,
  0b00000,
  0b00000
};

byte raincloud[7] = {
  0b01100,
  0b11110,
  0b11111,
  0b01110,
  0b01000,
  0b00010,
  0b01000
};

byte raincloud2[7] = {
  0b11010,
  0b11111,
  0b11111,
  0b01110,
  0b10001,
  0b00100,
  0b10001,
};

byte sun[7] = {
  0b00000,
  0b10101,
  0b01110,
  0b11111,
  0b01110,
  0b10101,
  0b00000
};

byte sun_l_half[7] = {
    0b10001,
    0b01011,
    0b00111,
    0b11111,
    0b00111,
    0b01011,
    0b10001
};

byte sun_r_half[7] = {
    0b10001,
    0b11010,
    0b11100,
    0b11111,
    0b11100,
    0b11010,
    0b10001
};



void setup() {
  Serial.begin(9600);
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  pinMode(led1, OUTPUT);
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.clear();
  

  lcd.createChar(0, degree_symbol);
	// create a new character
  lcd.createChar(1, cloud);
	// create a new character
  lcd.createChar(2, cloud2);
	// create a new character
  lcd.createChar(3, raincloud);
	// create a new character
  lcd.createChar(4, raincloud2);

  lcd.createChar(5, sun);

  lcd.createChar(6, sun_l_half);

  lcd.createChar(7, sun_r_half);
  
  lcd.clear();
}

void loop() {
  
  float temp = bmp.readTemperature();
  int pres = bmp.readPressure();
  float pressure = pres/100;
  float alt = bmp.readAltitude(1013.25);
  float tempc = temp + 273.15;
  float fact = (1-((0.0065*alt)/(tempc+0.0065*alt)));
  float slp = pressure*pow(fact, -5.257)+1;

  float zal = 144 - 0.13*slp;
  int z = zal;

  lcd.setCursor(4, 1);
  lcd.print("Project AIR!");
  lcd.setCursor(6, 2);
  lcd.print("Group 09");
  delay(5000);
  lcd.clear(); 

  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print(temp);
  lcd.setCursor(6, 1);
  lcd.write(1);
  lcd.createChar(1, degree_symbol);
  lcd.setCursor(7, 1);
  lcd.print("C");
  lcd.setCursor(0, 2);
  lcd.print("Pressure:");
  lcd.setCursor(0, 3);
  lcd.print(pressure);
  lcd.setCursor(7,3);
  lcd.print(" hPa");
  digitalWrite(led1, HIGH);
  delay(7000);
  digitalWrite(led1, LOW);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Z Prediction:");

  if (z == 10){
    lcd.setCursor(9,1);
    lcd.write(byte(6));
    lcd.setCursor(10, 1);
    lcd.write(byte(7));
    lcd.setCursor(9,2);
    lcd.print("10");
    lcd.setCursor(0,3);
    lcd.print("Settled fine");
  }
    else if (z == 11){
    lcd.setCursor(9,1);
    lcd.write(byte(6));
    lcd.setCursor(10,1);
    lcd.write(byte(7));
    lcd.setCursor(9,2);
    lcd.print("11");
    lcd.setCursor(0,3);
    lcd.print("Fine Weather");
  }
    else if (z == 12){
    lcd.setCursor(9,1);
    lcd.write(byte(5));
    lcd.setCursor(10,1);
    lcd.write(byte(2));
    lcd.setCursor(9,2);
    lcd.print("12");
    lcd.setCursor(0,3);
    lcd.print("Fine possibly shower");
  }
    else if (z == 13){
    lcd.setCursor(9,1);
    lcd.write(byte(5));
    lcd.setCursor(10,1);
    lcd.write(byte(2));
    lcd.setCursor(9,2);
    lcd.print("13");
    lcd.setCursor(0,3);
    lcd.print("Fair/showery later");
  }
    else if (z == 14){
    lcd.setCursor(9,1);
    lcd.write(byte(5));
    lcd.setCursor(10,1);
    lcd.write(byte(4));
    lcd.setCursor(9,2);
    lcd.print("14");
    lcd.setCursor(0,3);
    lcd.print("Shwry bright intrvls");
  }
    else if (z == 15){
    lcd.setCursor(9,1);
    lcd.write(byte(5));
    lcd.setCursor(10,1);
    lcd.write(byte(4));
    lcd.setCursor(9,2);
    lcd.print("15");
    lcd.setCursor(0,3);
    lcd.print("Changable, some rain");
  }
    else if (z == 16){
    lcd.setCursor(9,1);
    lcd.write(byte(3));
    lcd.setCursor(10,1);
    lcd.write(byte(4));
    lcd.setCursor(9,2);
    lcd.print("16");
    lcd.setCursor(0,3);
    lcd.print("Unsettled, may rain");
  }
    else if (z == 17){
    lcd.setCursor(9,1);
    lcd.write(byte(3));
    lcd.setCursor(10,1);
    lcd.write(byte(4));
    lcd.setCursor(9,2);
    lcd.print("17");
    lcd.setCursor(0,3);
    lcd.print("Very Unsettled, Rain");
  }
    else if (z == 18){
    lcd.setCursor(9,1);
    lcd.write(byte(3));
    lcd.setCursor(10,1);
    lcd.write(byte(4));
    lcd.setCursor(9,2);
    lcd.print("18");
    lcd.setCursor(0,3);
    lcd.print("Very Unsettled, Rain");
  }
    else if (z == 19){
    lcd.setCursor(9,1);
    lcd.write(byte(3));
    lcd.setCursor(10,1);
    lcd.write(byte(4));
    lcd.setCursor(9,2);
    lcd.print("19");
    lcd.setCursor(0,3);
    lcd.print("Stormy, much rain");
  }
  
  delay(8000);
  lcd.clear();

  int sensorValue = analogRead(A0);

  lcd.setCursor(0,0);
  lcd.print("Gas Concentartion: ");
  lcd.setCursor(0,1);
  lcd.print(sensorValue);
  lcd.setCursor(7,2);
  if (sensorValue > 869)
  {
  lcd.print("UNSAFE");
  }
  else{
  lcd.print("SAFE");        
  }  

  delay(5000);
  lcd.clear();

}
