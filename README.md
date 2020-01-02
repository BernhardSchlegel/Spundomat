![ov1](/Info/Spundomat01.jpg)

# Spundomat

## Changelog

Version 2.01 - 2.02e
- Update:   Bibliotheken aktualisiert
- Add:      Dateisystem SPIFFS
- Add:      Konfigurationsdatei im JSON Format
- Add:      Webserver
- Add:      mDNS, Telnet
- Add:      ESP WebUpdate
- Add:      Modus Zwangskarbonisieren
- Add:      Anzeige IP und WLAN im Display
- Add:      Menü Speichern der Konfiguration im Display
- Removed:  Delay Befehle

## Generelle Informationen
### Was ist Spundomat?

Spunden https://hobbybrauer.de/forum/wiki/doku.php/lagern:spunden

Unter dem Spunden versteht man Bier in einem druckdicht abgeschlossenen Faß oder Gärbehälter 
kontrolliert zu Carbonisieren damit Kohlendioxid im Bier gelöst wird. 
Um eine Explosion des Druckfasses zu vermeiden und einen erwünschten Kohlensäuregehalt des Bieres 
laut Spundungstabelle zu erreichen muss ein einstellbares Druckbegrenzungsventil, 
das Spundventil (Spunddruckregler) am Fass angebracht werden. 
Bei der Spundung muss keine genaue Menge an Zucker oder Würze zugegeben und berechnet werden, 
weil das Spundventil überschüssiges CO2 automatisch ablässt.
Am einfachsten schlaucht man nach 5-7 Tagen in die Drucktanks oder Kegs und hängt das nach 
Tanktemperatur (bzw. Raumtemperatur wenn nicht temperaturreguliert) richtig eingestellte Spundventil an.

Zum Anschließen, Umkonfigurieren, Carbonisieren und leichten Reinigen der CO2-Schäuche eignen 
sich Pneumatische Schnellkupplungen hervorragend.

### Was bietet diese Firmware?

* Ein Web Interface (WebIf) für die Konfiguration
* Encoder Unterstützung
* Sensoren
  * Temperatursensor
  * Drucksensor
* Aktoren
  * PIN Auswahl
  * PINs in Verwendung werden ausgeblendet
  * Invertierte GPIO
  * Power Percentage: Es werden Werte zwischen 0 und 100% gesendet. Das ESP8266 "pulses" mit einem Zyklus von 1000ms
* OLED display Integration
* mDNS Server Support
* Telnet Client Support (putty)


### Voraussetzungen: (2020.01)

* Arduino IDE 1.8.10
* Optional Microsoft VSCode + Arduino + ESP8266FS
* ESP8266 by ESP8266 Community version 2.6.3
* Folgende Bibliotheken müssen über die Arduino IDE hinzugefügt werden:
    * ArduinoJSON by Benoit Blanchon Version 6.13.0 
    * DallasTemperature by Miles Burton Version 3.8.0
    * OneWire By Jim Studt Version 2.3.5
    * WiFiManager by tzapu Version 0.15.0
    * EEPROM.h
    * RotaryEncoder.h
    * OneButton.h
    * Wire.h
    * LiquidCrystal_PCF8574.h
    * FS.h

### Hardware: die Platine zum Projekt



### Wie kann die Firmware geflashed werden ohne den Quellcode zu komplilieren

* Mit Hilfe von esptool.exe (see https://github.com/igrr/esptool-ck/releases ) aus dem Ordner tools kann die Firmware auf das ESP Modul geladen werden

Beispiel für ein ESP8266 Modul vom Typ D1 mini mit 4MB Flash verbunden mit COM3

	* Eingabeaufforderung öffnen

	* in den Order ./MQTTDevice/build wechseln

		* Firmware aufspielen: ../tools/esptool.exe -ca 0x000000 -cd nodemcu -cp COM3 -cb 921600 -cf Spundomat.ino.bin

	* Das ESP8266 Modul resetten

	* Das ESP8266 Modul startet anschließend im Access Point Modus mit der IP Adresse 192.168.4.1

	* Das ESP8266 Modul über einen Webbrowser mit dem WLAN verbinden

    * Das SPIFFS kann nun über das WebIf ausgespielt werden <ip-address>/update

    * alternativ mit esptool: ../tools/esptool.exe -ca 0x100000 -cd nodemcu -cp COM3 -cb 921600 -cf Spundomat.spiffs.bin


* Updates
	Updates (firmware und SPIFFS) können über das WebIf geladen werden: <IP Adresse ESP Modul>/update

* Backup and restore der Konfiguration
    Der FileBrowser ist erreichbar über <IP Adresse ESP Modul>/edit download oder upload config.json 
