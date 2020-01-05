void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    yield(); // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("");
  Serial.println("*** SYSINFO: Starte Setup Spundomat");

  if (!SPIFFS.begin())
  {
    Serial.println("*** SYSINFO: Fehler - Dateisystem SPIFFS konnte nicht eingebunden werden!");
  }
  else if (SPIFFS.exists("/config.json")) // Load configuration
  {
    Serial.println("*** SYSINFO: Konfigurationsdatei config.json vorhanden. Lade Konfiguration ...");
    loadConfig();
    if (testModus)
      sensorValueTest = setSensorValueTest(setMode);
  }
  else
    Serial.println("*** SYSINFO: Konfigurationsdatei config.json nicht vorhanden. Setze Standardwerte ...");

  // Verbinde WLAN
  wifiManager.setDebugOutput(false);
  wifiManager.setMinimumSignalQuality(10);
  wifiManager.setConfigPortalTimeout(300);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect(nameMDNS);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  aktIP = WiFi.localIP();
  aktWLAN = WiFi.SSID();
  Serial.print("*** SYSINFO: Verbunden mit WLAN SSID: ");
  Serial.println(aktWLAN);

  // Starte Webserver
  setupServer();

  //Starte mDNS Dienst
  if (startMDNS)
    setMDNS();
  else
  {
    Serial.print("*** SYSINFO: ESP8266 IP Addresse: ");
    Serial.println(aktIP.toString());
  }

  // Starte Telnet Server
  if (startTEL)
    setTELNET();

  // Start I2C
  Wire.begin();
  Wire.beginTransmission(0x27);

  // Start Temperatursensor
  sensors.begin();

  // Pin Definitionen
  if (startMV1)
    pinMode(PIN_MV1, OUTPUT);    // D8
  if (startMV2)
    pinMode(PIN_MV2, OUTPUT);    // D0
  if (startBuzzer)
    pinMode(PIN_BUZZER, OUTPUT); // D4

  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), tick, CHANGE); // D5
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), tick, CHANGE); // D6

  button.attachClick(click);

  // EEPROM
  EEPROM.begin(512);
  offsetVoltage = readFloat(0); // Lese Offset (Kalibrierung)

  setClock();

  // LCD
  startLCD();

  readTemparature();

  // Timer Temperatur einlesen
  os_timer_setfn(&TimerTemp, timerTempCallback, NULL);
  os_timer_arm(&TimerTemp, 30000, true); // Zeitintervall Temperatursensor 30sek
  // Timer Druck einlesen
  os_timer_setfn(&TimerPressure, timerPressureCallback, NULL);
  os_timer_arm(&TimerPressure, 1000, true); // Zeitintervall Drucksensor 1sek

}

// Webserver
void setupServer()
{
  server.on("/", handleRoot);
  server.on("/reboot", rebootDevice);     // Spundomat reboot
  server.on("/kalibrieren", kalibrieren); // Spundomat reboot

  server.on("/reqMisc", handleRequestMisc); // System Infos für WebConfig
  server.on("/setMisc", handleSetMisc);     // Einstellungen ändern
  server.on("/reqMiscSet", handleRequestMiscSet);
  server.on("/reqMode", handlereqMode);
  //server.on("/startHTTPUpdate", startHTTPUpdate);
  server.on("/startHTTPUpdate", startHTTPUpdate);
  // FSBrowser initialisieren
  server.on("/list", HTTP_GET, handleFileList); // list directory
  server.on("/edit", HTTP_GET, []() {           // load editor
    if (!handleFileRead("/edit.htm"))
    {
      server.send(404, "text/plain", "FileNotFound");
    }
  });
  server.on("/edit", HTTP_PUT, handleFileCreate);    // create file
  server.on("/edit", HTTP_DELETE, handleFileDelete); // delete file
  server.on("/edit", HTTP_POST, []() {
    server.send(200, "text/plain", "");
  },
            handleFileUpload);

  server.onNotFound(handleWebRequests); // Sonstiges

  httpUpdate.setup(&server); // ESP8266HTTPUpdateServer
  server.begin();
}
