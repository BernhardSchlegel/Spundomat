void loop()
{
  // Webserver (80)
  server.handleClient();

  // Prüfe WiFi status
  if (WiFi.status() != WL_CONNECTED)
  {
    DEBUG_MSG("*** SYSINFO: WLAN nicht verbunden: %s\n", WiFi.status());
    DEBUG_MSG("*** SYSINFO: WLAN IP %s\n", WiFi.localIP().toString().c_str());
    WiFi.mode(WIFI_STA);
    WiFi.begin();
  }

  // Check mDNS
  if (startMDNS)
    MDNS.update();

  // Check NTP
  timeClient.update();

  // Check DS18B20 Ticker
  TickerTemp.update();

  // Check Encoder Ticker
  TickerEncoder.update();
  TickerButton.update();

  // InfluxDB Ticker
  TickerInfluxDB.update();

  if (reflashLCD)
    showLCD();

  // Betriebsmodi
  switch (setMode)
  {
  case AUS: // aus
    // Check Drucksensor Ticker
    TickerPressure.update();
    break;
  case SPUNDEN_CO2: // CO2 Spunden
    // TickerPressure.pause();
    updateMV1();
    break;
  case SPUNDEN_DRUCK: // Druck Spunden
    // TickerPressure.pause();
    updateMV1();
    break;
  case KARBONISIEREN_CO2: // CO2 Karbonisieren
    // TickerPressure.pause();
    updateMV2();
    break;
  case KARBONISIEREN_DRUCK: // CO2 Karbonisieren
    // TickerPressure.pause();
    updateMV2();
    break;
  case SPUNDOMAT: // CO2 Spunden & Karbonisieren
    TickerSpundomat.update();
    break;
  case PLAN1:
  case PLAN2:
  case PLAN3:
    // TickerPressure.pause();
    startPlan();
    break;
  }
  up = false;
  down = false;
}
