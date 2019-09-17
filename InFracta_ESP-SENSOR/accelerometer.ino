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

  delay(100); // Wait for sensor to stabilize
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  //Solicita os dados do sensor
  Wire.requestFrom(MPU,6,true);  
  //Armazena o valor dos sensores nas variaveis correspondentes
  AcX=Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  roll  = atan(AcY / sqrt(AcX * AcX + AcZ * AcZ)) * RAD_TO_DEG;
  pitch = atan2(-AcX, AcZ) * RAD_TO_DEG;

  // Set starting angle
  kalmanX.setAngle(roll); 
  kalmanY.setAngle(pitch);
  
  gyroXangle = roll;
  gyroYangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;

  timer = micros();
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

  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();
  
  roll  = atan(AcY / sqrt(AcX * AcX + AcZ * AcZ)) * RAD_TO_DEG;
  pitch = atan2(-AcX, AcZ) * RAD_TO_DEG;

  gyroXrate = GyX / 131.0; // Convert to deg/s
  gyroYrate = GyY / 131.0; // Convert to deg/s

  if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) 
  {
    kalmanY.setAngle(pitch);
    compAngleY = pitch;
    kalAngleY = pitch;
    gyroYangle = pitch;
  } 
  else 
  {
    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter
  } 

  if (abs(kalAngleY) > 90) {gyroXrate = -gyroXrate;} // Invert rate, so it fits the restriced accelerometer reading
  
  kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
    
}

void dump_mpu() {
  sprintf(outputbuffer, "Acelerometro: %d, %d, %d\nGiroscopio: %d, %d, %d\n", AcX, AcY, AcZ, GyX, GyY, GyZ);
  Serial.write(outputbuffer);
}
