#define FC28_SENSOR_PIN 4  // Conectar el sensor FC-28 al pin digital D4 del ESP32

void setup() {
  Serial.begin(115200);
  pinMode(FC28_SENSOR_PIN, INPUT);
}

void loop() {
   // Leer el valor analógico del sensor FC-28
  int sensorValue = analogRead(FC28_SENSOR_PIN);

  // Imprimir el valor sin mapeo para la depuración
  Serial.print("Valor del sensor: ");
  Serial.println(sensorValue);

  // Convertir el valor analógico a porcentaje de humedad
  float humidity = map(sensorValue, 0, 4095, 100, 0);

  // Imprimir el valor de humedad en el puerto serie
  Serial.print("Humedad del suelo: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(1000);  // Pausa de 1 segundo entre lecturas
}
