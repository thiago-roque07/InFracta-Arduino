/*
  Functions for the MPU accelerometer
*/
void init_mpu() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Serial.println("Inicializei transmissao");
  Wire.write(0x6B); 
  
  //Inicializa o MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);
  Serial.println("Fim");
}

void capture_mpu() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  //Solicita os dados do sensor
  Wire.requestFrom(MPU,14,true);  
  //Armazena o valor dos sensores nas variaveis correspondentes
  AcX=Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void dump_mpu() {
  sprintf(outputbuffer, "Acelerometro: %d, %d, %d\nGiroscopio: %d, %d, %d\n", AcX, AcY, AcZ, GyX, GyY, GyZ);
  Serial.write(outputbuffer);
}
