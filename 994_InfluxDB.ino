void setInfluxDB()
{
    // Setze Parameter 
    dbClient.setConnectionParamsV1(dbServer, dbDatabase, dbUser, dbPass);
}

void checkDBConnect()
{
    if (dbClient.validateConnection())
        DEBUG_MSG("Verbinde mit InfluxDB: %s\n", dbClient.getServerUrl().c_str());
    else
        DEBUG_MSG("Verbindung zu InfluxDB Datenbank fehlgeschlagen: ", dbClient.getLastErrorMessage().c_str());
}

void sendDBData()
{
    // sensor.clearFields();
    Point dbData("spundomat_status");
    dbData.addTag("Device", nameMDNS);
    if (dbVisTag[0] != '\0')
            dbData.addTag("Sud-ID", dbVisTag);
    dbData.addField("Temperatur", temperature);
    dbData.addField("Pressure", pressure);
    dbData.addField("ZielCO2", setCarbonation);
    dbData.addField("IstCO2", calcCarbonation(pressure, temperature));
    DEBUG_MSG("Sende an InfluxDB: %s\n", dbData.toLineProtocol().c_str());
    if (!dbClient.writePoint(dbData))
    {
        // DEBUG_MSG("InfluxDB Schreibfehler1: %s\n", dbClient.getLastErrorMessage().c_str());
        millis2wait(PAUSE100MS);
        if (!dbClient.writePoint(dbData)) // 2. Versuch in Datenbank zu schreiben
        {
            visState = dbClient.getLastErrorMessage().c_str();   // setze Schreibstatus auf Fehler
            DEBUG_MSG("InfluxDB Schreibfehler: %s\n", dbClient.getLastErrorMessage().c_str());
        }
        else
            visState = "0";
    }
    else
        visState = "0";
}