//Niculae Alexia



#include "dht.h" 
#include <U8g2lib.h> 

dht DHT;

char umiditate_sol [5];
char umiditate_aer [5];
char temperatura [5];

#define dht_apin A1 

int uscat = 495;
int umed = 225;

// Constructor
U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /*CS=*/ 10, /*RESET=*/ 8);


void setup() {
  // Se lasa timp pemtru system boot
  delay(500);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  // LCD
  u8g2.begin(); // Boot LCD
  u8g2.enableUTF8Print(); // Encoding
  u8g2.setFont(u8g2_font_6x10_mf); //Lista cu fonturi poate fi gasita aici: https://github.com/olikraus/u8glib/wiki/fontsize (cel folosit aici este de 7 pixeli)
  u8g2.setColorIndex(1); 
}


void loop() {
  u8g2.firstPage();
  do{
    draw();
  } while(u8g2.nextPage() );
}

void draw() {

  read_capacity();
  read_hum_temp();

  u8g2.drawStr(1, 13, "Temperatura: ");
  u8g2.drawStr(78, 13, temperatura);
  u8g2.drawStr(105, 13, "C"); 
  u8g2.drawStr(1, 33, "Umiditate aer: ");
  u8g2.drawStr(90, 33, umiditate_aer);
  u8g2.drawStr(115, 33, "%");
  u8g2.drawStr(1, 53, "Umiditate sol: ");
  u8g2.drawStr(90, 53, umiditate_sol);
  u8g2.drawStr(120, 53, "%");
}


void read_hum_temp() {

  
  DHT.read11(dht_apin);
  int air_humidity = DHT.humidity;
  int temperature = DHT.temperature;
  dtostrf(air_humidity, 3, 1, umiditate_aer); 
  dtostrf(temperature, 3, 1, temperatura); 
  delay(100);
}


void read_capacity() {

  int sensorVal = analogRead(A0);
  int percentageHumidity = map(sensorVal, umed, uscat, 100, 0);
  dtostrf(percentageHumidity, 3, 1, umiditate_sol);
  delay(100);
}
