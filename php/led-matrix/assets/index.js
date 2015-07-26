var updateInterval = 5;

$(function() {
  $("#run").submit(function(e) {
    e.preventDefault();
    var effect = $("#effect option:selected").text();
    var color = $("#color option:selected").text();
    if (effect == "text") {
      var text = $("#text").val().replace(" ", "_");
      if (text === "") text = "_";
      apiCall("run", "text", color, text);
    } else
      apiCall("run", effect, color);
  });

  $("#effect").change(function() {
    if ($("#effect option:selected").text() == "text")
      $("#text").css("display", "inline");
    else
      $("#text").css("display", "none");
  });

  function loadLog() {
    $(".log.badge").css("display", "inline");
    $.ajax("api.php?control/log").then(function(res) {
      $("#log").text(res);
      $(".log.badge").css("display", "none");
    });
  }

  function loadState() {
    $.ajax("api.php?state").then(function(res) {
      $("#state").text(res);
    });
  }

  loadLog();
  loadState();
  window.setInterval(loadLog, updateInterval * 1000);
  window.setInterval(loadState, updateInterval * 1000);
});

apiCall = function(/* func, args ..., callback */) {
  var callback = arguments[arguments.length - 1];
  var end = typeof callback === "function" ? -1 : arguments.length;
  var args = Array.prototype.slice.call(arguments, 0, end).join("/");
  $.ajax("api.php?" + args).then(function(res) {
    if (res.indexOf("Connection Error") !== -1)
      $("#error").text(res);
    if (typeof callback === "function")
      callback(res);
  });
};