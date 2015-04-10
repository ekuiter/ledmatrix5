var rgbLedNumber  =  25;
var colorNumber   =   3;
var stateInterval = 600;
var intervalId;

$(function() {
  for (i = 0; i < rgbLedNumber; i++)
    $(".matrix").append("<li></li>");

  intervalId = window.setInterval(getState, stateInterval);
  getState();

  $(window).focus(function() {
    if (!intervalId)
      intervalId = setInterval(getState, stateInterval);
  });

  $(window).blur(function() {
    clearInterval(intervalId);
    intervalId = 0;
  });

  $("#run").submit(function(e) {
    e.preventDefault();
    effect = $("#effect").val();
    color = $("#color").val();
    if ($("#effect option:selected").text() == "text") {
      text = $("#text").val().replace(" ", "_");
      if (text === "") text = "_";
      apiCall("text", text, function() {
        apiCall("run", effect, color);
      });
    } else
      apiCall("run", effect, color);
  });

  $("#effect").change(function() {
    if ($("#effect option:selected").text() == "text")
      $("#text").css("display", "inline");
    else
      $("#text").css("display", "none");
  });
});

binary = function(hex) {
  var bytes = [], str;
  for(var i = 0; i < hex.length - 1; i += 2)
      bytes.push(pad(parseInt(hex.substr(i, 2), 16).toString(2), 8));
  return bytes.join("");    
};

pad = function(n, width) {
  return n.length >= width ? n : new Array(width - n.length + 1).join("0") + n;
};

reverse = function(s) {
  return s.split("").reverse().join("");
};

getState = function() {
  apiCall("getState", function(res) {
    if (res.indexOf("Connection Error") !== -1)
      res = "";
    $(".matrixContainer .overlay").text(res === "" ? "?" : "");
    res = reverse(binary(res));
    console.log(res);
    $(".matrix li").each(function(rgbLed, elem) {
      red   = parseInt(res[rgbLed * colorNumber]);
      green = parseInt(res[rgbLed * colorNumber + 1]);
      blue  = parseInt(res[rgbLed * colorNumber + 2]);
      if (res === "" || red && green && blue)
        klass = "white";
      else if (red && green)
        klass = "yellow";
      else if (red && blue)
        klass = "magenta";
      else if (green && blue)
        klass = "cyan";
      else if (red)
        klass = "red";
      else if (green)
        klass = "green";
      else if (blue)
        klass = "blue";
      else
        klass = "none";
      $(elem).removeClass().addClass(klass);
    });
  });
};

apiCall = function(/* func, args ..., callback */) {
  callback = arguments[arguments.length - 1];
  end = typeof callback === "function" ? -1 : arguments.length;
  args = Array.prototype.slice.call(arguments, 0, end).join(",");
  $.ajax("api.php?" + args).then(function(res) {
    if (res.indexOf("Connection Error") === -1)
      $("#status").text("Connected.").removeClass().addClass("text-success");
    else
      $("#status").text("Connection failed.").removeClass().addClass("text-danger");
    if (typeof callback === "function")
      callback(res);
  }); 
};