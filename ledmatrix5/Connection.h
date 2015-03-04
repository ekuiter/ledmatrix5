#ifndef CONNECTION_H
#define CONNECTION_H

class Connection {
  public:
    Connection(Effects& effects);
    void setup();
    void receive();
  private:
    Effects& effects;
    String command;
    void process();
    int getEffectIndex(int& effectIndex);
    Color getColor(int idx);
};

#endif
