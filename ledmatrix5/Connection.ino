Connection::Connection(Effects& e): effects(e) { }

void Connection::setup() {
  Bridge.begin();
  Console.begin();
  Mailbox.begin();
}

void Connection::receive() {
  while (Console.available() > 0) {
    char c = Console.read();
    if (c == '\n')
      process();
    else
      command += String(c);
  }
}

void Connection::process() {
  int delimiter;
  int idx = getEffectIndex(delimiter);
  Color color = getColor(delimiter);
  command = "";
  if (idx < 0)
    mode = SWITCHING_LOOP;
  else if (idx >= EFFECT_NUMBER);
  else {
    Effect effect = effects.effects[idx];
    if (effect.func) {
      mode = SWITCHING_MANUAL;
      effects.schedule(effect, color);
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

Color Connection::getColor(int delimiter) {
  if (delimiter == -1) return UNDEF;
  String colorString = command.substring(delimiter + 1);
  colorString.trim();
  return effects.leds.numberToColor(colorString.toInt());
}
