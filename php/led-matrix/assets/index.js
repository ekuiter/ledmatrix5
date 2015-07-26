var updateInterval = 5;
var loadingMaxWidth = 100;
var intervals = [];

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

  $("#off").click(function() {
    $.ajax("api.php?run/idle");
  });

  $("#on").click(function() {
    $.ajax("api.php?run/light");
  });

  $("#pluginMode").click(function() {
    $.ajax("api.php?control/start");
  });

  function update() {
    intervals.forEach(function(interval) {
      window.clearInterval(interval);
    });

    $.ajax("api.php?control/log").then(function(res) {
      $("#log").text(res);
      var nextUpdate = Date.now() + updateInterval * 1000;
      intervals.push(window.setInterval(function() {
        var remainingMs = (updateInterval * 1000) - (nextUpdate - Date.now());
        $(".log.badge").width(remainingMs / (updateInterval * 1000 / loadingMaxWidth));
      }, 50));
    });

    $.ajax("api.php?state").then(function(res) {
      $("#state").text(res);
      var state = JSON.parse(res);
      if (state["mode"] == "manual")
        $("#pluginMode").attr("disabled", false);
      else
        $("#pluginMode").attr("disabled", true);
    });
  }

  update();
  window.setInterval(update, updateInterval * 1000);
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