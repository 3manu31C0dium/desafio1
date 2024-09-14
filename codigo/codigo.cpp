
#include <Adafruit_LiquidCrystal.h>

// Definición de pines
const int PIN_SENAL = A0;
const int PIN_PULSADOR_INICIO = 2;
const int PIN_PULSADOR_ANALISIS = 3;

// Configuración del LCD
Adafruit_LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Variables globales
const int TAMANO_BUFFER = 1000;
float* muestras = nullptr;
int indice_muestra = 0;
bool adquisicion_activa = false;

// Prototipos de funciones
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
  
  // Asignación dinámica de memoria para el buffer
  muestras = new float[TAMANO_BUFFER];
}

