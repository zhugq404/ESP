// scripts.js
function toggleDevice(device) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', '/' + device + '_TOGGLE', true);
    xhr.send();
    xhr.onreadystatechange = function() {
      if (xhr.readyState == 4 && xhr.status == 200) {
        var response = JSON.parse(xhr.responseText);
        document.getElementById(device.toLowerCase() + '_status').innerText = response.status;
        document.getElementById(device.toLowerCase() + '_img').src = response.image;
        document.querySelector('#' + device.toLowerCase() + ' .toggle').innerText = response.buttonText;
      }
    };
  }
  
  function activateScene(scene) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', '/SCENE_' + scene, true);
    xhr.send();
    xhr.onreadystatechange = function() {
      if (xhr.readyState == 4 && xhr.status == 200) {
        alert(scene + '模式已激活');
      }
    };
  }