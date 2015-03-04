#ifndef CONNECTION_H
#define CONNECTION_H

/*
  Connection
  This class manages the communication with Linino.
  It receives new commands, schedules them and switches modes.
*/

class Connection {
  public:
    Connection(Effects& effects);
    void setup(); // this takes some time
    void receive(); // receives a new command if available
  private:
    Effects& effects;
    String command; // the current command
    void process(); // schedules a newly received command
    // returns an effect index from a command, e.g. "1,2" => 1
    int getEffectIndex(int& effectIndex);
    // returns a color from a command, e.g. "1,2" => GREEN
    Color getColor(int idx);
};

#endif
