#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FS.h>
#include <Wire.h>
#include <MPU6050_tockn.h>


// MPU variables
//Variaveis para armazenar valores dos sensores
float AcX, AcY, AcZ;
float GyX, GyY, GyZ; 
float AngleX, AngleY, AngleZ;
float Tmp;

long timer = 0;

MPU6050 mpu6050(Wire);

const char ConfigFileName[] = "/var/network.conf";
WiFiUDP Client;
Ticker blink_scheduler, talk_scheduler, adc_scheduler, acc_scheduler;


char outputbuffer[40];

uint8_t led_status=LOW;

/* Network configuration variables */
#define NETWORK_CONFIG_STRING_LEN 20
char ssid[NETWORK_CONFIG_STRING_LEN];
char password[NETWORK_CONFIG_STRING_LEN];
char server_ip[NETWORK_CONFIG_STRING_LEN];
int server_port;
const int timeout=20;


/* Buffer for general-purpose udp output */
char fifo_udp_out[50];

/* Buffer for osc output */
char osc_buffer[50];
int buffer_len = 0;

/* Serial command variables */
#define COMMAND_BUFFER_SIZE 40
char command_buffer[COMMAND_BUFFER_SIZE];
uint8_t command_buffer_pointer = 0;
enum {
  INVALID,
  WIFISCAN,
  WIFICONNECT,
  SETSERVERPORT,
  SETSERVERIP,
  SENDPING,
  SENDDUMMY,
  AUTOCONNECT,
  PING,
  STARTSAMPLE,
  STOPSAMPLE,

  MPUCAL,
} mycomm;

bool flag_parse = false;


void setup() {
  // put your setup code here, to run once:
  File f;
  Serial.begin(9600, SERIAL_8N1);

  blink_scheduler.attach(0.05, blink_led);
  init_mpu();
  
  blink_scheduler.attach(0.1, blink_led);
  
  talk();
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.println("Checking filesystem...");
  
  if (SPIFFS.begin()==false) {
    Serial.println("Filesystem is down. Formatting...");
    if(SPIFFS.format()) {
      Serial.println("Formating successful! Please, reboot device!");
    } else {
      Serial.println("Error formating filesystem.");
    }
  } else {
    Serial.println("Filesystem operational!");
    Serial.println("Checking for network configuration file");
    if (SPIFFS.exists(ConfigFileName)==false) {
      Serial.println("No configuration file found. Please, connect manually");
    } else {
      f = SPIFFS.open(ConfigFileName, "r");
      f.readBytes(ssid, NETWORK_CONFIG_STRING_LEN);
      f.readBytes(password, NETWORK_CONFIG_STRING_LEN);
      f.readBytes(server_ip, NETWORK_CONFIG_STRING_LEN);
      f.readBytes((char*)&server_port, 2);
      f.close();
      Serial.println("Autoconnecting!");
      print_connection();
      connect();
    }
    
    
  }
  
  blink_scheduler.attach(0.5, blink_led);
  adc_scheduler.attach(0.02, sample_and_send);
  acc_scheduler.attach(0.02, acc_and_send);
}


void loop() 
{
  int ssid_n;
  int16_t ip[4];
  if (Serial.available()) serialEvent();
  
  if (flag_parse == true) 
  {
    flag_parse = false;
    if (parse_commands()==1) 
    {
      
      switch (mycomm) {
        case WIFISCAN:
          blink_scheduler.attach(0.1, blink_led);
          network_search();
          blink_scheduler.attach(0.5, blink_led);
          break;
        case WIFICONNECT:
          blink_scheduler.attach(0.1, blink_led);
          sscanf(command_buffer, "%*s %d %s", &ssid_n, password);
          config_connection(ssid_n, password);
          connect();
          blink_scheduler.attach(0.5, blink_led);
          break;
        case SETSERVERIP:
          blink_scheduler.attach(0.1, blink_led);
          //sscanf(command_buffer, "%*s %d.%d.%d.%d %d", &(ip[0]), &(ip[1]), &(ip[2]), &(ip[3]), &server_port);
          sscanf(command_buffer, "%*s %s", server_ip);
          //sprintf(server_ip, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
          Serial.printf("Server IP=%s, port=%u\n", server_ip, server_port);
          blink_scheduler.attach(0.5, blink_led);
          break;
        case SETSERVERPORT:
          blink_scheduler.attach(0.1, blink_led);
          sscanf(command_buffer, "%*s %d", &server_port);
          Serial.printf("Server IP=%s, port=%u\n", server_ip, server_port);
          blink_scheduler.attach(0.5, blink_led);
          break;
        case AUTOCONNECT:
          blink_scheduler.attach(0.1, blink_led);
          int enable;
          sscanf(command_buffer, "%*s %d", &enable);
          config_autoconnect(enable);
          blink_scheduler.attach(0.5, blink_led);
          break;
        case PING:
          Serial.printf("PONG");
          break;
        case SENDPING:
          blink_scheduler.attach(0.1, blink_led);
          make_osc_message("/ping", 1);
          oscsend();
          blink_scheduler.attach(0.5, blink_led);
          break;
        case SENDDUMMY:
          float dummyvalue;
          blink_scheduler.attach(0.1, blink_led);
          sscanf(command_buffer, "%*s %f", &dummyvalue);
          make_osc_message("/dummy", dummyvalue);
          oscsend();
          blink_scheduler.attach(0.5, blink_led);
          break;
        case STARTSAMPLE:
          adc_scheduler.attach(0.02, sample_and_send);
          break;
        case STOPSAMPLE:
          adc_scheduler.detach();
          break;
        case MPUCAL:
          mpu6050.calcGyroOffsets(true);
          break;  
        case INVALID:
        default:
          Serial.write("Invalid command\n");
      }
    }
  }
}


void blink_led() {
  if (led_status==LOW) {
    led_status = HIGH;
  } else {
    led_status = LOW;
  }
  digitalWrite(LED_BUILTIN, led_status);
}

void talk() {
  Serial.write("ALIVE!\n");
}
