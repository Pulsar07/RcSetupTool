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
      <button type="button" id="id_settings" onclick="selectiveClick()">Einstellungen ändern</button>
     </div>
     <div class="col-text">
      <p> diverse Einstellungen, zu WiFi, Sensoren und anderes </p>
     </div>
    </div>
   </div>
   </div class="container">
   <br><br><a href="https://github.com/Pulsar07/RcSetupTool">Projektseite auf GitHub</a>
  </div>
 
  <script>
   <!-- Mac key handling from https://github.com/MichaelZelensky/jsLibraries/blob/master/macKeys.js -->
   (function(){
    var saywho, isMac, webkit, mozilla, opera, kC;
    isMac = navigator.platform.toUpperCase().indexOf('MAC') >= 0;
    window.macKeys = {
        cmdKey : false,
        ctrlKey : false,
        shiftKey : false,
        altKey : false,
        reset : function(){
            this.cmdKey = false;
            this.ctrlKey = false;
            this.shiftKey = false;
            this.altKey = false;
        }
    };
    if (isMac) {
        //browser detection, originates from: http://stackoverflow.com/questions/2400935/browser-detection-in-javascript
        saywho = (function(){
            var ua = navigator.userAgent, tem,
                M = ua.match(/(opera|chrome|safari|firefox|msie|trident(?=\/))\/?\s*(\d+)/i) || [];
            if (/trident/i.test(M[1])) {
                tem = /\brv[ :]+(\d+)/g.exec(ua) || [];
                return { 'browser': 'IE', 'version': (tem[1] || '') };
            }
            if (M[1] === 'Chrome') {
                tem = ua.match(/\b(OPR|Edge)\/(\d+)/);
                //if(tem != null) return tem.slice(1).join(' ').replace('OPR', 'Opera');
                if (tem != null) return {'browser':tem.slice(1)[0].replace('OPR', 'Opera'), 'version': tem.slice(1)[1]}
            }
            M = M[2] ? [M[1], M[2]]: [navigator.appName, navigator.appVersion, '-?'];
            if ((tem = ua.match(/version\/(\d+)/i))!= null) M.splice(1, 1, tem[1]);
            return { 'browser': M[0], 'version': M[1] };
        })();
        webkit = (saywho.browser === 'Chrome' || saywho.browser === 'Safari');
        mozilla = saywho.browser === 'Firefox';
        opera = saywho.browser === 'Opera';
        window.onkeydown = function(e){
            kC = e.keyCode;
            if (((webkit || opera) && (kC === 91 || kC === 93)) || (mozilla && kC === 224)) {
                macKeys.cmdKey = true;
            } else if (kC === 16) {
                macKeys.shiftKey = true;
            } else if (kC === 17) {
                macKeys.ctrlKey = true;
            } else if (kC === 18) {
                macKeys.altKey = true;
            }
        };
        window.onkeyup = function(e){
            kC = e.keyCode;
            if (((webkit || opera) && (kC === 91 || kC === 93)) || (mozilla && kC === 224)) {
                macKeys.cmdKey = false;
            } else if (kC === 16) {
                macKeys.shiftKey = false;
            } else if (kC === 17) {
                macKeys.ctrlKey = false;
            } else if (kC === 18) {
                macKeys.altKey = false;
            }
        };
        window.onblur = function(){
            macKeys.reset();
        };
    }
   })();

   function selectiveClick() {
    if (window.event.ctrlKey || macKeys.ctrlKey) {
     window.location.href='/expertPage';
    } else {
     window.location.href='/adminPage';
    }
   }
   getData( "id_version");
  </script>
 </body>
</html>
)=====";
