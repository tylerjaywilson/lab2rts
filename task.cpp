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
}

//Constructor
Task::Task(int id_t, int extime_t, int period_t)
{
  id = id_t;
  extime = extime_t;
  period = period_t;
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