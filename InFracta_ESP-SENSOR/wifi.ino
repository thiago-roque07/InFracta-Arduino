/*
  Functions to setup the WIFI connection
*/
void network_search() {
  /*talk_scheduler.attach(5, talk);*/
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  int n = WiFi.scanNetworks();
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" dB @ channel ");
      Serial.print(WiFi.channel(i));
      Serial.print(")\n");
      //Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");
}

void config_connection(int ssid_n, char *passwd) {
  WiFi.SSID(ssid_n).toCharArray(ssid, 20);
  strcpy(password, passwd);
}

void connect() {
  int timecount = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    timecount++;
    if (timecount > timeout) {
      Serial.println("Connection timeout!");
      return;
    }
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
/*
void config_autoconnect(int enable) {
  File f;
  
  if (enable!=0) {
      Serial.println("Enabling autoconnect on reboot");
      print_connection();
      f = SPIFFS.open(ConfigFileName, "w");
      f.write((const uint8_t*)ssid, NETWORK_CONFIG_STRING_LEN);
      f.write((const uint8_t*)password, NETWORK_CONFIG_STRING_LEN);
      f.write((const uint8_t*)server_ip, NETWORK_CONFIG_STRING_LEN);
      f.write((const uint8_t*)&server_port, 2);
      f.close();   
  } else {
    if (SPIFFS.exists(ConfigFileName) == true) {
      SPIFFS.remove(ConfigFileName);
      Serial.println("Not autoconnecting anymore!");
    } else {
      Serial.println("Autoconnect is already disabled!");
    }
  }

}
*/
