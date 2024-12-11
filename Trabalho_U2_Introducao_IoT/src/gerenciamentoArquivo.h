#include <ArduinoJson.h>

JsonDocument readFile(String path);
void writeFile(String path, String field, String value);
void formatFile();
void openFS(void);