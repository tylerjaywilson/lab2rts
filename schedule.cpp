/*
* Authors: Jorden Luke & Tyler Wilson
* Date: 02/16/2016
* Notes: This class is used to hold the schedule information. It was used as part of 
* Lab 2 for real time processors.
*/

#include "schedule.hpp"

//Default Constructor
Schedule::Schedule()
{
	curr_time = -1;
  task_id = -1;
  preempted_task = 0;
  deadline_missed_task = 0;
}

//Constructor
Schedule::Schedule(int curr_time_t, int task_id_t, int preempted_task_t, int deadline_missed_task_t)
{
  curr_time = curr_time_t;
  task_id = task_id_t;
  preempted_task = preempted_task_t;
  deadline_missed_task = deadline_missed_task_t;
} 
int Schedule::get_curr_time() const
{
  return curr_time;
}
void Schedule::set_curr_time(int curr_time_t)
{
  curr_time = curr_time_t;
}
int Schedule::get_task_id() const
{
  return task_id;
}
void Schedule::set_task_id(int task_id_t)
{
  task_id = task_id_t;
}
int Schedule::get_preempted_task() const
{
  return preempted_task;
}
void Schedule::set_preempted_task(int preempted_task_t)
{
  preempted_task = preempted_task_t;
}
int Schedule::get_deadline_missed_task() const
{
  return deadline_missed_task;
}
void Schedule::set_deadline_missed_task(int deadline_missed_task_t)
{
  deadline_missed_task = deadline_missed_task_t;
}