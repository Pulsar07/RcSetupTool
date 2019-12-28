const char EXPERT_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
 <head> 
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
  <meta http-equiv="cache-control" content="no-cache, must-revalidate, post-check=0, pre-check=0" /> 
  <meta http-equiv="cache-control" content="max-age=0" /> 
  <meta http-equiv="expires" content="0" /> 
  <meta http-equiv="expires" content="Tue, 01 Jan 1980 1:00:00 GMT" /> 
  <meta http-equiv="pragma" content="no-cache" /> 
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="icon" href="#" />
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
    <h2>Experten-Einstellungen</h2>
   </div>
   <hr> <!-- ------------------------------------------------------------ -->
   <div class="container">
    <h3>Sensor-Kalibrierung </h3>
    <div class="row">
     <div class="col-setting-values">
      <button type="button" id="id_calibrate" name="cmd_calibrate" value="yes"
        onclick="setElementValue('id_resp_calibrate', 'Kalibrierung läuft ...'); sendNameValue(this.name, this.value); getDataRS('id_resp_calibrate');">
        Kalibrierung</button>
     </div>
     <div class="col-setting-descr">
      <label for="id_calibrate">
       Kalibriere den Sensor (nur MPU-6050)
      </label>
     </div>
    </div>
    <h4> Sensor Status: <span id="id_resp_calibrate"> - </span></h4>
    <p> Zur Kalibrierung muss der MPU-6050 Sensor mit der Unterseite (glatte Seite des Boards) möglichst exakt horizontal liegen und darf sich ca. 10-15s nicht bewegen.
     </p>
    <hr>
    <h4> Manuelle Kalibrierungsoffsets</h4>
    <p> Manuelle Kalibrierungsoffsets werden benötigt, wenn ein Sensor sich nicht kalibrieren lässt oder trotz Kalibrierung schlechte Werte liefert. 
     Das sind vermutlich Sensoren, die eigentlich Ausschuss sind und nicht das spezifizierte Verhalten aufweisen. Dennoch sind diese Sensoren mittels der Kalibrierungsoffsets problemlos zu betreiben und liefern auch wirklich akzeptable Werte.
     Dazu den Sensor auf den Tisch legen, TARA aktivieren und z.B. mit einem Geo-Dreieck sich die Werte der realen Winkel +/-45°notieren. <br>
     Diese Werte sind dann hier einzutragen.
     </p>

    <div class="row">
     <div class="col-setting-values">
      <input type="checkbox" id="id_caloffset_enabled" name="caloffset" value="x"
       onchange="sendNameValue(this.id, this.checked)">
     </div>
     <div class="col-setting-descr">
      <label for="id_caloffset_enabled">Nutze die Kalibrierungs-Offsets </label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="number" id="id_caloffset_h" onchange="sendNameValue(this.id, this.value*10)"
       style="width: 6em" maxlength="3" min="0.0" max="90.0" step="0.1" value="45.0"></input>
     </div>
     <div class="col-setting-descr">
      <label for="id_caloffset_h" >oberer Kalibrierungswert/Messwert bei 45°</label>
     </div>
    </div>
    <div class="row">
     <div class="col-setting-values">
      <input type="number" id="id_caloffset_l" onchange="sendNameValue(this.id, this.value*10)"
       style="width: 6em" maxlength="3" min="-90.0" max="0.0" step="0.1" value="-45.0"></input>
     </div>
     <div class="col-setting-descr">
      <label for="id_caloffset_l" >unterer Kalibrierungswert/Messwert bei -45°</label>
     </div>
    </div>
   </div>
   <hr> <!-- ------------------------------------------------------------ -->
   <div class="container">
    <div class="row">
     <div class="col-setting-values">
      <button type="button" id="id_save" name="cmd_saveConfig" value="yes" onclick="sendNameValue(this.name, this.value)">
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
      <button type="button" id="id_backToRoot" onclick="back()">Zur&uuml;ck</button>
     </div>
     <div class="col-setting-descr">
      <label for="id_backToRoot">
       zur&uuml;ck zur Hauptseite, ohne Speichern. 
      </label>
     </div>
    </div>
   </div>
   <br><br><a href="https://github.com/Pulsar07/RcSetupTool">Projektseite auf GitHub</a>
  </div>

  <script>
   function back() {
     setTimeout(backToRoot, 200, );
   }
 
   function backToRoot() {
     window.location.href='/';
   }
 
   getData(
     "id_resp_calibrate",
     "id_caloffset_enabled",
     "id_caloffset_h",
     "id_caloffset_l",
     "id_version");
  </script>
 </body>
</html>
)=====";
