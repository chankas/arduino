#include <Adafruit_NeoPixel.h>

// Configuración de la tira de LED
#define PIN        6   // Pin al que está conectada la tira de LED
#define NUMPIXELS  16  // Número de LEDs en la tira

// Configuración del sensor PIR
#define PIR_PIN    2   // Pin al que está conectado el sensor PIR

// Crea el objeto NeoPixel
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Funciones para controlar los LEDs

unsigned long startTime;
int waitOff = 0;
void cometEffect();
void gradualIllumination();
int readSensor();
void gradualOff();

void setup() {
  pixels.begin();   // Inicializa la tira de LED
  pixels.clear();   // Asegúrate de que todos los LEDs estén apagados al inicio
  pixels.show();

  pinMode(PIR_PIN, INPUT); // Configura el pin del sensor PIR como entrada
}

void loop() {
  if (digitalRead(PIR_PIN) == HIGH or waitOff == 1) { // Si se detecta movimiento
    if (waitOff == 0){
      cometEffect();
      gradualIllumination();
    }
    waitOff = readSensor();
    if (waitOff == 0){
      gradualOff();
    }
  }
  delay(100); // Pequeña espera antes de volver a comprobar el sensor
}

// Función 1: Efecto cometa
void cometEffect() {
  for (int i = 0; i < NUMPIXELS + 5; i++) {
    pixels.clear();
    if (i < NUMPIXELS) pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // LED más intenso
    if (i > 0) pixels.setPixelColor(i - 1, pixels.Color(223, 233, 233)); // Cola
    if (i > 1) pixels.setPixelColor(i - 2, pixels.Color(201, 201, 201)); // Cola
    if (i > 2) pixels.setPixelColor(i - 3, pixels.Color(169, 169, 169)); // Cola
    if (i > 3) pixels.setPixelColor(i - 4, pixels.Color(137, 137, 137)); // Cola
    if (i > 4){
      for (int j = 0; j < i - 4; j++){
        pixels.setPixelColor(j, pixels.Color(105, 105, 105));  // Cola
      }
    }
    pixels.show();
    delay(50);
  }
}

// Función 2: Iluminación gradual
void gradualIllumination() {
  for (int j = 105; j <= 180; j++) { // Incrementa la intensidad hasta la mitad (128)
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(j, j, j)); // Aumenta la intensidad
    }
    pixels.show();
    delay(50);
  }
}
// Función 3: Detecta si aun se detecta movimiento
int readSensor() {
  startTime = millis(); // Guarda el tiempo de inicio

  while (millis() - startTime < 5000) {
    if (digitalRead(PIR_PIN) == HIGH ) {
      return 1;
    }
    delay(100);
  }
  return 0;
}

// Función 4: Apagado gradual
void gradualOff() {
  for (int j = 180; j >= 0; j--) { // Decrementa la intensidad desde la mitad (128) hasta 0
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(j, j, j)); // Disminuye la intensidad
    }
    pixels.show();
    delay(10);
  }
}
