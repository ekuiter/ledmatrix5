Connection::Connection(Effects& e): effects(e) { }

void Connection::setup() {
  Serial.begin(9600);
}

void Connection::receive() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n')
      process();
    else
      command += String(c);
  }
}

void Connection::process() {
  int delimiter, delimiter2;
  int idx = getEffectIndex(delimiter);
  Color color = getColor(delimiter, delimiter2);
  int argument = getArgument(delimiter2);
  command = "";
  if (idx < 0)
    mode = SWITCHING_LOOP;
  else if (idx >= EFFECT_NUMBER);
  else {
    Effect effect = effects.effects[idx];
    if (effect.func) {
      mode = SWITCHING_MANUAL;
      effects.schedule(effect, color, argument);
    }
  }
}

int Connection::getEffectIndex(int& delimiter) {
  delimiter = command.indexOf(",");
  String effectString;
  if (delimiter == -1)
    effectString = command;
  else
    effectString = command.substring(0, delimiter);
  effectString.trim();
  return effectString.toInt();
}

Color Connection::getColor(int delimiter, int& delimiter2) {
  if (delimiter == -1) return UNDEF;
  delimiter2 = command.indexOf(",", delimiter + 1);
  String colorString;
  if (delimiter2 == -1)
    colorString = command.substring(delimiter + 1);
  else
    colorString = command.substring(delimiter + 1, delimiter2);
  colorString.trim();
  return effects.leds.numberToColor(colorString.toInt());
}

int Connection::getArgument(int delimiter2) {
  if (delimiter2 == -1) return -1;
  String argumentString = command.substring(delimiter2 + 1);
  argumentString.trim();
  return argumentString.toInt();
}
