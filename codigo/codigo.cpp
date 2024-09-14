
#include <Adafruit_LiquidCrystal.h>

//definición de pines
const int PIN_SENAL = A0;
const int PIN_PULSADOR_INICIO = 2;
const int PIN_PULSADOR_ANALISIS = 3;

//configuración del LCD
Adafruit_LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//variables globales
const int TAMANO_BUFFER = 1000;
float* muestras = nullptr;
int indice_muestra = 0;
bool adquisicion_activa = false;

//prototipos de funciones
void iniciar_adquisicion();
void detener_adquisicion();
void analizar_senal();
float calcular_frecuencia();
float calcular_amplitud();
String identificar_forma_onda();
void mostrar_resultados(float frecuencia, float amplitud, String forma);

void setup() {
  pinMode(PIN_PULSADOR_INICIO, INPUT_PULLUP);
  pinMode(PIN_PULSADOR_ANALISIS, INPUT_PULLUP);
  
  lcd.begin(16, 2);
  lcd.print("Listo para iniciar");
  
  //asignación dinámica de memoria para el buffer
  muestras = new float[TAMANO_BUFFER];
}

//iniciar y detener adquisicion
//
void loop() {
  if (digitalRead(PIN_PULSADOR_INICIO) == LOW && !adquisicion_activa) {
    iniciar_adquisicion();
  }

  if (digitalRead(PIN_PULSADOR_ANALISIS) == LOW && adquisicion_activa) {
    detener_adquisicion();
    analizar_senal();
    delay(5000);  // mostrar resultados por 5 segundos
    iniciar_adquisicion();
  }

  if (adquisicion_activa) {
    if (indice_muestra < TAMANO_BUFFER) {
      muestras[indice_muestra] = analogRead(PIN_SENAL) * (5.0 / 1023.0);  // convertir a voltios
      indice_muestra++;
    } else {
      indice_muestra = 0;  // reiniciar buffer
    }
  }




void iniciar_adquisicion() {
  adquisicion_activa = true;
  indice_muestra = 0;
  lcd.clear();
  lcd.print("Adquiriendo...");
}




void detener_adquisicion(){

        adquisicion_activa = false;

}



//analizar señal
//
void analizar_senal() {
  float frecuencia = calcular_frecuencia();
  float amplitud = calcular_amplitud();
  String forma = identificar_forma_onda();
  
  mostrar_resultados(frecuencia, amplitud, forma);
}

//mostrar resultados
//
void mostrar_resultados(float frecuencia, float amplitud, String forma) {
  lcd.clear();
  lcd.print("F:");
  lcd.print();
  lcd.print("Hz A:");
  lcd.print();
  lcd.print("V");
  lcd.setCursor(0, 1);
  lcd.print("Forma: ");
  lcd.print();
}
