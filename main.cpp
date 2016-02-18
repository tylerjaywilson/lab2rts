/*
* Authors: Jorden Luke & Tyler Wilson
* Date: 02/16/2016
* Notes: This is used to ren the main operation of the the algorithms
*/
#include <iostream>
#include <fstream>
#include "task.hpp"
#include "schedule.hpp"
#include <string>
#include <stdlib.h>
#include <sstream>
#include <algorithm> 

using namespace std;

//Functions used to for scheduling input tasks
void rm_sch(Task *, int, int, Schedule *);
void edf_sch(Task *, int, int, Schedule *);
void edf_sch(Task *, int, int);
bool period_cmp(Task const &a, Task const  &b);

//Print functions
void printId(Task *, int);
void printPeriod(Task *, int);
void printCost(Task *, int);

//main function
int main()
{
  ifstream systeminfo ("input_file.txt");
  int num_of_tasks = -1;		//The total number of tasks to be scheduled - Determined by an input file.
  int sim_time = -1;        //The total simulation time of the system - Determined by an input file.
  //int hyper_period = -1;    //The hyper_period is used to determine the length of the schedule - WE MIGHT NOT USE THIS
  string input_line;        //Store each line from the input file to allow for parsing.
  Task *taskset;            //A Task pointer that holds the information of all the tasks from the input file
  Schedule *taskSchedule;   //The taskSchedule holds information regarding the operation of tasks at any given time
  
  /*******************************************************************************************************************/
  /**************** This section handles the parsing of the input file ***********************************************/
  if (systeminfo.is_open())
  {
    getline(systeminfo, input_line);              //Determine the total number of tasks to be scheduled.
    istringstream(input_line) >> num_of_tasks; 
    getline(systeminfo, input_line);              //Determine the total simulation time.
    istringstream(input_line) >> sim_time;
    taskset = new Task[num_of_tasks];             //Allocate memory for the total number of tasks.
    taskSchedule = new Schedule[sim_time];        //Allocate memory for the simulation time schedule.

    cout << "Number of tasks: " << num_of_tasks << endl;
    cout << "Total Simulation Time: " << sim_time << endl;

    for (int i=0; i < num_of_tasks; i++)       // Continue to read lines from the input until we reach the end of the file.
    {
      getline(systeminfo, input_line);
      stringstream ss(input_line);
      int temp = 0;

      ss >> temp;
      cout << "ID: " << temp << endl;
      taskset[i].set_id(temp);        //Set the current task's ID

      if(ss.peek() == ',')
        ss.ignore();

      ss >> temp;
      cout << "Execution Time: " << temp << endl;
      taskset[i].set_extime(temp);      //Set the current task's execution time
      taskset[i].set_rem_extime(temp);  //Set the current task's remaining execution time

      if(ss.peek() == ',')
        ss.ignore();

      ss >> temp;
      cout << "Period: " << temp << endl;
      taskset[i].set_period(temp);    //Set the current task's period

    }
    systeminfo.close();
  }
  else
  {
  	cout << "Unable to open the input file!";
  	return 0;
  }
    
  /*******************************************************************************************************************/
  /**************** This ends the section handling the parsing of the input file *************************************/

  printId(taskset, num_of_tasks);
  //Perform RM Scheduling
  rm_sch(taskset, num_of_tasks, sim_time, taskSchedule);
  printId(taskset, num_of_tasks);

  delete [] taskSchedule; //Free up the allocated space for the taskSchedule pointer.
  delete [] taskset;    //Free up the allocated space for the taskset pointer.
  return 1;
}

//Use Rate Monotonic scheduling - This function schedules the task set
void rm_sch(Task *tasks, int numTasks, int simTime, Schedule *taskSch)
{
  cout << "--- Beginning the RM scheduling algorithm ---" << endl;

  //Sort the tasks in the Task array to be sorted in ascending order of their period (RM)
  sort(tasks, tasks + numTasks, period_cmp);

  int curr_entry = 0;
  int waitingQueue[numTasks];

  //Run in a loop for the entire simulation time to create a schedule
  for(int runtime=0; runtime<simTime; runtime++)
  {
    //First - check to see if another task is waiting to be scheduled
    //Second - If another task is ready to be scheduled, does it have higher priority (smaller period)?
    //Third - If so, preempt the current task and add the current task to the waiting queue.
    //         If not, continue execution of the current task. Add the new task in the waiting queue
    
    //Add items to the waitinqQueue that are being released at the current runtime
    for(int i=0; i<numTasks; i++)
    {
      if(runtime % tasks[i].get_period() == 0)
        waitingQueue[i] = i;
    }

    cout << "waiting 1: " << waitingQueue[0] << endl;
    cout << "waiting 2: " << waitingQueue[1] << endl;
    cout << "waiting 3: " << waitingQueue[2] << endl;


    //Check to see if any tasks have a higher priority than the current task
    bool possible_preemption = true;  //Preemption can occur if a task is currently using the CPU
    //There is no current task, see if the waiting queue has anything ready to execute
    if(curr_entry == -1)
    {        
      int m=0;
      bool found, endlist = false;
      possible_preemption = false;    //Preemption cannot occur. The CPU is free.

      while(!found && !endlist)
      {
        if(waitingQueue[m] != -1)
        {
          curr_entry = m;
          found = true;
        }
        else if(m == numTasks)
          endlist = true;
        else
          m++;
      }
    }

    
    if(curr_entry != -1)
    {
      //Preemption occurs and a new task begins running. Old task state is saved.
      if((tasks[l].get_period() < tasks[curr_entry].get_period()) && (waitingQueue[l] != -1) && (possible_preemption))
      { 
        cout << "Preemption occured!" << endl;        
        taskSch[runtime].set_preempted_task(tasks[curr_entry].get_id());
        curr_entry = l;  
      } 
      else
      {
        cout << "No preemption!" << endl;  
        taskSch[runtime].set_preempted_task(-1);  //No preemption occured.
      }
     
      //Execute one time unit for the current task and save the information to the schedule
      tasks[curr_entry].set_rem_extime((tasks[curr_entry].get_rem_extime()-1));         //Take one time unit off the execution time remaining
      //taskSch[runtime].set_curr_time(runtime);
      taskSch[runtime].set_task_id(tasks[curr_entry].get_id());        
    }
    else
    {
      cout << "No task is running at this time unit!" << endl;
    }  
    
    //Check to see if the current task finished execution - if so, free it from the waiting queue
    if(tasks[curr_entry].get_rem_extime() == 0)
    {
      cout << "The task finished its execution!" << endl;
      tasks[curr_entry].set_rem_extime(tasks[curr_entry].get_extime());   //Reset the task execution time.
      waitingQueue[curr_entry] = -1;
      curr_entry = -1;                  //There is no current task because it just finished
   }        
  }

  for(int p=0; p<simTime; p++)
  {
    cout << taskSch[p].get_curr_time() << "\t" << taskSch[p].get_task_id() << "\t" << taskSch[p].get_preempted_task() << endl;
  }

  cout << "--- Ending the RM scheduling algorithm ---" << endl;
}

//Use Earliest Deadline First scheduling - This function schedules the task set
void edf_sch(Task *tasks, int numTasks, int simTime, Schedule *taskSch)
{
  cout << "--- Beginning the RM scheduling algorithm ---" << endl;

  //Sort the tasks in the Task array to be sorted in ascending order of their period (RM)
  sort(tasks, tasks + numTasks, period_cmp);


  cout << "--- Ending the RM scheduling algorithm ---" << endl;
}

//Compare the periods of the different tasks to organize them in ascending order
bool period_cmp(const Task &a, const Task &b)
{
  return a.get_period() < b.get_period();
}

//Print the ID of each task
void printId(Task *tasks, int numTasks)
{
  for(int i = 0; i < numTasks; i ++ )
  {
    cout << tasks[i].get_id() << endl;    
  }
}

//Print the Period of each task
void printPeriod(Task *tasks, int numTasks)
{
  for(int i = 0; i < numTasks; i ++ )
  {
    cout << tasks[i].get_period() << endl;    
  }
}

//Print the Execution time of each task
void printCost(Task *tasks, int numTasks)
{
  for(int i = 0; i < numTasks; i ++ )
  {
    cout << tasks[i].get_extime() << endl;    
  }
}