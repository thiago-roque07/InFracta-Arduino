/*
  Functions for the serial commands
*/
int parse_commands() 
{
  char command_string[15];
  sscanf(command_buffer, "%s", command_string);
  Serial.print("Command: ");
  Serial.println(command_string);
  if (strcmp(command_string, "AUTOCONNECT")==0) {
    mycomm = AUTOCONNECT;
    return 1;
  }
  if (strcmp(command_string, "WIFISCAN")==0) {
    mycomm = WIFISCAN;
    return 1;
  }
  if (strcmp(command_string, "WIFICONNECT")==0) {
    mycomm = WIFICONNECT;
    return 1;
  }
  if (strcmp(command_string, "SETSERVERPORT")==0) {
    mycomm = SETSERVERPORT;
    return 1;
  }
  if (strcmp(command_string, "SETSERVERIP")==0) {
    mycomm = SETSERVERIP;
    return 1;
  }
  if (strcmp(command_string, "SENDPING")==0) {
    mycomm = SENDPING;
    return 1;
  }
  if (strcmp(command_string, "SENDDUMMY")==0) {
    mycomm = SENDDUMMY;
    return 1;
  }
  if (strcmp(command_string, "PING")==0) {
    mycomm = PING;
    return 1;
  }
  if (strcmp(command_string, "STARTSAMPLE")==0) {
    mycomm = STARTSAMPLE;
    return 1;
  }
  if (strcmp(command_string, "STOPSAMPLE")==0) {
    mycomm = STOPSAMPLE;
    return 1;
  }
  mycomm = INVALID;
  return 0;
}

void serialEvent() {
  while (Serial.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    Serial.write(inChar);
    // add it to the inputString:
    if (command_buffer_pointer < COMMAND_BUFFER_SIZE-1) 
    {
      command_buffer[command_buffer_pointer] = inChar;
      command_buffer_pointer++;
    }
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      command_buffer[command_buffer_pointer] = '\0';
      command_buffer_pointer = 0;
      flag_parse = true;
    }
  }
}

void print_connection() {
  Serial.println("Connection settings:");
  Serial.print("Network SSID: ");
  Serial.println(ssid);
  Serial.print("Network password: ");
  Serial.println(password);
  Serial.print("Server port: ");
  Serial.println(server_port);
  Serial.print("Server IP: ");
  Serial.println(server_ip);
}
