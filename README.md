
<div class="PageDoc">

<div class="header">

<div class="headertitle">

<div class="title">

RC-Einstell-Tool Projekt zur Ansteuerung eines Modellflug-Servos und zum
Vermessen von Ruderausschlägen mittels eines Winkelsensors

</div>

</div>

</div>

<div class="contents">

<div class="textblock">

# <span id="intro_sec_de" class="anchor"></span> Übersicht

Das RC Einstell Tool kann eine Servo mittels PWM Pulserzeugung steuern
und die Ansteuerdaten anzeigen. Zusatzlich kann mittels eines
Winkelsensors der Ruderausschlag sehr genau vermessen werden (besser als
0.5mm). Die Benutzeroberfläches ist als Web-Oberfläche ausgelegt, was
eine Bedienung auch mit dem Smartphone zulässt. Eine einfach kleine
Power-Bank dient als Stromversorgung und man kann extrem einfach und
schnell auf der Werkbank beim Bau oder bei Endeinstellarbeiten alle
wichtigen Daten schnell, genau und vollkommen reproduzierbar ablesen.
Hiermit lassen sich Einstellarbeiten auf der Werkbank oder am
aufgebauten Modell sehr professionell, schnell, genau und reproduzierbar
durchführen. Dieses Projekt ist die Zusammenführung von
<https://github.com/Pulsar07/RuderwegMessSensor> und
<https://github.com/Pulsar07/ServoController>
![Architektur](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_Architecture.png)

# <span id="hardware_sec_de" class="anchor"></span> Hardware

## <span id="hardware_subsec_de_mk" class="anchor"></span> Mikrokontroller

Als Mikrokontroller wird der Wemos D1/ESP8266 benutzt, der ausreichende
Rechenpower und Speicherresourcen bietet und eine WLAN Schnittstelle
hat. Es stehen ausgereifte Bibliotheken zur Nutzung der WiFi
Schnittstelle, zur Bereitstellung eines Web-Servers (GUI) und auch zur
Ansteuerung von Servos zur Verfügung.

Hier ein paar Links:

  - <https://www.az-delivery.de/products/d1-mini>
  - <https://github.com/esp8266/Arduino>

## <span id="hardware_subsec_de_ms" class="anchor"></span> Messsensor

Als Messsensor wird der GY-521/MPU-6050 benutzt. Die Genauigkeit liegt
nach Kalibrierung bei Winkeln bis +/- 45° kleiner als 0.5°. Der Baustein
MPU-6050 wird von einer wirklich sehr gut gemachten Libs von J.Rowberg
unterstützt (siehe Link) Der Aufbau sollte auch ohne den Winkelsensor
funktionieren, um nur die Funktion des Servo-Controllers zu erhalten.

Hier ein paar Links:

  - <https://www.invensense.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf>
  - <https://www.az-delivery.de/products/gy-521-6-achsen-gyroskop-und-beschleunigungssensor>
  - <https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050>

## <span id="hardware_subsec_de_sc" class="anchor"></span> Servo-Controller

Für den Servo-Controller ist außer dem Signal-Kabel vom D7-Pin des
Microcontrollers keinerlei Hardware notwendig. Will man nur einen
Winkelmesser bauen, kann dieses Kabel einfach weggelassen werden.

## <span id="hardware_subsec_de_sp" class="anchor"></span> Schaltplan

Der Schaltplan ist denkbar einfach. Es werden nur 4 Verbindungen
zwischen Sensorplatine und Mikrokontroller benötigt. Das Layout und die
Software sind so ausgelegt, dass mit einer Stiftleiste 4x1 das
Sensorboard mit dem Gesicht in Richtung Mikrokontroller direkt verlötet
werden kann.
![Schaltplan](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_CircuitDiagram.png)  
Der Servo wird lediglich mittels eines Signal-Kabels auf den D7 Pin des
Microcontrollers verbunden. Die Plus- und Ground-Verbindung wird mittels
eines auftrennten und wieder zusammengefügten USB-MicroUSB Kabels
hergestellt. Die Servo Stromversorgung wird einfach mittels eines
Servo-Buchsenkabels erstellt und sollte nicht über den Microkontroller
erfolgen, da die Stromstärke zu hoch für diesen ist.

## <span id="hardware_subsec_de_kv" class="anchor"></span> Ruderklemmvorrichtung

Hier kann der geneigte Modellbauer die Kombination aus Microprozessor
und Sensor in eine geeignete Klemmvorrichtung \[ein-\]bauen, die ein
rutschfestes Klemmen am Ruder gewährleistet. So sieht der Prototyp des
Authors aus:
![Klemm](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_InOperation.jpg)

# <span id="hmi_sec_de" class="anchor"></span> Anleitung

## <span id="hmi_subsec_de_ib" class="anchor"></span> Inbetriebnahme

  - Stromversorgung
      - Der Sensor / das Mikroprozessorboard ist mit einem
        Micro-USB-Anschluss ausgestattet, hier kann man jedes
        handelsübliche USB-Netzteil anschließen oder besser jede
        normale Powerbank. Damit ist man in der Werkstatt oder auf dem
        Flugfeld mobil ausgestattet.
  - WiFi
      - Der Sensor muss zuerst mit Smartphone oder PC verbunden werden.
        Dazu stellt der Sensor per WiFi einen Accesspoint mit der SSID
        "UHU" und Kennwort "12345678" zur Verfügung. Ist das Gerät mit
        diesem WLAN verbunden, kann im Web-Browser über die Adresse
        <http://192.168.4.1> die Benutzeroberfläche benutzt und der
        Sensor konfiguriert werden. Sowohl obige SSID als auch das
        Kennwort können danach geändert werden.
      - Auf der Einstellseite kann eine SSID und ein Kennwort für ein
        WLAN (WLAN-Client) konfiguriert werden, mit dem sich der Sensor
        verbinden. Dabei wird dem Sensor eine IP-Adresse zugewiesen, die
        am WLAN-Router abgefragt werden muss. Änderungen der WLAN
        Einstellungen müssen gespeichert werden und werden erst nach
        Neustart aktiv.
      - Ist die Verbindung zu einem WLAN konfiguriert (WLAN-Client),
        kann auf der Einstellungsseite, der Accesspoint deaktiviert
        werden (nach Speichern und Neustart). Kann beim Neustart keine
        Verbindung zum konfigurierten WLAN aufgebaut werden, wird der
        Accesspoint-Mode aber trotzdem aktiviert, damit ein Zugang zum
        Gerät möglich ist.
  - Nutzung des Sensorboard GY-521 mit MPU-6050
      - Genauigkeit: Der MEMS Chip des MPU-6050 sollte Winkelauflösungen
        besser als 0.5° bei 45° Ausschlag messen können, was bei einer
        60mm Rudertiefe von 60mm einen Fehler von kleiner als 0.5mm
        ergibt. Zudem sind diverse Anzeigegenauigkeiten für die Winkel
        und die Ruderwegs-Messung auswählbar. Die Anzeige hat zwar immer
        2 Dezimalstellen, intern wird aber gerundet.
      - Experten-Einstellungen:
          - Kalibrierung: Damit der MPU-6050 allerdings diese
            Genauigkeit erreicht, muss er nachträglich kalibriert
            werden. Die Software unterstützt diese Funktion und kann die
            Werte intern speichern. Zur Kalibrierung muss die
            GY-521-Sensorplatine mit der flachen Rückseite möglichst
            exakt horizontal aufgelegt werden. Dann den Kalibrier-Button
            drücken und ca. 5s warten bis die Kalibrierung beendet ist.
            Dabei sollte die Auflagefläche (Tisch) nicht bewegt werden
            und frei von Vibrationen sein.
          - Einbaulage: Die Sensorplatine sollte auch genau so, wie bei
            der Kalibrierung, betrieben werden. Also die flache Seite
            nach unten und die Seite mit den Elektronikbausteinen nach
            oben. Nur so wird die oben genannte Genauigkeit erreicht.
          - Achsen und Anzeige-Genauigkeit: Auf der Konfigurationsseite,
            kann die Bezugs-Achse der Winkelmessung, je nach Einbaulage
            in der Klemmeinrichtung ausgewählt werden.
          - Kalibrierungsoffset: Hier können Kalibrierungs-Messwerte für
            +/- 45° Referenzmessungen eingebeben werden und
            aktiviert/deaktiviert werden, um die höchst mögliche
            Genauigkeit zu erreichen. Damit werden dann die Messwerte
            auf die Offsetwerte interpoliert. Der Grund hierfür ist, wie
            die Erfahrung zeigt, dass die verfügbaren günstggen
            China-Importe, nicht die höchste Qualität aufweisen. Z.T.
            lassen sich diese Sensoren einfach nicht kalibrieren. Ein
            manuelles Kalibrieren mit diesem Kalibrierungsoffset, bringt
            jedoch meist den gewünschten Erfolg (siehe
            Experten-Einstellungen) und gewährleistet genaues Arbeiten.

## <span id="hmi_subsec_de_me" class="anchor"></span> Winkel-Messung

  - Der mit dem Mikrokontroller verbundene Messensor sollte mit einer
    Klemmvorrichtung fest verbunden sein, und kann dann einfach an eine
    beliebige Stelle des Ruders aufgeklemmt werden. Die Ruderdrehachse,
    sollte möglichst parallel zur ausgewählten Dreh-Achse (X- oder
    Y-Achse) sein. Wie schon beschrieben, muss der Sensor mit dem
    Gesicht nach oben auf dem Ruder befestigt sein.
  - Einschränkungen: Der Sensor kann nur Winkel in Bezug auf die
    Schwerkraft messen. Somit sind Ruderwegsmessungen für das
    Seitenruder nur möglich wenn der Rumpf um 90° gedreht liegt.
  - Der Ruderweg ist abhänig von der Rudertiefe. Diese ist an der Stelle
    zu Messen, an der man den Ruderweg messen will. In der
    Web-Oberfläche des Sensor kann diese Rudertiefe eingegeben werden.
  - Ist der Sensor so auf dem Ruder angebracht, und die Rudertiefe
    eingestellt, ist die Ruderstellung in die Null-Lage zu bringen.
    Jetzt können Winkel und Ruderweg per "Tara"-Button auf 0 gesetzt
    werden.
  - Bewegt man das Ruder nun nach oben oder unten werden die Ausschläge
    in Grad und Millimeter angezeigt. Sollte das Vorzeichen nicht den
    Erwartungen entsprechen, kann dies bei den Einstellungen angepasst
    werden.
  - Zur Flugphasenmessung kann die Min-/Max-Rudermessung benutzt werden.
    Hier sollte man das Ruder in die Neutralstellung der Flugphase
    bringen. Nun den Schalter für die Min-/Max-Ruderweg-Messung
    aktivieren. Damit wird der aktuelle Ruderausschlag als Neutralwert
    übernommen. Jetzt können die beiden Min-/Max-Werte angefahren
    werden. Alle drei Werte werden bis zur Deaktiverung der Messung
    angezeigt.
    ![AngleSensorPage](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_AngleSensorPage.png)

## <span id="hmi_subsec_de_sc" class="anchor"></span> Servo-Controller

Die Bedienung am Web-GUI ist denkbar einfach. Die Servo-Position kann
über zwei Eingabefelder prozentual oder als Impulsbreite gesteuert
werden. Zusätzlich kann die Servoposition über ein Slider-Widget
gesteuert werden. Zudem erlaubt eine aktivierbare Maus-Wheel Funktion,
mit Beschleunigungsfaktor, das Steuern des Servowegs mittels der Maus.
Es sind für diverse Aufgaben noch 5 vordefinierte und einstellbare
(Save) Positions-Buttons verfügbar. Die Limit-Buttons, können zum
Begrenzen des Servo-Wegs benutzt werden, um ein versehntliches
Überfahren eines mechanischen Limits zu verhindern. Auf der
Einstellseite, können die Limits und 100%-Settings für verschiedene
Hersteller von RC-Systemen voreingestellt werden, damit die Anzeige
exakt mit den Werte des genutzten RC-System übereinstimmt.
![ServoPage](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_ServoPage.png)

## <span id="hmi_subsec_de_ms" class="anchor"></span> Steuern und Messen

In dieser Ansicht kann sowohl der Servo gesteurt als auch der
Winkel-Sensor abgelesen werden. Dies ist vor allem bei Servo- und
Gestänge-Einbauten ein große Hilfe.
![MultiToolPage](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_MultiToolPage.png)
Zusätzlich können den in den Preset-Buttons gespeicherten Servo-Werte
(und die gemessenen Winkel- und Ruderauschlagswerte) und eine
Nutzer-Beschreibung des Datensatzes festgehalten und mittels der
Funktion "Zeige Protokoll" in Tabellenform ausgegeben werden.
![ProtocolPage](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_ProtocolPage.png)

Weitere Details gibt es unter [Albatross, Seite für Modellflug und
Technik](http://www.so-fa.de/nh/RcSetupTool)

## <span id="hmi_subsec_de_flash" class="anchor"></span> Bin-Datei direkt zum Flashen

ist im bin-Verzeichnis verfügbar. Download NodeMCU-Flasher (z. Bspl. Von
hier:
<https://github.com/marcelstoer/nodemcu-pyflasher/releases/tag/v4.0> )  
![UploadBin](https://raw.githubusercontent.com/Pulsar07/RcSetupTool/master/doc/img/RCST_UploadBin.jpg)  
Flasher-Programm starten und Port wählen, Bin-Datei öffnen und 115200
Baud einstellen.  
Flash-Button drücken.  
Das war es ;-)  

</div>

</div>

</div>

-----

<span class="small">Generated by  
[![doxygen](doxygen.png)](http://www.doxygen.org/index.html)
1.8.15</span>
