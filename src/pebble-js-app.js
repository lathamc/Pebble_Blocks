var mConfig = {};

Pebble.addEventListener("ready", function(e) {
    loadLocalData();
    returnConfigToPebble();
    console.log("PebbleKit JS ready!");
  }
);

Pebble.addEventListener("showConfiguration",
  function(e) {
    //Load the remote config page
    Pebble.openURL(mConfig.configureUrl);
    //Pebble.openURL("https://dl.dropboxusercontent.com/u/74351928/pebble/configuration.html");
  }
);
/*
Pebble.addEventListener("webviewclosed",
  function(e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));
 
    //Send to Pebble, persist there
    Pebble.sendAppMessage(
      {"KEY_ZERO": configuration.zero},
      {"KEY_BATTERY": configuration.battery},
      {"KEY_BLUETOOTH": configuration.bluetooth},
      function(e) {
        console.log("Sending settings data...");
      },
      function(e) {
        console.log("Settings feedback failed!");
      }
    );
  }
);
*/

Pebble.addEventListener("webviewClosed", 
  function(e){
    if(e.response){
      var config = JSON.parse(e.response);
      saveLocalData(config);
      returnConfigToPebble();
    }
  }
);

function saveLocalData(config){
  console.log("Save Local data...");
  localStorage.setItem("zero_padding", parseInt(config.userzero));
  localStorage.setItem("battery_bar", parseInt(config.userbattery));
  localStorage.setItem("bluetooth_set", parseInt(config.userbluetooth));
 
  loadLocalData();
}

function loadLocalData(){
  console.log("Load local data....");
  mConfig.userzero = parseInt(localStorage.getItem("zero_padding"));
  mConfig.userbattery = parseInt(localStorage.getItem("battery_bar"));
  mConfig.userbluetooth = parseInt(localStorage.getItem("bluetooth_set"));
  mConfig.configureUrl = "https://dl.dropboxusercontent.com/u/74351928/pebble/configuration.html";
  
  if(isNaN(mConfig.userzero))  {
    mConfig.userzero = 1;
  }
  if(isNaN(mConfig.userbattery))  {
    mConfig.userbattery = 1;
  }
  if(isNaN(mConfig.userbluetooth))  {
    mConfig.userbluetooth = 1;
  }
}

function returnConfigToPebble(){
  Pebble.sendAppMessage({
    "zero_padding":parseInt(mConfig.userzero),
    "battery_bar":parseInt(mConfig.userbattery),
    "bluetooth_set":parseInt(mConfig.userbluetooth)
  });
}