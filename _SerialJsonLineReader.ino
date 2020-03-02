// SerialJsonLineReader
// ver 0.1.0
//
// Requirements:
// #include <ArduinoJson.h>

String serialBuffer = "";

bool nextSerialLine(String *serialLine) {
  *serialLine = "";

  while (Serial.available() > 0) {
    int ch = Serial.read();
    if (ch != -1) {
      if (ch == '\n') {
        *serialLine = serialBuffer;
        serialBuffer = "";
        return true;
      }

      serialBuffer += (char) ch;
    }
  }

  return false;
}

bool nextSerialJson(JsonDocument *serialJson, bool *jsonError) {
  String serialLine = "";

  *jsonError = false;
  serialJson->clear();

  if (nextSerialLine(&serialLine)) {
    DeserializationError error;
    error = deserializeJson(*serialJson, serialLine);

    if (error == DeserializationError::Ok) {
      return true;
    }

    *jsonError = true;
    serialJson->clear();
  }

  return false;
}
