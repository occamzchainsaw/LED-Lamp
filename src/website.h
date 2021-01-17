/*
 * website.h
 *
 * Created on Sun Jan 10 2021
 * 
 * This header file contains the text of the HTML to be used as the website on my webserver.
 *
 * Copyright (c) 2021 occamzchainsaw
 */

#include <Arduino.h>                                                //  Arduino Framework

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 LED Controls</title>
<script src="https://kit.fontawesome.com/684240325d.js" crossorigin="anonymous"></script>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.4.5/jscolor.min.js"></script>
<!--  Script that controls the dropdown's behaviour 
      and other updates to the appearance -->
<script>
  $(document).ready(function() {
    $(".dropbtn").hover(function() {
      $(".dropdown-content").show();
    });

    $(".dropdown-content").click(function() {
      $(".dropdown-content").hide();
    });

    document.getElementById('solid-slider').oninput = function() {
      document.getElementById('bVal').innerHTML = this.value;
    };
    document.getElementById('rainbow-slider').oninput = function() {
      document.getElementById('rbVal').innerHTML = this.value;
    };

    document.getElementById('init-hue-slider').oninput = function() {
      document.getElementById('hVal').innerHTML = this.value;
    };

    document.getElementById('delta-hue-slider').oninput = function() {
      document.getElementById('dhVal').innerHTML = this.value;
    };
  });
</script>

<!-- Script that sets up the jscolor picker I stole :) -->
<script>
  jscolor.presets.default = {
    format: 'rgb',
    previewSize: 200,
    borderRadius: 2,
    padding: 15,
    width: 200,
    height: 144,
    closeButton: true,
    sliderSize: 20,
    alphaChannel: false
  };
</script>

<style>
body {margin: 0;}

html {
  font-family: sans-serif;
}

h2 {
  font-size: 20px;
  font-weight: lighter;
}

ul {
  list-style-type: none;
  margin: 0;
  padding: 0;
  overflow: hidden;
  background-color: #333;
  font-size: 20px;
}

li {
  float: left;
}

li a, .dropbtn {
  display: inline-block;
  color: white;
  text-align: center;
  padding: 14px 16px;
  text-decoration: none;
}

li a:hover, .dropbtn:hover .dropbtn {
  background-color: #008CBA;
}

li.dropdown {
  display: inline-block;
}

.dropdown-content {
  display: none;
  position: absolute;
  background-color: #333;
  min-width: 160px;
  box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
  z-index: 1;
}

.dropdown-content a {
  color: white;
  padding: 12px 16px;
  text-decoration: none;
  display: block;
  text-align: left;
}

.dropdown-content a:hover {
  background-color: #008CBA;
}

.window {
    display:none;
    padding: 16px 30px;
}

.colourbtn {
  font-size: 20px;
  font-weight: lighter;
  border: 1px solid #333;
  outline: none;
}

.bright-slider, .color-slider {
  -webkit-appearance: none;
  width: 200px;
  height: 12px;
  border-radius: 5px;
  background: #d3d3d3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.bright-slider:hover, .color-slider:hover {
  opacity: 1;
}

.bright-slider::-webkit-slider-thumb, .color-slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  background: #0072B5;
  width: 24px;
  height: 24px;
  border-radius: 50%;
}

.sendbtn {
  position: absolute;
  bottom: 50px;
  font-size: 20px;
  padding: 12px 24px;
  text-align: center;
  text-decoration: none;
  font-weight: lighter;
  outline: none;
  border: none;
  color: white;
  background-color: #555555;
}

@media screen and (max-width: 600px) {
  ul li.right, 
  ul li {float: none;}
}
</style>
</head>
<body>

<ul>
  <li class="dropdown">
    <a class="dropbtn"><i class="fas fa-lightbulb" style="color:white;"></i>  Lighting</a>
    <div class="dropdown-content">
      <a href="#l_solid" onclick="sceneChange('l_solid')">Solid color</a>
      <a href="#l_rainbow" onclick="sceneChange('l_rainbow')">Rainbow</a>
      <a href="#l_gradient" onclick="sceneChange('l_gradient')">Gradient</a>
    </div>
  </li>
</ul>

<div class="window" id="l_solid">
  <h2>Select your Colour:</h2>
  <button data-jscolor="{value: '#0072B5', alpha:1}" class="colourbtn" id="colour-picker"></button>
  <p>&nbsp;</p>
  <h2>Set the Brightness:</h2>
  <input type="range" min="1" max="100" value="20" class="bright-slider" id="solid-slider">
  <p>Current Brightness: <span id="bVal"></span></p>
  <p>&nbsp;</p>
  <input type="checkbox" name="breathe" id="breathing-checkbox">
  <label for="breathe">Turn on Breathing Effect</label>
  <p>&nbsp;</p>
  <button class="sendbtn" onclick="sendParamsSolid()">Lemme See</button>
</div>

<div class="window" id="l_rainbow">
  <h2>If you insist...</h2>
  <p>&nbsp;</p>
  <h2>Choose a starting hue:</h2>
  <input type="range" min="0" max="255" value="0" class="color-slider" id="init-hue-slider">
  <p>Current Hue: <span id="hVal"></span></p>
  <p>&nbsp;</p>
  <h2>Choose a hue delta:</h2>
  <input type="range" min="1" max="16" value="2" class="color-slider" id="delta-hue-slider">
  <p>Current Hue Delta: <span id="dhVal"></span></p>
  <p>&nbsp;</p>
  <h2>Set the Brightness:</h2>
  <input type="range" min="1" max="100" value="20" class="bright-slider" id="rainbow-slider">
  <p>Current Brightness: <span id="rbVal"></span></p>
  <p>&nbsp;</p>
  <input type="checkbox" name="run-rainbow" id="running-rainbow-checkbox">
  <label for="breathe">Running</label>
  <p>&nbsp;</p>
  <button class="sendbtn" onclick="sendParamsRainbow()">Lemme See</button>
</div>

<div class="window" id="l_gradient">
  <p>This is Gradient</p>
</div>

<script type='text/javascript'>
    function sceneChange(elementId) {
        allDivs = document.getElementsByClassName('window');
        for (var i = 0; i < allDivs.length; i++) {
            allDivs[i].style.display = 'none';
        }
        document.getElementById(elementId).style.display = 'block';
    };

    function debug() {
      var r = Math.round(document.getElementById('colour-picker').jscolor.channel('R'));
      var g = Math.round(document.getElementById('colour-picker').jscolor.channel('G'));
      var b = Math.round(document.getElementById('colour-picker').jscolor.channel('B'));
      console.log("Color: " + r + ", " + g + ", " + b);
    };

    function sendParamsSolid() {
      var request = new XMLHttpRequest();
      var url = "/updateParameters";
      request.open("POST", url, true);
      request.setRequestHeader("Content-Type", "application/json");
      request.onreadystatechange = function() {
        if (request.readyState === 4 && request.status === 200) {
        }
      };
      var type = "solid_color";
      var r = Math.round(document.getElementById('colour-picker').jscolor.channel('R')).toString();
      var g = Math.round(document.getElementById('colour-picker').jscolor.channel('G')).toString();
      var b = Math.round(document.getElementById('colour-picker').jscolor.channel('B')).toString();
      var brite = document.getElementById('solid-slider').value;
      var breahting = document.getElementById('breathing-checkbox').checked;
      var params = JSON.stringify({"type": type, "red": r, "green": g, "blue": b, "brightness": brite, "breathingOn": breahting});
      request.send(params);
    };

    function sendParamsRainbow() {
      var request = new XMLHttpRequest();
      var url = "/updateParameters";
      request.open("POST", url, true);
      request.setRequestHeader("Content-Type", "application/json");
      request.onreadystatechange = function() {
        if (request.readyState === 4 && request.status === 200) {
        }
      };
      var type = "rainbow";
      var init_hue = document.getElementById('init-hue-slider').value;
      var delta_hue = document.getElementById('delta-hue-slider').value;
      var brite = document.getElementById('rainbow-slider').value;
      var running = document.getElementById('running-rainbow-checkbox').checked;
      var params = JSON.stringify({"type": type, "init_hue": init_hue, "delta_hue": delta_hue, "brightness": brite, "runningOn": running});
      request.send(params);
    };
</script>

</body>
</html>
)rawliteral";