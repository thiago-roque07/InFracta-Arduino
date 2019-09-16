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
  oscsend();
}

void acc_and_send() {
  capture_mpu();
  make_osc_message("/AcX", AcX);
  oscsend();
  make_osc_message("/AcY", AcY);
  oscsend();
  make_osc_message("/AcZ", AcZ);
  oscsend();
}

void oscsend()
{
  Client.beginPacket(server_ip,server_port);
  Client.write(osc_buffer, buffer_len);
  buffer_len = 0;
  Client.endPacket();
  return;
}

void udpsend()
{
  Client.beginPacket(server_ip,server_port);
  Client.write(fifo_udp_out);
  Client.endPacket();
  return;
}
