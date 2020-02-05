const char MULTI_TOOL_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
 <head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
  <meta name="viewport" content="width=device-width, initial-scale=0.5>
  <meta http-equiv="cache-control" content="no-cache, must-revalidate, post-check=0, pre-check=0" />
  <meta http-equiv="cache-control" content="max-age=0" />
  <meta http-equiv="cache-control" content="no-cache" />
  <meta http-equiv="expires" content="0" />
  <meta http-equiv="expires" content="Tue, 01 Jan 1980 1:00:00 GMT" />
  <meta http-equiv="pragma" content="no-cache" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=0, minimum-scale=1.0, maximum-scale=1.0">
  <link rel="icon" href="#" />
  <title>RC-Einstell-Tool</title>
  <link rel="stylesheet" href="./styles.css">
  <script type="text/javascript" src="./script.js"></script>
 </head>
 <body id="id_body" oncontextmenu="return false">
  <div class="container">
   <div class="row">
    <div class="col-appname">RC-Einstell-Tool:</div>
    <div class="col-version">Version: <span id="id_version">0.00</span></div>
   </div>
  </div>
  <div class="container">
   <p>Ruderweg-Messung und Servo-Steuerung einer RC-Ruderanlenkung über den Browser:
  </div>
  <div class="container">
   <div class="row">
    <div class="col-declaration">
     <label>Tara<wbr>/Ruder&shy;tiefe :</label>
    </div>
    <div class="col-half">
     <button type="button" id="id_tara" name="cmd_taraAngle" value="true"
      onclick="sendNameValue(this.name, this.value)">TARA</button>
    </div>
    <div class="col-half">
     <input type="number" class=measureInput id="id_rudderSize" onchange="sendNameValue(this.id, this.value*10)"
      style="width: 4em" maxlength="3" min="5" max="150" step="1" value="5"></input>mm
    </div>
   </div>
   <div class="row">
    <div class="col-declaration">
     <label>Winkel/<wbr>Weg :</label>
    </div>
    <div class="col-half">
     <label class=measureValue id="id_amplitude">  <span id="id_amplitudeValue">0</span>mm</label>
    </div>
    <div class="col-half">
     <label class=measureValue id="id_angle">  <span id="id_angleValue">0</span>&deg;</label>
    </div>
   </div>
   <div class="row">
    <div class="col-declaration">
     <label>Servo&shy;weg :</label>
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
     <label> mod&shy;ifi&shy;zieren :</label>
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
     <label>Servo-Definition :</label>
    </div>
    <div class="col-full">
      <select id="id_proto_servo_function_def" onchange="sendNameValue(this.id, this.value)">
       <option value="option_fd_QR-1-L">Querruder-1 links</option>
       <option value="option_fd_QR-1-R">Querruder-1 rechts</option>
       <option value="option_fd_QR-2-L">Querruder-2 links</option>
       <option value="option_fd_QR-2-R">Querruder-2 rechts</option>
       <option value="option_fd_WK-1-L">Wölkklappe-1 links</option>
       <option value="option_fd_WK-1-R">Wölkklappe-1 rechts</option>
       <option value="option_fd_WK-2-L">Wölkklappe-2 links</option>
       <option value="option_fd_WK-2-R">Wölkklappe-2 rechts</option>
       <option value="option_fd_VL-L">V-Leitwerk links</option>
       <option value="option_fd_VL-R">V-Leitwerk rechts</option>
       <option value="option_fd_SR-1">Seitenruder-1</option>
       <option value="option_fd_SR-2">Seitenruder-2</option>
       <option value="option_fd_HR-1">Höhenruder-1</option>
       <option value="option_fd_HR-2">Höhenruder-2</option>
       <option value="option_fd_SF-1">Servo-Funktion-1</option>
       <option value="option_fd_SF-2">Servo-Funktion-2</option>
       <option value="option_fd_SF-3">Servo-Funktion-3</option>
       <option value="option_fd_SF-4">Servo-Funktion-4</option>
     </select>
     <label>
       <input type="checkbox" id="id_servo_direction" checked="false"
         onchange="sendNameValue(this.id, this.checked)"> invertiere Servolaufrichtung
     </label>
    </div>
   </div>
   <div class="row">
    <div class="col-declaration">
     <label>Maus-Rad bewegt Servo :</label>
    </div>
    <div class="col-full">
     <label>
       <input type="checkbox" id="id_wheel_activate" checked="false"
         onchange="sendNameValue(this.id, this.checked)" > : aktiv 
       &emsp;
       <input type="number" id="id_wheel_factor" style="width: 4em"
         onchange="sendNameValue(this.id, this.value)"
         maxlength="2" min="1" max="20" step="1" value="1"> : V<sub>maus</sub>
     </label>
    </div>
   </div>
   <div class="row">
    <div class="col-declaration">
     <label>Ein&shy;stell&shy;ungen :</label>
    </div>
    <div class="col-full">
      <label>
         <span id="id_vendor_settings">Futaba</span>
         (
         min: <span id="id_pulse_width_min">960</span>&micro;s/
         -100%: <span id="id_pulse_width_n100">1100</span>&micro;s/
         +100%: <span id="id_pulse_width_p100">1900</span>&micro;s/
         max: <span id="id_pulse_width_max">2225</span>&micro;s
         )
     </label>
    </div>
   </div>
   <div class="row">
    <div class="col-declaration">
     <label>Protokoll :</label>
    </div>
    <div class="col-full">
     <button type="button" class="protocol" id="id_dataset" name="cmd_dataset" value="true"
      onclick="sendNameValue(this.name, this.value)">speichere Datensatz</button>
     <input type="text" class=mmeasureInput id="id_dataset_descr" onchange="sendNameValue(this.id, this.value)"
      style="width: 20em" maxlength="100" placeholder="Beschreibung des Datensatzes"></input>
     <button type="button" class="protocol" onclick="window.location.href='/showProtocolTable'">zeige Protokoll</button>
    </div>
   </div>
  </div class="container">
  <hr>
  <div class="container">
  <div class="row">
   <div class="col-declaration">
    <label for="id_settings">
     zurück zum Menü
    </label>
   </div>
   <div class="col-full">
    <button type="button" id="id_settings" onclick="window.location.href='/'">Zurück</button>
   </div>
  </div>
 </div class="container">
 <br><br><a href="https://github.com/Pulsar07/RcSetupTool">Projektseite auf GitHub</a>

 <script>
 
  getData(
    "id_rudderSize", "id_amplitudeValue", "id_angleValue",
    "id_proto_servo_function_def",
    "id_pwm_value", "id_percent_value", "id_pwm_setvalue", "id_percent_setvalue", 
    "id_pos_slider", "id_servo_direction",
    "id_btn_set_pos_0", "id_btn_set_pos_1", "id_btn_set_pos_2",
    "id_btn_store_pos_0", "id_btn_store_pos_1", "id_btn_store_pos_2",
    "id_btn_store_limit_min", "id_btn_set_limit_min", "id_btn_store_limit_max", "id_btn_set_limit_max",
    "id_pulse_width_min", "id_pulse_width_max", "id_pulse_width_p100", "id_pulse_width_n100", 
    "id_dataset_descr",
    "id_wheel_activate", "id_wheel_factor",
    "id_vendor_settings", "id_version"
  );

  setInterval(function() {
    // Call a function repetatively 
    getData("id_angleValue", "id_amplitudeValue");
  }, 500); // 500ms update rate
 
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

  function handleClick(aEvt, aEle) {
    var name = aEle.getAttribute("name");
    if (name == null) {
      name = "-";
    }
    console.log("handleClick() : " + aEle.id + "/" + name + "/" + aEvt.which);
    console.log("event() : " + aEvt.type);
    console.log("nodeName : " + aEvt.target.nodeName);
    console.log("id : " + aEvt.target.id);
    if (name.startsWith("tcmd_")) {
      sendNameValue(name, "-");
    } else 
    if (aEvt.target.nodeName === "A") {
      if (aEvt.type = "mouseup" ) {
        if ( handleClick.mymousedown == true) {
          handleClick.mymousedown = false;
          aEvt.preventDefault(); 
          aEvt.stopPropagation();
          if (aEvt.which == 1) {
            sendNameValue(aEle.id, "left_mouse");
            // alert("right clicked!: " + this.id);
          }
          if (aEvt.which == 2) {
            sendNameValue(aEle.id, "middle_mouse");
            // alert("right clicked!: " + this.id);
          }
          if (aEvt.which == 3) {
              sendNameValue(aEle.id, "right_mouse");
              // alert("right clicked!: " + this.id);
          }
        }
      } else if (aEvt.type == "mousedown") {
         handleClick.mymousedown = true;
      }
    }
  }
  
   var ongoingTouches = [];
   window.onload = function () {
    var is_touch_device = 'ontouchstart' in window || navigator.maxTouchPoints;
    console.log("is_touch_device) : " + is_touch_device);
    if(is_touch_device ) { // || true) {
      var x = document.getElementsByClassName("dropdown-content");
      var i;
      for (i = 0; i < x.length; i++) {
        x[i].classList.add("dropdown-touch");
      }
    } 

    var percentValElement = document.getElementById('id_percent_value');
 
    var someElement = document.getElementById('id_body');
    someElement.addEventListener('touchmove', function(e) {
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
