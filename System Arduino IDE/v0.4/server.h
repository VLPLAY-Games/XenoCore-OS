// Сервер

// HTML, CSS, JS для сервера обновлений

/*
 * Server Index Page
 */

const char* serverUpdateSystem =
"<title>Update System</title>"
"<h2>System Firmware Update</h2>"
"<h3>XenoClock 1</h3>"
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<div id='upload-container'>"
"  <form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"    <h3>Current System Version: XenoCore v0.4</h3>"
"    <label for='file-upload' class='choose-file'>Choose file</label>"
"    <input type='file' name='update' id='file-upload'>"
"    <span class='file-name'>No file chosen</span>"
     "<h4 id = 'warning'></h4>"
"    <input type='submit' value='Update' id='update-btn'>"
"  </form>"
"  <div id='prg'>Progress: 0%</div>"
"</div>"
"<script>"
"$(document).ready(function() {"
"  $('form').submit(function(e){"
"    e.preventDefault();"
"    var form = $('#upload_form')[0];"
"    var data = new FormData(form);"
"    $.ajax({"
"      url: '/update',"
"      type: 'POST',"
"      data: data,"
"      contentType: false,"
"      processData: false,"
"      xhr: function() {"
"        var xhr = new window.XMLHttpRequest();"
"        xhr.upload.addEventListener('progress', function(evt) {"
"          if (evt.lengthComputable) {"
"            var per = evt.loaded / evt.total;"
"            $('#prg').html('Progress: ' + Math.round(per * 100) + '%');"
"          }"
"        }, false);"
"        return xhr;"
"      },"
"      success:function(d, s) {"
"        console.log('success!');"
"      },"
"      error: function (a, b, c) {"
"      }"
"    });"
"  });"

"  $('#file-upload').on('change', function() {"
"    var fileInput = $(this);"
"    var fileName = fileInput.val().split('\\\\').pop();"
"    $('.file-name').text(fileName);"
"    if (!fileName.includes('XenoCoreOS')) {"
"      $('#warning').html('Caution: This file may not be safe for updating.');"
"    }"
"  });"
"});"
"</script>"
"<style>"
"  body {"
"    font-family: Arial, sans-serif;"
"    background-color: #f2f2f2;"
"  }"
""
"  h2,h3 {"
"    text-align: center;"
"    margin-top: 20px;"
"    font-weight: bold;"
"  }"
"  h4 {"
"    text-align: center;"
"    margin-top: 20px;"
"    font-weight: bold;"
"    background-color: #dc143c;"
"  }"
""
"  #upload-container {"
"    width: 50%;"
"    margin: 20px auto;"
"    padding: 20px;"
"    border: 1px solid #ccc;"
"    background-color: #ffffff;"
"    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);"
"  }"
""
"  .choose-file {"
"    display: inline-block;"
"    padding: 10px 20px;"
"    background-color: #4CAF50;"
"    color: #fff;"
"    border: none;"
"    border-radius: 4px;"
"    cursor: pointer;"
"    font-weight: bold;"
"  }"
""
"  #file-upload {"
"    display: none;"
"  }"
""
"  .file-name {"
"    margin-left: 10px;"
"    color: #999;"
"  }"
""
"  #update-btn {"
"    display: block;"
"    width: 100%;"
"    margin-top: 20px;"
"    padding: 10px;"
"    background-color: #4CAF50;"
"    color: #fff;"
"    border: none;"
"    border-radius: 4px;"
"    cursor: pointer;"
"    font-weight: bold;"
"  }"
""
"  #prg {"
"    margin-top: 20px;"
"    font-weight: bold;"
"  }"
"</style>";





const char* serverMainPage = 
"<html>"
"<head>"
    "<title>Main Page</title>"
    "<style>"
        "body {"
            "font-family: Arial, sans-serif;"
            "background-color: #f2f2f2;"
        "}"
        ""
        "#update-container {"
            "text-align: center;"
            "margin-top: 20px;"
        "}"

        "#sysupdatepagebtn {"
            "display: inline-block;"
            "padding: 20px;"
            "width: 200px;"
            "background-color: #4CAF50;"
            "color: white;"
            "border: none;"
            "border-radius: 4px;"
            "cursor: pointer;"
            "font-weight: bold;"
            "font-size: 18px;"
            "box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);"
        "}"
        ""
        "#sysupdatepagebtn:hover {"
            "background-color: #45a049;"
        "}"
        "  h2, h3{"
        "  text-align: center;"
        "  margin-top: 20px;"
        "  font-weight: bold;"
        "  }"
    "</style>"
"</head>"
"<body>"
    "<h2>XenoClock Main Page</h2>"
    "<h3>Model - XenoClock 1</h3>"
    "<div id='update-container'>"
        "<button id='sysupdatepagebtn'>Update system</button>"
    "</div>"
    "<script>"
        "const updateButton = document.getElementById('sysupdatepagebtn');"
        "updateButton.addEventListener('click', () => {"
            "window.location.href = '/updatesystem';"
        "});"
    "</script>"
"</body>"
"</html>";