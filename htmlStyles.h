const char STYLES[] PROGMEM = R"=====(
* {
  box-sizing: border-box;
}
canvas {
  border: 1px solid #ccc;
}

h2, h3, h4, p {
  margin: 3px;
  padding: 0px 0px 0px 40px;
}

input[type=text], textarea {
  width: 100%;
  padding: 5px;
  border: 1px solid #ccc;
  border-radius: 4px;
  resize: vertical;
}


input[type=checkbox].config, input[type=radio].config, input[type=number].config, select {
  margin: 5px 12px 5px 5px;
  border: 1px solid #ccc;
  border-radius: 4px;
  display: block;
  display: inline-block;
}

label {
  padding: 6px 12px 6px 0;
  display: block;
  display: inline-block;
}

label.input {
  border: 1px solid #ccc;
  border-radius: 4px;
  display: inline-block;
}

input[type=submit] {
  background-color: #4CAF50;
  color: white;
  padding: 12px 20px;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  float: right;
}

button {
  padding: 3px;
  width: 12em;
}

button:hover {
  background-color: #99f;
}

button.protocol {
  width: 12em;
}

table {
  table-layout: auto;
  border-collapse: collapse;
  font-size: .9em;
  width: 100%;
} 

#protodata td, #protodata th {
  overflow:visible;
  border: 1px solid #ddd;
  padding: 8px;
}

#protodata tr:nth-child(even){background-color: #f2f2f2;}

#protodata tr:hover {background-color: #ddd;}

#protodata th {
  padding-top: 12px;
  padding-bottom: 12px;
  text-align: left;
  background-color: #4CAF50;
  color: white;
}

.container {
  border-radius: 5px;
  background-color: #f2f2f2;
  padding: 8px;
}

.measureRemark {
  font-size: .8em;
}

.measureValue, .flightphaseValue {
  font-size: 1.5em;
  font-weight:bold;
}

.measureInput {
  font-size: 1.3em;
  font-weight:bold;
}

.col-version {
  text-align: right;
  font-size: 0.8em;
  font-weight:normal;
}

.col-appname{
  float: left;
  text-align: left;
  font-size: 1.3em;
  font-weight:bold;
}

.col-declaration, .col-angleView-declaration {
  text-align: right;
  float: left;
  width: 25%;
}

.col-angleView-declaration {
  width: 40%;
}

.col-full, .col-angleView-full{
  text-align: left;
  float: right;
  width: 75%;
  padding-left: 5px;
}

.col-angleView-full{
  width: 55%;
}

.col-full input[type=number] {
  width: 5em;
}

.col-button {
  float: left;
  width: 20%;
  padding: 5px;
}

.col-half {
  text-align: center;
  float: left;
  width: 32.5%;
  padding-left: 5px;
}

.col-third {
  text-align: center;
  float: left;
  width: 25%;
  padding-left: 5px;
}

.col-small {
  text-align: center;
  float: left;
  width: 32.5%;
  padding-left: 5px;
  background-color: #ccc;
}

.col-declaration-long {
  text-align: right;
  float: left;
  width: 25%;
}

.col-declaration-long  input[type=number] {
  width: 3em;
}

.col-button {
  text-align: left;
  float: left;
  width: 20%;
  padding-left: 5px;
}

.col-text {
  text-align: left;
  float: left;
  width: 50%;
  padding-left: 5px;
}

.col-setting-values {
  text-align: left;
  float: right;
  width: 74%;
  padding-left: 5px;
}

.col-setting-descr {
  text-align: right;
  float: left;
  width: 25%;
}

.row {
  background-color: #fff;
  margin: 5px 0px 5px 0px;
}

.row:after {
  content: "";
  display: table;
  clear: both;
}

.c_button_grp, .label_grp {
  display:inline-block;
  padding:0.3em 0.8em;
  margin:0 0.3em 0.3em 0;
}

.c_button_grp {
  width: 17%;
}
a.c_button,a.show_button {
  display:inline-block;
  padding:0.3em 0.8em;
  margin:0 0.3em 0.3em 0;
  width: 100%;
  border-radius:2em;
  box-sizing: border-box;
  text-decoration:none;
  font-weight:500;
  color:#000000;
  background-color: #4CAF50;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
  text-align:center;
  transition: all 0.2s;
}

a.show_button {
  background-color: #666666;
}

a.set_servo {
  background-color: #01DF01;
}

a.store_servo {
  font-size: .8em;
  background-color: #848484;
}

a.c_button:hover {
  opacity: 1;
}

.switch {
 position: relative;
 display: inline-block;
 width: 60px;
 height: 34px;
}

/* Hide default HTML checkbox */
.switch input {
 opacity: 0;
 width: 0;
 height: 0;
}

/* The slidebox */
.slidebox {
 position: absolute;
 cursor: pointer;
 top: 0;
 left: 0;
 right: 0;
 bottom: 0;
 background-color: #ccc;
 -webkit-transition: .4s;
 transition: .4s;
}

.slidebox:before {
 position: absolute;
 content: "";
 height: 26px;
 width: 26px;
 left: 4px;
 bottom: 4px;
 background-color: white;
 -webkit-transition: .4s;
 transition: .4s;
}

input:checked + .slidebox {
 background-color: #2196F3;
}

input:focus + .slidebox {
 box-shadow: 0 0 1px #2196F3;
}

input:checked + .slidebox:before {
 -webkit-transform: translateX(26px);
 -ms-transform: translateX(26px);
 transform: translateX(26px);
}

/* Rounded slidebox */
.slidebox.round {
 border-radius: 34px;
}

.slidebox.round:before {
 border-radius: 50%;
}

.slider {
  -webkit-appearance: none;
  width: 100%;
  height: 25px;
  background: #d3d3d3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 25px;
  height: 25px;
  background: #4CAF50;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 25px;
  height: 25px;
  background: #4CAF50;
  cursor: pointer;
}

a.limiter {
  background-color: #FE642E;
  opacity: 1.0;
}

.tooltip {
  position: relative;
  display: inline-block;
  border-bottom: 1px dotted black;
}
.tooltip .tooltiptext {
  visibility: hidden;
  width: 120px;
  background-color: black;
  color: #fff;
  text-align: center;
  border-radius: 6px;
  padding: 5px 0;
  position: absolute;
  z-index: 1;
  top: -5px;
  right: 110%;
}

.tooltip .tooltiptext::after {
  content: "";
  position: absolute;
  top: 50%;
  left: 100%;
  margin-top: -5px;
  border-width: 5px;
  border-style: solid;
  border-color: transparent transparent transparent black;
}
.tooltip:hover .tooltiptext {
  visibility: visible;
  opacity: 0.7;
}

 /* Dropdown Button */
.dropbtn {
  background-color: #4CAF50;
  color: white;
  padding: 16px;
  font-size: 16px;
  border: none;
}

/* The container <div> - needed to position the dropdown content */
.dropdown {
  position: relative;
  position: absolute;
  display: inline-block;
}

/* Dropdown Content (Hidden by Default) */
.dropdown-content {
  display: none;
  position: absolute;
  background-color: #f1f1f1;
  min-width: 160px;
  box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
  z-index: 1;
}

/* Links inside the dropdown */
.dropdown-content a {
  color: black;
  padding: 12px 16px;
  text-decoration: none;
  display: block;
}

/* Change color of dropdown links on hover */
.dropdown-content a:hover {background-color: #ddd;}

/* Show the dropdown menu on hover */
.dropdown:hover .dropdown-content {display: block;}
.dropdown-rs:hover .dropdown-content {display: block;}
.dropdown-rs:hover .dropdown-touch {display: block;}
.dropdown-rs:hover:active {display: none;}


/* Change the background color of the dropdown button when the dropdown content is shown */
.dropdown:hover .dropbtn {background-color: #3e8e41;}

/* Responsive layout - when the screen is less than 600px wide, make the two columns stack on top of each other instead of next to each other
@media screen and (max-width: 600px) {
*/
@media (max-aspect-ratio: 5/5) {
  body {
    background: #66f;
    font-family: Arial, Helvetica, sans-serif;
  }
  
  h2, h3, h4, p {
    margin: 3px;
    padding: 0px 0px 0px 0px;
  }
  
  .slidecontainer {
    width: 95%;
  }

  .slider {
    height: 50px;
  }
  
  .slider::-webkit-slider-thumb {
    height: 70px;
  }
  
  .slider::-moz-range-thumb {
    height: 70px;
  }
  

  a.c_button, a.show_button   {
    border: 3px solid #000;
    height: 4em;
    writing-mode: tb-rl;
    padding: 0px 7px 0px 7px;
  }
  
  .col-declaration {
    font-size: .7em;
    width: 15%;
  }

  .col-angleView-declaration {
    margin-top: 0;
    width: 100%;
    background-color: #ddf;
    padding: 5px;
    text-align: left;
  }
  
  .col-declaration-long {
    text-align: center;
    width: 100%;
  }
  
  .col-button {
    text-align: center;
    width: 100%;
  }
  
  .col-angleView-full {
    width: 100%;
    text-align: left;
  }
  
  .col-full {
    width: 85%;
  }
  
  .col-half {
    width: 42.5%;
  }
  .col-third {
    width: 28%;
  }
  
  .col-text {
    display: none;
  }
  
  col-setting-values {
    width: 100%;
  }
  
  .col-setting-descr {
    text-align: left;
    width: 100%;
  }
}

)=====";
