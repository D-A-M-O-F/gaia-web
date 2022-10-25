
function getElementInsideContainer(containerID, childID) {
    var elm = {};
    var elms = document.getElementById(containerID).getElementsByTagName("*");
    for (var i = 0; i < elms.length; i++) {
        if (elms[i].id === childID) {
            elm = elms[i];
            break;
        }
    }
    return elm;
}

var xhr;
//File uploading method
function uploadFile( divid, element_id ) {
  var file_element = getElementInsideContainer(divid, 'file' );
  var enable_element = null;
  var fileObj = file_element.files[0]; // js get file object
  if ( fileObj === null || fileObj === undefined )
  {
    alert("Error: No file selected.");
    return;    
  }

  if ( element_id !== null && element_id !== undefined )
  {
    enable_element = document.getElementById(element_id);
  }

  var url =  "/upload_endpoint"; 

  var form = new FormData(); // FormData object
  form.append(file_element, fileObj); // File object

  xhr = new XMLHttpRequest();  // XMLHttpRequest object
  xhr.open("post", url, true); //post

  // Handle success event
  xhr.onload = function (evt) {
    var data = evt.target.responseText;
    alert("File has been uploaded.\n" + data);
    if ( enable_element !== null )
    {
      enable_element.disabled = false;
    }
  } 

  // Handle error event
  xhr.onerror = function (evt) {
    alert("Upload failed!");
  }

  // Handle progress bar event
  xhr.upload.onprogress = function (evt) {
    var progressBar   = getElementInsideContainer(divid, 'progressBar' );
    var percentageDiv = getElementInsideContainer(divid, 'percentage'  );

    if (evt.lengthComputable) {//
        progressBar.max = evt.total;
        progressBar.value = evt.loaded;
        percentageDiv.innerHTML = Math.round(evt.loaded / evt.total * 100) + "%";
    }

    var time = getElementInsideContainer(divid, 'time' );
    var nt = new Date().getTime();
    var pertime = (nt-ot)/1000; 
    ot = new Date().getTime(); 
    var perload = evt.loaded - oloaded; 
    oloaded = evt.loaded;
    var speed = perload/pertime;
    var bspeed = speed;
    var units = 'b/s';
    if(speed/1024>1){
        speed = speed/1024;
        units = 'k/s';
    }
    if(speed/1024>1){
        speed = speed/1024;
        units = 'M/s';
    }
    speed = speed.toFixed(1);
    var resttime = ((evt.total-evt.loaded)/bspeed).toFixed(1);
    time.innerHTML = ',Speed: '+speed+units+', the remaining time: '+resttime+'s';
    if(bspeed==0) time.innerHTML = 'Upload cancelled';
  }

  xhr.upload.onloadstart = function(){
      ot = new Date().getTime();
      oloaded = 0;
  };

  xhr.send(form);   
}

function cancelUploadFile( divid ){
    xhr.abort();
}
