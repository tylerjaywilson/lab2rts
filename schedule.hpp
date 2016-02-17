#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

class Schedule{

  int curr_time;
  int task_id;
  int preempted_task;
  int deadline_missed_task;

public:
  Schedule();
  Schedule(int curr_time_t, int task_id_t, int preempted_task_t, int deadline_missed_task_t); 
  int get_curr_time() const;
  void set_curr_time(int curr_time_t);
  int get_task_id() const;
  void set_task_id(int task_id_t);
  int get_preempted_task() const;
  void set_preempted_task(int preempted_task_t);
  int get_deadline_missed_task() const;
  void set_deadline_missed_task(int deadline_missed_task_t);
};

#endif