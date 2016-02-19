/*
* Authors: Jorden Luke & Tyler Wilson
* Date: 02/16/2016
* Notes: This class is used to hold the data for task. It was used as part of 
* Lab 2 for real time processors.
*/

#include "task.hpp"

//Default Constructor
Task::Task()
{
	id = -1;
	extime = -1;
	period = -1;
  rem_extime = -1;
  deadline = -1;
}

//Constructor
Task::Task(int id_t, int extime_t, int period_t)
{
  id = id_t;
  extime = extime_t;
  period = period_t;
  rem_extime = extime_t;
  deadline = period_t;
} 
int Task::get_id() const
{
  return id;
}
void Task::set_id(int id_t)
{
  id = id_t;
}
int Task::get_extime() const
{
  return extime;
}
void Task::set_extime(int extime_t)
{
  extime = extime_t;
}
int Task::get_period() const
{
  return period;
}
void Task::set_period(int period_t)
{
  period = period_t;
}
int Task::get_rem_extime() const
{
  return rem_extime;
}
void Task::set_rem_extime(int rem_extime_t)
{
  rem_extime = rem_extime_t;
}
int Task::get_deadline() const
{
  return deadline;
}
void Task::set_deadline(int deadline_t)
{
  deadline = deadline_t;
}