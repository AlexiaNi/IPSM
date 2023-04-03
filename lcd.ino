// SeraSMART - Arduino UNO R3
// Niculae Alexia
// Alimentarea placii se face cu ajutorul a doi acumulatori cu litiu de 3.7V pusi in serie (7.4V, 1800mA) prin pinurile VIN si GND


//Import librarii 
#include "dht.h" // Necesara senzorului de temperatura si umiditate DHT22 
#include <U8g2lib.h> // Necesare modulului LCD 128X64 (nu este oficiala, repositoriul github poate fi gasit aici: https://github.com/olikraus/u8g2)
 // #include <U8x8lib.h> etse o varinata mai primitiva a librariei importante mai sus
 // U8g2lib foloseste o mica parte (37%) din memoria microcontrolerului ca "buffer" pentru afisarea obiectelor pe display

dht DHT;


// Declarare variabile globale (pot fi accesate in orice functie)
char umiditate_sol [5];
char umiditate_aer [5];
char temperatura [5];
#define dht_apin A1 // Pentru a nu repeta "A1" pe tot parcursul programului
// Dupa ce am testat sensorul de umiditate sol in medii uscate si umede am determinat cele doua valori maxime gasite pentru a putea converi rezultatul final intr-un procent
int uscat = 495;
int umed = 225;

// Contructori necesari functionarii modulului LCD (pot fi gasiti in sketch-ul exemplu)
// Un constructor este o functie speciala care se apeleaza automat de fiecare data cand se creaza un obiect
U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /*CS=*/ 10, /*RESET=*/ 8);


// Functie specifica Arduino, se apeleaza o singura data la inceputul programului
void setup() {
  
  // Se lasa timp pemtru system boot
  delay(500);
  // Pentru alimentarea senzorului de umiditate a solului am setat un pin digital "HIGH" pentru a obtine un output de 5V (pinul principal era deja ocupat)
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  // LCD
  u8g2.begin(); // Boot LCD
  u8g2.enableUTF8Print(); // Encoding
  u8g2.setFont(u8g2_font_6x10_mf); //Lista cu fonturi poate fi gasita aici: https://github.com/olikraus/u8glib/wiki/fontsize (cel folosit aici este de 7 pixeli)
  u8g2.setColorIndex(1); // Setare culoare
}


// Se reapeleaza pe tot parcursul programului
void loop() {
  // In functia draw() se scrie/deseneaza pe display
  // in loop() se apeleaza functia draw(), care apeleaza la randul ei alte doua functii read_capacity() si read_hum_temp()
  // Manual Update - in cazul acesta display-ul se updateaza in functie de valorile senzorilor (documentatia am preluat-o din repositoriul oficial al librariei)
  u8g2.firstPage();
  do{
    draw();
  } while(u8g2.nextPage() );
}

void draw() {

  //Se apeleaza pentru a putea accesa variabilele neglobale
  read_capacity();
  read_hum_temp();

  // 1 si 13 reprezinta positia X respectiv Y (intr-un sistem cartezian) a obiectului desenat
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

// Pentru folosirea senzorilor am creat doua functii separate, tot de tip void (nu returneaza nimic)

// Citire valoare umiditate aer si temperatura
void read_hum_temp() {

  // Se citeste valoare pinului analog "A1"
  // Temperatura (Celsius) si umiditatea (procente)
  DHT.read11(dht_apin);
  int air_humidity = DHT.humidity;
  int temperature = DHT.temperature;
  // Functie care converteste orice variabila de tip float (numar cu zecimale) in string (text)
  // Este necesara deoarece functia "u8g2.drawStr" nu accepta variabile de tip integer
  dtostrf(air_humidity, 3, 1, umiditate_aer); // Functia ia 3 parametrii: variabila de tip float care trebuie convertita, spatiul minim ocupat de textul output, 
  dtostrf(temperature, 3, 1, temperatura); //  numarul de zecimale de la sfarsitul numarului, variabila de tip "char" unde va fi stocat stringul (declarata mai sus)
  // Se asteapta pentru a nu supraincarca sistemul
  delay(100);
}


// Citire valoare umiditate sol
void read_capacity() {

  // Citim valoarea pinilui analog "A0" unde este conectat senzorul
  // Returneaza umiditatea, in general o valoare intre 495 si 225, insa poate varia in functie de senzor (de aceea este necesara o testare preliminara pentru a putea determina maximul si minimul)
  int sensorVal = analogRead(A0);
  // Functia "map" ajuta la convertirea unui numar intr-un procent, in functie de pozitia acestuia intre doua extreme (495-225)
  int percentageHumidity = map(sensorVal, umed, uscat, 100, 0);
  dtostrf(percentageHumidity, 3, 1, umiditate_sol);
  delay(100);
}
