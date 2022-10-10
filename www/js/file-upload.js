
var xhr;
//File uploading method
function uploadFile() {
  var fileObj = document.getElementById("file").files[0]; // js get file object
  if ( fileObj === null || fileObj === undefined )
  {
    alert("Error: No file selected.");
    return;    
  }

  var url =  "/upload_endpoint"; 

  var form = new FormData(); // FormData object
  form.append("file", fileObj); // File object

  xhr = new XMLHttpRequest();  // XMLHttpRequest object
  xhr.open("post", url, true); //post
  xhr.onload = uploadComplete; 
  xhr.onerror =  uploadFailed; 

  xhr.upload.onprogress = progressFunction;
  xhr.upload.onloadstart = function(){
      ot = new Date().getTime();
      oloaded = 0;
  };

  xhr.send(form); 
}

function uploadComplete(evt) {
    var data = evt.target.responseText;
    alert("File has been uploaded.\n" + data);
}

function uploadFailed(evt) {
    alert("Upload failed!");
}

function cancelUploadFile(){
    xhr.abort();
}

function progressFunction(evt) {
    var progressBar = document.getElementById("progressBar");
    var percentageDiv = document.getElementById("percentage");
    if (evt.lengthComputable) {//
        progressBar.max = evt.total;
        progressBar.value = evt.loaded;
        percentageDiv.innerHTML = Math.round(evt.loaded / evt.total * 100) + "%";
    }
    var time = document.getElementById("time");
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