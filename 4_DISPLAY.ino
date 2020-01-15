// LCD Ausgabe
void showLCD()
{
  reflashLCD = false;
  switch (page)
  {
  case 1: // Startseite
    Menu1[0] = "CO2:   ";
    Menu1[0] += calcCarbonation(pressure, temperature);
    Menu1[0] += "g/l ";
    if (setMode == 0)
      Menu1[0] += "Aus";
    else if (setMode == 1)
      Menu1[0] += "Spund";
    else if (setMode == 3)
      Menu1[0] += " Karb";
    Menu1[1] = "Druck: ";
    if (pressure == -1)
      Menu1[1] += "-       ";
    else
    {
      Menu1[1] += pressure;
      Menu1[1] += "bar ";
    }
    if (setMode == 2)
      Menu1[1] += "Spund";
    Menu1[2] = "Temp: ";
    Menu1[2] += sTemperature;
    Menu1[2] += "\337C"; // °C
    Menu1[3] = "Volt:  ";
    Menu1[3] += voltage;
    Menu1[3] += "V";

    // Navigation
    if (up)
    {
      menuitem++;
      if (menuitem > 1)
      {
        menuitem = 1;
      }
    }
    else if (down)
    {
      menuitem--;
      if (menuitem < 0)
      {
        menuitem = 0;
      }
    }

    if (buttonPressed)
    {
      menuitem = 0;
      page = 2;
      reflashLCD = true;
    }

    // Ausgabe
    if (menuitem == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(Menu1[menuitem]);
      lcd.setCursor(0, 1);
      lcd.print(Menu1[(menuitem + 1)]);
      lcd.setCursor(0, 2);
      lcd.print(Menu1[(menuitem + 2)]);
      lcd.setCursor(0, 3);
      lcd.print(Menu1[(menuitem + 3)]);
    }
    else
    {
      aktIP = WiFi.localIP();
      aktWLAN = WiFi.SSID();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("IP   ");
      lcd.print(aktIP.toString());
      lcd.setCursor(0, 1);
      lcd.print("SSID ");
      lcd.print(aktWLAN);
    }
    break;

  case 2:                   // switch(page) Einstellungen
    Menu2[0] = "Zur\365ck"; // ü
    Menu2[1] = "Modus: ";
    Menu2[1] += modes[setMode];
    Menu2[2] = "Soll: ";

    if (setMode <= 1 || setMode == 3) // Aus oder CO2 oder Karbonisierung
    {
      Menu2[2] += setCarbonation;
      Menu2[2] += " g/l";
    }
    else // Druck
    {
      Menu2[2] += setPressure;
      Menu2[2] += " bar";
    }

    Menu2[3] = "Kalibrierung";
    Menu2[4] = "Speichern";

    // Navigation
    if (up && edititem == 0)
    {
      menuitem++;
      if (menuitem > 4)
      {
        menuitem = 4;
      }
    }
    else if (down && edititem == 0)
    {
      menuitem--;
      if (menuitem < 0)
      {
        menuitem = 0;
      }
    }

    if (buttonPressed)
    {
      switch (menuitem)
      {
      case 0: // Zurück
        page = 1;
        menuitem = 0;
        reflashLCD = true;
        lcd.noBlink();
        break;
      case 1:
      case 2:
        if (edititem == 0)
        {
          edititem = menuitem;
        }
        else
        {
          edititem = 0;
        }
        break;
      case 3: // Kalibriere (speichere Offset)
        page = 3;
        menuitem = 0;
        reflashLCD = true;
        lcd.noBlink();
        break;
      case 4: // Einstellungen speichern
        page = 4;
        menuitem = 0;
        reflashLCD = true;
        lcd.noBlink();
        break;
      }
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">");
    lcd.print(Menu2[menuitem]);
    if ((menuitem + 1) <= 4)
    {
      lcd.setCursor(1, 1);
      lcd.print(Menu2[(menuitem + 1)]);
    }

    // Ändere Einstellungen
    switch (edititem)
    {
    case 1: // Modus einstellen
      lcd.setCursor(8, 0);
      lcd.blink();
      if (up && setMode < sizeOfModes - 1) // Aus oder CO2
      {
        setMode++;
        reflashLCD = true;
      }
      else if (down && setMode > 0) // CO2 oder Druck
      {
        setMode--;
        reflashLCD = true;
      }
      break;

    case 2: // Sollwert
      lcd.setCursor(7, 0);
      lcd.blink();
      if (up && (setMode <= 1 || setMode == 3)) // Aus oder CO2 oder Karbonisieren
      {
        setCarbonation += 0.05;
        reflashLCD = true;
      }
      else if (down && (setMode <= 1 || setMode == 3))
      {
        setCarbonation -= 0.05;
        reflashLCD = true;
      }
      else if (up && setMode == 2) // Druck
      {
        setPressure += 0.05;
        reflashLCD = true;
      }
      else if (down && setMode == 2) // Druck
      {
        setPressure -= 0.05;
        reflashLCD = true;
      }
      break;

    default:
      lcd.noBlink();
      break;
    }
    break;

  case 3: // Switch(page) Kalibrierung
    Menu3[0] = "Kalibrieren?";
    Menu3[1] = "Nein          Ja";

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(Menu3[0]);
    lcd.setCursor(0, 1);
    lcd.print(Menu3[1]);
    lcd.setCursor(0, 1);
    lcd.blink();

    // Navigation
    if (up)
    {
      menuitem++;
      lcd.setCursor(14, 1);
      if (menuitem > 1)
      {
        menuitem = 1;
      }
    }
    else if (down)
    {
      menuitem--;
      lcd.setCursor(0, 1);
      if (menuitem < 0)
      {
        menuitem = 0;
      }
    }

    if (buttonPressed)
    {
      switch (menuitem)
      {
      default: // Auswahl Nein - Zurück
        page = 2;
        menuitem = 0;
        reflashLCD = true;
        lcd.noBlink();
        break;
      case 1: // Auswahl Ja - Kalibrierung
        readPressure();
        offsetVoltage = voltage;
        writeFloat(0, offsetVoltage);
        readPressure();
        page = 2;
        menuitem = 0;
        reflashLCD = true;
        lcd.noBlink();
        break;
      }
    }
    break;
  case 4: // Switch(page) Speichern
    Menu4[0] = "Speichern?";
    Menu4[1] = "Nein          Ja";

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(Menu4[0]);
    lcd.setCursor(0, 1);
    lcd.print(Menu4[1]);
    lcd.setCursor(0, 1);
    lcd.blink();

    // Navigation
    if (up)
    {
      menuitem++;
      lcd.setCursor(14, 1);
      if (menuitem > 1)
      {
        menuitem = 1;
      }
    }
    else if (down)
    {
      menuitem--;
      lcd.setCursor(0, 1);
      if (menuitem < 0)
      {
        menuitem = 0;
      }
    }

    if (buttonPressed)
    {
      switch (menuitem)
      {
      case 1: // Auswahl Ja - Speichern
        saveConfig();
        page = 2;
        menuitem = 0;
        reflashLCD = true;
        lcd.noBlink();
        break;
      default: // Auswahl Nein - Zurück
        page = 2;
        menuitem = 0;
        reflashLCD = true;
        lcd.noBlink();
        break;
      }
    }
    break;
  } // Switch(page)
  buttonPressed = false;
}

void startLCD()
{
  lcd.clear();
  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.setCursor(0, 0);
  lcd.print(" Spundomat V");
  lcd.print(Version);
  lcd.setCursor(0, 2);
  lcd.print("IP   ");
  lcd.print(aktIP.toString());
  lcd.setCursor(0, 3);
  lcd.print("WLAN ");
  lcd.print(aktWLAN);
  millis2wait(PAUSE5SEC);
}
