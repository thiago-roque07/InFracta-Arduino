/*
  Functions for the MPU accelerometer
*/
void init_mpu() {
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  Serial.println("Inicializei transmissao");
  mpu6050.calcGyroOffsets(true);
  Serial.println("Fim");
}

void capture_mpu() {

  mpu6050.update();
  //Solicita os dados do sensor

  //Armazena o valor dos sensores nas variaveis correspondentes
  AcX = mpu6050.getAccX();
  AcY = mpu6050.getAccY();
  AcZ = mpu6050.getAccZ();
  
  GyX = mpu6050.getGyroX();
  GyY = mpu6050.getGyroY();
  GyZ = mpu6050.getGyroZ();

  Tmp = mpu6050.getTemp();

  AngleX = mpu6050.getAngleX();
  AngleY = mpu6050.getAngleY();
  AngleZ = mpu6050.getAngleZ();
}

void dump_mpu() {
  sprintf(outputbuffer, "Acelerometro: %d, %d, %d\nGiroscopio: %d, %d, %d\n", AcX, AcY, AcZ, GyX, GyY, GyZ);
  Serial.write(outputbuffer);
}
