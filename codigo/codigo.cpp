
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

void loop() {
  if (digitalRead(PIN_PULSADOR_INICIO) == LOW && !adquisicion_activa) {
    iniciar_adquisicion();
  }
  
  if (digitalRead(PIN_PULSADOR_ANALISIS) == LOW && adquisicion_activa) {
    detener_adquisicion();
    analizar_senal();
    delay(5000);  // Mostrar resultados por 5 segundos
    iniciar_adquisicion();
  }
  
  if (adquisicion_activa) {
    if (indice_muestra < TAMANO_BUFFER) {
      muestras[indice_muestra] = analogRead(PIN_SENAL) * (5.0 / 1023.0);  // Convertir a voltios
      indice_muestra++;
    } else {
      indice_muestra = 0;  // Reiniciar buffer circular
    }
  }
}

void iniciar_adquisicion() {
  adquisicion_activa = true;
  indice_muestra = 0;
  lcd.clear();
  lcd.print("Adquiriendo...");
}

void detener_adquisicion() {
  adquisicion_activa = false;
}

void analizar_senal() {
  float frecuencia = calcular_frecuencia();
  float amplitud = calcular_amplitud();
  String forma = identificar_forma_onda();
  
  mostrar_resultados(frecuencia, amplitud, forma);
}

float calcular_frecuencia() {
 
  int cruces_cero = 0;
  float umbral = 2.5;  // Asumiendo una señal centrada en 2.5V
  
  for (int i = 1; i < TAMANO_BUFFER; i++) {
    if ((muestras[i-1] < (umbral && muestras[i]) >= umbral) ||
        (muestras[i-1] > (umbral && muestras[i]) <= umbral)) {
      cruces_cero++;
    }
  }
  
  float tiempo_total = TAMANO_BUFFER / 1000.0;  // Asumiendo muestreo a 1kHz
  return cruces_cero / (2 * tiempo_total);
}



float calcular_amplitud() {
  const int pinAnalog = A0; // Pin de entrada analógica
  const int numSamples = 100; // Número de muestras para analizar
  int valorMaximo = 0;
  int valorMinimo = 1023; // Valor máximo posible de la lectura analógica (10 bits de resolución)
  
  for (int i = 0; i < numSamples; i++) {
    int valorActual = analogRead(pinAnalog); // Lee el valor actual del pin analógico
    
    if (valorActual > valorMaximo) {
      valorMaximo = valorActual;
    }
    
    if (valorActual < valorMinimo) {
      valorMinimo = valorActual;
    }
    
  }
  
  // Amplitud es la diferencia entre el valor máximo y el valor mínimo
  float amplitud = valorMaximo - valorMinimo;
  
  return amplitud;
}

  
String identificar_forma_onda() {

  float suma_derivadas = 0;
  for (int i = 0; i < TAMANO_BUFFER; i++) {
    suma_derivadas + (abs(muestras[i] - muestras[i-1]));
  }
  float promedio_derivadas = (suma_derivadas / TAMANO_BUFFER);
  
  if (promedio_derivadas < 0.1) return "Cuadrada";
  else if (promedio_derivadas < 0.5) return "Triangular";
  else return "Senoidal";
}

void mostrar_resultados(float frecuencia, float amplitud, String forma) {
  lcd.clear();
  lcd.print("F:");
  lcd.print(frecuencia, 2);
  lcd.print(" A:");
  lcd.print(amplitud, 2);
  lcd.print("V");
  lcd.setCursor(0, 1);
  lcd.print("Forma: ");
  lcd.print(forma);
}
