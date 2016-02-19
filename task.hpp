#ifndef TASK_HPP 
#define TASK_HPP

class Task{

  int id;
  int extime;
  int period;
  int rem_extime;
  int deadline;

public:
  Task();
  Task(int id_t, int extime_t, int period_t); 
  int get_id() const;
  void set_id(int id_t);
  int get_extime() const;
  void set_extime(int extime_t);
  int get_period() const;
  void set_period(int period_t);
  int get_rem_extime() const;
  void set_rem_extime(int rem_extime_t);
  int get_deadline() const;
  void set_deadline(int deadline_t);
};

#endif
