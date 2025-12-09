#include <SPI.h> // https://github.com/esp8266/Arduino/tree/master/libraries/SPI  
#include <FS.h>            // https://github.com/esp8266/Arduino/tree/master/libraries/FSTools
#include <LittleFS.h>      // https://github.com/esp8266/Arduino/tree/master/libraries/LittleFS
#include <SD.h>            // https://github.com/esp8266/Arduino/tree/master/libraries/SD
#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson

const uint8_t CS_SD = 15;
const uint32_t hardSerialBautRate = 115200;

bool master_loadConfig = false;
bool slave_loadConfig = false;
bool flag_readConfig = false;
bool flag_loadConfig = false;
bool flag_writeConfig = false;
bool flag_saveConfig = false;
bool flag_restart = true;
String receivedString1 = "Valor por defecto 1";
String receivedString2 = "Valor por defecto 2";
String receivedString3 = "Valor por defecto 3";
uint16_t valPinA0 = 0, valPinA1 = 0, valPinA2 = 0, valPinA3 = 0, valPinA4 = 0, valPinA5 = 0;
uint8_t /*valPinD0 = 0, valPinD1 = 0, valPinD2 = 0,*/ valPinD3 = 0, /*valPinD4 = 0,*/ valPinD5 = 0;
uint8_t valPinD6 = 0, valPinD7 = 0, valPinD8 = 0, valPinD9 = 0, valPinD10 = 0, valPinD11 = 0, valPinD12 = 0, valPinD13 = 0;

// Configuracion de FileSystem
const char* fsName = "LittleFS";
FS* fileSystem = &LittleFS;
LittleFSConfig fileSystemConfig = LittleFSConfig();
static bool fsOK;
String unsupportedFiles = String();
File uploadFile;

// Prototipos de funciones
void setupInit(); // Configuraciónes iniciales del hardwardware
void setupHardSerial(); // Configura el Hardware Serial 
void setupSPI(); // Inicializa puerto de comunicacion SPI
void setupFileSystem(); // Configurar Sistema de Archivos Interno ./LittleFS
void setupFileSystemExt(); // Configurar Sistema de Archivos Externo ./SD 
void setupConfigPins(); // Configura los pines como entrada o salida según corresponda
void setupEnd(); // Configuraciones finales y Mensaje de finalización del setup
void printFileConfig(); // Imprime el contenido del archivo de configuración
void readSaveConfig(); // Lee el Archivo de Configuraciones
void writeSaveConfig(); // Escribe el Archivo de Configuraciones
void deleteSaveConfig(); // Elimina el Archivo de Configuraciones

void setup() {
  setupInit(); // Configuraciónes iniciales del hardwardware
  setupEnd();  // Configuraciones finales y Mensaje de finalización del setup
}
void loop() {

}

void setupInit(){ // Configuraciónes iniciales del hardwardware
  setupHardSerial(); // Configura el Hardware Serial 
  setupSPI(); // Inicializa puerto de comunicacion SPI
  setupFileSystem(); // Configurar Sistema de Archivos Interno ./LittleFS
  setupFileSystemExt(); // Configurar Sistema de Archivos Externo ./SD 
  setupConfigPins(); // Configura los pines como entrada o salida según corresponda
  setupEnd(); // Configuraciones finales y Mensaje de finalización del setup
}
void setupHardSerial(){ // Configura el Hardware Serial
  Serial.begin(hardSerialBautRate, SERIAL_8N1); // Inicializa puerto de comunicacion Hardware Serial
  Serial.println(F("Iniciando..."));
  Serial.println(F("Puerto Hardware Serial: Iniciado Correctamente."));
}
void setupSPI(){  // Inicializa puerto de comunicacion SPI
  SPI.begin();
  Serial.println("Puerto SPI: Iniciado Correctamente.");
}
void setupFileSystem(){ // Configurar Sistema de Archivos Interno ./LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Fallo el Punto de Montaje ./LittleFS");
    return;
  }
  fileSystemConfig.setAutoFormat(false);
  fileSystem->setConfig(fileSystemConfig);
  fsOK = fileSystem->begin();
  Serial.println(fsOK ? F("Filesystem inicializado.") : F("Filesystem Fallo!"));
}
void setupFileSystemExt(){ // Configurar Sistema de Archivos Externo ./SD
  if (!SD.begin(CS_SD)) {  // Asegúrate de usar el pin correcto para tu módulo SD
    Serial.println("Error al inicializar SD");
    return;
  }
}
void setupConfigPins(){ // Configura los pines como entrada o salida según corresponda
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println(F("Inicialización y configuración de pines completada."));  
}
void setupEnd(){ // Configuraciones finales y Mensaje de finalización del setup
  // Cargar configuración después de inicializar los sistemas de archivos y configuracion de los pines
  Serial.println("Buscando archivo de configuraciones..");
  readSaveConfig(); // Lee el Archivo de Configuraciones
  printFileConfig(); // Imprime el contenido del archivo de configuración
  Serial.println(F("Inicializando Funciones de Algo++..."));
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(F("Gracias por esperar"));
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println(F("Inicializaciones Terminadas Correctamente"));
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(F("usted es el jefe de los minisuper?"));
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println(F("enserio??"));
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(F("usted???"));
  digitalWrite(LED_BUILTIN, LOW);

}

void printFileConfig() { // Imprime el contenido del archivo de configuración
  StaticJsonDocument<512> doc;

  // Intentar abrir el archivo en LittleFS
  File file = LittleFS.open("/config.txt", "r");
  if (!file) {
    Serial.println(F("Archivo no encontrado en LittleFS. Intentando en SD..."));
    file = SD.open("/config.txt", FILE_READ);
    if (!file) {
      Serial.println(F("Archivo de configuración no encontrado ni en LittleFS ni en SD."));
      return;
    }
  }

  // Deserializar el archivo JSON
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("Error al deserializar el archivo de configuración."));
    file.close();
    return;
  }

  Serial.println(F("Contenido del archivo de configuración:"));
  serializeJsonPretty(doc, Serial); // Imprimir JSON con formato legible
  Serial.println();
  file.close();
}

void readSaveConfig() { // Lee el Archivo de Configuraciones
  StaticJsonDocument<512> doc;

  // Intentar abrir el archivo en LittleFS
  File file = LittleFS.open("/config.txt", "r");
  if (!file) {
    Serial.println(F("Archivo no encontrado en LittleFS. Intentando en SD..."));
    file = SD.open("/config.txt", FILE_READ);
    if (file) {
      Serial.println(F("Archivo encontrado en SD. Copiando a LittleFS..."));

      // Copiar a LittleFS
      File newFile = LittleFS.open("/config.txt", "w");
      if (newFile) {
        while (file.available()) {
          newFile.write(file.read());
        }
        newFile.close();
        Serial.println(F("Archivo copiado desde SD a LittleFS."));
      } else {
        Serial.println(F("Error al crear archivo en LittleFS."));
      }
    } else {
      Serial.println(F("Archivo no encontrado ni en SD ni en LittleFS."));
      // Forzamos la creacion de un archivo de configuraciones con valores predeterminados.
      Serial.println(F("Generando archivo de configuración predeterminado..."));
      writeSaveConfig(); // Guardar configuración predeterminada en LittleFS
      return;
    }
  }

  // Leer configuración desde el archivo
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("Error al deserializar el archivo de configuración."));
    file.close();
    return;
  }

      // Cargar valores del archivo o usar valores predeterminados
      receivedString1 = doc["setString1"].isNull() ? "Valor por defecto 1" : doc["setString1"].as<String>();
      receivedString2 = doc["setString2"].isNull() ? "Valor por defecto 2" : doc["setString2"].as<String>();
      receivedString3 = doc["setString3"].isNull() ? "Valor por defecto 3" : doc["setString3"].as<String>();
  
      // Leer los valores de los pines analógicos
      valPinA0 = doc["valPinA0"] | 0;
      valPinA1 = doc["valPinA1"] | 0;
      valPinA2 = doc["valPinA2"] | 0;
      valPinA3 = doc["valPinA3"] | 0;
      valPinA4 = doc["valPinA4"] | 0;
      valPinA5 = doc["valPinA5"] | 0;

      // Leer los valores de los pines digitales
      valPinD3 = doc["valPinD3"] | 0;
      valPinD5 = doc["valPinD5"] | 0;
      valPinD6 = doc["valPinD6"] | 0;
      valPinD7 = doc["valPinD7"] | 0;
      valPinD8 = doc["valPinD8"] | 0;
      valPinD9 = doc["valPinD9"] | 0;
      valPinD10 = doc["valPinD10"] | 0;
      valPinD11 = doc["valPinD11"] | 0;
      valPinD12 = doc["valPinD12"] | 0;
      valPinD13 = doc["valPinD13"] | 0;

  file.close();
  Serial.println(F("Archivo de configuración cargado correctamente."));
  printFileConfig(); // Imprime el contenido del archivo de configuración
}
void writeSaveConfig() { // Escribe el Archivo de Configuraciones
  StaticJsonDocument<512> doc;

    // Establecer valores actuales en el JSON
    doc[String("setString1")] = receivedString1;
    doc[String("setString2")] = receivedString2;
    doc[String("setString3")] = receivedString3;
    
    // Pines analógicos
    doc["valPinA0"] = valPinA0;
    doc["valPinA1"] = valPinA1;
    doc["valPinA2"] = valPinA2;
    doc["valPinA3"] = valPinA3;
    doc["valPinA4"] = valPinA4;
    doc["valPinA5"] = valPinA5;

    // Pines digitales
    doc["valPinD3"] = valPinD3;
    doc["valPinD5"] = valPinD5;
    doc["valPinD6"] = valPinD6;
    doc["valPinD7"] = valPinD7;
    doc["valPinD8"] = valPinD8;
    doc["valPinD9"] = valPinD9;
    doc["valPinD10"] = valPinD10;
    doc["valPinD11"] = valPinD11;
    doc["valPinD12"] = valPinD12;
    doc["valPinD13"] = valPinD13;

  // Guardar en LittleFS
  File file = LittleFS.open("/config.txt", "w");
  if (!file) {
    Serial.println(F("Error al abrir el archivo para escritura en LittleFS."));
    return;
  }

  // Serializar y escribir
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Error al escribir el archivo de configuración."));
  } else {
    Serial.println(F("Archivo de configuración guardado correctamente."));
  }

  file.close();
  printFileConfig(); // Imprime el contenido del archivo de configuración
}
void deleteSaveConfig() { // Elimina el Archivo de Configuraciones
  if (LittleFS.remove("/config.txt")) {
    Serial.println(F("Archivo de configuración eliminado."));
  } else {
    Serial.println(F("No se pudo eliminar el archivo de configuración."));
  }
}
