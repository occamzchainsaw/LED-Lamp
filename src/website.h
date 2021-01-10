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
<style>
body {margin: 0;}

ul.topnav {
  list-style-type: none;
  margin: 0;
  padding: 0;
  overflow: hidden;
  background-color: #333;
}

ul.topnav li {float: left;}

ul.topnav li a, .dropbtn {
  display: block;
  color: white;
  text-align: center;
  padding: 14px 16px;
  text-decoration: none;
}

ul.topnav li a:hover:not(.active), .dropdown:hover .dropbtn {background-color: #111;}

ul.topnav li.dropdown {display: inline-block;}

.dropdown-content {
  display: none;
  position: absolute;
  background-color: #f9f9f9;
  min-width: 160px;
  box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
  z-index: 1;
}

.dropdown-content a {
  color: black;
  padding: 12px 16px;
  text-decoration: none;
  display: block;
  text-align: left;
}

ul.topnav li a.active {background-color: #4CAF50;}

ul.topnav li.right {float: right;}

.dropdown-content a:hover {background-color: #f1f1f1;}

.dropdown:hover .dropdown-content {
  display: block;
}

@media screen and (max-width: 600px) {
  ul.topnav li.right, 
  ul.topnav li {float: none;}
}
</style>
</head>
<body>

<ul class="topnav">
  <li><a class="active" href="#home">Home</a></li>
  <li><a href="#news">News</a></li>
  <li><a href="#contact">Contact</a></li>
  <li class="dropdown">
    <a href="javascript:void(0)" class="dropbtn">Dropdown</a>
    <div class="dropdown-content">
      <a href="#">Link 1</a>
      <a href="#">Link 2</a>
      <a href="#">Link 3</a>
    </div>
  </li>
  <li class="right"><a href="#about">About</a></li>
</ul>

<div style="padding:0 16px;">
  <h2>Responsive Topnav Example</h2>
  <h3>With an added dropdown menu</h3>
  <p>This example use media queries to stack the topnav vertically when the screen size is 600px or less.</p>
</div>

</body>
</html>
)rawliteral";