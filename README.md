# DOTclock V2.2

DOTclock is an ESP8266-based digital clock that displays time and additional information on a MAX7219 LED matrix. The device also provides a local web interface for setting the clock, managing alarms, and changing the scrolling text.

This repository contains the Arduino source code for the DOTclock V2.2 project.

## Features

- Digital clock display using a 32×8 MAX7219 LED matrix.
- Time and date managed by a DS1307 RTC module.
- Local Wi-Fi access point and web interface.
- Alarm settings that can be added, edited, deleted, and enabled or disabled through the web interface.
- DHT11 temperature and humidity sensor support.
- Scrolling custom text on the LED matrix.
- Prayer time calculation based on the configured location and calculation method.
- Over-the-air (OTA) firmware upload.
- Alarm output through a dedicated output pin.

## Hardware

The source code is configured for a Wemos D1 mini / ESP8266 board.

| Component | Connection | ESP8266 pin |
|---|---|---|
| MAX7219 DIN | Data | D8 (GPIO15) |
| MAX7219 CS | Chip Select | D7 (GPIO13) |
| MAX7219 CLK | Clock | D6 (GPIO12) |
| DHT11 data | Sensor data | D4 (GPIO2) |
| Alarm output | Output signal | D5 (GPIO14) |
| DS1307 SDA | I²C data | D2 (GPIO4) |
| DS1307 SCL | I²C clock | D1 (GPIO5) |

Connect the modules to a suitable power supply and make sure all components share a common ground. Check the voltage requirements of each module before connecting it to the ESP8266.

## Software and Libraries

Use the Arduino IDE with ESP8266 board support installed. The project uses the following libraries:

- **ESP8266WiFi** – Wi-Fi access point functionality.
- **ESP8266WebServer** – local web server.
- **ArduinoOTA** – wireless firmware upload.
- **ESP8266mDNS** – OTA hostname support.
- **ArduinoJson** – handling alarm and configuration data.
- **RTClib** – communication with the DS1307 RTC.
- **DHT sensor library** – reading the DHT11 sensor.
- **PrayerTimes** – calculating daily prayer times.

The project also includes local source files for the MAX7219 display driver, fonts, web page, and EEPROM data handling. These files are part of the repository and should remain in the same sketch folder.

## Repository Structure

```text
DOTclock_V2-2/
├── DOTclock_V2-2.ino   Main program and initialization
├── clock.ino           Clock display and scrolling text
├── alarm.ino           Alarm checking and output control
├── sensor.ino          DHT11 temperature and humidity reading
├── rtc_eeprom.ino      RTC setup and EEPROM data storage
├── rtc_eeprom.h        RTC and EEPROM declarations
├── webserver.ino       Web server routes and request handling
├── webpage.h           Web interface HTML, CSS, and JavaScript
├── wifi_ap.ino         Wi-Fi access point configuration
├── ota.ino             OTA update configuration
├── max7219.h           MAX7219 LED matrix driver
└── fonts.h             LED matrix font definitions
```

`PrayerTimes.zip` is also included in the repository as a supporting library archive.

## Installation and Setup

1. Install the Arduino IDE and add ESP8266 board support.
2. Install the libraries listed in the **Software and Libraries** section.
3. Extract or clone this repository.
4. Open `DOTclock_V2-2.ino` from the `DOTclock_V2-2` folder in the Arduino IDE. Keep all project files together in that folder.
5. Select the appropriate ESP8266 board and port.
6. Check the pin connections and configured location coordinates in the source code.
7. Compile the sketch and upload it to the board.
8. Connect the RTC, LED matrix, DHT11, and other hardware according to the table above.

The RTC is used as the clock time source. The web interface provides a way to set the date and time manually.

## Connecting to DOTclock

After startup, the ESP8266 creates a Wi-Fi access point with the default credentials below:

| Setting | Default value |
|---|---|
| Wi-Fi name (SSID) | `DOTclock` |
| Wi-Fi password | `12345678` |
| Web interface | `http://192.168.4.1` |
| OTA hostname | `DOTclock` |
| OTA password | `12345678` |

Connect a phone or computer to the DOTclock Wi-Fi network, then open `http://192.168.4.1` in a browser.

The web interface can be used to:

- View the available sensor readings.
- Set the RTC date and time.
- Add, edit, remove, and save alarms.
- Set the custom text shown on the LED matrix.

Change the default Wi-Fi and OTA passwords before using the device in an environment where other people may be able to access it.

## Configuration

The default prayer time location is configured in the main program:

```cpp
float latitudeData = -6.16;
float longitudeData = 106.61;
```

Change these values to match the device's installation location. The current calculation settings are configured in `JadwalSholat()` in `rtc_eeprom.ino`, including the Karachi calculation method and Shafii Asr method.

The display pins, DHT11 pin, and alarm output pin are defined in the source files. Update the corresponding definitions if the hardware wiring is changed.

## Notes

- The project is configured for an ESP8266-based board. It may require pin and library adjustments to run on a different microcontroller.
- Alarm settings and custom text are stored in the ESP8266's emulated EEPROM.
- The default access point and OTA credentials are intended as initial settings. Replace them with your own credentials when needed.
- The alarm output is a control signal on D5. Connect an appropriate driver circuit if the alarm device requires more current or a different voltage.
- Prayer time results depend on the configured coordinates and calculation parameters.

## License

No license file is currently included in this repository. Contact the project owner before redistributing or reusing the source code in another project.
