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
Task::Task(int id_t, int time, int period_t)
{
  id = id_t;
  extime = time;
  period = period_t;
} 
int Task::get_id()
{
  return id;
}
void Task::set_id(int id_t)
{
  id = id_t;
}
int Task::get_extime()
{
  return extime;
}
void Task::set_extime(int time)
{
  extime = time;
}
int Task::get_period()
{
  return period;
}
void Task::set_period(int period_t)
{
  period = period_t;
}
bool Task::period_cmp(const Task &a, const Task &b)
{
  return a.get_period() < b.get_period();
}
