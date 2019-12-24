const char ADMIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
 <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
 <meta name="viewport" content="width=device-width, initial-scale=1">
 <head>
  <meta http-equiv="cache-control" content="no-cache, must-revalidate, post-check=0, pre-check=0" />
  <meta http-equiv="cache-control" content="max-age=0" />
  <meta http-equiv="expires" content="0" />
  <meta http-equiv="expires" content="Tue, 01 Jan 1980 1:00:00 GMT" />
  <meta http-equiv="pragma" content="no-cache" />
  <link rel="stylesheet" href="./styles.css">
  <script type="text/javascript" src="./script.js"></script>
  <title>RC-Einstell-Tool</title>
 </head>
 <body>
  <div id="id_body">
   <div class="container">
    <div class="row">
     <div class="col-appname">RC-Einstell-Tool:</div>
     <div class="col-version">Version: <span id="id_version">0.00</span></div>
    </div>
   </div>
   <div class="container">
    <h2>Einstellungen</h2>
   </div>
   <hr> <!-- ------------------------------------------------------------ -->
   <div class="container">
    <h3>WiFi - Einstellungen</h3>
    <p>&Auml;nderungen des WiFi-Einstellungen m&uuml;ssen zuerst gespeichert werden und wirken sich erst nach Neustart aus.</p>
    <h4>Zugangsdaten zur Verbindung mit WLAN:</h4>
    <div class="row">
     <div class="col-setting-values">
      <input type="text" id="id_wlanSsid" name="ssid"
       onchange="sendNameValue(this.id, this.value)" placeholder="WLAN SSID">
     </div>
     <div class="col-setting-descr">
      <label for="ssid">WLAN - SSID (max. 15 Zeichen)</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="text" id="id_wlanPasswd" name="password"
       onchange="sendNameValue(this.id, this.value)" placeholder="WLAN Passwort">
     </div>
     <div class="col-setting-descr">
      <label for="password">WLAN - Passwort (max. 63 Zeichen) </label>
     </div>
    </div>
    <p>WLAN ist: <span id="id_wlanConnetion">nicht verbunden</span></p>
    <h4>Zugangsdaten f&uuml;r AccessPoint  / 192.168.4.1 : </h4>
    <div class="row">
     <div class="col-setting-values">
      <input class="config" type="checkbox" id="id_apActive" name="id_apActive" value="xx"
       onchange="sendNameValue(this.id, this.checked)" >
     </div>
     <div class="col-setting-descr">
      <label for="id_apActive"> AccessPoint ist aktiv</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="text" id="id_apSsid" name="apssid"
       onchange="sendNameValue(this.id, this.value)" placeholder="AccessPoint SSID">
     </div>
     <div class="col-setting-descr">
      <label for="id_apSsid">AccesPoint - SSID (max. 15 Zeichen)</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="text" id="id_apPasswd" name="id_apPasswd"
       onchange="sendNameValue(this.id, this.value)" placeholder="AccessPoint Passwort">
     </div>
     <div class="col-setting-descr">
      <label for="id_apPasswd">
       Passwort des AccessPoint (AP) Zuganges (min. 8, max. 64 Zeichen) </label>
     </div>
    </div>
   </div>
   <hr> <!-- ------------------------------------------------------------ -->
   <div class="container">
    <h3>Servo-Controller - Einstellungen</h3>
    <div class="row">
     <div class="col-setting-values">
      <label class="input">
       min pulse in &micro;s
       <input class="config cl_pwm_input" type="number" id="id_pulse_width_min" 
        onchange="sendNameValue(this.id, this.value)"
        maxlength="4" min="0" max="5000" step="1" value="0" readonly="true" disabled="true">
      </label>
      <label class="input">
       -100% pulse in &micro;s
       <input class="config cl_pwm_input" type="number" id="id_pulse_width_n100" 
        onchange="sendNameValue(this.id, this.value)"
        maxlength="4" min="0" max="5000" step="1" value="0" readonly="true" disabled="true">
      </label>
      <label class="input">
       +100% pulse in &micro;s
       <input class="config cl_pwm_input" type="number" id="id_pulse_width_p100" 
        onchange="sendNameValue(this.id, this.value)"
        maxlength="4" min="0" max="5000" step="1" value="0" readonly="true" disabled="true">
      </label>
      <label class="input">
       max pulse in &micro;s
       <input class="config cl_pwm_input" type="number" id="id_pulse_width_max" 
        onchange="sendNameValue(this.id, this.value)"
        maxlength="4" min="0" max="5000" step="1" value="0" readonly="true" disabled="true">
      </label>
     </div>
     <div class="col-setting-descr">
      <label>Pulsweiten in Mikrosekunden MIN,-100%,+100,MAX</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <label class="input">
       Jeti
      <input class="config" type="radio" id="id_rcvendor_jeti" name="nm_rcvendor" value="Jeti"
       onchange="sendNameValue(this.id, this.value); setClassElementsReadonly('cl_pwm_input', 'true');">
      </label>
      <label class="input">
       HoTT
      <input class="config" type="radio" id="id_rcvendor_hott" name="nm_rcvendor" value="HoTT"
       onchange="sendNameValue(this.id, this.value); setClassElementsReadonly('cl_pwm_input', 'true');">
      </label>
      <label class="input">
       Spektrum
      <input class="config" type="radio" id="id_rcvendor_spektrum" name="nm_rcvendor" value="Spektrum"
       onchange="sendNameValue(this.id, this.value); setClassElementsReadonly('cl_pwm_input', 'true');">
      </label>
      <label class="input">
       Futaba
      <input class="config" type="radio" id="id_rcvendor_futaba" name="nm_rcvendor" value="Futaba"
       onchange="sendNameValue(this.id, this.value); setClassElementsReadonly('cl_pwm_input', 'true');">
      </label>
      <label class="input">
       Taranis
      <input class="config" type="radio" id="id_rcvendor_taranis" name="nm_rcvendor" value="Taranis"
       onchange="sendNameValue(this.id, this.value); setClassElementsReadonly('cl_pwm_input', 'true');">
      </label>
      <label class="input">
       MPX
      <input class="config" type="radio" id="id_rcvendor_multiplex" name="nm_rcvendor" value="Multiplex"
       onchange="sendNameValue(this.id, this.value); setClassElementsReadonly('cl_pwm_input', 'true');">
      </label>
      <label class="input">
       Custom
      <input class="config" type="radio" id="id_rcvendor_custom" name="nm_rcvendor" value="Custom"
       onchange="sendNameValue(this.id, this.value); setClassElementsReadonly('cl_pwm_input', 'false');">
      </label>
     </div>
     <div class="col-setting-descr">
      <label for="id_rcvendor_jeti">Servo PWM Werte wie Hersteller </label>
     </div>
    </div>
   </div>

   <hr> <!-- ------------------------------------------------------------ -->

   <div class="container">
    <h3>Einstellungen für den Ruderweg-Messsensor</h3>

    <h4>Winkel - Einstellungen:</h4>
    <div class="row">
     <div class="col-setting-values">
      <input type="checkbox" id="id_invertAngle" name="invertAngle" value="x"
       onchange="sendNameValue(this.id, this.checked)">
     </div>
     <div class="col-setting-descr">
      <label for="id_invertAngle">Invertiere die Winkelanzeige (+/-)</label>
     </div>
    </div>

    <div class="row">
     <div class="col-setting-values">
      <input type="radio" id="id_anglePrec_P001" name="nm_anglePrecision" value="P001"
       onchange="sendNameValue(this.name, this.value)">
     </div>
     <div class="col-setting-descr">
      <label for="id_anglePrec_P001">0.01&deg;-Anzeige-Genauigkeit</label>
     </div>
    </div>

    <div class="row">
     <div class="col-setting-values">
      <input type="radio" id="id_anglePrec_P010" name="nm_anglePrecision" value="P010"
        onchange="sendNameValue(this.name, this.value)" >
     </div>
     <div class="col-setting-descr">
      <label for="id_anglePrec_P010"> 0.1&deg;-Anzeige-Genauigkeit</label>
     </div>
    </div>

    <h4>Winkel - Bezugsachse</h4>
    <p>Wähle die Bezugsachse aus die benutzt werden soll:</p>
    <div class="row">
     <div class="col-setting-values">
      <input type="radio" id="id_xAxis" name="nm_referenceAxis" value="xAxis"
       onchange="sendNameValue(this.name, this.value)" >
     </div>
     <div class="col-setting-descr">
      <label for="id_xAxis"> X-Achse</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="radio" id="id_yAxis" name="nm_referenceAxis" value="yAxis"
        onchange="sendNameValue(this.name, this.value)" >
     </div>
     <div class="col-setting-descr">
      <label for="id_yAxis"> Y-Achse</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="radio" id="id_zAxis" name="nm_referenceAxis" value="zAxis"
        onchange="sendNameValue(this.name, this.value)" >
     </div>
     <div class="col-setting-descr">
      <label for="id_zAxis"> Z-Achse</label>
     </div>
    </div>
    <p>Auf dem Sensor sind oft Pfeile mit den Drehachsen aufgedruckt.
      Die X-Achse liegt meist orthogonal zur langen Ausrichtung des Chips</p>

    <h4>Ruderausschlag - Einstellungen:</h4>
    <div class="row">
     <div class="col-setting-values">
      <input type="checkbox" id="id_invertAmplitude" name="invertAmplitudeValue" value="xx"
        onchange="sendNameValue(this.id, this.checked)">
     </div>
     <div class="col-setting-descr">
      <label for="id_invertAmplitude"> Invertiere die Weganzeige (+/-)</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="radio" id="id_amplPrec_P001" name="nm_precisionAmplitude" value="P001"
        onchange="sendNameValue(this.name, this.value)" >
     </div>
     <div class="col-setting-descr">
      <label for="id_amplPrec_P001"> 0.01 mm-Anzeige-Genauigkeit</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="radio" id="id_amplPrec_P010" name="nm_precisionAmplitude" value="P010"
        onchange="sendNameValue(this.name, this.value)" >
     </div>
     <div class="col-setting-descr">
      <label for="id_amplPrec_P010"> 0.1 mm-Anzeige-Genauigkeit</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="radio" id="id_amplPrec_P050" name="nm_precisionAmplitude" value="P050"
        onchange="sendNameValue(this.name, this.value)" >
     </div>
     <div class="col-setting-descr">
      <label for="id_amplPrec_P050"> 0.5 mm-Anzeige-Genauigkeit</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="radio" id="id_amplPrec_P100" name="nm_precisionAmplitude" value="P100"
        onchange="sendNameValue(this.name, this.value)" >
     </div>
     <div class="col-setting-descr">
      <label for="id_amplPrec_P100"> 1.0 mm-Anzeige-Genauigkeit</label>
     </div>
    </div>
    <h4>Messverfahren Ruderausschlag:</h4>
    <div class="row">
     <div class="col-setting-values">
      <input type="radio" id="id_distance_arc" name="nm_distancetype" value="arc"
        onchange="sendNameValue(this.name, this.value)" >
     </div>
     <div class="col-setting-descr">
      <label for="id_distance_arc">Messverfahren Kreisbogen</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="radio" id="id_distance_chord" name="nm_distancetype" value="chord"
        onchange="sendNameValue(this.name, this.value)" >
     </div>
     <div class="col-setting-descr">
      <label for="id_distance_chord">Messverfahren Kreissehne / Abstand der Punkte</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="radio" id="id_vertical_distance" name="nm_distancetype" value="vertical_distance"
        onchange="sendNameValue(this.name, this.value)" >
     </div>
     <div class="col-setting-descr">
      <label for="id_distance_vertical">Messverfahren senkrechter Abstand (Tisch)</label>
     </div>
    </div>
   </div>
   <hr> <!-- ------------------------------------------------------------ -->
   <div class="container">
    <div class="row">
     <div class="col-setting-values">
      <button type="button" id="id_save" name="cmd_saveConfig" value="yes" onclick="sendAll(); sendNameValue(this.name, this.value)">
       Speichern</button>
     </div>
     <div class="col-setting-descr">
      <label for="id_save">
       Speichert die Einstellungen
      </label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <button type="button" id="id_reset" name="cmd_resetConfig" value="yes" onclick="sendNameValue(this.name, this.value)">
       Alles zur&uuml;cksetzten</button>
     </div>
     <div class="col-setting-descr">
      <label for="id_reset">
       Setzt alle Einstellungen zur&uuml;ck, WLAN Zugangsdaten werden gel&ouml;scht!
      </label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <button type="button" id="id_backToRoot" onclick="back()">Zur&uuml;ck</button>
     </div>
     <div class="col-setting-descr">
      <label for="id_backToRoot"> zur&uuml;ck zur Hauptseite, ohne Speichern </label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <button type="button" id="id_mcrestart" name="cmd_mcrestart" value="yes" onclick="sendNameValue(this.name, this.value)">
       Neustart Controller</button>
     </div>
     <div class="col-setting-descr">
      <label for="id_mcrestart"> Startet den Sensor neu </label>
     </div>
    </div>
    <p>
     Alle Änderungen wirken sich sofort aus und können mit dem "Zurück"-Button 
     benutzt/getestet werden. Ausnahmen sind WiFi-Daten, die sich erst nach 
     "Speichern" und "Neustart" auswirken.
    </p>
    <p>
     Sollen Einstellungen nach einem Neustart erhalten bleiben, 
     muss auf jeden Fall "Speichern" geklickt werden.
    </p>
   </div>
  </div>
  
  <script>
   function back() {
     sendAll();
     setTimeout(backToRoot, 200, );
   }

   function backToRoot() {
     window.location.href='/';
   }

   function sendAll() {
     sendValueForId("id_wlanSsid");
     sendValueForId("id_wlanPasswd");
     sendValueForId("id_apSsid");
     sendValueForId("id_apPasswd");
   }

   function initVendorSettings() {
     var htmlElement = document.getElementById('id_rcvendor_custom');
     if (htmlElement.checked) {
       setClassElementsReadonly('cl_pwm_input', 'false');
     } else {
       setClassElementsReadonly('cl_pwm_input', 'true');
     }
   }

   function init() {
     window.setTimeout(initVendorSettings,500);
   }


   getData(
     "id_wlanSsid",
     "id_apSsid",
     "id_apActive",
     "id_wlanConnetion",
     "id_rcvendor_",
     "id_pulse_width_min",
     "id_pulse_width_max",
     "id_pulse_width_n100",
     "id_pulse_width_p100",
     "id_rcvendor_jeti",
     "id_rcvendor_hott",
     "id_rcvendor_spektrum",
     "id_rcvendor_futaba",
     "id_rcvendor_multiplex",
     "id_rcvendor_taranis",
     "id_rcvendor_custom",
     "id_invertAmplitude",
     "id_invertAngle",
     "nm_anglePrecision",
     "nm_referenceAxis",
     "id_invertAmplitude",
     "nm_precisionAmplitude",
     "nm_distancetype",
     "id_resp_calibrate",
     "id_version"
   );
   init();
   setInterval(function() {
     var htmlElement = document.getElementById("id_wlanConnetion");
     htmlElement.innerHTML = "offline";
     getData("id_wlanConnetion" );
   }, 5000); // update rate in ms
  </script>
 </body>
</html>
)=====";
