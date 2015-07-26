var iframeInterval = 10;

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

  window.setInterval(function() {
    var $iframe = $("iframe");
    $iframe.attr("src", $iframe.attr("src"));
  }, iframeInterval * 1000);
});

apiCall = function(/* func, args ..., callback */) {
  var callback = arguments[arguments.length - 1];
  var end = typeof callback === "function" ? -1 : arguments.length;
  var args = Array.prototype.slice.call(arguments, 0, end).join("/");
  $.ajax("api.php?" + args).then(function(res) {
    $("#status").text(res).removeClass();
    if (res.indexOf("Connection Error") === -1)
      $("#status").addClass("text-success");
    else
      $("#status").addClass("text-danger");
    if (typeof callback === "function")
      callback(res);
  });
};