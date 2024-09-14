
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
