<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>LED matrix</title>
<link href="assets/bootstrap.min.css" rel="stylesheet">
<link href="assets/index.css" rel="stylesheet">
<script src="assets/jquery.min.js"></script>
<script src="assets/bootstrap.min.js"></script>
<script src="assets/index.js"></script>
</head>
<body>
<nav class="navbar navbar-inverse" role="navigation">
  <div class="container">
    <div class="navbar-header">
      <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
        <span class="sr-only">Toggle navigation</span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
      <a class="navbar-brand" href="#">LED matrix</a>
    </div>
    <div id="navbar" class="collapse navbar-collapse">
      <ul class="nav navbar-nav navbar-left">
      </ul>
      <ul class="nav navbar-nav navbar-right">
        <li><a><span id="error" class="text-danger"></span></a></li>
        <li><a><span id="state" style="color: #ddd"></span></a></li>
      </ul>
    </div>
  </div>
</nav>
<div class="container">
  <div class="row" style="margin-top: 20px">
    <div class="col-lg-8" style="margin-bottom: 10px">
      <form id="run" class="form-inline">
        <strong style="padding-right: 10px">Manual mode</strong>
        <select id="effect" class="form-control">
          <?php
          require_once "config.inc.php";
          foreach ($effects as $effectName => $effectId)
            echo "<option>$effectName</option>";
          ?>
        </select>
        <select id="color" class="form-control">
          <?php
          foreach ($colors as $colorName => $colorId)
            echo "<option" . ($colorName == "undef" ? " selected" : "") . ">$colorName</option>";
          ?>
        </select>
        <input type="text" id="text" class="form-control" placeholder="text" />
        <input type="submit" class="btn btn-primary" value="Run" />
      </form>
    </div>
    <div class="col-lg-4">
      <strong style="padding-right: 10px">Plugin mode</strong>
      <a id="pluginMode" class="btn btn-primary">Run</a>
      <strong style="padding: 0 10px">Tools</strong>
      <a id="off" class="btn btn-primary">Off</a> <a id="on" class="btn btn-primary">On</a>
    </div>
  </div>
  <hr />
  <h3>Control script log <span class="log badge"></span></h3>
  <pre id="log"></pre>
</div>
</body>
</html>
