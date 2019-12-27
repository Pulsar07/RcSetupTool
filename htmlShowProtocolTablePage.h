const char SHOW_PROTO_TABLE_page[] PROGMEM = R"=====(
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
  <meta name="viewport" content="width=device-width">
  <!-- <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=0, minimum-scale=1.0, maximum-scale=1.0"> -->
  <link rel="icon" href="#" />
  <title>RC-Einstelltool</title>
  <link rel="stylesheet" href="./styles.css">
  <script type="text/javascript" src="./script.js"></script>
 </head>
 <body id="id_body">
  <div class="container">
   <div class="row">
    <div class="col-appname">RC-Einstelltool:</div>
    <div class="col-version">Version: <span id="id_version">0.00</span></div>
   </div>
  </div>
  <div class="container">
   <p>Protokoll einer Ruderweg-Messung und Servo-Steuerung Messung:
  </div>
  <div class="container">
   <div class="row">
  ###<TABLE>###
   </div>
  </div>
  <hr>
   <div class="row">
     <button type="button" onclick="backToMultiPage();">zurück zur Messseite</button>
     <button type="button" id="id_dataset" name="cmd_dataset_init" value="true"
      onclick="sendNameValue(this.name, this.value); back();">lösche Protokoll</button>
   </div>
 </body>
 <script>
  function back() {
   setTimeout(backToMultiPage, 400, );
  }
 
  function backToMultiPage() {
    window.location.href='/multiToolPage'+'?'+Math.random()*Math.random();
  }

  getData(
    "id_version"
  );
 </script>

</html>
)=====";
