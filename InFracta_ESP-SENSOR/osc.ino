/* 
  Functions to capture the accelerometer data and send it on an OSC message
*/
void make_osc_message(char *address, float param) {
  buffer_len = 0;
  for (int i=0; i<50; i++) osc_buffer[i]='\0';
  strcpy(osc_buffer, address);
  buffer_len = strlen(osc_buffer);
  buffer_len ++;
  while ( (buffer_len % 4) != 0) buffer_len++; /* Complete 4 bytes */
  osc_buffer[buffer_len++] = ',';
  osc_buffer[buffer_len++] = 'f';
  while ( (buffer_len % 4) != 0) buffer_len++; /* Complete 4 bytes */

  char *p = (char*) (&param);
  for (int i=0; i<4; i++) {
    osc_buffer[buffer_len++] = p[3-i];
  }
}

void sample_and_send() {
  uint16_t adc_in;
  float voltage;
  adc_in = analogRead(A0);
  voltage = adc_in;
  make_osc_message("/adc", voltage);
  //oscsend();
}

void acc_and_send() {
  capture_mpu();
  OSCMessage msg1("/AcX");
  msg1.add((int16_t) AcX);
  oscsend(&msg1);
  OSCMessage msg2("/AcY");
  msg2.add((int16_t) AcY);
  oscsend(&msg2);
  OSCMessage msg3("/AcZ");
  msg3.add((int16_t) AcZ);
  oscsend(&msg3);

  /*
  make_osc_message("/AcX", AcX);
  oscsend();
  make_osc_message("/AcY", AcY);
  oscsend();
  make_osc_message("/AcZ", AcZ);
  oscsend();
*/

  OSCMessage msg4("/Rol");
  msg4.add((int16_t) roll);
  oscsend(&msg4);
  OSCMessage msg5("/Pit");
  msg5.add((int16_t) pitch);
  oscsend(&msg5);
  OSCMessage msg6("/GyX");
  msg6.add((int16_t) gyroXangle);
  oscsend(&msg6);
  OSCMessage msg7("/GyY");
  msg7.add((int16_t) gyroYangle);
  oscsend(&msg7);
  OSCMessage msg8("/KaX");
  msg8.add((int16_t) kalAngleX);
  oscsend(&msg8);
  OSCMessage msg9("/KaY");
  msg9.add((int16_t) kalAngleY);
  oscsend(&msg9);

  /*
  make_osc_message("/Rol", roll);
  oscsend();
  make_osc_message("/Pit", pitch);
  oscsend();

  make_osc_message("/GyX", gyroXangle);
  oscsend();
  make_osc_message("/GyY", gyroYangle);
  oscsend();

  make_osc_message("/KaX", kalAngleX);
  oscsend();
  make_osc_message("/KaY", kalAngleY);
  oscsend();
  */
}

void oscsend(OSCMessage * msg)
{
  Client.beginPacket(server_ip,server_port);
  msg->send(Client);
  //Client.write(osc_buffer, buffer_len);
  buffer_len = 0;
  Client.endPacket();
  msg->empty();
  return;
}

void udpsend()
{
  //Client.beginPacket(server_ip,server_port);
  //Client.write(fifo_udp_out);
  //Client.endPacket();
  return;
}
