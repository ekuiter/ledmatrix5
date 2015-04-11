$(function() {
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