const char ANGLE_SENSOR_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
 <head> 
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
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
   <h2>Aktuelle Messwerte:</h2>
   <p>Sensortyp: &lt;<span id="id_sensortype"></span>&gt;</p>
   <div class="row">
    <div class="col-angleView-full">
     <button type="button" id="id_tara" name="cmd_taraAngle" value="true"
      onclick="sendNameValue(this.name, this.value)">TARA</button>
    </div>
    <div class="col-angleView-declaration">
     <label for="id_invertAmplitude">Setzte Messwerte auf 0</label>
    </div>
   </div>
   <div class="row">
    <div class="col-angleView-full">
     <label class=measureValue id="id_angle">  <span id="id_angleValue">0</span>&deg;</label>
    </div>
    <div class="col-angleView-declaration">
     <label class=measureValue for="id_angle">Winkel</label>
    </div>
   </div>
   <div class="row">
    <div class="col-angleView-full">
     <label class=measureValue id="id_amplitude">  <span id="id_amplitudeValue">0</span>mm</label>
    </div>
    <div class="col-angleView-declaration">
     <label class=measureValue for="id_angle">Ruderweg</label>
     <label class=measureRemark for="id_angleCalcMethod">(<span type=text id="id_amplitudeCalcMethod">unbekannte Messmethode</span>)</label>
    </div>
   </div>
 
   <div class="row">
    <div class="col-angleView-full">
     <input type="number" class=measureInput id="id_rudderSize" onchange="sendNameValue(this.id, this.value*10)"
      style="width: 6em" maxlength="3" min="5" max="150" step="1" value="0"></input>
    </div>
    <div class="col-angleView-declaration">
     <label for="id_rudderSize" class=measureValue >Setze Rudertiefe in mm</label>
    </div>
   </div>

  <hr>
  <div class="row">
   <div class="col-angleView-full">
    <input type="number" id="id_targetAmplitude" onchange="sendNameValue(this.id, this.value*10)" 
     style="width: 6em" maxlength="3" min="-100" max="100" step="1" value="5"></input>
    <input type="checkbox" id="id_traceTargetAmplitude" onclick="beep(1,800,10)">
   </div>
   <div class="col-angleView-declaration">
     <label for="id_targetAmplitude">Setze Soll-Ruderweg [mm]</label>
   </div>
  </div>

  <hr>
  <h2>Min-/Max-Ruderweg-Messung:</h2>
  <div class="row">
   <div class="col-angleView-full">
    <label class="switch" data-on="Start" data-off="Stop">
    <input id="cmd_flightphaseActive" type="checkbox" onclick="sendNameValue(this.id, this.checked)">
    <span class="slidebox"></span>
    </label>
    </div>
    <div class="col-angleView-declaration">
     <label for="cmd_flightphaseActive">
      Misst die akutelle Neutrallage, den Min- und Max-Wert und zeigt sie an
     </label>
   </div>
  </div>
  <div class="row">
    <div class="col-angleView-full">
	 <label class="flightphaseValue" id="id_ruddermaxlabel">
  	 <span id="id_ruddermax">0.00</span>mm
	 </label>
    </div>
    <div class="col-angleView-declaration">
      <label class="flightphaseValue" for="id_ruddermaxlabel">
        Max-Wert
      </label>
    </div>
  </div>
  <div class="row">
    <div class="col-angleView-full">
	 <label class="flightphaseValue" id="id_rudderneutrallabel">
  	 <span id="id_rudderneutral">0.00</span>mm
	 </label>
    </div>
    <div class="col-angleView-declaration">
      <label class="flightphaseValue" for="id_rudderneutrallabel">
        Neutral-Wert
      </label>
    </div>
  </div>
  <div class="row">
    <div class="col-angleView-full">
	 <label class="flightphaseValue" id="id_rudderminlabel">
  	 <span id="id_ruddermin">0.00</span>mm
	 </label>
    </div>
    <div class="col-angleView-declaration">
      <label class="flightphaseValue" for="id_rudderminlabel">
        Min-Wert
      </label>
    </div>
  </div>
  <hr>
  <div class="row">
    <div class="col-angleView-full">
      <button type="button" id="id_settings" onclick="window.location.href='/'">zurück</button>
    </div>
    <div class="col-angleView-declaration">
      <label for="id_settings">
        Zurück zur Auswahl
      </label>
    </div>
  </div>
  </div class="container">
  <br><br><a href="https://github.com/Pulsar07/RcSetupTool">Projektseite auf GitHub</a>
  <script>

    audioCtx = new ( window.AudioContext // Default
      || window.webkitAudioContext ); // Safari and old versions of Chrome


    function beep(vol, freq, duration){
      if (audioCtx) {
        // console.log("audio beep: yes");
        var osci = audioCtx.createOscillator();
 
        var gain = audioCtx.createGain();
        osci.connect(gain);
        osci.frequency.value = freq;
        osci.type="square";
        gain.connect(audioCtx.destination);
        gain.gain.value = vol*0.01;
        osci.start(audioCtx.currentTime);
        osci.stop(audioCtx.currentTime+duration*0.001);
      } else {
        // console.log("audio beep: no");
      }
    }
 
    function beepTarget() {
      deltaAmplitude= id_targetAmplitude.value-document.getElementById('id_amplitudeValue').innerText;
      beepFreq = 800 + Math.sign(deltaAmplitude)*200 + deltaAmplitude*10;
      // console.log("deltaAmplitude: " + deltaAmplitude + "   beepFreq: " + beepFreq);
      if (id_traceTargetAmplitude.checked) {
        beep(50, beepFreq, 80);
        // console.log("dA: " + deltaAmplitude + "   bFr: " + beepFreq);
        if (Math.abs(deltaAmplitude) < 0.2) {
          beep(100,900,700);
          window.navigator.vibrate([150,100,150]);
        }
      }
    }
 
   setInterval(function() {
     // Call a function repetatively with 2 Second interval
     getData("id_angleValue", "id_amplitudeValue", "cpx_flightphase" );
     if (audioCtx) {
       beepTarget();
     } else {
     }
   }, 500); // 500mSeconds update rate
   getData("id_version", "id_rudderSize", "id_targetAmplitude", "id_sensortype", "id_amplitudeCalcMethod"); 
  </script>
  </body>
  </html>
)=====";
