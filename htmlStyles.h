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

input[type=text], select, textarea {
  width: 100%;
  padding: 5px;
  border: 1px solid #ccc;
  border-radius: 4px;
  resize: vertical;
}

input[type=checkbox].config, input[type=radio].config, input[type=number].config {
  margin: 5px 12px 5px 5px;
  border: 1px solid #ccc;
  border-radius: 4px;
  display: block;
}

label {
  padding: 6px 12px 6px 0;
  display: block;
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

.col-declaration {
  text-align: right;
  float: left;
  width: 25%;
}

.col-full {
  text-align: left;
  float: right;
  width: 75%;
  padding-left: 5px;
}

.col-full input[type=number] {
  width: 5em;
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
 /*  margin: 2px */
  content: "";
  display: table;
  clear: both;
}

a.c_button {
  display:inline-block;
  padding:0.3em 0.8em;
  margin:0 0.3em 0.3em 0;
  width: 10%;
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

a.store {
  background-color: #01DF01;
}

a.set {
  background-color: #848484;
}

a.limiter {
  background-color: #FE642E;
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

/* Responsive layout - when the screen is less than 600px wide, make the two columns stack on top of each other instead of next to each other
@media screen and (max-width: 600px) {
*/
@media (max-aspect-ratio: 8/5) {
  body {
    background: #66f;
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
  
  a.c_button   {
    border: 3px solid #000;
    height: 4em;
    writing-mode: tb-rl;
    padding: 0px 7px 0px 7px;
  }
  
  .col-declaration {
    font-size: .7em;
    width: 15%;
  }
  
  .col-declaration-long {
    text-align: center;
    width: 100%;
  }
  
  .col-button {
    text-align: center;
    width: 100%;
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
