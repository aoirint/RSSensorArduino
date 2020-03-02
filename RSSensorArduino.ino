
#include <ArduinoJson.h>

#define BAUDRATE             38400

#define DOOR_LOG_SIZE         5

#define SENSOR_LIGHT         A0
#define SENSOR_TMPR          A1
#define TMPR_VOLT            5.0
#define TMPR_V_OFFSET        0.6
#define TMPR_DEGREE_PER_VOLT 0.01

#define SENSOR_DOOR          2


// BEGIN: Door Sensor
int doorRaw = LOW; // LOW=closed, HIGH=open
int prevDoorRaw = LOW;

// When doorRaw is LOW for 1.0sec, door expected open
long doorCurrentStateSince = 0; // millis


bool prevDoorIsOpen = false;
bool doorIsOpen = false;
//int doorSensorLog[DOOR_LOG_SIZE];
//int doorSensorLogCounter = 0;
// END: Door Sensor

// BEGIN: Sensor
int lightRaw = 0;
int temperatureRaw = 0;
// END: Sensor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);

  pinMode(SENSOR_LIGHT, INPUT);
  pinMode(SENSOR_TMPR, INPUT);
  pinMode(SENSOR_DOOR, INPUT_PULLUP); // connect to GND
}

void loop() {
  DynamicJsonDocument data(1024);

  // Sensor
  lightRaw = analogRead(SENSOR_LIGHT);
  temperatureRaw = analogRead(SENSOR_TMPR);

  // calcSensor();
  data.clear();
  data["type"] = "sensor";
  data["light"] = lightRaw;
  data["temperature"] = temperatureRaw;
  serializeJson(data, Serial);
  Serial.println("");


  // Door Sensor
  doorRaw = digitalRead(SENSOR_DOOR);
  if (doorRaw != prevDoorRaw) {
    doorCurrentStateSince = millis();
  }

  if (doorRaw == HIGH && millis() - doorCurrentStateSince > 500) { // 0.5sec
    doorIsOpen = true;
  }
  else {
    doorIsOpen = false;
  }

  data.clear();
  if (prevDoorIsOpen != doorIsOpen) {
    data["type"] = "doorSensor";
    data["doorState"] = doorIsOpen ? "open" : "closed";

    serializeJson(data, Serial);
    Serial.println("");
  }

  prevDoorRaw = doorRaw;
  prevDoorIsOpen = doorIsOpen;

}

void serialEvent() {
  StaticJsonDocument<256> serialJson;
  bool jsonError = false;
  DynamicJsonDocument data(1024);

  if (nextSerialJson(&serialJson, &jsonError)) {
    String type = serialJson["type"];

    if (type.equals("null")) { // no type specified in json document
      data["type"] = "noTypeSpecified";
      serializeJson(data, Serial);
      Serial.println("");
    }
    else if (type.equals("nop")) {
      /*
      DynamicJsonDocument data(1024);
      data["type"] = "nopReceived";
      serializeJson(data, Serial);
      Serial.println("");
      */
    }
    else {
      data["type"] = "unknownTypeReceived";
      serializeJson(data, Serial);
      Serial.println("");
    }


  }
  else if (jsonError) {
      data["type"] = "jsonError";
      serializeJson(data, Serial);
      Serial.println("");
  }

}
