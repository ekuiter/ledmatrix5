#include "main.h"
#include "Registers.h"
#include "Leds.h"
#include "Effects.h"
#include <fstream>
#include <sstream>
#include <wiringPi.h>
#include <unistd.h>

void receiveHook();
void dumpHook();

Registers registers(CLOCK_PIN, LATCH_PIN, DATA_PIN);
Leds leds(registers, &receiveHook, &dumpHook);
Effects effects(leds);

class schedule_event : public std::exception {};

char stateFileName[] = "state.dat",
      dumpStateFileName[] = "dump_state.cmd",
      scheduleFileName[] = "schedule.cmd";

int main(int argc, char** argv) {
  char line[32];
  FILE* cmd = popen("pidof ledmatrix5", "r");
  fgets(line, 32, cmd);
  pclose(cmd);
  int pid;
  std::stringstream s(line);
  s >> pid;
  if (s.get() == ' ') {
    fprintf(stderr, "ledmatrix5 is already running!\n");
    exit(1);
  }
  
  wiringPiSetup();
  registers.setup();
  remove(dumpStateFileName);
  remove(scheduleFileName);
  
  while (1) {
    try {
      effects.run();
    } catch (schedule_event) {}
  }
  
  return 0;
}

void receiveHook() {
  std::ifstream scheduleFile(scheduleFileName);
  if (!scheduleFile)
    return;
  std::string effectName, effectColor, effectArg;
  scheduleFile >> effectName >> effectColor;
  std::getline(scheduleFile, effectArg);
  effects.schedule({
    .effect = effects.find(effectName),
    .color = leds.stringToColor(effectColor),
    .arg = effectArg
  });
  scheduleFile.close();
  remove(scheduleFileName);
  throw schedule_event();
}

void dumpHook() {
  if (access(dumpStateFileName, F_OK) == -1)
    return;
  remove(dumpStateFileName);
  FILE* stateFile = fopen(stateFileName, "w");
  char buf[LED_NUMBER + 1];
  for (int i = 0; i < LED_NUMBER; i++)
    buf[leds.getDumpMap()[i]] = leds[i] ? '1' : '0';
  buf[LED_NUMBER] = 0;
  fprintf(stateFile, "%s", buf);
  fclose(stateFile);
}
