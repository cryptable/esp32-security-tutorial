/*
 * Sketch to test ESP32 with RC522 RFID module
 * 
 * Typical pin layout used for RC522:
 * ---------------------------------
 *             MFRC522      ESP32
 *             Reader/PCD   NodeMCU
 * Signal      Pin          Pin
 * ---------------------------------
 * RST/Reset   RST          G21
 * SPI SS      SDA(SS)      G5
 * SPI MOSI    MOSI         G23
 * SPI MISO    MISO         G19
 * SPI SCK     SCK          G18
 * 3V3         3V3          3V3
 * GND         GND          GND
 *
 * Typical pin layout used for LED: G12
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// Wifi Module
const char WIFI_SSID[]			=	"<SSID>";
const char WIFI_PASSWORD[]	= "<SSID Password>";

// MQTT Module
const char MQTT_HOST[]      = "<MQTT Broker>";
const int  MQTT_PORT			  = 1883;
const char MQTT_USER[]      = "<USER>";
const char MQTT_PASSWORD[]  = "<PASSWORD>";
#define STUDENT_ID          "<STUDENT_ID>"

// RC522 Module
#define  RST      21
#define  SDA      5
#define  MOSI     23
#define  MISO     19
#define  SCK      18
#define  LED      12

unsigned char ALLOWED_UID[] = { 0x9C, 0xF5, 0x2E, 0x31 };

// Declaration of RC522 Module
MFRC522DriverPinSimple ss_pin(SDA); // Configurable, see typical pin layout above.
MFRC522DriverSPI driver{ss_pin}; 		// Create SPI driver.
MFRC522 mfrc522{driver};  					// Create MFRC522 instance.

// Declaration of MQTT client
WiFiClient espClient;
PubSubClient mqttClient(espClient);
const char *subscribe_topic       = "AP/rfid/deur/" STUDENT_ID "/+";
const char *subscribe_door_topic  = "AP/rfid/deur/" STUDENT_ID "/001";
const char *publish_topic         = "AP/rfid/lezer/" STUDENT_ID "/deur001";

// Helper function
char* convert_to_hex(const unsigned char *bytes, const int size);

void setup_WiFi() {
	WiFi.mode(WIFI_STA);										// Set Wifi to station mode
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);		// Connect to Wifi station
  Serial.print("Connecting to WiFi ..");	
  while (WiFi.status() != WL_CONNECTED) {	// Wait until connected
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());					// Print received IP address	
}

// Function which is called when a message is subscribed
void callback_MQTT(const char *topic, byte* payload, unsigned int length) {
  Serial.printf("Message received from [%s]\n", topic);	
	if (strcmp(topic, subscribe_door_topic) == 0) {
		if (strncmp((const char *)payload, "ON", length) == 0) {
	    digitalWrite(12, HIGH);  // turn the LED on (HIGH is the voltage level)
		}
		if (strncmp((const char *)payload, "OFF", length) == 0) {
	    digitalWrite(12, LOW);  // turn the LED on (HIGH is the voltage level)
		}
	}	
}

void setup_MQTT() {
 	mqttClient.setServer(MQTT_HOST, MQTT_PORT);
	mqttClient.setCallback(callback_MQTT);

	while (!mqttClient.connected()) {
    const char client_id[] = "esp32-" STUDENT_ID;
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id);

    if (mqttClient.connect(client_id, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("MQTT broker connected");
    }
    else {
    	Serial.printf("failed with state %s\n", mqttClient.state());
    	delay(2000);
    }
	}

	if (mqttClient.subscribe(subscribe_topic)) {
    Serial.printf("Subscribe successful %s\n", subscribe_door_topic);
  }
  else {
    Serial.printf("Subscribe to %s failed with state %s\n", subscribe_door_topic, mqttClient.state());
  }
}

void setup() {
  Serial.begin(115200);  	// Initialize serial communications with the PC for debugging.
  while (!Serial);       	// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4).
  
  setup_WiFi();

  setup_MQTT();

  pinMode(LED, OUTPUT);		// Initialize the LED (simulating door switch)

  mfrc522.PCD_Init();  		// Init MFRC522 board.
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);		// Show details of PCD - MFRC522 Card Reader details.
	
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  mqttClient.loop();

	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

  // Check if allowed UID is the same as the entered card
  if (memcmp(mfrc522.uid.uidByte, ALLOWED_UID, sizeof(ALLOWED_UID)) == 0) {
  	Serial.println(F("Card present ..."));

    // Send message to MQTT
    char *card_uid = convert_to_hex(mfrc522.uid.uidByte, mfrc522.uid.size);
  	Serial.printf("Send Card UID [%s] to MQTT server\n", card_uid);
  	mqttClient.publish(publish_topic, card_uid);
    delay(5000);
  }
}

char uid_str_buffer[32];
char* convert_to_hex(const unsigned char *bytes, const int size) {
	const char hex_char[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
  char *result = uid_str_buffer;
  int  i_size = size;

  if ((i_size*2)>=sizeof(uid_str_buffer)) {
  	i_size = sizeof(uid_str_buffer)/2 - 1;
  }

	for (int i=0; i<i_size; i++) {
		char byte = bytes[i];
		uid_str_buffer[i*2+1] = hex_char[byte & 0x0F];
		uid_str_buffer[i*2] = hex_char[byte>>4 & 0x0F];
	}
	uid_str_buffer[i_size*2] = '\0';

	return uid_str_buffer;
}
