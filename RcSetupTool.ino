#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
// #define DEBUG_ESP_MDNS

#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <Servo.h>

// *** CONFIGURAITON ***
// Used Ports as as summary (WiFi-Kit-8)
/*
D0 : OLED Reset
D1 : I2C-SCL OLED + GY521
D2 : I2C-SDA OLED + GY521
D3 : Drehwinkelgeber CLK
D4 : free
D5 : ServoOut
D6 : Drehwinkelgeber PushButton
D7 : Drehwinkelgeber DT
D8 : Buzzer
*/

#define WIFI_KIT_8 
#define OLED
#define CURRENT_SENSOR
#define ROTARY_ENCODER
#define SERVO_PIN D5
#define PIN_BUZZER_OUT D8
#define PIN_PUSH_BUTTON D6

// **** Voltage measurement settings ****

// analog input pin
#define VOLTAGE_PIN                   A0

// supply voltage

static const char myName[] = "RcSetupTool";
long ourDebug1;
long ourDebug2;
long ourDebug3;



#ifdef OLED
#include <U8g2lib.h>        // Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

#ifdef WIFI_KIT_8 
  // Wifi Kit 8 has a fixed wired 128x32 display 
  // U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C ourOLED(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4); 
  U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C ourOLED(U8G2_R0, /* reset=*/ D0, /* clock=*/ D1, /* data=*/ D2); 
#else 
  U8G2_SH1106_128X64_NONAME_1_HW_I2C ourOLED(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ D3, /* data=*/ D4); 
  //U8G2_SSD1306_128X64_NONAME_1_HW_I2C ourOLED(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ D3, /* data=*/ D4); 
#endif 
#endif

#ifdef ROTARY_ENCODER
#include <Encoder.h>
Encoder ourRotaryEncoder(D7, D3);
#define RE_MULITPLIER_SLOW 1
#define RE_MULITPLIER_NORMAL 5
#define RE_MULITPLIER_FAST 15
long ourRotaryEncoderPosition=0;
long ourRotaryMenuPosition=0;
uint8_t ourRotaryEncoderMultiplier;

#include <Bounce2.h>
Bounce2::Button ourPushButton = Bounce2::Button();
#endif

String ourOLEDLogString;


#define OTA
#ifdef OTA
#include <ArduinoOTA.h>
#endif


#include "htmlAdminPage.h"
#include "htmlExpertPage.h"
#include "htmlMenuPage.h"
#include "htmlServoPage.h"
#include "htmlAngleSensorPage.h"
#include "htmlMultiToolPage.h"
#include "htmlShowProtocolTablePage.h"
#include "htmlScript.h"
#include "htmlStyles.h"
#include "Config.h"


// !!!!!!!!!!!!!!!!!!
// if you enable this define, the sligthly patched lib
// Adafruit_MMA8451_Library from 
// https://github.com/Pulsar07/Adafruit_MMA8451_Library
// has to be added to the arduino library folder
// #define SUPPORT_MMA8451
// !!!!!!!!!!!!!!!!!!
#ifdef SUPPORT_MMA8451
#include <Adafruit_MMA8451.h>         // MMA8451 library
#endif
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
// #define USE_MPU6050_MPU
#ifdef USE_MPU6050_MPU
#include "MPU6050_6Axis_MotionApps20.h"
#else
#include "MPU6050.h"
#endif

// #define DO_LOG

// Version history
// V0.10 : full functional initial version
//         merge of ServoController and RuderwegMesssensor
// V0.11 : protocol page to multiToolPage added and some bug fixes
// V0.12 : some small beautifications
// V0.13 : support for all ASCII special chars in SSID and passwords
//         more enhanced preset and protocol behaviour
// V0.14 : provided patched Adafruit_MMA8451_Library in the repository
// V0.15 : fixed support for USE_MPU6050_MPU
// V0.16 : fixed layout problems in angle measure view and bug in audio context for reference value handling
// V0.161 : fixed CTRL-Key handling for Macs
// V0.162 : expert menu now as a separate button, and AJAX get requets will work with ESP32 web server
// V0.170 : first draft: support for working OLED GUI for servo page, angle page and settings for angle tara, rudder depth, servo steps and servo null value

#define APP_VERSION "V0.170"

/**
 * \file RcSetupTool.ino
 *
 * \brief small tool to control an rc-servo and measure the rudder throw via a web browser
 *
 * \author Author: Rainer Stransky
 *
 * \copyright This project is released under the GNU Public License v3
 *          see https://www.gnu.org/licenses/gpl.html.
 * Contact: opensource@so-fa.de
 *
 */

/**
 * \mainpage RC-Einstell-Tool Projekt zur Ansteuerung eines Modellflug-Servos und zum Vermessen von Ruderausschlägen mittels eines Winkelsensors
 *
 * \section intro_sec_de Übersicht
 * Das RC Einstell Tool kann eine Servo mittels PWM Pulserzeugung steuern und die Ansteuerdaten anzeigen.
 * Zusatzlich kann mittels eines Winkelsensors der Ruderausschlag sehr genau vermessen werden (besser als 0.5mm).
 * Die Benutzeroberfläches ist als Web-Oberfläche ausgelegt, was eine Bedienung auch mit dem Smartphone zulässt.
 * Eine einfach kleine Power-Bank dient als Stromversorgung und man kann extrem einfach und schnell auf der Werkbank beim
 * Bau oder bei Endeinstellarbeiten alle wichtigen Daten schnell, genau und vollkommen reproduzierbar ablesen.
 * Hiermit lassen sich Einstellarbeiten auf der Werkbank oder am aufgebauten Modell sehr professionell, schnell,
 * genau und reproduzierbar durchführen.
 * Dieses Projekt ist die Zusammenführung von https://github.com/Pulsar07/RuderwegMessSensor und https://github.com/Pulsar07/ServoController
 * ![Architektur](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_Architecture.png)
 *
 * \section hardware_sec_de Hardware
 * \subsection hardware_subsec_de_mk Mikrokontroller
 * Als Mikrokontroller wird der Wemos D1/ESP8266 benutzt, der ausreichende Rechenpower und
 * Speicherresourcen bietet und eine WLAN Schnittstelle hat. Es stehen ausgereifte Bibliotheken zur Nutzung der WiFi
 * Schnittstelle, zur Bereitstellung eines Web-Servers (GUI) und auch zur Ansteuerung von Servos zur Verfügung.
 *
 * Hier ein paar Links:
 * * https://www.az-delivery.de/products/d1-mini
 * * https://github.com/esp8266/Arduino
 *
 * \subsection hardware_subsec_de_ms Messsensor
 * Als Messsensor wird der GY-521/MPU-6050 benutzt. Die Genauigkeit liegt nach Kalibrierung bei
 * Winkeln bis +/- 45° kleiner als 0.5°. Der Baustein MPU-6050 wird von einer wirklich sehr gut
 * gemachten Libs von J.Rowberg unterstützt (siehe Link)
 * Der Aufbau sollte auch ohne den Winkelsensor funktionieren, um nur die Funktion des Servo-Controllers zu erhalten.
 *
 * Hier ein paar Links:
 * * https://www.invensense.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
 * * https://www.az-delivery.de/products/gy-521-6-achsen-gyroskop-und-beschleunigungssensor
 * * https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050
 *
 * \subsection hardware_subsec_de_sc Servo-Controller
 * Für den Servo-Controller ist außer dem Signal-Kabel vom D7-Pin des Microcontrollers keinerlei Hardware notwendig.
 * Will man nur einen Winkelmesser bauen, kann dieses Kabel einfach weggelassen werden.
 *
 * \subsection hardware_subsec_de_sp Schaltplan
 * Der Schaltplan ist denkbar einfach. Es werden nur 4 Verbindungen zwischen Sensorplatine und
 * Mikrokontroller benötigt. Das Layout und die Software sind so ausgelegt, dass mit einer Stiftleiste
 * 4x1 das Sensorboard mit dem Gesicht in Richtung Mikrokontroller direkt verlötet werden kann.
 * ![Schaltplan](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_CircuitDiagram.png)
 * <br>Der Servo wird lediglich mittels eines Signal-Kabels auf den D7 Pin des Microcontrollers verbunden.
 * Die Plus- und Ground-Verbindung wird mittels eines auftrennten und wieder zusammengefügten USB-MicroUSB Kabels
 * hergestellt. Die Servo Stromversorgung wird einfach mittels eines Servo-Buchsenkabels erstellt und
 * sollte nicht über den Microkontroller erfolgen, da die Stromstärke zu hoch für
 * diesen ist.

 * \subsection hardware_subsec_de_kv Ruderklemmvorrichtung
 * Hier kann der geneigte Modellbauer die Kombination aus Microprozessor und Sensor in eine geeignete Klemmvorrichtung [ein-]bauen,
 * die ein rutschfestes Klemmen am Ruder gewährleistet.
 * So sieht der Prototyp des Authors aus:
 * ![Klemm](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_InOperation.jpg)
 *
 * \section hmi_sec_de Anleitung
 * \subsection hmi_subsec_de_ib Inbetriebnahme
 * * Stromversorgung
 *  * Der Sensor / das Mikroprozessorboard ist mit einem Micro-USB-Anschluss ausgestattet,
 * hier kann man jedes handelsübliche USB-Netzteil anschließen oder besser jede
 * normale Powerbank. Damit ist man in der Werkstatt oder auf dem Flugfeld mobil ausgestattet.
 * * WiFi
 *  * Der Sensor muss zuerst mit Smartphone oder PC verbunden werden. Dazu stellt
 * der Sensor per WiFi einen Accesspoint mit der SSID "UHU" und Kennwort "12345678"
 * zur Verfügung. Ist das Gerät mit diesem WLAN verbunden, kann im Web-Browser
 * über die Adresse http://192.168.4.1 die Benutzeroberfläche benutzt und der Sensor
 * konfiguriert werden. Sowohl obige SSID als auch das Kennwort können danach geändert werden.
 *  * Auf der Einstellseite kann eine SSID und ein Kennwort für ein WLAN (WLAN-Client)
 * konfiguriert werden, mit dem sich der Sensor verbinden. Dabei wird dem Sensor
 * eine IP-Adresse zugewiesen, die am WLAN-Router abgefragt werden muss. Änderungen
 * der WLAN Einstellungen müssen gespeichert werden und werden erst nach Neustart aktiv.
 *  * Ist die Verbindung zu einem WLAN konfiguriert (WLAN-Client), kann auf der
 * Einstellungsseite, der Accesspoint deaktiviert werden (nach Speichern und Neustart).
 * Kann beim Neustart keine Verbindung zum konfigurierten WLAN aufgebaut werden,
 * wird der Accesspoint-Mode aber trotzdem aktiviert, damit ein Zugang zum Gerät möglich ist.
 * * Nutzung des Sensorboard GY-521 mit MPU-6050
 *  * Genauigkeit: Der MEMS Chip des MPU-6050 sollte Winkelauflösungen besser als 0.5°
 * bei 45° Ausschlag messen können, was bei einer 60mm Rudertiefe von 60mm einen Fehler von kleiner als 0.5mm ergibt.
 * Zudem sind diverse Anzeigegenauigkeiten für die Winkel und die Ruderwegs-Messung auswählbar.
 * Die Anzeige hat zwar immer 2 Dezimalstellen, intern wird aber gerundet.
 *  * Experten-Einstellungen: 
 *   * Kalibrierung: Damit der MPU-6050 allerdings diese Genauigkeit erreicht, muss
 * er nachträglich kalibriert werden. Die Software unterstützt diese Funktion und
 * kann die Werte intern speichern. Zur Kalibrierung muss die GY-521-Sensorplatine
 * mit der flachen Rückseite möglichst exakt horizontal aufgelegt werden. Dann den
 * Kalibrier-Button drücken und ca. 5s warten bis die Kalibrierung beendet ist.
 * Dabei sollte die Auflagefläche (Tisch) nicht bewegt werden und frei von Vibrationen sein.
 *   * Einbaulage: Die Sensorplatine sollte auch genau so, wie bei der Kalibrierung,
 * betrieben werden. Also die flache Seite nach unten und die Seite mit den
 * Elektronikbausteinen nach oben. Nur so wird die oben genannte Genauigkeit erreicht.
 *   * Achsen und Anzeige-Genauigkeit: Auf der Konfigurationsseite, kann die Bezugs-Achse
 * der Winkelmessung, je nach Einbaulage in der Klemmeinrichtung ausgewählt werden.
 *   * Kalibrierungsoffset: Hier können Kalibrierungs-Messwerte für +/- 45° Referenzmessungen eingebeben werden und
 * aktiviert/deaktiviert werden, um die höchst mögliche Genauigkeit zu erreichen. Damit werden
 * dann die Messwerte auf die Offsetwerte interpoliert.
 * Der Grund hierfür ist, wie die Erfahrung zeigt, dass die verfügbaren günstggen China-Importe, nicht
 * die höchste Qualität aufweisen. Z.T. lassen sich diese Sensoren
 * einfach nicht kalibrieren. Ein manuelles Kalibrieren mit diesem Kalibrierungsoffset, bringt jedoch
 * meist den gewünschten Erfolg
 * (siehe Experten-Einstellungen) und gewährleistet genaues Arbeiten.
 *
 * \subsection hmi_subsec_de_me  Winkel-Messung
 * * Der mit dem Mikrokontroller verbundene Messensor sollte mit einer Klemmvorrichtung fest
 * verbunden sein, und kann dann einfach an eine beliebige Stelle des Ruders aufgeklemmt werden.
 * Die Ruderdrehachse, sollte möglichst parallel zur ausgewählten Dreh-Achse (X- oder Y-Achse)
 * sein. Wie schon beschrieben, muss der Sensor mit dem Gesicht nach oben auf dem Ruder
 * befestigt sein.
 * * Einschränkungen: Der Sensor kann nur Winkel in Bezug auf die Schwerkraft messen.
 * Somit sind Ruderwegsmessungen für das Seitenruder nur möglich wenn der Rumpf um
 * 90° gedreht liegt.
 * * Der Ruderweg ist abhänig von der Rudertiefe. Diese ist an der Stelle zu Messen,
 * an der man den Ruderweg messen will. In der Web-Oberfläche des Sensor kann
 * diese Rudertiefe eingegeben werden.
 * * Ist der Sensor so auf dem Ruder angebracht, und die Rudertiefe eingestellt,
 * ist die Ruderstellung in die Null-Lage zu bringen. Jetzt können Winkel
 * und Ruderweg per "Tara"-Button auf 0 gesetzt werden.
 * * Bewegt man das Ruder nun nach oben oder unten werden die Ausschläge in Grad und
 * Millimeter angezeigt. Sollte das Vorzeichen nicht den Erwartungen entsprechen,
 * kann dies bei den Einstellungen angepasst werden.
 * * Zur Flugphasenmessung kann die Min-/Max-Rudermessung benutzt werden.
 * Hier sollte man das Ruder in die Neutralstellung der Flugphase bringen.
 * Nun den Schalter für die Min-/Max-Ruderweg-Messung aktivieren. Damit wird der
 * aktuelle Ruderausschlag als Neutralwert übernommen. Jetzt können die beiden
 * Min-/Max-Werte angefahren werden. Alle drei Werte werden bis zur
 * Deaktiverung der Messung angezeigt.
 * ![AngleSensorPage](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_AngleSensorPage.png)
 *
 * \subsection hmi_subsec_de_sc  Servo-Controller
 * Die Bedienung am Web-GUI ist denkbar einfach. Die Servo-Position kann über zwei
 * Eingabefelder prozentual oder als Impulsbreite gesteuert werden. Zusätzlich kann die Servoposition
 * über ein Slider-Widget gesteuert werden.
 * Zudem erlaubt eine aktivierbare Maus-Wheel Funktion, mit Beschleunigungsfaktor, das Steuern des Servowegs
 * mittels der Maus.
 * Es sind für diverse Aufgaben noch 5 vordefinierte und einstellbare (Save) Positions-Buttons
 * verfügbar.
 * Die Limit-Buttons, können zum Begrenzen des Servo-Wegs benutzt werden, um ein versehntliches Überfahren eines
 * mechanischen Limits zu verhindern.
 * Auf der Einstellseite, können die Limits und 100%-Settings für verschiedene Hersteller von RC-Systemen voreingestellt werden,
 * damit die Anzeige exakt mit den Werte des genutzten RC-System übereinstimmt.
 * ![ServoPage](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_ServoPage.png)
 *
 * \subsection hmi_subsec_de_ms  Steuern und Messen
 * In dieser Ansicht kann sowohl der Servo gesteurt als auch der Winkel-Sensor abgelesen werden.
 * Dies ist vor allem bei Servo- und Gestänge-Einbauten ein große Hilfe.
 * ![MultiToolPage](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_MultiToolPage.png)
 * Zusätzlich können den in den Preset-Buttons gespeicherten Servo-Werte
 * (und die gemessenen Winkel- und Ruderauschlagswerte) und eine Nutzer-Beschreibung
 * des Datensatzes festgehalten und mittels der Funktion "Zeige Protokoll"
 * in Tabellenform ausgegeben werden.
 * ![ProtocolPage](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_ProtocolPage.png)
 *
 * Weitere Details gibt es unter <a href="http://www.so-fa.de/nh/RcSetupTool">Albatross, Seite für Modellflug und Technik</a>
 *
 * \subsection hmi_subsec_de_flash  Bin-Datei direkt zum Flashen
 *  ist im bin-Verzeichnis verfügbar.
 *  Download NodeMCU-Flasher (z. Bspl. Von hier: https://github.com/marcelstoer/nodemcu-pyflasher/releases/tag/v4.0  )<br>
 * ![UploadBin](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_UploadBin.jpg)
 * <br>Flasher-Programm starten und Port wählen, Bin-Datei öffnen und 115200 Baud einstellen.<br>
 * Flash-Button drücken.<br>
 * Das war es ;-)<br>
 */


// #define MYSEP €
// #define MYSEP ";"
#define MYSEP_STR "~~~"
const byte DNS_PORT = 53;
DNSServer ourDNSServer;
IPAddress ourApIp(192,168,4,1);
IPAddress ourNetmask(255,255,255,0);

static configData_t ourConfig;

#ifdef SUPPORT_MMA8451
static Adafruit_MMA8451 mma;
#endif
static MPU6050 mpu;
static const uint8_t MPU6050ADDR1 = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
static const uint8_t MPU6050ADDR2 = 0x69; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
#ifdef SUPPORT_MMA8451
static const uint8_t MMA8451ADDR1 = 0x1C; // I2C address of the MMA-8451. If AD0 pin is set to LOW, the I2C address will be 0x1C.
static const uint8_t MMA8451ADDR2 = 0x1D; // I2C address of the MMA-8451. If AD0 pin is set to LOW, the I2C address will be 0x1C.
#endif
static uint8_t ourSCL_Pin;
static uint8_t ourSDA_Pin;
static uint8_t ourI2CAddr;
static String ourSensorTypeName;
static boolean ourTriggerCalibrateMPU6050 = false;
static unsigned long ourTriggerRestart = 0;

int16_t ourAccelerometer_x, ourAccelerometer_y, ourAccelerometer_z; // variables for ourAccelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
static double ourTara = 0;
static double ourSmoothedAngle_x = 0;
static double ourSmoothedAngle_y = 0;
static double ourSmoothedAngle_z = 0;
static double ourTaraAngle_x = 0;
static double ourTaraAngle_y = 0;
static double ourTaraAngle_z = 0;
static double ourSmoothedGyro_x = 0;
static double ourSmoothedGyro_y = 0;
static double ourSmoothedGyro_z = 0;
static double ourTaraGyro_x = 0;
static double ourTaraGyro_y = 0;
static double ourTaraGyro_z = 0;
static double ourRudderSize = 30;
static double ourTargetAmplitude = 5;

static float ourNullAmpl;
static float ourMinAmpl;
static float ourMaxAmpl;
static boolean ourIsMeasureActive=false;
float mySmoothedCurrent = 0.0f;
float ourServoCurrent;

enum ToolContext {
  BASE_MENU_PAGE,
  SERVO_MENU_PAGE,
  SERVO_PAGE,
  ANGLE_SENSOR_PAGE,
  ADMIN_PAGE,
  SET_SERVO_STEPS,
  SET_RUDDER_DEPTH,
  EXPERT_ADMIN_PAGE,
  DEBUG_PAGE,
};

ToolContext ourContext=BASE_MENU_PAGE;
const char* ourBaseMenu0 = "0:Servotester";
const char* ourBaseMenu1 = "1:Winkelmesser";
const char* ourBaseMenu2 = "2:Debug-Anzeige";
const char* ourBaseMenuItems[] = {ourBaseMenu0, ourBaseMenu1, ourBaseMenu2};
const uint8_t ourBaseMenuSize = sizeof(ourBaseMenuItems) / sizeof(char*);;

const char* ourServoMenu0 = "0:Servo-Pos=0%";
const char* ourServoMenu1 = "1:Tara-Winkel";
const char* ourServoMenu2 = "2:Setze Rudertiefe";
const char* ourServoMenu3 = "3:Setze Servoschritte";
const char* ourServoMenu4 = "4:zurück";
const char* ourServoMenuItems[] = {ourServoMenu0, ourServoMenu1, ourServoMenu2, ourServoMenu3, ourServoMenu4};
const uint8_t ourServoMenuSize = sizeof(ourServoMenuItems) / sizeof(char*);;

#define MIN_PULSE_WITDH 700 // us
#define MAX_PULSE_WITDH 2300 // us

// LOGGING LOGGING
enum LogSeverity {
  LS_START=0,
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  LS_END
};

#define LOG_MOD_HTTP 0x01
#define LOG_MOD_PERF 0x02
#define LOG_MOD_RTEST 0x04
#define LOG_MOD_RADIO 0x08
#define LOG_MOD_5 0x10
#define LOG_MOD_6 0x20
#define LOG_MOD_7 0x40
#define LOG_MOD_8 0x80

LogSeverity ourLogSeverity=DEBUG;
  
byte ourLogModules = 0;
    
void setLogModule(byte aModule) {
  ourLogModules = ourLogModules | aModule;
} 
// some forward declarations
  
void setupLog()  {
  setLogModule(LOG_MOD_HTTP);
  setLogModule(LOG_MOD_PERF);
  setLogModule(LOG_MOD_RTEST);
  setLogModule(LOG_MOD_RADIO);
}
 
#define LOGGY3(a, b, c) logMsg(a, b, c)
#define LOGGY2(a, b) logMsg(a, b) 
// #define LOGGY2(a, b) 
    
void logMsg(LogSeverity aSeverity, String aMessage);
void log_printSecond();
  
void logMsg(byte aModule, LogSeverity aSeverity, String aMessage) {
  if (aModule & ourLogModules) {
    logMsg(aSeverity, aMessage);
  }
}

void logMsg(LogSeverity aSeverity, String aMessage) {
  if (aSeverity >= ourLogSeverity) {
    log_printSecond();
    Serial.print(myName);
    Serial.print(':');
    Serial.print(aMessage);
    Serial.println();
  }
}

void log_printSecond() {
  char buf[25];
  snprintf (buf, 25, "%08d: ", millis());
  Serial.print(buf);
}

enum {
  RD_QR1_L,
  RD_QR1_R,
  RD_QR2_L,
  RD_QR2_R,
  RD_WK1_L,
  RD_WK1_R,
  RD_WK2_L,
  RD_WK2_R,
  RD_VL_L,
  RD_VL_R,
  RD_SR_1,
  RD_SR_2,
  RD_HR_1,
  RD_HR_2,
  RD_SF_1,
  RD_SF_2,
  RD_SF_3,
  RD_SF_4,
  RD_LAST
 };

 const char *ourFunctionDefMap[] = {
        [RD_QR1_L] = "option_fd_QR-1-L",
        [RD_QR1_R] = "option_fd_QR-1-R",
        [RD_QR2_L] = "option_fd_QR-2-L",
        [RD_QR2_R] = "option_fd_QR-2-R",
        [RD_WK1_L] = "option_fd_WK-1-L",
        [RD_WK1_R] = "option_fd_WK-1-R",
        [RD_WK2_L] = "option_fd_WK-2-L",
        [RD_WK2_R] = "option_fd_WK-2-R",
        [RD_VL_L]  = "option_fd_VL-L",
        [RD_VL_R]  = "option_fd_VL-R",
        [RD_SR_1]  = "option_fd_SR-1",
        [RD_SR_2]  = "option_fd_SR-2",
        [RD_HR_1]  = "option_fd_HR-1",
        [RD_HR_2]  = "option_fd_HR-2",
        [RD_SF_1]  = "option_fd_SF-1",
        [RD_SF_2]  = "option_fd_SF-2",
        [RD_SF_3]  = "option_fd_SF-3",
        [RD_SF_4]  = "option_fd_SF-4",
    };

typedef struct {
  int rudderIdx;
  String descr;
  int8_t functionIdx;
  int servoPresets[CONFIG_SERVO_PRESET_MAX];
  int limitLow;
  int limitHigh;
  int8_t servoDirection;
  float angle;
  int servoPos;
  float rudderSize;
  float presetAngles[CONFIG_SERVO_PRESET_MAX];
} servoDataSet_t;

#define DATA_SET_IDX_MAX RD_LAST
#define ANGLE_UNSET_VAL 999.0f
typedef struct {
  String datasetDescription;
  int8_t currentFunctionIdx=0;
  int8_t dataSetIdxUnused=0;
  servoDataSet_t rudderData[DATA_SET_IDX_MAX];
  float currentPresetAngles[CONFIG_SERVO_PRESET_L];
} protocolData_t;

protocolData_t ourProtocolData;

Servo ourServo;
int16_t ourServoPos;
int8_t ourServoDirection = 1;
boolean ourWheelActivation = false;
uint8_t ourWheelFactor = 1;
int ourServoLimit[2];

#ifdef USE_MPU6050_MPU
#define INTERRUPT_PIN 15 // use pin 15 on ESP8266
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
#endif

ESP8266WebServer server(80);    // Server Port  hier einstellen

void setDefaultConfig();

void setup()
{
  delay(1000);
  setupLog();
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.print("Starting ServoController :");
  Serial.println(APP_VERSION);
  delay(1000);

  // check HW Pin 4 for HW Reset
  // checkHWReset(D6);

  loadConfig();
  initConfig();
  printConfig("stored configuration:");

  detectSensor();
  if (isI2C_MPU6050Addr()) {
    Wire.begin(ourSDA_Pin, ourSCL_Pin); //SDA, SCL
    initMPU6050();
#ifdef SUPPORT_MMA8451
  } else if (isI2C_MMA8451Addr()) {
    initMMA8451();
#endif
  }
  #ifdef OLED
  setupDisplay();
  #endif

  setupWiFi();
  setupWebServer();
  initServo();
  initProtocolData();
  #ifdef ROTARY_ENCODER
  setupRotaryEncoder();
  setupPushButton();
  #endif
  setupBuzzer();
  if (WiFi.status() == WL_CONNECTED) {
    #ifdef OTA
    setup_ota();
    #endif
  }

}

#ifdef OLED
const uint8_t *oledFontLarge;
const uint8_t *oledFontBig;
const uint8_t *oledFontNormal;
const uint8_t *oledFontSmall;
const uint8_t *oledFontTiny;
const uint8_t *oledFontIcon;
  
  
void setupDisplay() {
  // Wifi Kit 8 has a fixed wired 128x32 display 
  ourOLED.begin();
  // ourOLED.enableUTF8Print();	
  int oledDisplayHeight = ourOLED.getDisplayHeight();
  int oledDisplayWidth = ourOLED.getDisplayWidth();
  logMsg(INFO, F("init OLED display: ") + String(oledDisplayWidth) + String(F("x")) + String(oledDisplayHeight));
  oledFontLarge  = u8g2_font_helvB18_tr;
  oledFontLarge  = u8g2_font_profont17_tf;
  oledFontBig    = u8g2_font_helvR12_tr;
  oledFontNormal = u8g2_font_helvR10_tr;
  oledFontSmall  = u8g2_font_5x7_tr; 
  oledFontTiny   = u8g2_font_4x6_tr;
  oledFontIcon   = u8g2_font_m2icon_7_tf;
  ourOLED.firstPage();
  do {
    showHello();
  } while ( ourOLED.nextPage() );
    
}   

void showHello() {
  ourOLED.setFont(oledFontNormal);
  ourOLED.setCursor(0, 15);
  ourOLED.print("Hello I am ");
  ourOLED.setCursor(0, 32);
  ourOLED.print(myName);
  ourOLED.print(" ");
  ourOLED.print(APP_VERSION);
}

void showOLEDAngleSensorPage() {
  ourOLED.setDrawColor(1);
  ourOLED.setFontMode(0);
  ourOLED.setFont(oledFontLarge);
  String angle = String(getRoundedAngle(), 1); 
  // ourOLED.setCursor(120-ourOLED.getStrWidth(angle.c_str()), 28);
  ourOLED.setCursor(0, 28);
  ourOLED.print(angle.c_str());
  ourOLED.print(":");
  ourOLED.print((char) 176);
  ourOLED.print((char) 58);
  ourOLED.print(":");
}

/**
  return the math result of a modulo operation instead of the symmetric (as implemented in the %-operator
*/
int8_t getModulo(long aDivident, uint8_t aDivisor) {
  // Umrechnung der Modulo Resultate von der in C++ implementierten symmetrischen Modulo-Variante in die mahtematische Variante
  return ((aDivident % aDivisor) + aDivisor) % aDivisor;
}

void showOLEDMenu(const char* aItems[], uint8_t aNumItems) {
  ourOLED.drawBox(0, 11, 128, 10);
  ourOLED.setDrawColor(2);
  ourOLED.setFontMode(1);
  ourOLED.setFont(oledFontNormal);
 
  for (int8_t i=-1; i<2; i++) {
    ourOLED.drawStr(0,21+i*11, aItems[getModulo(ourRotaryMenuPosition+i, aNumItems)]);
  }
}

void showOLEDDebugInfo() {
  ourOLED.setFont(oledFontNormal);
  ourOLED.setCursor(0, 8);
  ourOLED.print(F("D1:"));
  ourOLED.print(ourDebug1);
  ourOLED.setCursor(0, 16);
  ourOLED.print(F("D2:"));
  ourOLED.print(ourDebug2);
  ourOLED.setCursor(0, 24);
  ourOLED.print(F("D3:"));
  ourOLED.print(ourDebug3);
}

void showOLEDSetServoSteps() {
  ourOLED.setFont(oledFontNormal);
  ourOLED.setCursor(0, 15);
  ourOLED.print("Servo-Steps=");
  ourOLED.print(String(ourRotaryEncoderMultiplier));
  ourOLED.print("%");
}

void showOLEDSetRudderDepth() {
  ourOLED.setFont(oledFontNormal);
  ourOLED.setCursor(0, 15);
  ourOLED.print("Rudertiefe=");
  ourOLED.print(String(ourRudderSize, 1));
  ourOLED.print("mm");
}

void showOLEDServoPositionPage(int16_t aPosition) {
  ourOLED.setDrawColor(1);
  ourOLED.setFontMode(0);
  ourOLED.setFont(oledFontLarge);
  // ourOLED.setCursor(22, 24);
  // ourOLED.print(aPosition);
  // ourOLED.print(F("%"));
  String position = String(aPosition) + F("%");
  ourOLED.setCursor(128-ourOLED.getStrWidth(position.c_str()), 24);
  ourOLED.print(position.c_str());
  ourOLED.setFont(oledFontSmall);
  ourOLED.setCursor(20, 32);
  switch(ourRotaryEncoderMultiplier) {
    case RE_MULITPLIER_SLOW:
      ourOLED.print(F("."));
      break;
    case RE_MULITPLIER_NORMAL:
      ourOLED.print(F("o"));
      break;
    case RE_MULITPLIER_FAST:
      ourOLED.print(F("O"));
      break;
  } 
  ourOLED.print(F(":"));
  ourOLED.print(F("I="));
  ourOLED.print(ourServoCurrent, 1);
  ourOLED.print(F("A"));
  ourOLED.print(F(":Raw I="));
  ourOLED.print(mySmoothedCurrent);
  // showServoPosition(0, 13, 64, 5, (150.0f+aPosition)/300);
  showServoPositionVertical(0, 0, 18, 32, (150.0f-aPosition)/300);

  ourOLED.setCursor(24, 7);
  ourOLED.setFont(oledFontSmall);
  String angle = String("Winkel=") + String(getRoundedAngle(), 1) + String("°"); 
  ourOLED.print(angle.c_str());
}

void showServoPositionVertical(int x, int y, int w, int h, float value) {
    ourOLED.drawFrame(x, y, w,  h);
    ourOLED.drawBox(x + 2, y + 2, (w - 4),  (h - 3) * value);
}

void showServoPosition(int x, int y, int w, int h, float value) {
    ourOLED.drawFrame(x, y, w,  h);
    ourOLED.drawBox(x + 2, y + 2, (w - 4) * value,  h - 3);
}

void updateOLED(unsigned long aNow) {
  static unsigned long last=0;
  #define OLED_REFRESH_CYCLE 100
  if (aNow < last) {
    return;
  }
  last = aNow + OLED_REFRESH_CYCLE;

  ourOLED.firstPage();
  do {
    switch (ourContext) {
      case SERVO_PAGE:
        showOLEDServoPositionPage(getServoPosInPercent());
        break;
      case ANGLE_SENSOR_PAGE:
        showOLEDAngleSensorPage();
        break;
      case SERVO_MENU_PAGE:
        showOLEDMenu(ourServoMenuItems, ourServoMenuSize);
        break;
      case DEBUG_PAGE:
        showOLEDDebugInfo();
        break;
      case SET_SERVO_STEPS:
        showOLEDSetServoSteps();
        break;
      case SET_RUDDER_DEPTH:
        showOLEDSetRudderDepth();
        break;
      case BASE_MENU_PAGE:
      default: // MENU
        showOLEDMenu(ourBaseMenuItems, ourBaseMenuSize);
        break;
    } 
  } while ( ourOLED.nextPage() );

}

#endif

#ifdef ROTARY_ENCODER



void setupRotaryEncoder() {
  ourRotaryEncoderMultiplier = RE_MULITPLIER_NORMAL;
}
void setupPushButton() {
  // BUTTON SETUP 
  // INPUT_PULLUP for bare ourSignalButton connected from GND to input pin
  ourPushButton.attach(PIN_PUSH_BUTTON, INPUT); // USE EXTERNAL PULL-UP

  // DEBOUNCE INTERVAL IN MILLISECONDS
  ourPushButton.interval(50);
  
  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  ourPushButton.setPressedState(LOW);
} 

void updatePushButton(unsigned long aNow) {
  ourPushButton.update();

  static unsigned long history[5] = {0UL};;
  static unsigned long reactOnMultiplePressed = 0;
  static uint8_t buttonCnt=0;
  
  #define CLEAR_HISTORY history[0] = 0L
  #define MULTI_PRESS_TIMERANGE 1500
  #define MULTI_PRESS_REACTION_TIME 700

  // save the button press history
  boolean buttonPressed = ourPushButton.pressed();
  if (buttonPressed) {
    for (int i=4; i>0; i--) {
      history[i] = history[i-1];
    }
    history[0] = aNow;

    buttonCnt=0;
    for (int i=0; i<5; i++) {
      if ((aNow - history[i]) < MULTI_PRESS_TIMERANGE) {
         buttonCnt++;
      } else {
        break;
      }
    }
    logMsg(INFO, F("Button pressed: ") + String(buttonCnt));
    reactOnMultiplePressed = history[buttonCnt-1] + MULTI_PRESS_REACTION_TIME;

  }
    

  if (reactOnMultiplePressed && aNow > reactOnMultiplePressed) {
    reactOnMultiplePressed=0;
    CLEAR_HISTORY;
    switch (buttonCnt) {
      case 1:
        switch (ourContext) {
          case SERVO_PAGE:
            // buzzOn(10);
            // setServoPosInPercent(0);
            ourContext=SERVO_MENU_PAGE;
            resetRotaryEncoder();
            break;
          case SERVO_MENU_PAGE:
            switch (getModulo(ourRotaryMenuPosition, ourServoMenuSize)){
              case 0:
                // "0:Servo-Pos=0%";
                setServoPosInPercent(0);
                ourContext=SERVO_PAGE;
                resetRotaryEncoder();
                break;
              case 1:
                // "1:Tara-Winkel";
                taraAngle();
                ourContext=SERVO_PAGE;
                resetRotaryEncoder();
                break;
              case 2:
                // "2:Setze Rudertiefe";
                ourContext=SET_RUDDER_DEPTH;
                resetRotaryEncoder();
                break;
              case 3:
                // "3:Setze Servoschritte";
                ourContext=SET_SERVO_STEPS;
                resetRotaryEncoder();
                break;
              case 4:
                // "4:zurück";
                ourContext=SERVO_PAGE;
                resetRotaryEncoder();
                break;
            }
            break;
          case SET_SERVO_STEPS:
            ourContext=SERVO_PAGE;
            resetRotaryEncoder();
            break;
          case SET_RUDDER_DEPTH:
            ourContext=SERVO_PAGE;
            resetRotaryEncoder();
            break;
          case BASE_MENU_PAGE:
            switch (getModulo(ourRotaryMenuPosition, ourBaseMenuSize)){
              case 0:
                ourContext = SERVO_PAGE;
                resetRotaryEncoder();
                break;
              case 1:
                ourContext = ANGLE_SENSOR_PAGE;
                resetRotaryEncoder();
                break;
              case 2:
                ourContext = DEBUG_PAGE;
                resetRotaryEncoder();
                break;
            }
            break;
          case ANGLE_SENSOR_PAGE:
            taraAngle();
            break;
        } 
        break;
      case 2:
        switch (ourContext) {
          case SERVO_PAGE:
            switch(ourRotaryEncoderMultiplier) {
              case RE_MULITPLIER_SLOW:
                ourRotaryEncoderMultiplier = RE_MULITPLIER_NORMAL;
                break;
              case RE_MULITPLIER_NORMAL:
                ourRotaryEncoderMultiplier = RE_MULITPLIER_FAST;
                break;
              default :
                ourRotaryEncoderMultiplier = RE_MULITPLIER_SLOW;
                break;
            }
            ourOLEDLogString = "x=" + String(ourRotaryEncoderMultiplier);
          break;
          case ANGLE_SENSOR_PAGE:
            // invert the sensor angle
            ourConfig.angleInversion = ourConfig.angleInversion == 1 ? -1 : 1;
            break;
        } 
        break;
      case 3:
        switch (ourContext) {
          default:
            ourContext = BASE_MENU_PAGE;
            resetRotaryEncoder();
            break;
        }
        break;
    }
  }
}

static long ourREOldPos  = 0;
void resetRotaryEncoder() {
  ourRotaryEncoder.write(0);
  ourREOldPos = 0;
  ourRotaryEncoderPosition = 0;
  ourRotaryMenuPosition = 0;
  ourDebug1 = 0;
  ourDebug2 = 0;
  ourDebug3 = 0;
}

void updateRotaryEncoder(unsigned long aNow) {
  long position = ourRotaryEncoder.read()/4;
  if (position != ourREOldPos) {
    int8_t delta = position - ourREOldPos;
    ourRotaryEncoderPosition = position;
    ourRotaryMenuPosition = position/3;
    ourDebug1 = position;
    ourDebug2 = ourRotaryEncoderPosition;
    ourDebug3 = ourRotaryMenuPosition/2;

    // encoder changes value 3 per grid step
    // logMsg(INFO, F("Encoder:") + String(ourRotaryEncoderPosition));
    // logMsg(INFO, F("Encoder mod:") + String(ourRotaryEncoderPosition));

    switch (ourContext) {
      case SERVO_PAGE:
        {
          int16_t servoPos = getServoPosInMicroSeconds();
          servoPos += delta*5*ourRotaryEncoderMultiplier;
          if (convertMicroSeconds2Percent(servoPos) > 150) servoPos=convertPercent2MicroSeconds(150);
          if (convertMicroSeconds2Percent(servoPos) < -150) servoPos=convertPercent2MicroSeconds(-150);
          if (convertMicroSeconds2Percent(servoPos) == 0 ) {
            buzzOn(5);
          } else if (convertMicroSeconds2Percent(servoPos)%10 == 0 ) {
            // buzzOn(2);
          }
          setServoPosInMicroSeconds(servoPos);
        }
        break;
      case SET_SERVO_STEPS:
        {
          uint8_t steps = getModulo(position, 3);
          switch (steps) {
            case 1:
              ourRotaryEncoderMultiplier = RE_MULITPLIER_FAST;
              break;
            case 2:
              ourRotaryEncoderMultiplier = RE_MULITPLIER_SLOW;
              break;
            default :
              ourRotaryEncoderMultiplier = RE_MULITPLIER_NORMAL;
          }
        }
        break;
      case SET_RUDDER_DEPTH:
        ourRudderSize += delta;
        break;
    }
    ourREOldPos = position;
  }
} 
#endif

#ifdef CURRENT_SENSOR
void updateCurrentSensor(unsigned long aNow) {
  static unsigned long last=0;
  #define CURRENT_SENSOR_REFRESH_CYCLE 500

  // IIR Low Pass Filter
  // y[i] := α * x[i] + (1-α) * y[i-1]
  //      := y[i-1] + α * (x[i] - y[i-1])
  // mit α = 1- β
  // y[i] := (1-β) * x[i] + β * y[i-1]
  //      := x[i] - β * x[i] + β * y[i-1]
  //      := x[i] + β (y[i-1] - x[i])
  // see: https://en.wikipedia.org/wiki/Low-pass_filter#Simple_infinite_impulse_response_filter
  float currentValue = analogRead(A0);
  mySmoothedCurrent = currentValue + 0.05f * (mySmoothedCurrent - currentValue);

  if (aNow < last) {
    return;
  }
  last = aNow + CURRENT_SENSOR_REFRESH_CYCLE;

  // ACS712 : 185mV/A for the ACS712T ELC-05B (5 Ampere max current)
  // ESP8266 : A0 provides 0-1023 for 0 - 3300mV => max resolution: 3.2mV
  const static int zeroCurrentValue = 810;
  ourServoCurrent = (mySmoothedCurrent - zeroCurrentValue)*3.3/1024/0.185;
  ourServoCurrent = max(0.0f, ourServoCurrent);
  if (ourServoCurrent > 0.5 ) {
    buzzOn(100);
  }
  logMsg(INFO, F("ACS712: [current raw]") + String(currentValue));
  logMsg(INFO, F("ACS712: [smoothed raw]") + String(mySmoothedCurrent));
  logMsg(INFO, F("ACS712: Current[A]") + String(ourServoCurrent));
}
#endif

static unsigned long ourBuzzTimeTill = 0;
static boolean ourBuzzOn = false; 

void setupBuzzer() {
  Serial.println(F("setupBuzzer"));
  pinMode (PIN_BUZZER_OUT, OUTPUT );
  digitalWrite(PIN_BUZZER_OUT, LOW);
}   

void buzzOn(uint16_t aDuration) {
  #ifndef NOBUZZ
  if (ourBuzzOn) {
    unsigned long dura = millis() + aDuration;
    ourBuzzTimeTill = max(dura, ourBuzzTimeTill);
  } else { 
    ourBuzzTimeTill = millis() + aDuration;
  } 
  ourBuzzOn = true;
  digitalWrite(PIN_BUZZER_OUT, HIGH);
   logMsg(INFO, F("buzzOn duration/till: ")
          + String(aDuration) + "/"
          + String(ourBuzzTimeTill));
  #endif
}


void updateBuzzer(unsigned long aNow) {
  if (ourBuzzOn) {
    // digitalWrite(PIN_BUZZER_OUT, HIGH);  
    if (aNow > ourBuzzTimeTill) {
      logMsg(INFO, F("buzz_off"));
      ourBuzzOn = false;
      digitalWrite(PIN_BUZZER_OUT, LOW);  
    }
  }
} 



void loop()
{
  static unsigned long last = 0;
  unsigned long now = millis();
  server.handleClient();
  ourDNSServer.processNextRequest();
  MDNS.update();

  moveServo();
  readMotionSensor();
  #ifdef ROTARY_ENCODER
  updateRotaryEncoder(now);
  updatePushButton(now);
  #endif
  #ifdef OLED
  updateOLED(now);
  #endif
  updateBuzzer(now);
  #ifdef CURRENT_SENSOR
  updateCurrentSensor(now);
  #endif

  doAsync();

  if (WiFi.status() == WL_CONNECTED) {
    #ifdef OTA
    ArduinoOTA.handle();
    #endif
  }


  if ( (millis() - last) > 500) {
    // showServoPos();
    prepareMotionData();
    last = millis();
  }
}


// OVER THE AIR
#ifdef OTA
void setup_ota() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname(String("esp8266" + WiFi.macAddress()).c_str());

  // No authentication by default
  ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = F("sketch");
    } else { // U_FS
      type = F("filesystem");
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println(F("Start updating ") + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println(F("\nEnd"));
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println(F("Auth Failed"));
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println(F("Begin Failed"));
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println(F("Connect Failed"));
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println(F("Receive Failed"));
    } else if (error == OTA_END_ERROR) {
      Serial.println(F("End Failed"));
    }
  });
  ArduinoOTA.begin();
  Serial.print(F("ArduinoOTA: setup ready"));
  Serial.print(F("  IP address: "));
  Serial.println(WiFi.localIP());
}
#endif
// End: OVER THE AIR


// =================================
// sensor data processing functions
// =================================

void initConfig() {
  initServoControllerConfig();
  initAngleMeasure();
}

void initAngleMeasure() {
  // if EEPROM config was not properly stored
  if (ourConfig.amplitudeInversion != -1) {
    ourConfig.amplitudeInversion = 1;
  }
  if (ourConfig.angleInversion != -1) {
    ourConfig.angleInversion = 1;
  }
  if (String((uint8_t) ourConfig.calibrationOffsetEnabled).equals("255")) {
    // if config is not stored before
    ourConfig.calibrationOffsetEnabled = false;
    ourConfig.calibrationOffsetHigh = 450;
    ourConfig.calibrationOffsetLow = -450;
  }
}


/**
  set the value of the preset store given by aNum in percent
*/
void initPresetInPercent(uint8_t aNum, uint16_t aPercent) {
  if (aNum < CONFIG_SERVO_PRESET_MAX) {
    ourConfig.servoPresets[aNum] = convertPercent2MicroSeconds(aPercent);
    ourProtocolData.currentPresetAngles[aNum] = ANGLE_UNSET_VAL;
  }
}

void initServoControllerConfig() {

  if (ourConfig.servoRangeByVendor < 0 || ourConfig.servoRangeByVendor > custom) {
    ourConfig.servoRangeByVendor = jeti;
  }
  initServoRangeSettings(ourConfig.servoRangeByVendor);
  for (int i=0; i<CONFIG_SERVO_PRESET_MAX; i++) {
    initPresetInPercent(i, (-100 + i*100));
  }
}

void initProtocolData() {
  for (int i=0; i<CONFIG_SERVO_PRESET_MAX; i++) {
    ourProtocolData.currentPresetAngles[i] = ANGLE_UNSET_VAL;
  }
}

void initServo() {
  Serial.print("Servo connected to: ");
  Serial.println(SERVO_PIN);
  ourServo.attach(SERVO_PIN,
    ourConfig.servoPulseWidthPairFullRange[MIN_IDX],
    ourConfig.servoPulseWidthPairFullRange[MAX_IDX]);
  setServoPosInMicroSeconds((ourConfig.servoPulseWidthPairFullRange[MIN_IDX] +
     ourConfig.servoPulseWidthPairFullRange[MAX_IDX]) /2);
  ourServo.writeMicroseconds(getServoPosInMicroSeconds());

  // check and reset servo preset values if not valid initialized
  for (int i=0; i<CONFIG_SERVO_PRESET_MAX; i++) {
    if (getPreset(i) < -125 || getPreset(i) > +125) {
      initPresetInPercent(i, (-100 + i*100));
    }
  }
}

void initServoRangeSettings(rc_vendor_t aVendor) {
  switch (aVendor) {
  case jeti:
    ourConfig.servoPulseWidthPairFullRange[MIN_IDX] = 750;
    ourConfig.servoPulseWidthPairFullRange[MAX_IDX] = 2250;
    ourConfig.servoPulseWidthPair100Percent[MIN_IDX] = 1000;
    ourConfig.servoPulseWidthPair100Percent[MAX_IDX] = 2000;
    break;
  case futaba:
    ourConfig.servoPulseWidthPairFullRange[MIN_IDX] = 920;
    ourConfig.servoPulseWidthPairFullRange[MAX_IDX] = 2150;
    ourConfig.servoPulseWidthPair100Percent[MIN_IDX] = 960;
    ourConfig.servoPulseWidthPair100Percent[MAX_IDX] = 2080;
    break;
  case hott:
  case spektrum:
    ourConfig.servoPulseWidthPairFullRange[MIN_IDX] = 900;
    ourConfig.servoPulseWidthPairFullRange[MAX_IDX] = 2100;
    ourConfig.servoPulseWidthPair100Percent[MIN_IDX] = 1100;
    ourConfig.servoPulseWidthPair100Percent[MAX_IDX] = 1900;
    break;
  case taranis:
    ourConfig.servoPulseWidthPairFullRange[MIN_IDX] = 732;
    ourConfig.servoPulseWidthPairFullRange[MAX_IDX] = 2268;
    ourConfig.servoPulseWidthPair100Percent[MIN_IDX] = 988;
    ourConfig.servoPulseWidthPair100Percent[MAX_IDX] = 2012;
    break;
  case multiplex:
    ourConfig.servoPulseWidthPairFullRange[MIN_IDX] = 750;
    ourConfig.servoPulseWidthPairFullRange[MAX_IDX] = 2250;
    ourConfig.servoPulseWidthPair100Percent[MIN_IDX] = 950;
    ourConfig.servoPulseWidthPair100Percent[MAX_IDX] = 2050;
    break;
  default: // custom
    break;
  }
  ourServoLimit[MIN_IDX] = ourConfig.servoPulseWidthPairFullRange[MIN_IDX];
  ourServoLimit[MAX_IDX] = ourConfig.servoPulseWidthPairFullRange[MAX_IDX];
  printServoRanges();
}

void printServoRanges() {
  Serial.print("FullRange[MIN_IDX]"); Serial.println(ourConfig.servoPulseWidthPairFullRange[MIN_IDX]);
  Serial.print("FullRange[MAX_IDX]"); Serial.println(ourConfig.servoPulseWidthPairFullRange[MAX_IDX]);
  Serial.print("100Percent[MAX_IDX]"); Serial.println(ourConfig.servoPulseWidthPair100Percent[MIN_IDX]);
  Serial.print("100Percent[CFG_MIN]"); Serial.println(ourConfig.servoPulseWidthPair100Percent[MAX_IDX]);
}

void readMotionSensor() {
  if (isI2C_MPU6050Addr()) {
    mpu.getMotion6(&ourAccelerometer_x, &ourAccelerometer_y, &ourAccelerometer_z, &gyro_x, &gyro_y, &gyro_z);
  }
#ifdef SUPPORT_MMA8451
  else if (isI2C_MMA8451Addr()) {
    mma.read();
    ourAccelerometer_x = mma.x;
    ourAccelerometer_y = mma.y;
    ourAccelerometer_z = mma.z;
  }
#endif

  const static double smooth = 0.98d;
  // print out data
  ourSmoothedAngle_x = irr_low_pass_filter(ourSmoothedAngle_x,
    atan2(ourAccelerometer_y, ourAccelerometer_z) * 180 / M_PI, smooth);
  ourSmoothedAngle_y = irr_low_pass_filter(ourSmoothedAngle_y,
    atan2(ourAccelerometer_z, ourAccelerometer_x) * 180 / M_PI, smooth);
  ourSmoothedAngle_z = irr_low_pass_filter(ourSmoothedAngle_z,
    atan2(ourAccelerometer_x, ourAccelerometer_y) * 180 / M_PI, smooth);
  ourSmoothedGyro_x = irr_low_pass_filter(ourSmoothedGyro_x, gyro_x, smooth);
  ourSmoothedGyro_y = irr_low_pass_filter(ourSmoothedGyro_y, gyro_y, smooth);
  ourSmoothedGyro_z = irr_low_pass_filter(ourSmoothedGyro_z, gyro_z, smooth);
}

void prepareMotionData() {
  double effAngle_x = ourSmoothedAngle_x - ourTaraAngle_x;
  double effAngle_y = ourSmoothedAngle_y - ourTaraAngle_y;
  double effAngle_z = ourSmoothedAngle_z - ourTaraAngle_z;
  double effGyro_x = ourSmoothedGyro_x - ourTaraGyro_x;
  double effGyro_y = ourSmoothedGyro_y - ourTaraGyro_y;
  double effGyro_z = ourSmoothedGyro_z - ourTaraGyro_z;
  // Serial.print(String("WX = ") + roundToDot5(effAngle_x));
  // Serial.print(String(" WY = ") + roundToDot5(effAngle_y));
  // Serial.print(String(" WZ = ") + roundToDot5(effAngle_z));
  // Serial.print(String(" GX = ") + roundToDot5(effGyro_x));
  // Serial.print(String(" GY = ") + roundToDot5(effGyro_y));
  // Serial.print(String(" GZ = ") + roundToDot5(effGyro_z));
  // Serial.println();
}

double getAngle() {
  double theAngle = 0;
  switch (ourConfig.axis) {
    case yAxis:
      theAngle = ourSmoothedAngle_y;
      break;
    case zAxis:
      theAngle = ourSmoothedAngle_z;
      break;
    default:
      theAngle = ourSmoothedAngle_x;
      break;
  }
  // support range -180 - +180, independent from tara
  if (theAngle < (ourTara-180.0d)) {
    theAngle = theAngle + 360.0d;
  }
  double angle = (theAngle - ourTara) * ourConfig.angleInversion;
  if (ourConfig.calibrationOffsetEnabled == true) {
    long iangle = angle*100;
    if (iangle >= 0) {
      long iangle = angle*100;
      iangle = map(iangle, 0, ourConfig.calibrationOffsetHigh*10, 0, 4500);
      angle = ((double) iangle)/100;
    } else {
      long iangle = angle*100;
      iangle = map(iangle, 0, ourConfig.calibrationOffsetLow*10, 0, -4500);
      angle = ((double) iangle)/100;
    }
  }

  return angle;
}

double getAmplitude(double aAngle) {
  double res;
  if (ourConfig.amplitudeCalcMethod == CHORD) {
    // sin (angle/2) = sin(angle/2 *M_PI/180)
    res = sin(aAngle*M_PI/360) * 2 * ourRudderSize * ourConfig.amplitudeInversion;
  } else if (ourConfig.amplitudeCalcMethod == VERTICAL_DISTANCE) {
    res =  sin(aAngle*M_PI/180) * ourRudderSize * ourConfig.amplitudeInversion;
  } else {
    res =  (aAngle/180*M_PI * ourRudderSize) * ourConfig.amplitudeInversion;
  }
  return res;
}

float getRoundedAngle(double aAngle) {
  return roundPrecision(aAngle, ourConfig.anglePrecision);
}

float getRoundedAngle() {
  return getRoundedAngle(getAngle());
}

float getRoundedAmplitude(double aAngle) {
  return roundPrecision(getAmplitude(aAngle), ourConfig.amplitudePrecision);
}

float getRoundedAmplitude() {
  return getRoundedAmplitude(getAngle());
}

float getRudderAmplitude() {
  float ampl = getRoundedAmplitude();
  if (ourIsMeasureActive) {
    ourMinAmpl = min(ourMinAmpl, ampl);
    ourMaxAmpl = max(ourMaxAmpl, ampl);
  }
  return ampl;
}

float roundPrecision(double aVal, precision_t aPrecision) {
  float res = aVal;
  switch(aPrecision) {
    case P001:
      break;
    case P010:
      res = round(res * 10)/10;
      break;
    case P050:
      res = round(res * 2)/2;
      break;
    case P100:
      res = round(res);
      break;
    }
  return res;
}

void taraAngle() {
  ourTaraAngle_x = ourSmoothedAngle_x;
  ourTaraAngle_y = ourSmoothedAngle_y;
  ourTaraAngle_z = ourSmoothedAngle_z;
  switch (ourConfig.axis) {
    case yAxis:
      ourTara = ourSmoothedAngle_y;
      break;
    case zAxis:
      ourTara = ourSmoothedAngle_z;
      break;
    default:
      ourTara = ourSmoothedAngle_x;
      break;
  }
  Serial.println("tara angle set to :" + String(ourTara));
}

void flightPhaseMeasure(boolean aStart) {
  Serial.println(String("flightPhaseMeasure(")+aStart+")");
  if (aStart) {
    ourNullAmpl = getRudderAmplitude();
    ourMinAmpl = ourMaxAmpl = ourNullAmpl;
    ourIsMeasureActive=true;
  } else {
    ourIsMeasureActive=false;
  }
}

void moveServo() {
  int16_t pos = getServoPosInMicroSeconds();
  if (ourServoDirection == -1 ) {
    pos = -pos
      +  (ourConfig.servoPulseWidthPairFullRange[MIN_IDX]
      +   ourConfig.servoPulseWidthPairFullRange[MAX_IDX] );
  }
  ourServo.writeMicroseconds(pos);
}

int16_t convertMicroSeconds2Percent(int16_t aMicroSeconds) {
  int16_t retVal;
  retVal = map(aMicroSeconds,
    ourConfig.servoPulseWidthPair100Percent[MIN_IDX],
    ourConfig.servoPulseWidthPair100Percent[MAX_IDX],
    -100, +100);
  return retVal;
}

int16_t convertPercent2MicroSeconds(int16_t aPercent) {
  // Serial.print(" convertPercent2MicroSeconds: ");
  // Serial.print(aPercent);
  // Serial.print(" return: ");
  int16_t retVal;
  retVal = map(aPercent,
    -100, +100,
    ourConfig.servoPulseWidthPair100Percent[MIN_IDX],
    ourConfig.servoPulseWidthPair100Percent[MAX_IDX]);
  return retVal;
}

int16_t getServoPosInPercent(int16_t aPWMValue) {
  return convertMicroSeconds2Percent(aPWMValue);
}

int16_t getServoPosInPercent() {
  return getServoPosInPercent(getServoPosInMicroSeconds());
}

void setServoPosInPercent(int16_t aPercentValue) {
  setServoPosInMicroSeconds(convertPercent2MicroSeconds(aPercentValue));
}

int16_t getServoPosInMicroSeconds() {
  return ourServoPos;
}

void setServoPosInMicroSeconds(int16_t aValue) {
  if (aValue > ourServoLimit[MAX_IDX]) {
    ourServoPos = ourServoLimit[MAX_IDX];
    #ifdef DO_LOG
    Serial.print("++");
    #endif
  } else if (aValue < ourServoLimit[MIN_IDX]) {
    ourServoPos = ourServoLimit[MIN_IDX];
    #ifdef DO_LOG
    Serial.print("--");
    #endif
  } else if (aValue > ourConfig.servoPulseWidthPairFullRange[MAX_IDX]) {
    ourServoPos = ourConfig.servoPulseWidthPairFullRange[MAX_IDX];
    #ifdef DO_LOG
    Serial.print("+");
    #endif
  } else if (aValue < ourConfig.servoPulseWidthPairFullRange[MIN_IDX]) {
    ourServoPos = ourConfig.servoPulseWidthPairFullRange[MIN_IDX];
    #ifdef DO_LOG
    Serial.print("-");
    #endif
  } else {
    #ifdef DO_LOG
    Serial.print(".");
    #endif
    ourServoPos = aValue;
  }
  #ifdef DO_LOG
  showServoPos();
  #endif
}


void showServoPos() {
  Serial.print(" servo pos: ");
  Serial.print(getServoPosInMicroSeconds());
  Serial.print("us, ");
  Serial.print(getServoPosInPercent());
  Serial.print("%");
  Serial.println();
}


/**
 * store the actual servo position in the preset given by aNum
*/
void storePreset(uint8_t aNum){
  Serial.print(" storePreset: ");
  if (aNum < CONFIG_SERVO_PRESET_MAX) {
    ourConfig.servoPresets[aNum] = getServoPosInMicroSeconds();
    ourProtocolData.currentPresetAngles[aNum] = getAngle();
    Serial.print(ourConfig.servoPresets[aNum]);
    Serial.print("/");
    Serial.print(ourConfig.servoPresets[aNum]);
  }
  Serial.println();
}

/**
 * load the stored preset value given by aNum
 */
void loadPreset(uint8_t aNum) {
  Serial.print(" loadPreset[");
  Serial.print(aNum);
  Serial.print("]: ");
  if (aNum < CONFIG_SERVO_PRESET_MAX) {
    setServoPosInMicroSeconds(getPreset(aNum));
    Serial.print(convertMicroSeconds2Percent(getPreset(aNum)));
    Serial.print("/");
    Serial.print(getPreset(aNum));
  } else {
    Serial.print(" !! error idx preset");
  }
  Serial.println();
}

/**
  get the value in percent of the preset store given by aNum
*/
int16_t getPresetInPercent(uint8_t aNum) {
  return aNum < CONFIG_SERVO_PRESET_MAX ? convertMicroSeconds2Percent(getPreset(aNum)) : 0;
}

/**
  get the value in us of the preset store given by aNum
*/
int16_t getPreset(uint8_t aNum) {
  return aNum < CONFIG_SERVO_PRESET_MAX ? ourConfig.servoPresets[aNum] : 1500;
}


// =================================
// web server functions
// =================================

void setupWebServer() {
  // react on these "pages"
  server.on("/",htmlMenuPage);
  server.on("/menuPage",htmlMenuPage);
  server.on("/adminPage",htmlAdminPage);
  server.on("/expertPage",htmlExpertPage);
  server.on("/servoPage",htmlServoPage);
  server.on("/angleSensorPage",htmlAngleSensorPage);
  server.on("/multiToolPage",htmlMultiToolPage);
  server.on("/showProtocolTable",htmlShowProtocolTable);
  server.on("/styles.css",cssStylesPage);
  server.on("/script.js",jsScriptPage);
  server.on("/getDataReq",getDataReq);
  server.on("/setDataReq",setDataReq);
  server.onNotFound(handleWebRequests); //Set setver all paths are not found so we can handle as per URI
  Serial.println("HTTP Server started");

  // Add service to MDNS-SD
  Serial.println("mDNS Service added");

  server.begin();               // Starte den Server
}

void jsScriptPage() {
  Serial.print(server.client().remoteIP().toString());
  Serial.println(" : jsScriptPage()");
  checkHTMLArguments();
  String s = FPSTR(SCRIPT); //Read HTML contents
  server.send(200, "text/javascript", s); //Send web page
}

void cssStylesPage() {
  Serial.print(server.client().remoteIP().toString());
  Serial.println(" : cssStylesPage()");
  checkHTMLArguments();
  server.send(200, "text/css", FPSTR(STYLES)); //Send web page
}

void htmlMultiToolPage() {
  Serial.print(server.client().remoteIP().toString());
  Serial.println(" : htmlMultiToolPage()");
  checkHTMLArguments();
  server.send(200, "text/html", FPSTR(MULTI_TOOL_page)); //Send web page
}

void htmlShowProtocolTable() {
  Serial.print(server.client().remoteIP().toString());
  Serial.println(" : htmlShowProtocolTable()");
  checkHTMLArguments();
  String s = FPSTR(SHOW_PROTO_TABLE_page); //Read HTML contents
  String table;
  table.concat("<table id='protodata'>");
  table.concat("<tr>");
  table.concat("<th>"); table.concat("</th>");
  table.concat("<th colspan=4>"); table.concat("Limit"); table.concat("</th>");
  for (int i=0; i<CONFIG_SERVO_PRESET_MAX; i++) {
  table.concat("<th colspan=4>"); table.concat(String("Pos-")+(i+1)); table.concat("</th>");
  }
  table.concat("<th colspan=2>"); table.concat("</th>");
  table.concat("</tr>\n");
  table.concat("<tr>");
  table.concat("<th>"); table.concat("</th>");
  table.concat("<th colspan=2>"); table.concat("min."); table.concat("</th>");
  table.concat("<th colspan=2>"); table.concat("max."); table.concat("</th>");
  for (int i=0; i<CONFIG_SERVO_PRESET_MAX; i++) {
  table.concat("<th colspan=2>"); table.concat("Servo"); table.concat("</th>");
  table.concat("<th>"); table.concat("Winkel"); table.concat("</th>");
  table.concat("<th>"); table.concat("Ruder"); table.concat("</th>");
  }
  table.concat("<th>"); table.concat("Invers"); table.concat("</th>");
  table.concat("<th>"); table.concat("Rudertiefe"); table.concat("</th>");
  table.concat("</tr>\n");
  table.concat("<tr>");
  table.concat("<th>"); table.concat("Beschreibung/Funktion"); table.concat("</th>");
  table.concat("<th>"); table.concat("%"); table.concat("</th>");
  table.concat("<th>"); table.concat("&micro;s"); table.concat("</th>");
  table.concat("<th>"); table.concat("%"); table.concat("</th>");
  table.concat("<th>"); table.concat("&micro;s"); table.concat("</th>");
#ifdef SERVOPOS
  table.concat("<th>"); table.concat("Servo Pos in %/&micro;s"); table.concat("</th>");
#else
  for (int i=0; i<CONFIG_SERVO_PRESET_MAX; i++) {
    table.concat("<th>"); table.concat("%"); table.concat("</th>");
    table.concat("<th>"); table.concat("&micro;s"); table.concat("</th>");
    table.concat("<th>"); table.concat("°"); table.concat("</th>");
    table.concat("<th>"); table.concat("mm"); table.concat("</th>");
  }
#endif
  table.concat("<th>"); table.concat("1/-1"); table.concat("</th>");
  table.concat("<th>"); table.concat("in mm"); table.concat("</th>");
  table.concat("</tr>\n");
  for (int i=0; i < ourProtocolData.dataSetIdxUnused; i++) {
    table.concat("<tr>");
    servoDataSet_t *dataSet = &ourProtocolData.rudderData[i];
    table.concat("<td>"); table.concat(dataSet->descr);
    table.concat("/"); table.concat(servoFunctionIdx2Shortcut(dataSet->functionIdx)); table.concat("</td>");
    table.concat("<td>"); table.concat(getServoPosInPercent(dataSet->limitLow)); table.concat("</td>");
    table.concat("<td>"); table.concat(dataSet->limitLow); table.concat("</td>");
    table.concat("<td>"); table.concat(getServoPosInPercent(dataSet->limitHigh)); table.concat("</td>");
    table.concat("<td>"); table.concat(dataSet->limitHigh); table.concat("</td>");
#ifdef SERVOPOS
    table.concat("<td>"); table.concat(getServoPosInPercent(dataSet->servoPos));table.concat("/"); table.concat(dataSet->servoPos); table.concat("</td>");
#else
    for (int j=0; j<CONFIG_SERVO_PRESET_MAX; j++) {
      int theServoPos = dataSet->servoPresets[j];
      float theAngle = dataSet->presetAngles[j];
      if (theAngle == ANGLE_UNSET_VAL) {
        table.concat("<td>"); table.concat("-"); table.concat("</td>");
        table.concat("<td>"); table.concat("-"); table.concat("</td>");
        table.concat("<td>"); table.concat("-"); table.concat("</td>");
        table.concat("<td>"); table.concat("-"); table.concat("</td>");
      } else {
        table.concat("<td>"); table.concat(getServoPosInPercent(theServoPos)); table.concat("</td>");
        table.concat("<td>"); table.concat(theServoPos); table.concat("</td>");
        table.concat("<td>"); table.concat(getRoundedAngle(theAngle)); table.concat("</td>");
        table.concat("<td>"); table.concat(getRoundedAmplitude(theAngle)); table.concat("</td>");
      }
    }
#endif
    table.concat("<td>"); table.concat(dataSet->servoDirection); table.concat("</td>");
    table.concat("<td>"); table.concat(dataSet->rudderSize); table.concat("</td>");
    table.concat("</tr>\n");
  }
  table.concat("</table>");
  s.replace("###<TABLE>###", table);
  server.send(200, "text/html", s); //Send web page
}

void htmlMenuPage() {
  Serial.print(server.client().remoteIP().toString());
  Serial.println(" : htmlMenuPage()");
  ourContext=BASE_MENU_PAGE;
  checkHTMLArguments();
  server.send(200, "text/html", FPSTR(MENU_page)); //Send web page
}

void htmlAngleSensorPage() {
  Serial.print(server.client().remoteIP().toString());
  Serial.println(" : htmlAngleSensorPage()");
  ourContext=ANGLE_SENSOR_PAGE;
  checkHTMLArguments();
  server.send(200, "text/html", FPSTR(ANGLE_SENSOR_page)); //Send web page
}

void htmlServoPage() {
  Serial.print(server.client().remoteIP().toString());
  Serial.println(" : htmlServoPage()");
  ourContext=SERVO_PAGE;
  checkHTMLArguments();
  server.send(200, "text/html", FPSTR(SERVO_page)); //Send web page
}

void htmlAdminPage() {
  Serial.print(server.client().remoteIP().toString());
  Serial.println(" : htmlAdminPage()");
  ourContext=ADMIN_PAGE;
  server.send(200, "text/html", FPSTR(ADMIN_page)); //Send web page
}

void htmlExpertPage() {
  Serial.print(server.client().remoteIP().toString());
  Serial.println(" : htmlExpertPage()");
  ourContext=EXPERT_ADMIN_PAGE;
  server.send(200, "text/html", FPSTR(EXPERT_page)); //Send web page
}

String createDynValueResponse(String aIdForcingValue) {
  String result = "";
  if (aIdForcingValue != "id_pwm_setvalue") {
    result += String("id_pwm_setvalue") + "=" + getServoPosInMicroSeconds() + MYSEP_STR;
  }
  if (aIdForcingValue != "id_pwm_value") {
    result += String("id_pwm_value") + "=" + getServoPosInMicroSeconds() + MYSEP_STR;
  }
  if (aIdForcingValue != "id_percent_setvalue") {
    result += String("id_percent_setvalue") + "=" + getServoPosInPercent() + MYSEP_STR;
  }
  if (aIdForcingValue != "id_percent_value") {
    result += String("id_percent_value") + "=" + getServoPosInPercent() + MYSEP_STR;
  }
  if (aIdForcingValue != "id_pos_slider") {
    result += String("id_pos_slider") + "=" + getServoPosInPercent() + MYSEP_STR;
  }
  return result;
}

void restoreProtocolDataSet(uint8_t aIdx) {
  int dataSetIdx =  getProtocolDataSetIdx(aIdx);

  if (dataSetIdx == ourProtocolData.dataSetIdxUnused) {
    Serial.print("setting defaults to protocol data set : ");
    Serial.println(dataSetIdx);
    for (int i=0; i<CONFIG_SERVO_PRESET_MAX; i++) {
      initPresetInPercent(i, (-100 + i*100));
    }
    // dataSet->servoPos = getServoPosInMicroSeconds();
    // ourRudderSize = dataSet->rudderSize = ourRudderSize;
    // dataSet->angle = getRoundedAngle();
    ourServoLimit[MIN_IDX] = ourConfig.servoPulseWidthPairFullRange[MIN_IDX];
    ourServoLimit[MAX_IDX] = ourConfig.servoPulseWidthPairFullRange[MAX_IDX];
  } else {
    Serial.print("setting stored values to protocol data set : ");
    Serial.println(dataSetIdx);
    servoDataSet_t *dataSet = &ourProtocolData.rudderData[dataSetIdx];
    ourProtocolData.datasetDescription = dataSet->descr;
    ourProtocolData.currentFunctionIdx = dataSet->functionIdx;

    for (int i=0; i<CONFIG_SERVO_PRESET_MAX; i++) {
      ourConfig.servoPresets[i] = dataSet->servoPresets[i];
      ourProtocolData.currentPresetAngles[i] = dataSet->presetAngles[i];
    }
    // dataSet->servoPos = getServoPosInMicroSeconds();
    ourRudderSize = dataSet->rudderSize;
    ourServoDirection = dataSet->servoDirection;
    dataSet->angle = getRoundedAngle();
    ourServoLimit[MIN_IDX] = dataSet->limitLow;
    ourServoLimit[MAX_IDX] = dataSet->limitHigh;
  }
}

String getResponse4Presets() {
  String retVal = "";
  // preset buttons
  for (int i=0; i<CONFIG_SERVO_PRESET_MAX; i++) {
    retVal += getResponse4Preset(i);
  }

  // limit display
  retVal += String("id_btn_set_limit_min") + "=" + convertMicroSeconds2Percent(ourServoLimit[MIN_IDX]) + "%" + MYSEP_STR;
  retVal += String("id_btn_set_limit_max") + "=" + convertMicroSeconds2Percent(ourServoLimit[MAX_IDX]) + "%" + MYSEP_STR;

  // limit buttons
  if ( ourServoLimit[MIN_IDX] == ourConfig.servoPulseWidthPairFullRange[MIN_IDX]) {
    retVal += String("id_btn_store_limit_min") + "=" + String("limit") + MYSEP_STR;
  } else {
    retVal += String("id_btn_store_limit_min") + "=" + String("unlimit") + MYSEP_STR;
  }
  if ( ourServoLimit[MAX_IDX] == ourConfig.servoPulseWidthPairFullRange[MAX_IDX]) {
    retVal += String("id_btn_store_limit_max") + "=" + String("limit") + MYSEP_STR;
  } else {
    retVal += String("id_btn_store_limit_max") + "=" + String("unlimit") + MYSEP_STR;
  }

  // servo direction
  if (ourServoDirection == -1) {
    retVal += String("id_servo_direction") + "=" + "checked" + MYSEP_STR;
  } else {
    retVal += String("id_servo_direction") + "=" + "unchecked" + MYSEP_STR;
  }

  // rudder size
  retVal += String("id_rudderSize") + "=" + ourRudderSize + MYSEP_STR;
  return retVal;
}

String getResponse4Preset(uint8_t aPos) {
  String retVal = "";
  if (aPos < CONFIG_SERVO_PRESET_MAX) {
    retVal += String("id_btn_set_pos_") + String(aPos) + "=" + convertMicroSeconds2Percent(getPreset(aPos)) + MYSEP_STR;
    float theAngle = ourProtocolData.currentPresetAngles[aPos];
    retVal += String("id_btn_store_pos_") + String(aPos) + "=";
    if (theAngle == ANGLE_UNSET_VAL) {
      retVal += String("-") + MYSEP_STR;
    } else {
      retVal += String(getRoundedAngle(theAngle), 1)+"&deg;/"+ String(getRoundedAmplitude(theAngle),1)+"mm"+MYSEP_STR;
    }
  }
  return retVal;
}

void storeProtocolDataSet(uint8_t aIdx) {
  servoDataSet_t *dataSet = &ourProtocolData.rudderData[aIdx];
  dataSet->descr = ourProtocolData.datasetDescription;
  dataSet->functionIdx = ourProtocolData.currentFunctionIdx;
  for (int i=0; i<CONFIG_SERVO_PRESET_MAX; i++) {
    dataSet->servoPresets[i] = ourConfig.servoPresets[i];
    dataSet->presetAngles[i] = ourProtocolData.currentPresetAngles[i];
  }
  dataSet->servoPos = getServoPosInMicroSeconds();
  dataSet->rudderSize = ourRudderSize;
  dataSet->servoDirection = ourServoDirection;
  dataSet->angle = getRoundedAngle();
  dataSet->limitLow = ourServoLimit[MIN_IDX];
  dataSet->limitHigh = ourServoLimit[MAX_IDX];
}

int getProtocolDataSetIdx(int8_t aFunctionIdx) {
  for (int i=0; i < ourProtocolData.dataSetIdxUnused; i++) {
    String currDescr = ourProtocolData.datasetDescription + aFunctionIdx;
    String existingDescr = ourProtocolData.rudderData[i].descr + ourProtocolData.rudderData[i].functionIdx;
    if (currDescr.equals(existingDescr)) {
      return i;
    }
  }
  return ourProtocolData.dataSetIdxUnused;
}

void storeProtocolDataSet() {
  Serial.print("storeProtocolDataSet:");
  Serial.print(ourProtocolData.dataSetIdxUnused);
  Serial.print("/");
  Serial.print(ourProtocolData.datasetDescription);
  Serial.print("/");
  Serial.print(servoFunctionIdx2Shortcut(ourProtocolData.currentFunctionIdx));
  Serial.println();

  int dataSetIdx =  getProtocolDataSetIdx(ourProtocolData.currentFunctionIdx);
  storeProtocolDataSet(dataSetIdx);
  if (dataSetIdx == ourProtocolData.dataSetIdxUnused) {
    ourProtocolData.dataSetIdxUnused++;
  }
  return;
}


void setDataReq() {
  String name = server.arg("name");
  String value = server.arg("value");
  #ifdef DO_LOG
  Serial.print(server.client().remoteIP().toString());
  Serial.println(" : setDataReq()");
  Serial.print("  "); Serial.print(name); Serial.print("="); Serial.println(value);
  #endif
  boolean sendResponse = true;

  String response = "";
  int htmlResponseCode=200; // OK

  // general settings stuff
  if (name == "cmd_saveConfig") {
    saveConfig();
  } else
  if (name == "cmd_resetConfig") {
    setDefaultConfig();
  } else
  if (name == "cmd_mcrestart") {
    Serial.println("resetting micro controller");
    triggerRestart();
  } else
  // WiFi stuff
  if ( name == "id_apActive") {
    if (value == "true") {
      ourConfig.apIsActive = true;
    } else {
      ourConfig.apIsActive = false;
    }
    Serial.println("setting AP active : " + String(ourConfig.apIsActive));
  } else
  if ( name == "id_wlanSsid") {
    strncpy(ourConfig.wlanSsid, value.c_str(), CONFIG_SSID_L);
    Serial.println("setting wlan ssid : " + String(ourConfig.wlanSsid));
  } else
  if ( name == "id_wlanPasswd") {
    strncpy(ourConfig.wlanPasswd, value.c_str(), CONFIG_PASSW_L);
    Serial.println("setting wlan password : " + String(ourConfig.wlanPasswd));
  } else
   if ( name == "id_apSsid") {
    strncpy(ourConfig.apSsid, value.c_str(), CONFIG_SSID_L);
    Serial.println("setting AccessPoint ssid : " + String(ourConfig.apSsid));
  } else
  if ( name == "id_apPasswd") {
    if (String(value).length() >= 8) {
      strncpy(ourConfig.apPasswd, value.c_str(), CONFIG_PASSW_L);
      Serial.println("setting AP password : " + String(ourConfig.apPasswd));
    } else {
      Serial.println("not setting AP password, too short. Old PW : " + String(ourConfig.apPasswd));
    }
  } else
  // angle measuring stuff
  if ( name == "cmd_taraAngle") {
    taraAngle();
  } else
  if ( name == "id_targetAmplitude") {
    ourTargetAmplitude = double(atoi(value.c_str()))/10;
    Serial.println("setting targetAmplitude: " + String(ourTargetAmplitude));
  } else
  if ( name == "cmd_flightphaseActive") {
    if (value == "true") {
      flightPhaseMeasure(true);
    } else {
      flightPhaseMeasure(false);
    }
  } else
  if ( name == "id_rudderSize") {
    ourRudderSize = double(atoi(value.c_str()))/10;
    Serial.println("setting rudderSize: " + String(ourRudderSize));
  } else
  if ( name.equals("id_dataset_descr")) {
    ourProtocolData.datasetDescription = value;
  } else
  if ( name.equals("cmd_dataset_init")) {
    Serial.println("cmd_dataset_init");
    ourProtocolData.dataSetIdxUnused=0;
  } else
  if ( name.equals("cmd_dataset")) {
    storeProtocolDataSet();
  } else
  if ( name == "id_proto_servo_function_def") {
    ourProtocolData.currentFunctionIdx = servoFunctionDef2Idx(value);
    Serial.print("setting / restoring currentFunctionIdx: ");
    Serial.println(ourProtocolData.currentFunctionIdx);
    restoreProtocolDataSet(ourProtocolData.currentFunctionIdx);
    response += getResponse4Presets();
    Serial.print("response: ");
    Serial.println(response);

  } else
  if ( name == "id_invertAngle") {
    if (value == "true") {
      ourConfig.angleInversion = -1;
    } else {
      ourConfig.angleInversion = 1;
    }
    Serial.println("setting invert angle: " + String(ourConfig.angleInversion));
  } else
  if ( name == "id_invertAmplitude") {
    if (value == "true") {
      ourConfig.amplitudeInversion = -1;
    } else {
      ourConfig.amplitudeInversion = 1;
    }
    Serial.println("setting invert amplitude " + String(ourConfig.amplitudeInversion));
  } else
  if ( name == "nm_referenceAxis") {
    if (value == "xAxis") {
      ourConfig.axis = xAxis;
    } else if (value == "yAxis") {
      ourConfig.axis = yAxis;
    } else if (value == "zAxis") {
      ourConfig.axis = zAxis;
    }
    Serial.println("setting angle reference axis: " + String(ourConfig.axis));
  } else
  if ( name == "nm_anglePrecision") {
    if (value == "P010") {
      ourConfig.anglePrecision = P010;
    } else if (value == "P001") {
      ourConfig.anglePrecision = P001;
    }
    Serial.println("setting angle precision: " + String(ourConfig.anglePrecision));
  } else
  if ( name == "nm_precisionAmplitude") {
    if (value == "P001") {
      ourConfig.amplitudePrecision = P001;
    } else if (value == "P010") {
      ourConfig.amplitudePrecision = P010;
    } else if (value == "P050") {
      ourConfig.amplitudePrecision = P050;
    } else if (value == "P100") {
      ourConfig.amplitudePrecision = P100;
    }
    Serial.println("setting amplitude precision: " + String(ourConfig.amplitudePrecision));
  } else
  if ( name == "nm_distancetype") {
    if (value == "arc") {
      ourConfig.amplitudeCalcMethod = ARC;
    } else if (value == "chord") {
      ourConfig.amplitudeCalcMethod = CHORD;
    } else if (value == "vertical_distance") {
      ourConfig.amplitudeCalcMethod = VERTICAL_DISTANCE;
    }
    Serial.println("setting calculation method : " + value + "/" + String(ourConfig.amplitudeCalcMethod));
  } else
  if ( name == "id_caloffset_enabled") {
    if (value == "true") {
      ourConfig.calibrationOffsetEnabled = true;
    } else {
      ourConfig.calibrationOffsetEnabled = false;
    }
    Serial.println("calibration offset enabled : " + String(ourConfig.calibrationOffsetEnabled));
  } else
  if (name == "id_caloffset_h") {
    ourConfig.calibrationOffsetHigh = value.toInt();
    Serial.println("high calibration offset: " + String(ourConfig.calibrationOffsetHigh));
  } else
  if (name == "id_caloffset_l") {
    ourConfig.calibrationOffsetLow = value.toInt();
    Serial.println("low calibration offset: " + String(ourConfig.calibrationOffsetLow));
  } else
  if (name == "cmd_calibrate") {
    if (isI2C_MPU6050Addr()) {
      Serial.println("triggering calibration");
      triggerCalibrateMPU6050();
    }
  } else
  // servo controll stuff
  if ( name == "id_pwm_setvalue") {
    setServoPosInMicroSeconds(value.toInt());
    response += createDynValueResponse(name);
  } else
  if ( name == "id_percent_setvalue") {
    setServoPosInPercent(value.toInt());
    response += createDynValueResponse(name);
  } else
  if ( name == "id_pos_slider") {
    setServoPosInPercent(value.toInt());
    response += createDynValueResponse(name);
  } else
  if ( name == "id_servo_direction") {
    if ( value == "true") {
      ourServoDirection = -1;
    } else {
      ourServoDirection = 1;
    }
  } else
  if ( name == "id_wheel_activate") {
    if ( value == "true") {
      ourWheelActivation = true;
    } else {
      ourWheelActivation = false;
    }
  } else
  if ( name == "id_wheel_factor") {
    ourWheelFactor = value.toInt();
  } else
  if ( name == "evt_wheel") {
    if (ourWheelActivation == true) {
      int8_t f = value.toInt() * ourWheelFactor;
      setServoPosInPercent(getServoPosInPercent() - f);
      // Serial.print("wheel: ");
      // Serial.print(f);
      response += createDynValueResponse("-");
    }
  } else
  if ( name.startsWith("cmd_limit")) {
    if ( value.equals("id_set_min")) {
      if ( ourServoLimit[MIN_IDX] == ourConfig.servoPulseWidthPairFullRange[MIN_IDX]) {
        ourServoLimit[MIN_IDX] = getServoPosInMicroSeconds();
        response += String("id_btn_store_limit_min") + "=" + String("unlimit") + MYSEP_STR;
      } else {
        ourServoLimit[MIN_IDX] = ourConfig.servoPulseWidthPairFullRange[MIN_IDX];
        response += String("id_btn_store_limit_min") + "=" + String("limit") + MYSEP_STR;
      }
      response += String("id_btn_set_limit_min") + "=" + convertMicroSeconds2Percent(ourServoLimit[MIN_IDX]) + "%" + MYSEP_STR;
    } else
    if ( value.equals("id_set_max")) {
      if ( ourServoLimit[MAX_IDX] == ourConfig.servoPulseWidthPairFullRange[MAX_IDX]) {
        ourServoLimit[MAX_IDX] = getServoPosInMicroSeconds();
        response += String("id_btn_store_limit_max") + "=" + String("unlimit") + MYSEP_STR;
      } else {
        ourServoLimit[MAX_IDX] = ourConfig.servoPulseWidthPairFullRange[MAX_IDX];
        response += String("id_btn_store_limit_max") + "=" + String("limit") + MYSEP_STR;
      }
      response += String("id_btn_set_limit_max") + "=" + convertMicroSeconds2Percent(ourServoLimit[MAX_IDX]) + "%" + MYSEP_STR;
    }
  } else
  if ( name.equals("cmd_store_servo_pos")) {
    // value = "id_store_pos_0";
    int pos = value.substring(13).toInt();
    if (pos < CONFIG_SERVO_PRESET_MAX) {
      storePreset(pos);
      response += getResponse4Preset(pos);
    } else {
      Serial.print("ERROR:  illegal idx of store pos for : ");
      Serial.println(value);
    }
  } else
  if ( name.equals("cmd_set_servo_pos")) {
    // value = "id_set_pos_0";
    int pos = value.substring(11).toInt();
    loadPreset(pos);
    response += createDynValueResponse(name);
  } else
  if ( name.startsWith("id_rcvendor_")) {
    if (value.equals("Jeti")) {
      ourConfig.servoRangeByVendor = jeti;
    } else
    if (value.equals("HoTT")) {
      ourConfig.servoRangeByVendor = hott;
    } else
    if (value.equals("Spektrum")) {
      ourConfig.servoRangeByVendor = spektrum;
    } else
    if (value.equals("Taranis")) {
      ourConfig.servoRangeByVendor = taranis;
    } else
    if (value.equals("Futaba")) {
      ourConfig.servoRangeByVendor = futaba;
    } else
    if (value.equals("Multiplex")) {
      ourConfig.servoRangeByVendor = multiplex;
    } else
    if (value.equals("Custom")) {
      ourConfig.servoRangeByVendor = custom;
    }
    initServoRangeSettings(ourConfig.servoRangeByVendor);
    response += String("id_pulse_width_min") + "=" + ourConfig.servoPulseWidthPairFullRange[MIN_IDX] +MYSEP_STR;
    response += String("id_pulse_width_max") + "=" + ourConfig.servoPulseWidthPairFullRange[MAX_IDX] +MYSEP_STR;
    response += String("id_pulse_width_n100") + "=" + ourConfig.servoPulseWidthPair100Percent[MIN_IDX] + MYSEP_STR;
    response += String("id_pulse_width_p100") + "=" + ourConfig.servoPulseWidthPair100Percent[MAX_IDX] + MYSEP_STR;
    Serial.println("setting servoRangeByVendor : " + String(ourConfig.servoRangeByVendor));
  } else
  if ( name.startsWith("id_pulse_width_")) {
    if ( name.equals("id_pulse_width_min")) {
      ourConfig.servoPulseWidthPairFullRange[MIN_IDX] = value.toInt();
    } else
    if ( name.equals("id_pulse_width_max")) {
      ourConfig.servoPulseWidthPairFullRange[MAX_IDX] = value.toInt();
    } else
    if ( name.equals("id_pulse_width_n100")) {
      ourConfig.servoPulseWidthPair100Percent[MIN_IDX] = value.toInt();
    } else
    if ( name.equals("id_pulse_width_p100")) {
      ourConfig.servoPulseWidthPair100Percent[MAX_IDX] = value.toInt();
    }
    printServoRanges();
  } else {
    Serial.print("ERROR: unknown name in set request: ");
    Serial.print("  "); Serial.print(name); Serial.print("="); Serial.println(value);
  }

  if (sendResponse) {
    #ifdef DO_LOG
    Serial.println();
    Serial.print("send response to server: ");
    Serial.println(response);
    #endif
    server.send(htmlResponseCode, "text/plane", response); // send an valid answer
  }
}

void getDataReq() {
  // #define DO_LOG_GET_DQ
  // Serial.print(server.client().remoteIP().toString());
  #ifdef DO_LOG_GET_DQ
  Serial.print(" : getDataReq() :");
  #endif
  String response;
  for (uint8_t i=0; i<server.args(); i++){
    String argName = server.argName(i);
    #ifdef DO_LOG_GET_DQ
    Serial.println("arg :"+argName);
    #endif

   //  Serial.print(argName); Serial.print(",");

    if (argName.equals("id_version")) {
      response += argName + "=" + APP_VERSION + MYSEP_STR;
    } else
    // WiFi stuff
    if (argName.equals("id_wlanSsid")) {
        response += argName + "=" + ourConfig.wlanSsid + MYSEP_STR;
    } else
    if (argName.equals("id_wlanPasswd")) {
        response += argName + "=" + "************" + MYSEP_STR;
    } else
    if (argName.equals("id_apSsid")) {
        response += argName + "=" + ourConfig.apSsid + MYSEP_STR;
    } else
    if (argName.equals("id_apPasswd")) {
      if (String(ourConfig.apPasswd).length() != 0) {
        response += argName + "=" + "************" + MYSEP_STR;
      }
    } else
    if (argName.equals("id_wlanConnetion")) {
      // Serial.print("wifi id_wlanConnetion: ");
      // Serial.println(WiFi.status());
      if (WiFi.status() == WL_CONNECTED) {
        response += argName + "=" + "verbunden, zugewiesene Adresse: " + WiFi.localIP().toString() +  MYSEP_STR;
      } else {
        response += argName + "=" + "nicht verbunden" + MYSEP_STR;
      }
    } else
    if (argName.equals("id_apActive")) {
      if (ourConfig.apIsActive == true) {
        response += argName + "=" + "checked" + MYSEP_STR;
      }
    } else
    if (argName.equals("id_online_status")) {
      Serial.print("wifi status: ");
      Serial.println(WiFi.status());
      if (WiFi.status() == WL_CONNECTED) {
        response += argName + "=online" + MYSEP_STR;
      } else {
        response += argName + "=offline" + MYSEP_STR;
      }
    } else
    // angle measure stuff
    if (argName.equals("id_angleValue")) {
      response += argName + "=" + String(getRoundedAngle()) + MYSEP_STR;
    } else
    if (argName.equals("id_amplitudeValue")) {
      response += argName + "=" + String(getRudderAmplitude()) + MYSEP_STR;
    } else
    if (argName.equals("id_targetAmplitude")) {
      response += argName + "=" + ourTargetAmplitude + MYSEP_STR;
    } else
    if (argName.equals("cpx_flightphase")) {
      if (ourIsMeasureActive) {
        response += String("id_rudderneutral") + "=" + String(ourNullAmpl) + MYSEP_STR;
        response += String("id_ruddermin" ) + "=" + String(ourMinAmpl) + MYSEP_STR;
        response += String("id_ruddermax" ) + "=" + String(ourMaxAmpl) + MYSEP_STR;
      }
    } else
    if (argName.equals("id_rudderSize")) {
      response += argName + "=" + ourRudderSize + MYSEP_STR;
    } else
    if (argName.equals("id_proto_servo_function_def")) {
      response += argName + "=" + ourProtocolData.currentFunctionIdx + MYSEP_STR;
    } else
    if (argName.equals("id_sensortype")) {
      response += argName + "=" + ourSensorTypeName + MYSEP_STR;
    } else
    if (argName.equals("id_invertAngle")) {
      if (ourConfig.angleInversion == -1) {
        response += argName + "=" + "checked" + MYSEP_STR;
      } else {
        response += argName + "=" + "unchecked" + MYSEP_STR;
      }
    } else
    if (argName.equals("id_invertAmplitude")) {
      if (ourConfig.amplitudeInversion == -1) {
        response += argName + "=" + "checked" + MYSEP_STR;
      } else {
        response += argName + "=" + "unchecked" + MYSEP_STR;
      }
    } else
    if (argName.equals("id_resp_calibrate")) {
      if (!isSensorCalibrated()) {
        response += argName + "=" + "Sensor ist nicht kalibriert" + MYSEP_STR;
      } else {
        if ( ourTriggerCalibrateMPU6050 ) {
          response += argName + "=" + "Kalibrierung gestartet ..." + MYSEP_STR;
        } else {
          response += argName + "=" + "Sensor ist kalibriert" + MYSEP_STR;
        }
      }
    } else
    if (argName.equals("id_caloffset_enabled")) {
      if (ourConfig.calibrationOffsetEnabled == true) {
        response += argName + "=" + "checked" + MYSEP_STR;
      } else {
        response += argName + "=" + "unchecked" + MYSEP_STR;
      }
    } else
    if (argName.equals("id_caloffset_h")) {
      response += argName + "=" + ((float)ourConfig.calibrationOffsetHigh)/10 + MYSEP_STR;
    } else
    if (argName.equals("id_caloffset_l")) {
      response += argName + "=" + ((float)ourConfig.calibrationOffsetLow)/10 + MYSEP_STR;
    } else
    if (argName.equals("nm_referenceAxis")) {
      switch (ourConfig.axis) {
         case xAxis:
           response += String("id_xAxis") + "=" + "checked" + MYSEP_STR;
           break;
         case yAxis:
           response += String("id_yAxis") + "=" + "checked" + MYSEP_STR;
           break;
         case zAxis:
           response += String("id_zAxis") + "=" + "checked" + MYSEP_STR;
           break;
      }
    } else
    if (argName.equals("nm_anglePrecision")) {
      if (ourConfig.anglePrecision == P010) {
        response += String("id_anglePrec_P010") + "=" + "checked" + MYSEP_STR;
      } else {
        response += String("id_anglePrec_P001") + "=" + "checked" + MYSEP_STR;
      }
    } else
    if (argName.equals("nm_precisionAmplitude")) {
      if (ourConfig.amplitudePrecision == P001) {
        response += String("id_amplPrec_P001") + "=" + "checked" + MYSEP_STR;
      } else if (ourConfig.amplitudePrecision == P010) {
        response += String("id_amplPrec_P010") + "=" + "checked" + MYSEP_STR;
      } else if (ourConfig.amplitudePrecision == P050) {
        response += String("id_amplPrec_P050") + "=" + "checked" + MYSEP_STR;
      } else if (ourConfig.amplitudePrecision == P100) {
        response += String("id_amplPrec_P100") + "=" + "checked" + MYSEP_STR;
      }
    } else
    if (argName.equals("id_amplitudeCalcMethod")) {
      if (ourConfig.amplitudeCalcMethod == ARC) {
        response += argName + "=" + "Kreisbogen" + MYSEP_STR;
      } else if (ourConfig.amplitudeCalcMethod == CHORD) {
        response += argName + "=" + "Kreissehne" + MYSEP_STR;
      } else if (ourConfig.amplitudeCalcMethod == VERTICAL_DISTANCE) {
        response += argName + "=" + "senkrechter Abstand" + MYSEP_STR;
      }
    } else
    if (argName.equals("nm_distancetype")) {
      if (ourConfig.amplitudeCalcMethod == ARC) {
        response += String("id_distance_arc") + "=" + "checked" + MYSEP_STR;
      } else if (ourConfig.amplitudeCalcMethod == CHORD) {
        response += String("id_distance_chord") + "=" + "checked" + MYSEP_STR;
      } else if (ourConfig.amplitudeCalcMethod == VERTICAL_DISTANCE) {
        response += String("id_vertical_distance") + "=" + "checked" + MYSEP_STR;
      }
    } else
    // servo control stuff
    if (argName.equals("id_pulse_width_min")) {
      response += argName + "=" + String(ourConfig.servoPulseWidthPairFullRange[MIN_IDX]) + MYSEP_STR;
    } else
    if (argName.equals("id_pulse_width_max")) {
      response += argName + "=" + String(ourConfig.servoPulseWidthPairFullRange[MAX_IDX]) + MYSEP_STR;
    } else
    if (argName.equals("id_pulse_width_n100")) {
      response += argName + "=" + String(ourConfig.servoPulseWidthPair100Percent[MIN_IDX]) + MYSEP_STR;
    } else
    if (argName.equals("id_pulse_width_p100")) {
      response += argName + "=" + String(ourConfig.servoPulseWidthPair100Percent[MAX_IDX]) + MYSEP_STR;
    } else
    if (argName.equals("id_pwm_value")) {
      response += argName + "=" + String(getServoPosInMicroSeconds()) + MYSEP_STR;
    } else
    if (argName.equals("id_pwm_setvalue")) {
      response += argName + "=" + String(getServoPosInMicroSeconds())
                  + "=" + String(ourConfig.servoPulseWidthPairFullRange[MIN_IDX])
                  + "=" + String(ourConfig.servoPulseWidthPairFullRange[MAX_IDX])
                  + MYSEP_STR;
    } else
    if (argName.equals("id_percent_value")) {
      response += argName + "=" + String(getServoPosInPercent()) + MYSEP_STR;
    } else
    if (argName.equals("id_percent_setvalue")) {
      response += argName + "=" + String(getServoPosInPercent())
                  + "=" + String(getServoPosInPercent(ourConfig.servoPulseWidthPairFullRange[MIN_IDX]))
                  + "=" + String(getServoPosInPercent(ourConfig.servoPulseWidthPairFullRange[MAX_IDX]))
                  + MYSEP_STR;
    } else
    if (argName.equals("id_pos_slider")) {
      response += argName + "=" + String(getServoPosInPercent())
                  + "=" + String(getServoPosInPercent(ourConfig.servoPulseWidthPairFullRange[MIN_IDX]))
                  + "=" + String(getServoPosInPercent(ourConfig.servoPulseWidthPairFullRange[MAX_IDX]))
                  + MYSEP_STR;
    } else
    if (argName.startsWith("id_btn_set_pos_")) {
      int pos = argName.substring(15).toInt();
      response += argName + "=" + String(convertMicroSeconds2Percent(getPreset(pos))) + MYSEP_STR;
    } else
    if (argName.startsWith("id_btn_set_limit_")) {
      if (argName.equals("id_btn_set_limit_min")) {
        response += argName + "=" + convertMicroSeconds2Percent(ourServoLimit[MIN_IDX]) + "%" + MYSEP_STR;
      } else {
        response += argName + "=" + convertMicroSeconds2Percent(ourServoLimit[MAX_IDX]) + "%" + MYSEP_STR;
      }
    } else
    if (argName.startsWith("id_btn_store_limit_")) {
      if (argName.endsWith("_min")) {
        if ( ourServoLimit[MIN_IDX] == ourConfig.servoPulseWidthPairFullRange[MIN_IDX]) {
          response += argName + "=" + String("limit") + MYSEP_STR;
        } else {
          response += argName + "=" + String("unlimit") + MYSEP_STR;
        }
      } else {
        if ( ourServoLimit[MAX_IDX] == ourConfig.servoPulseWidthPairFullRange[MAX_IDX]) {
          response += argName + "=" + String("limit") + MYSEP_STR;
        } else {
          response += argName + "=" + String("unlimit") + MYSEP_STR;
        }
      }
      // int pos = argName.substring(15).toInt();
      // response += argName + "=" + String(convertMicroSeconds2Percent(getPreset(pos))) + MYSEP_STR;
    } else
    if (argName.startsWith("id_btn_store_pos_")) {
      int pos = argName.substring(17).toInt();
      if (pos < CONFIG_SERVO_PRESET_MAX) {
        int theServoPos = ourConfig.servoPresets[pos];
        float theAngle = ourProtocolData.currentPresetAngles[pos];
        if (theAngle == ANGLE_UNSET_VAL) {
          response += argName + "=-" + MYSEP_STR;
        } else {
          response += argName + "=" + String(getRoundedAngle(theAngle), 1)+"&deg;/"+ String(getRoundedAmplitude(theAngle),1)+"mm"+MYSEP_STR;
        }
      } else {
        Serial.print("ERROR:  illegal idx of store pos for : ");
        Serial.println(argName);
      }
    } else
    if (argName.equals("id_vendor_settings")) {
      String vendor = getVendorString(ourConfig.servoRangeByVendor);
      response += argName + "=" + vendor + MYSEP_STR;
    } else
    if (argName.equals("id_rcvendor")) {
      String vendor = getVendorString(ourConfig.servoRangeByVendor);
      vendor.toLowerCase();
      response += argName+"_"+vendor + "=" + "checked" + MYSEP_STR;
    } else
    if (argName.startsWith("id_rcvendor_")) {
      String vendor = getVendorString(ourConfig.servoRangeByVendor);
      vendor.toLowerCase();
      response += "id_rcvendor_"+vendor + "=" + "checked" + MYSEP_STR;
    } else
    if (argName.equals("id_servo_direction")) {
      if (ourServoDirection == -1) {
        response += argName + "=" + "checked" + MYSEP_STR;
      } else {
        response += argName + "=" + "unchecked" + MYSEP_STR;
      }
    } else
    if (argName.equals("id_wheel_activate")) {
      if (ourWheelActivation == true) {
        response += argName + "=" + "checked" + MYSEP_STR;
      } else {
        response += argName + "=" + "unchecked" + MYSEP_STR;
      }
    } else
    if (argName.equals("id_wheel_factor")) {
      response += argName + "=" + String(ourWheelFactor) + MYSEP_STR;
    } else
    if (argName.equals("id_dataset_descr")) {
      response += argName + "=" + ourProtocolData.datasetDescription + MYSEP_STR;
    } else
    {
      Serial.print("ERROR: unknown name of get request: ");
      Serial.println(argName);
    }
  }
  #ifdef DO_LOG_GET_DQ
  Serial.println();
  Serial.print("response:");
  Serial.println(response);
  #endif
  server.send(200, "text/plane", response.c_str()); //Send the response value only to client ajax request
}

String servoFunctionIdx2Shortcut(int aIdx) {
  // "option_fd_QR-1-R",
  //            10
  return servoFunctionIdx2Def(aIdx).substring(10);
}


String servoFunctionIdx2Def(int aIdx) {
  if (aIdx < RD_LAST) {
    return String(ourFunctionDefMap[aIdx]);
  }
  return String("undefined");
}

int servoFunctionDef2Idx(String aDef) {
  for (int i=0; i < RD_LAST; i++) {
    if (aDef.equals(ourFunctionDefMap[i])) {
      return i;
    }
  }
  Serial.print("ERROR: illegal function def : ");
  Serial.println(aDef);
  return -1;
}


String getVendorString(rc_vendor_t aVendor) {
  String vendor;
  switch (aVendor) {
    case hott:
      vendor = "HoTT";
      break;
    case spektrum:
      vendor = "Spektrum";
      break;
    case multiplex:
      vendor = "Multiplex";
      break;
    case futaba:
      vendor = "Futaba";
      break;
    case custom:
      vendor = "Custom";
      break;
    case taranis:
      vendor = "Taranis";
      break;
    default:
     vendor = "Jeti";
      break;
  }
  return vendor;
}

void handleWebRequests(){
  // if(loadFromSpiffs(server.uri())) return;
  String message = "File Not Detected\n\n";

  message += "client : ";
  message += server.client().remoteIP().toString();
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}

void checkHTMLArguments() {
  Serial.println("checkHTMLArguments()");
  String name = server.arg("name");
  if (name.length() != 0) {
    setDataReq();
  }
}

// =================================
// helper function
// =================================

// similiar to map but will have increased accuracy that provides a more
// symmetric api (call it and use result to reverse will provide the original value)
int my_map(int value, int minIn, int maxIn, int minOut, int maxOut)
{
    const int rangeIn = maxIn - minIn;
    const int rangeOut = maxOut - minOut;
    const int deltaIn = value - minIn;
    // fixed point math constants to my_map accurancy of divide and rounding
    const int fixedHalfDecimal = 1;
    const int fixedDecimal = fixedHalfDecimal * 2;

    return ((deltaIn * rangeOut * fixedDecimal) / (rangeIn) + fixedHalfDecimal) / fixedDecimal + minOut;
}


double irr_low_pass_filter(double aSmoothedValue, double aCurrentValue, double aSmoothingFactor) {
  // IIR Low Pass Filter
  // y[i] := α * x[i] + (1-α) * y[i-1]
  //      := α * x[i] + (1 * y[i-1]) - (α * y[i-1])
  //      := α * x[i] +  y[i-1] - α * y[i-1]
  //      := α * ( x[i] - y[i-1]) + y[i-1]
  //      := y[i-1] + α * (x[i] - y[i-1])
  // mit α = 1- β
  //      := y[i-1] + (1-ß) * (x[i] - y[i-1])
  //      := y[i-1] + 1 * (x[i] - y[i-1]) - ß * (x[i] - y[i-1])
  //      := y[i-1] + x[i] - y[i-1] - ß * x[i] + ß * y[i-1]
  //      := x[i] - ß * x[i] + ß * y[i-1]
  //      := x[i] + ß * y[i-1] - ß * x[i]
  //      := x[i] + ß * (y[i-1] - x[i])
  // see: https://en.wikipedia.org/wiki/Low-pass_filter#Simple_infinite_impulse_response_filter
  return aCurrentValue + aSmoothingFactor * (aSmoothedValue - aCurrentValue);
}

float roundToDot5(double aValue) {
  return round(aValue * 2)/2;
}

void printMPU6050Offsets() {
   Serial.print("Accel Offsets [X,Y,Z]: [");
   Serial.print(mpu.getXAccelOffset());
   Serial.print(",");
   Serial.print(mpu.getYAccelOffset());
   Serial.print(",");
   Serial.print(mpu.getZAccelOffset());
   Serial.print("] X Gyro Offset [x,Y,Z]: [");
   Serial.print(mpu.getXGyroOffset());
   Serial.print(",");
   Serial.print(mpu.getYGyroOffset());
   Serial.print(",");
   Serial.print(mpu.getZGyroOffset());
   Serial.print("]");
   Serial.println();
}

void detectSensor() {
  // supported I2C HW connection schemas
  #define I2C_CONNECTIONS_SIZE 2
  uint8_t cableConnections[2][2] = {
   {D3, D4} ,   /* SDA, SCL */
   {D2, D1}    /* SDA, SCL */
  };

  // supported I2C devices / addresses
  #define I2C_ADDR_SIZE 4
  uint8_t I2CAddresses[I2C_ADDR_SIZE] = {
    MPU6050ADDR1, MPU6050ADDR2,
#ifdef SUPPORT_MMA8451
    MMA8451ADDR1, MMA8451ADDR2,
#endif
  };



  for (int i = 0; i < I2C_CONNECTIONS_SIZE; i++) {
    ourSDA_Pin = cableConnections[i][0];
    ourSCL_Pin = cableConnections[i][1];
    Wire.begin(ourSDA_Pin, ourSCL_Pin);
    for (int j = 0; j<I2C_ADDR_SIZE; j++) {
      ourI2CAddr = I2CAddresses[j];
      mpu = MPU6050(ourI2CAddr);
      Wire.beginTransmission(ourI2CAddr);
      byte result = Wire.endTransmission();
      if (result == 0){
        if (isI2C_MPU6050Addr()) {
          ourSensorTypeName = "MPU-6050/GY521";
        }
#ifdef SUPPORT_MMA8451
        if (isI2C_MMA8451Addr()) {
          ourSensorTypeName = "MMA-8451";
        }
#endif
        Serial.print("Sensor [");
        Serial.print(ourSensorTypeName);
        Serial.print("] found at I2C pins ");
        Serial.print(ourSDA_Pin);
        Serial.print("/");
        Serial.print(ourSCL_Pin);
        Serial.print(" (SDA/SCL) at address 0x");
        Serial.print(ourI2CAddr, HEX);
        Serial.println();
        return;
      }
    }
  }
}


#ifdef SUPPORT_MMA8451
boolean isI2C_MMA8451Addr() {
  boolean retVal = false;
  if (ourI2CAddr == MMA8451ADDR1 || ourI2CAddr == MMA8451ADDR2 ) {
    retVal = true;
  }
  return retVal;
}
#endif

boolean isI2C_MPU6050Addr() {
  boolean retVal = false;
  if (ourI2CAddr == MPU6050ADDR1 || ourI2CAddr == MPU6050ADDR2 ) {
    retVal = true;
  }
  return retVal;
}

#ifdef SUPPORT_MMA8451
void initMMA8451() {
  Serial.println("initializing MMA8451 device");
  mma = Adafruit_MMA8451();

  Serial.println("Testing MMA-8452 device connection...");
  if (mma.begin(ourSDA_Pin, ourSCL_Pin, ourI2CAddr)) {
    Serial.print("MMA8451 connection successful at : 0x");
    Serial.println(ourI2CAddr, HEX);
  } else {
    Serial.println("MMA8451 connection failed. MMA8452 assumed");
    ourSensorTypeName="MMA-8452";
  }
  mma.setRange(MMA8451_RANGE_2_G);
}
#endif

void initMPU6050() {
  #ifdef USE_MPU6050_MPU
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #endif
  Serial.println("Initializing MPU6050 device");
  Serial.println("Testing device connection...");
  mpu = MPU6050(ourI2CAddr);
  Serial.println("resetting the MPU6050 device ...");
  mpu.reset();
  delay(50);
  mpu.initialize();
  Serial.print("MPU6050 deviceId is: 0x");
  Serial.println(mpu.getDeviceID(), HEX);
  Serial.print("MPU6050 FullScaleAccelRange: ");
  Serial.println(mpu.getFullScaleAccelRange());
  if (mpu.testConnection()) {
    Serial.print("MPU6050 connection successful at : 0x");
    Serial.println(ourI2CAddr, HEX);
  } else {
    Serial.println("ERROR: MPU6050 connection failed");
  }
  if (isSensorCalibrated()) {
    // set stored calibration data
    mpu.setXAccelOffset(ourConfig.xAccelOffset);
    mpu.setYAccelOffset(ourConfig.yAccelOffset);
    mpu.setZAccelOffset(ourConfig.zAccelOffset);
    mpu.setXGyroOffset(ourConfig.xGyroOffset);
    mpu.setYGyroOffset(ourConfig.yGyroOffset);
    mpu.setZGyroOffset(ourConfig.zGyroOffset);
    Serial.println("MPU6050 ist kalibriert mit folgenden Werten: ");
    printMPU6050Offsets();
  } else {
    Serial.println("MPU6050 ist nicht kalibriert !!!!");
    Serial.println("MPU6050 hat folgende initialen Werte: ");
    printMPU6050Offsets();
  }

  #ifdef USE_MPU6050_MPU

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
#endif
}

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
ICACHE_RAM_ATTR void dmpDataReady() {
    mpuInterrupt = true;
}

void triggerCalibrateMPU6050() {
  ourTriggerCalibrateMPU6050 = true;
}

boolean isSensorCalibrated() {
  return (ourConfig.xAccelOffset != 0 || ourConfig.yAccelOffset != 0 || ourConfig.zAccelOffset != 0);
}

void calibrateMPU6050() {
  if (ourTriggerCalibrateMPU6050 == true) {
    printMPU6050Offsets();
    mpu.CalibrateAccel(15);
    mpu.CalibrateGyro(15);
    printMPU6050Offsets();
    Serial.println("\nat 1500 Readings");
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    Serial.println("\nat 600 Readings");
    printMPU6050Offsets();
    Serial.println();
    mpu.CalibrateAccel(1);
    mpu.CalibrateGyro(1);
    Serial.println("700 Total Readings");
    printMPU6050Offsets();
    Serial.println();
    mpu.CalibrateAccel(1);
    mpu.CalibrateGyro(1);
    Serial.println("800 Total Readings");
    printMPU6050Offsets();
    Serial.println();
    mpu.CalibrateAccel(1);
    mpu.CalibrateGyro(1);
    Serial.println("900 Total Readings");
    printMPU6050Offsets();
    Serial.println();
    mpu.CalibrateAccel(1);
    mpu.CalibrateGyro(1);
    Serial.println("1000 Total Readings");
    printMPU6050Offsets();
    ourConfig.xAccelOffset = mpu.getXAccelOffset();
    ourConfig.yAccelOffset = mpu.getYAccelOffset();
    ourConfig.zAccelOffset = mpu.getZAccelOffset();
    ourConfig.xGyroOffset = mpu.getXGyroOffset();
    ourConfig.yGyroOffset = mpu.getYGyroOffset();
    ourConfig.zGyroOffset = mpu.getZGyroOffset();
    ourTriggerCalibrateMPU6050 = false;
  }
}


void triggerRestart() {
  // the restart has to be delayed to avoid, that the response to the restart request is not
  // answerd properly to the http-client. If there is no response the client, will resend the
  // restart request ;-(
  ourTriggerRestart = millis();
}

void restartESP() {
  if (ourTriggerRestart != 0) {
    unsigned long delay = millis() - ourTriggerRestart;
     // wait for 200ms with the restart
    if (delay > 200) {
      ourTriggerRestart = 0;
      ESP.restart();
    }
  }
}

void doAsync() {
  calibrateMPU6050();
  restartESP();
}

// =================================
// WiFi functions
// =================================

void setupWiFi() {
  // first try to connect to the stored WLAN, if this does not work try to
  // start as Access Point
  // strncpy(ourConfig.wlanSsid , "hardcoded SSID", CONFIG_SSID_L);
  // strncpy(ourConfig.wlanPasswd, "hardcoded PWD", CONFIG_PASSW_L);

  if (String(ourConfig.wlanSsid).length() != 0 ) {
    WiFi.persistent(false);
    WiFi.mode(WIFI_STA) ; // client mode only
    WiFi.begin(ourConfig.wlanSsid, ourConfig.wlanPasswd);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(ourConfig.wlanSsid);
    int connectCnt = 0;
    while (WiFi.status() != WL_CONNECTED && connectCnt++ < 20) {
      delay(500);
      Serial.print(".");
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
   Serial.println("success!");
    Serial.print("IP Address is: ");
    Serial.println(WiFi.localIP());
    if (MDNS.begin("sensor")) {  //Start mDNS with name esp8266
      Serial.println("MDNS started");
    }

  } else {
    Serial.print("cannot connect to SSID :");
    Serial.println(ourConfig.wlanSsid);
    WiFi.mode(WIFI_AP) ; // client mode only
  }
  if (ourConfig.apIsActive || WiFi.status() != WL_CONNECTED) {
    Serial.print("Starting WiFi Access Point with  SSID: ");
    Serial.println(ourConfig.apSsid);
    WiFi.softAPConfig(ourApIp, ourApIp, ourNetmask);    //Password length minimum 8 char
    boolean res = WiFi.softAP(ourConfig.apSsid, ourConfig.apPasswd);    //Password length minimum 8 char
    if(res ==true) {
      IPAddress myIP = WiFi.softAPIP();
      ourDNSServer.setTTL(20);
      ourDNSServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
      ourDNSServer.start(DNS_PORT, "*", myIP);
      Serial.println("AP setup done!");
      Serial.print("Host IP Address: ");
      Serial.println(myIP);
      Serial.print("Please connect to SSID: ");
      Serial.print(ourConfig.apSsid);
      Serial.print(", PW: ");
      Serial.println(ourConfig.apPasswd);
      Serial.print("type in browser : ");
      Serial.println(myIP);
      Serial.print("or              : ");
      Serial.println("uhu.local");
    }
  }
}

// =================================
// EEPROM functions
// =================================

void printConfig(const char* aContext) {
  Serial.println(aContext);
  Serial.println("WiFi settings:");
  Serial.print("  cfg version = "); Serial.println(ourConfig.version);
  Serial.print("  wlanSsid    = "); Serial.println(ourConfig.wlanSsid);
  Serial.print("  wlanPasswd  = "); Serial.println(ourConfig.wlanPasswd);
  Serial.print("  apIsActive  = "); Serial.println(ourConfig.apIsActive);
  Serial.print("  apSsid      = "); Serial.println(ourConfig.apSsid);
  Serial.print("  apPasswd    = "); Serial.println(ourConfig.apPasswd);
  Serial.println("angle sensor settings:");
  Serial.print("  axis                     = "); Serial.println(ourConfig.axis);
  Serial.print("  amplitudePrecision       = "); Serial.println(ourConfig.amplitudePrecision);
  Serial.print("  anglePrecision           = "); Serial.println(ourConfig.anglePrecision);
  Serial.print("  angleInversion           = "); Serial.println(ourConfig.angleInversion);
  Serial.print("  amplitudeInversion       = "); Serial.println(ourConfig.amplitudeInversion);
  Serial.print("  xAccelOffet              = "); Serial.println(ourConfig.xAccelOffset);
  Serial.print("  yAccelOffet              = "); Serial.println(ourConfig.yAccelOffset);
  Serial.print("  zAccelOffet              = "); Serial.println(ourConfig.zAccelOffset);
  Serial.print("  amplitudeCalcMethod      = "); Serial.println(ourConfig.amplitudeCalcMethod);
  Serial.print("  calibrationOffsetEnabled = "); Serial.println(ourConfig.calibrationOffsetEnabled);
  Serial.print("  calibrationOffsetHigh    = "); Serial.println(ourConfig.calibrationOffsetHigh);
  Serial.print("  calibrationOffsetLow     = "); Serial.println(ourConfig.calibrationOffsetLow);

  Serial.println("servo control settings:");
  Serial.print("  servoRangeByVendor = "); Serial.println(ourConfig.servoRangeByVendor);
  Serial.print("  servoInversion     = "); Serial.println(ourConfig.servoInversion);
  for (int i=0; i<CONFIG_SERVO_PRESET_MAX; i++) {
    Serial.println(String("  servoPresets[")+i+"]    = " + ourConfig.servoPresets[i]);
  }
  Serial.print("  FullRange[MIN_IDX] = "); Serial.println(ourConfig.servoPulseWidthPairFullRange[MIN_IDX]);
  Serial.print("  FullRange[MAX_IDX] = "); Serial.println(ourConfig.servoPulseWidthPairFullRange[MAX_IDX]);
  Serial.print("  100Percent[MAX_IDX]= "); Serial.println(ourConfig.servoPulseWidthPair100Percent[MIN_IDX]);
  Serial.print("  100Percent[CFG_MIN]= "); Serial.println(ourConfig.servoPulseWidthPair100Percent[MAX_IDX]);
}

void setDefaultConfig() {
  Serial.println("setDefaultConfig()");
  // Reset EEPROM bytes to '0' for the length of the data structure
  strncpy(ourConfig.version , CONFIG_VERSION, CONFIG_VERSION_L);
  ourConfig.axis = xAxis;
  ourConfig.amplitudeInversion = 1;
  ourConfig.angleInversion = 1;
  ourConfig.apIsActive=true;
  ourConfig.anglePrecision = P010;
  ourConfig.amplitudePrecision = P050;
  strncpy(ourConfig.wlanSsid , "", CONFIG_SSID_L);
  strncpy(ourConfig.wlanPasswd, "", CONFIG_PASSW_L);
  strncpy(ourConfig.apSsid , "UHU", CONFIG_SSID_L);
  strncpy(ourConfig.apPasswd, "12345678", CONFIG_PASSW_L);
  ourConfig.xAccelOffset = 0;
  ourConfig.yAccelOffset = 0;
  ourConfig.zAccelOffset = 0;
  ourConfig.amplitudeCalcMethod = ARC;
  ourConfig.calibrationOffsetHigh = 450;
  ourConfig.calibrationOffsetLow = -450;
  ourConfig.calibrationOffsetEnabled = false;
  ourConfig.servoInversion = false;
  ourConfig.servoRangeByVendor = jeti;
  initServoControllerConfig();
  saveConfig();
}


void saveConfig() {
  Serial.println("saveConfig()");
  printConfig("saveConfig - start");
  // Save configuration from RAM into EEPROM
  EEPROM.begin(512);
  EEPROM.put(0, ourConfig );
  delay(10);
  EEPROM.commit();                      // Only needed for ESP8266 to get data written
  EEPROM.end();                         // Free RAM copy of structure
  printConfig("saveConfig - end");
}

void loadConfig() {
  Serial.println("loadConfig()");
  // Loads configuration from EEPROM into RAM
  EEPROM.begin(512);
  EEPROM.get(0, ourConfig );
  EEPROM.end();
  // config was never written to EEPROM, so set the default config data and save it to EEPROM
  if ( String(CONFIG_VERSION) == ourConfig.version ) {
     // ok do nothing, this is the expected version
  } else if (String("RSV1") == ourConfig.version ) {
     // ok do nothing, this is the expected version
    Serial.print("old but compatible config version found: ");
    Serial.println(ourConfig.version);
  } else {
    Serial.print("unexpected config version found: ");
    Serial.println(ourConfig.version);
    setDefaultConfig();
  }
}

void checkHWReset(uint8_t aPin) {
  // pull the given pin
  Serial.print("checking HW reset pin ");
  Serial.print(aPin);
  Serial.println(" for force HW config reset");
  pinMode(aPin, INPUT_PULLUP);
  delay(100);
  uint8_t cnt=0;
  for (int i=0; i<10; i++) {
    delay(20);
    if (digitalRead(aPin) == LOW) {
      cnt++;
    }
  }
  if (cnt == 10) {
    Serial.print("configuration reset by HW pin to GRD !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ");
    // Serial.println(cnt);
    setDefaultConfig();
  }
}
