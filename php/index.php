<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>LED matrix</title>
<link href="bootstrap.min.css" rel="stylesheet">
<link href="index.css" rel="stylesheet">
<script src="jquery.min.js"></script>
<script src="bootstrap.min.js"></script>
<script src="index.js"></script>
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
        <li class="active"><a href="#">Home</a></li>
      </ul>
      <ul class="nav navbar-nav navbar-right">
        <li><a><span id="status"></span></a></li>
      </ul>
    </div>
  </div>
</nav>
<div class="container">
  <h1>LED matrix</h1>
  <form id="run" class="form-inline">
    <select id="effect" class="form-control">
      <?php
        require_once "config.inc.php";
        foreach ($effects as $effect_name => $effect_index)
          echo "<option value=\"$effect_index\">$effect_name</option>";
      ?>
    </select>
    <select id="color" class="form-control">
      <option value="-2">random</option>
      <option value="-1">none</option>
      <option value="0" selected>undef</option>
      <option value="1">red</option>
      <option value="2">green</option>
      <option value="3">blue</option>
      <option value="4">yellow</option>
      <option value="5">magenta</option>
      <option value="6">cyan</option>
      <option value="7">white</option>
    </select>
    <input type="text" id="text" class="form-control" placeholder="text" />
    <input type="submit" class="btn btn-primary" value="Run" />
  </form>
</div>
</body>
</html>
