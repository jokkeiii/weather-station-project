/*
  TODO: Explain program

The circuit:
 * Wind speed input to digital pin 2
 *
 * Wind direction input to analog pin A0
 *
 * Button input to digital pin 15
 * Button input to digital pin 16
 * Button input to digital pin 17
 * Button input to digital pin 18
 *
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
*/

// Headers
#include <LiquidCrystal.h>  // include Liquid Crystal Library
#include <Ethernet.h>       // incluide Ethernet library W5100
#include <PubSubClient.h>   // include MQTT library
#include <TimerOne.h>       // include timer library

// Structs and enums
enum class Menu_state {
  state1,
  state2,
  state3,
  state4,
} static menu_state;

// MQTT and LCD-screen
//-------------------------------------------------------------------------------------------------

//  MQTT settings
const unsigned short Port = 1883;        // MQTT port number
const byte server[] = { 10, 6, 0, 21 };  // TAMK IP

EthernetClient ethClient;  // Ethernet object var
static byte mymac[6] = { 0x44, 0x76, 0x58, 0x17, 0x07, 0x62 };

//  MQTT Server settings
void callback(char* topic, byte* payload, unsigned int length);  // Subscription callback for received MQTTT messages
PubSubClient client(server, Port, callback, ethClient);          // MQTT client

const char* deviceId = "Kouru";      // Set your device id (will be the MQTT client username) *yksilöllinen*
const char* clientId = "KouruId23";  // Set a random string (max 23 chars, will be the MQTT client id) *yksilöllinen*
const char* deviceSecret = "tamk";   // Set your device secret (will be the MQTT client password) *kaikille yhteinen*

//  MQTT topic names
#define inTopic "ICT1B_in_2020"   // MQTT channel where data is received
#define outTopic "ICT4_out_2020"  // MQTT channel where data is sent

// Initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const byte rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Prototypes
//-------------------------------------------------------------------------------------------------

void cal_wind_speed();
void pulse_interrupt();
void change_menu(Menu_state& menu_choice, const byte input_pin);
void cal_wind_dir();
void det_wind_name();
void fetch_IP();
void send_MQTT_message(const double input_wind_speed, const double input_wind_direction);
void Connect_MQTT_server();

// Variables
//-------------------------------------------------------------------------------------------------

// Interrupt pin variables
#define SPD_PIN 2                         // Input pin for interrupt routine
static volatile byte intrpt_time{};       // 1/2 sec timer for interrupt count
static volatile double wind_spd_pulse{};  // Count pulse input every 3 secs
static volatile double wind_spd_freq{};   // Calculate frequency from pulse
static volatile double wind_speed{};      // Calculate wind speed from frequency

// Analog pin variables
#define DIR_PIN A0                        // Wind direction input pin A0
static volatile double wind_dir_input{};  // Receive input from analogread A0
static volatile double wind_dir_volt{};   // Calculate voltage from input
static volatile double wind_dir_deg{};    // Calculate wind direction from volt
static String wind_dir_name{};            // Determine wind direction name from voltage

// Button input variables
#define BTN_PIN1 15                         // Button input pin 15
#define BTN_PIN2 16                         // Button input pin 16
#define BTN_PIN3 17                         // Button input pin 17
#define BTN_PIN4 18                         // Button input pin 18
static volatile bool curr_btn_state{};      // Current reading from the input pin
static volatile bool last_btn_state{};      // Previous reading from the input pin
const byte dbnc_delay{ 50 };                // Debounce time, increase if the output flickers
static volatile unsigned long dbnc_time{};  // Last time the output pin was toggled

// Setup and loop
//-------------------------------------------------------------------------------------------------

void setup() {
  // TimerOne
  Timer1.initialize(500000);               // Initialize timer1, and set a 500 000 uS = 1/2 second period
  Timer1.attachInterrupt(cal_wind_speed);  // Attach calculate wind speed as a timer overflow

  // AttachInterrupt
  pinMode(SPD_PIN, INPUT_PULLUP);                                            // Counting Pin 2 an input
  attachInterrupt(digitalPinToInterrupt(SPD_PIN), pulse_interrupt, RISING);  // Interrupt routine definition

  // Digital input
  pinMode(BTN_PIN1, INPUT);  // Button input pin 15
  pinMode(BTN_PIN2, INPUT);  // Button input pin 16
  pinMode(BTN_PIN3, INPUT);  // Button input pin 17
  pinMode(BTN_PIN4, INPUT);  // Button input pin 18

  // Initalize the screen
  lcd.begin(16, 2);
  delay(500);

  // SerialPrint
  Serial.begin(9600);                 // Serial monitor baudrate  = 9600
  Serial.println("Start 19.3.2021");  // print to serial monitor

  // IP logic
  fetch_IP();             // initialize Ethernet connection
  Connect_MQTT_server();  // connect to MQTT server
}

void loop() {
  switch (menu_state) {
    case Menu_state::state1:
      lcd.setCursor(0, 0);     // Set cursor to upper left corner
      lcd.print("wind_spd=");  // Print string to lCD
      lcd.print(wind_speed);   // Print value to lCD
      lcd.print("       ");
      break;
    case Menu_state::state2:
      lcd.setCursor(0, 0);      // Set cursor to upper left corner
      lcd.print("wind_dir=");   // Print string to lCD
      lcd.print(wind_dir_deg);  // Print value to lCD
      lcd.print("       ");
      break;
    case Menu_state::state3:
      lcd.setCursor(0, 0);
      lcd.print(wind_dir_name);
      lcd.print("       ");
      break;
    case Menu_state::state4:
      lcd.setCursor(0, 0);
      lcd.print(Ethernet.localIP());
      lcd.print("       ");
      break;
  }

  cal_wind_dir();
  // lcd.setCursor(0, 0);
  // lcd.print("Are we connected?");
  Serial.println("Are we connected?");
  send_MQTT_message(wind_speed, wind_dir_deg);
  delay(1500);
}

// Function definitions
//-------------------------------------------------------------------------------------------------

// Attach interrupt fn
void cal_wind_speed() {  // Interrupt routine, called every 1/2 sec
  intrpt_time++;
  if (intrpt_time > 5) {                         // Count pulse every 6 secs
    wind_spd_freq = wind_spd_pulse / 3;          // Save pulse to value to frequency
    wind_speed = -0.24 + wind_spd_freq * 0.699;  // Calculate wind speed
    wind_spd_pulse = 0;                          // Reset pulse
    intrpt_time = 0;                             // reset time
  }
}

void pulse_interrupt() {
  wind_spd_pulse++;
}

// Read button input and change menu state
void change_menu(Menu_state& menu_choice, const byte input_pin) {
  bool pin_read = digitalRead(input_pin);  // Determine input state
  if (pin_read != last_btn_state) {        // If input has changed start counting
    dbnc_time = millis();
  }
  if ((millis() - dbnc_time) > dbnc_delay) {  // If time has passed debounce delay
    if (pin_read != curr_btn_state) {         // If Input is not the same as before
      curr_btn_state = pin_read;              // Change current button state
      menu_state = menu_choice;               // Change menu state
    }
  }
  last_btn_state = pin_read;  // Determine last button state
}

// Calculate analog input voltage and wind direction
void cal_wind_dir() {
  wind_dir_input = analogRead(DIR_PIN);
  wind_dir_volt = wind_dir_input * (5.0 / 1023);  // * ((R1 + R2) / R2);
  wind_dir_deg = wind_dir_volt * 95.745;          // Calculate Wind direction from voltage
};

// Rename wind direction name based on wind input voltage
void det_wind_name() {
  if (wind_dir_volt >= 0.00 && wind_dir_volt < 0.47) wind_dir_name = "N  ";
  else if (wind_dir_volt >= 0.47 && wind_dir_volt < 0.95) wind_dir_name = "NE ";
  else if (wind_dir_volt >= 0.95 && wind_dir_volt < 1.43) wind_dir_name = "E  ";
  else if (wind_dir_volt >= 1.43 && wind_dir_volt < 1.90) wind_dir_name = "SE ";
  else if (wind_dir_volt >= 1.90 && wind_dir_volt < 2.38) wind_dir_name = "S  ";
  else if (wind_dir_volt >= 2.38 && wind_dir_volt < 2.85) wind_dir_name = "SW ";
  else if (wind_dir_volt >= 2.85 && wind_dir_volt < 3.33) wind_dir_name = "W  ";
  else if (wind_dir_volt >= 3.33 && wind_dir_volt < 4.00) wind_dir_name = "NW ";
  else wind_dir_name = "Err";
};

//  GET IP number from DHCP server
void fetch_IP() {
  byte rev = 1;
  rev = Ethernet.begin(mymac);  // get IP number
  Serial.print(F("\nW5100 Revision "));

  if (rev == 0) {
    Serial.println(F("Failed to access Ethernet controller"));
  }

  Serial.println(F("Setting up DHCP"));
  Serial.print("Connected with IP: ");
  Serial.println(Ethernet.localIP());
  delay(1500);
}

// MQTT Routines
// Send MQTT message
void send_MQTT_message(const double input_wind_speed, const double input_wind_direction) {
  char bufa[100];
  char bufb[10];
  char bufc[10];

  if (client.connected()) {
    // create message with header and data
    dtostrf(input_wind_speed, 3, 2, bufb);
    dtostrf(input_wind_direction, 3, 2, bufc);
    sprintf(bufa, "IOTJS={\"group_name\":\"Kouru\",\"wind_speed\":%s,\"wind_direction\":%s}", bufb, bufc);

    Serial.println(bufa);            // Print message to serial monitor
    client.publish(outTopic, bufa);  // Send message to MQTT server
  } else {                           // Re connect if connection is lost
    delay(500);

    lcd.setCursor(0, 0);              // Set cursor to left upper corner
    lcd.print("VIRHE: IP ei saatu");  // Print string to lCD

    Serial.println("No, re-connecting");
    client.connect(clientId, deviceId, deviceSecret);
    delay(1000);  // wait for reconnecting
  }
}

//  MQTT server connection
void Connect_MQTT_server() {
  // lcd.setCursor(0, 0);               // Set cursor to left upper corner
  // lcd.print(" Connecting to MQTT");  // Print string to lCD
  Serial.println(" Connecting to MQTT");
  Serial.print(server[0]);
  Serial.print(".");  // Print MQTT server IP number to Serial monitor
  Serial.print(server[1]);
  Serial.print(".");
  Serial.print(server[2]);
  Serial.print(".");
  Serial.println(server[3]);
  delay(500);

  if (!client.connected()) {                                 // Check if allready connected
    if (client.connect(clientId, deviceId, deviceSecret)) {  // Connection to MQTT server
      Serial.println(" Connected OK ");
      client.subscribe(inTopic);  // Subscript to in topic
    } else {
      Serial.println(client.state());
    }
  }
}

//  Receive incoming MQTT message
void callback(char* topic, byte* payload, unsigned int length) {
  char* receiv_string;  // Copy the payload content into a char*
  receiv_string = (char*)malloc(length + 1);
  memcpy(receiv_string, payload, length);  // Copy received message to receiv_string
  receiv_string[length] = '\0';
  Serial.println(receiv_string);
  free(receiv_string);
}
