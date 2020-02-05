const char SERVO_page[] PROGMEM = R"=====(
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
    <p>Steuerung eines RC-Servos über den Browser:
   </div>
  <div class="row">
    <div class="col-declaration">
      <label>Servoweg in:</label>
    </div>
    <div class="col-third">
      <label class=measureValue><span id="id_pwm_value">1023</span>&micro;s</label>
    </div>
    <div class="col-third">
      <canvas id="ServoMovementCanvas" width="130" height="65"></canvas>
    </div>
    <div class="col-third">
      <label class=measureValue><span id="id_percent_value">150</span>%</label>
    </div>
  </div>
  <div class="row">
    <div class="col-declaration">
      <label> modifizieren :</label>
    </div>
    <div class="col-half">
      <input type="number" class=measureInput id="id_pwm_setvalue" onchange="sendNameValue(this.id, this.value)"
         style="width: 6em" maxlength="4" min="750" max="2250" step="1" value="1500"><span class=measureInput>&micro;s</span>
    </div>
    <div class="col-half">
      <input type="number" class=measureInput id="id_percent_setvalue" onchange="sendNameValue(this.id, this.value)"
         style="width: 6em" maxlength="3" min="-125" max="+125" step="1" value="0"><span class=measureInput>%</span>
    </div>
  </div>
  <div class="row">
    <div class="col-declaration">
      <label>Positions-Slider :</label>
    </div>
    <div class="col-full">
      <div class="slidecontainer">
        <input type="range" min="-125" max="125" value="0" 
          class="slider" id="id_pos_slider"
          onchange="sendNameValue(this.id, this.value)"
          oninput="sendNameValue(this.id, this.value)">
      </div>
    </div>
  </div>
   <div class="row">
    <div class="col-declaration">
     <div class="label_grp" >
      <label for="id_start">Setze Servo-Pos :</label>
      <label for="id_start">Store Servo-Pos :</label>
     </div>
    </div>
    <div class="col-full"> 
     <div class="c_button_grp" >
      <a class="c_button limiter tooltip" id="id_toggle_min" name="cmd_limit"
       onclick="sendNameValue(this.name, this.id)">
       <span id="id_btn_set_limit_min">-</span>
      </a>
      <a class="c_button store_servo" id="id_set_min" name="cmd_limit"
       onclick="sendNameValue(this.name, this.id)">
       <span id="id_btn_store_limit_min">limit</span>
      </a>
     </div>
     <div class="c_button_grp" >
      <a class="c_button set_servo tooltip" id="id_set_pos_0" name="cmd_set_servo_pos"
       onclick="sendNameValue(this.name, this.id)">
       <span id="id_btn_set_pos_0">-100</span>%
      </a>
      <a class="c_button store_servo" id="id_store_pos_0" name="cmd_store_servo_pos"
       onclick="sendNameValue(this.name, this.id)">
       <span id="id_btn_store_pos_0">-</span>
      </a>
     </div>
     <div class="c_button_grp" >
      <a class="c_button set_servo tooltip" id="id_set_pos_1" name="cmd_set_servo_pos"
       onclick="sendNameValue(this.name, this.id)">
       <span id="id_btn_set_pos_1">0</span>%
      </a>
      <a class="c_button store_servo" id="id_store_pos_1" name="cmd_store_servo_pos"
       onclick="sendNameValue(this.name, this.id)">
       <span id="id_btn_store_pos_1">-</span>
      </a>
     </div>
     <div class="c_button_grp" >
      <a class="c_button set_servo tooltip" id="id_set_pos_2" name="cmd_set_servo_pos"
       onclick="sendNameValue(this.name, this.id)">
       <span id="id_btn_set_pos_2">+100</span>%
      </a>
      <a class="c_button store_servo" id="id_store_pos_2" name="cmd_store_servo_pos"
       onclick="sendNameValue(this.name, this.id)">
       <span id="id_btn_store_pos_2">-</span>
      </a>
     </div>
     <div class="c_button_grp" >
      <a class="c_button limiter tooltip" id="id_toggle_max" name="cmd_limit"
       onclick="sendNameValue(this.name, this.id)">
       <span id="id_btn_set_limit_max">+</span>
      </a>
      <a class="c_button store_servo" id="id_set_max" name="cmd_limit"
       onclick="sendNameValue(this.name, this.id)">
       <span id="id_btn_store_limit_max">limit</span>
      </a>
     </div>
    </div>
   </div>
  <div class="row">
    <div class="col-declaration">
      <label>Servog-Dreh<br>Richtung:</label>
    </div>
    <div class="col-full">
      <label>
        <input type="checkbox" id="id_servo_direction" checked="false"
          onchange="sendNameValue(this.id, this.checked)"> invertiere Servolaufrichtung
      </label>
    </div>
  </div>
  <div class="row">
    <div class="col-declaration">
      <label>Maus-Rad bewegt Servo:</label>
    </div>
    <div class="col-full">
      <label>
        <input type="checkbox" id="id_wheel_activate" checked="false"
          onchange="sendNameValue(this.id, this.checked)" > : aktiviere Maus-Rad 
        &emsp;
        <input type="number" id="id_wheel_factor" style="width: 4em"
	  onchange="sendNameValue(this.id, this.value)"
          maxlength="2" min="1" max="20" step="1" value="1"> : Mausgeschwindigkeit 
      </label>
    </div>
  </div>
  <div class="row">
    <div class="col-declaration">
      <label>Ein&shy;stell&shy;ungen:</label>
    </div>
    <div class="col-full">
      <label>
          <span id="id_vendor_settings">?</span>
	  (
	  min: <span id="id_pulse_width_min">?</span>&micro;s/
	  -100%: <span id="id_pulse_width_n100">?</span>&micro;s/
	  +100%: <span id="id_pulse_width_p100">?</span>&micro;s/
	  max: <span id="id_pulse_width_max">?</span>&micro;s
	  )
      </label>
    </div>
  </div>
  <hr>
  <div class="row">
    <div class="col-declaration">
      <label for="id_settings">
      </label>
    </div>
    <div class="col-full">
      <button type="button" id="id_settings" onclick="window.location.href='/'">zurück</button>
    </div>
  </div>
  </div class="container">
  <br><br><a href="https://github.com/Pulsar07/RcSetupTool">Projektseite auf GitHub</a>
  <script>

   getData("id_pwm_value", "id_percent_value", "id_pwm_setvalue", "id_percent_setvalue", 
    "id_pos_slider", "id_servo_direction",
    "id_btn_set_pos_0", "id_btn_set_pos_1", "id_btn_set_pos_2",
    "id_btn_store_pos_0", "id_btn_store_pos_1", "id_btn_store_pos_2",
    "id_btn_store_limit_min", "id_btn_set_limit_min", "id_btn_store_limit_max", "id_btn_set_limit_max",
    "id_pulse_width_min", "id_pulse_width_max", "id_pulse_width_p100", "id_pulse_width_n100", 
    "id_wheel_activate", "id_wheel_factor",
    "id_vendor_settings", "id_version"
   );

  function moveServoByMouseWheel() {
    window.addEventListener("wheel", function(e) {
      var wheelActive = document.getElementById('id_wheel_activate');
      if (wheelActive.checked == true) {
        e.stopPropagation();
        var dir = Math.sign(e.deltaY);
        sendNameValue("evt_wheel", dir);
      }
    });
  }
  moveServoByMouseWheel();

  function drawServoCanvas() {
      var canvas = document.getElementById('ServoMovementCanvas');
      var context = canvas.getContext('2d');
     
     // Variables store centre and radius of arc
      var x = 65;
      var y = 65;
      var radius =60;
      context.clearRect(0,0,x,y);
      // set the direction to anticlockwise
      var counterClockwise = true;
     

      // draw the red circle
      context.beginPath();
      context.arc(x, y, radius, 0, Math.PI, counterClockwise);
      context.lineWidth =2;

      context.fillStyle	= '#FF0000';
      context.fill();
      
      context.strokeStyle = '#fa4b2a';
      context.stroke();

      // draw the blue servo arc/pie
      var percentValElement = document.getElementById('id_percent_value');
      var servoPos = percentValElement.innerHTML;
      var servoRadians = 1.5*Math.PI+(servoPos/50);
      // console.log("drawServoCanvas with : " + servoPos + "/" + servoPos/100 + "/" + servoRadians);
      counterClockwise = !counterClockwise;
      if ( servoRadians < 1.5*Math.PI ) {
        counterClockwise = !counterClockwise;
      }
      context.beginPath();
      context.fillStyle	= '#4444ff';
      context.moveTo(x,y);
      context.arc(x, y, radius, 1.5*Math.PI, 1.5*Math.PI+(servoPos/100), counterClockwise);
      context.lineTo(x,y);
      // context.lineWidth =10;

      context.fill();
      
      context.strokeStyle = '#fa4b2a';
      context.stroke();
  }
  
  var ongoingTouches = [];
  window.onload = function () {
    var percentValElement = document.getElementById('id_percent_value');

    var body = document.getElementById('id_body');
    body.addEventListener('touchmove', function(e) {
    // Iterate through the list of touch points that changed
    // since the last event and print each touch point's identifier.
    for (var i=0; i < e.changedTouches.length; i++) {
      // console.log("changedTouches[" + i + "].identifier = " + e.changedTouches[i].identifier);
      // console.log("changedTouches[" + i + "].pageX = " + e.changedTouches[i].pageX);
      // console.log("changedTouches[" + i + "].pageY = " + e.changedTouches[i].pageY);

       sendNameValue("evt_wheel", 1);
    }
  }, false);

    
    // document.addEventListener('touchmove', function (e) { 
    //     e.preventDefault(); }, isPassive() ? {
    //       capture: false,
    //       passive: false
    //     } : false);

    // var servoMover = new MutationObserver(function(mutations) {
    //    mutations.forEach(function(mutation) {
    //      console.log(mutation);
    //    });
    // });
    // servoMover.observe(document, {
    //   attributes: true,
    //   characterData: true,
    //   childList: true,
    //   subtree: true,
    //   attributeOldValue: true,
    //   characterDataOldValue: true
    // });

    var mutationObserver = new MutationObserver(function(mutations) {
      drawServoCanvas();
    });
    // Starts listening for changes in the root HTML element of the page.
    mutationObserver.observe(percentValElement, {
      attributes: true,
      characterData: true,
      childList: true,
      subtree: true,
      attributeOldValue: true,
      characterDataOldValue: true
    });

   drawServoCanvas();
  }

  </script>
 </body>
</html>
)=====";
