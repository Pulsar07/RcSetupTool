const char MENU_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
 <head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
  <meta name="viewport" content="width=device-width, initial-scale=0.5>
  <meta http-equiv="cache-control" content="no-cache, must-revalidate, post-check=0, pre-check=0" />
  <meta http-equiv="cache-control" content="max-age=0" />
  <meta http-equiv="expires" content="0" />
  <meta http-equiv="expires" content="Tue, 01 Jan 1980 1:00:00 GMT" />
  <meta http-equiv="pragma" content="no-cache" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=0, minimum-scale=1.0, maximum-scale=1.0">
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
    <h2>Funktionsauswahl:</h2>
   </div>
   <div class="container">
    <div class="row">
     <div class="col-declaration-long">
      <label>Ruder-Winkel-Messung:</label>
     </div>
     <div class="col-button">
      <button type="button" id="id_settings" onclick="window.location.href='/angleSensorPage'">Winkel-Messung</button>
     </div>
     <div class="col-text">
      <p> zur Bestimmung von Ruderausschlägen </p>
     </div>
    </div>
    <div class="row">
     <div class="col-declaration-long">
       <label>Servo-Steuerung:</label>
     </div>
     <div class="col-button">
       <button type="button" id="id_settings" onclick="window.location.href='/servoPage'">Servo-Controller</button>
     </div>
     <div class="col-text">
       <p>zur Steuerung eines RC-Servo</p>
     </div>
    </div>
    <div class="row">
     <div class="col-declaration-long">
      <label>Kombination Mess- und Steuerung:</label>
     </div>
     <div class="col-button">
      <button type="button" id="id_settings" onclick="window.location.href='/multiToolPage'">Steuern und Messen</button>
     </div>
     <div class="col-text">
      <p> Servo-Steuerung und Winkelmessung auf einer Seite </p>
     </div>
    </div>
   </div>
   <hr>
   <div class="container">
    <div class="row">
     <div class="col-declaration-long">
      <label for="id_settings"> Ein&shy;stell&shy;ungen:</label>
     </div>
     <div class="col-button">
      <button type="button" onclick="window.location.href='/adminPage'"> Einstellungen ändern</button>
     </div>
     <div class="col-text">
      <p> diverse Einstellungen, zu WiFi, Sensoren und anderes </p>
     </div>
    </div>
    <div class="row">
     <div class="col-declaration-long">
      <label for="id_settings"> Ein&shy;stell&shy;ungen:</label>
     </div>
     <div class="col-button">
      <button type="button" onclick="window.location.href='/expertPage'"> Experten-Einstellungen ändern</button>
     </div>
     <div class="col-text">
      <p> Sensor-Kalibrierung und manuelle Offsets </p>
     </div>
    </div>
   </div>
   </div class="container">
   <br><br><a href="https://github.com/Pulsar07/RcSetupTool">Projektseite auf GitHub</a>
  </div>
 
  <script>
   getData( "id_version");
  </script>
 </body>
</html>
)=====";
