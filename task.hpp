#ifndef TASK_HPP 
#define TASK_HPP

class Task{

  int id;
  int extime;
  int period;

public:
  Task();
  Task(int id_t, int extime, int period); 
  int get_id();
  void set_id(int id_t);
  int get_extime();
  void set_extime(int time);
  int get_period();
  void set_period(int period_t);
};

#endif