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
bool deadline_cmp(Task const &a, Task const &b);

//Print functions
void printId(Task *, int);
void printPeriod(Task *, int);
void printCost(Task *, int);
void printRMSch(Task *, Schedule *, int, int);
void printEDFSch(Task *, Schedule *, int, int);

//main function
int main()
{
  ifstream systeminfo ("input_file.txt");
  int num_of_tasks = -1;		//The total number of tasks to be scheduled - Determined by an input file.
  int sim_time = -1;        //The total simulation time of the system - Determined by an input file.
  //int hyper_period = -1;    //The hyper_period is used to determine the length of the schedule - WE MIGHT NOT USE THIS
  string input_line;        //Store each line from the input file to allow for parsing.
  Task *tasksetRM;            //A Task pointer that holds the information of all the tasks from the input file
  Task *tasksetEDF;
  Schedule *taskScheduleRM;   //The taskSchedule holds information regarding the operation of tasks at any given time
  Schedule *taskScheduleEDF;
  
  /*******************************************************************************************************************/
  /**************** This section handles the parsing of the input file ***********************************************/
  if (systeminfo.is_open())
  {
    getline(systeminfo, input_line);              //Determine the total number of tasks to be scheduled.
    istringstream(input_line) >> num_of_tasks; 
    getline(systeminfo, input_line);              //Determine the total simulation time.
    istringstream(input_line) >> sim_time;
    tasksetRM = new Task[num_of_tasks];             //Allocate memory for the total number of tasks.
    taskScheduleRM = new Schedule[sim_time];        //Allocate memory for the simulation time schedule.
    tasksetEDF = new Task[num_of_tasks];             //Allocate memory for the total number of tasks.
    taskScheduleEDF = new Schedule[sim_time];        //Allocate memory for the simulation time schedule.

    cout << endl << "Number of tasks: " << num_of_tasks << endl;
    cout << "Total Simulation Time: " << sim_time << endl;

    for (int i=0; i < num_of_tasks; i++)       // Continue to read lines from the input until we reach the end of the file.
    {
      getline(systeminfo, input_line);
      stringstream ss(input_line);
      int temp = 0;

      ss >> temp;
      //cout << "ID: " << temp << endl;
      tasksetRM[i].set_id(temp);        //Set the current task's ID
      tasksetEDF[i].set_id(temp);        //Set the current task's ID

      if(ss.peek() == ',')
        ss.ignore();

      ss >> temp;
      //cout << "Execution Time: " << temp << endl;
      tasksetRM[i].set_extime(temp);      //Set the current task's execution time
      tasksetRM[i].set_rem_extime(temp);  //Set the current task's remaining execution time
      tasksetEDF[i].set_extime(temp);      //Set the current task's execution time
      tasksetEDF[i].set_rem_extime(temp);  //Set the current task's remaining execution time



      if(ss.peek() == ',')
        ss.ignore();

      ss >> temp;
      //cout << "Period: " << temp << endl;
      tasksetRM[i].set_period(temp);    //Set the current task's period
      tasksetRM[i].set_deadline(temp);  //Set the current task's deadline
      tasksetEDF[i].set_period(temp);    //Set the current task's period
      tasksetEDF[i].set_deadline(temp);  //Set the current task's deadline
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

  //Perform RM scheduling
  rm_sch(tasksetRM, num_of_tasks, sim_time, taskScheduleRM);

  //Now perform EDF scheduling
  edf_sch(tasksetEDF, num_of_tasks, sim_time, taskScheduleEDF);

  //delete[] taskSchedule; //Free up the allocated space for the taskSchedule pointer.
  //delete[] taskset;    //Free up the allocated space for the taskset pointer.

  return 1;
}

//Use Rate Monotonic scheduling - This function schedules the task set
void rm_sch(Task *tasks, int numTasks, int simTime, Schedule *taskSch)
{
  cout << endl << "--- Beginning the RM scheduling algorithm ---" << endl;

  //Sort the tasks in the Task array to be sorted in ascending order of their period (RM)
  sort(tasks, tasks + numTasks, period_cmp);
  //cout << "Tasks: " << tasks[0].get_id() << " Deadline: " << tasks[0].get_deadline() << endl;
  //cout << "Tasks: " << tasks[1].get_id() << " Deadline: " << tasks[1].get_deadline() << endl;
  //cout << "Tasks: " << tasks[2].get_id() << " Deadline: " << tasks[2].get_deadline() << endl;

  int curr_entry = 0;
  int waitingQueue[numTasks];

  //Run in a loop for the entire simulation time to create a schedule
  for(int runtime=0; runtime<simTime; runtime++)
  {
    //Add the current runtime to the schedule
    taskSch[runtime].set_curr_time(runtime);

    //Check to see if any deadlines were missed
    int q=0;
    bool e_list = false;      

    while(!e_list)
    {
      //See if the deadline was missed for the current task on the waiting queue
      if(waitingQueue[q] != -1)
      {
        //The deadline was missed
        if(runtime >= tasks[q].get_deadline())
        {
          //cout << "The deadline for task: " << tasks[q].get_id() << " was missed!" << endl;
          tasks[q].set_num_misses(tasks[q].get_num_misses()+1);
          waitingQueue[q] = -1;
          curr_entry = -1;
          taskSch[runtime].set_deadline_missed_task(tasks[q].get_id());
          tasks[q].set_rem_extime(tasks[q].get_extime());
        }
      }
      if(q == (numTasks-1))
      {
        e_list = true;  //The end of the queue was reached
      }
      else
        q++;
    }

    //Add items to the waitinqQueue that are being released at the current runtime
    for(int i=0; i<numTasks; i++)
    {
      if(runtime % tasks[i].get_period() == 0)
      {
        waitingQueue[i] = i;    
        tasks[i].set_deadline(runtime + tasks[i].get_period());     //Update the next deadline for the task
      }  
    }
  
    bool ready_for_execution = false;

    //There is no current task, see if the waiting queue has anything ready to execute
    if(curr_entry == -1)
    {    
      int m=0;
      bool found = false, endlist = false;      

      while(!found && !endlist)
      {
        if(waitingQueue[m] != -1)     //See if anything is on the waiting queue ready to be executed
        {
          curr_entry = m;
          ready_for_execution = true; //Some task is ready to be executed
          found = true;
        }
        else if(m == (numTasks-1))        //If nothing is found on the waiting queue then we have reached the end of the queue
        {
          //cout << "No task is running at this time unit!" << endl;
          taskSch[runtime].set_task_id(-1);
          endlist = true;
        }
        else
          m++;                        //Continue searching through the queue
      }
    }
    else      //The current task still needs execution time - Determine if a preemption will occur 
    {   
      //Determine preemptions
      bool preemption_occured = false;
      ready_for_execution = true; //Some task is ready for execution

      //Check to see if any other tasks are waiting that have higher priority. We must check all the tasks to see if any deadlines
      //are sooner than the current task
      for(int index=0; index < curr_entry; index++)
      {
        if(waitingQueue[index] != -1) //Preemption will occur 
        {
          //cout << "Preemption occured! Task: " << tasks[curr_entry].get_id() << " was preempted!" << endl;        
          tasks[curr_entry].set_num_preemptions(tasks[curr_entry].get_num_preemptions()+1);
          taskSch[runtime].set_preempted_task(tasks[curr_entry].get_id());
          curr_entry = index;  
          preemption_occured = true;
        }
      }
      //Check to see if a preemption occured
      if(!preemption_occured)
      {
        //cout << "No Preemption!" << endl;
        taskSch[runtime].set_preempted_task(0);
      }      
    }

    //Execute for one time unit if we have a current task ready to execute
    if(ready_for_execution)
    {
      //cout << "Task: " << tasks[curr_entry].get_id() << " is executing!" << endl;
      tasks[curr_entry].set_rem_extime(tasks[curr_entry].get_rem_extime()-1);
      taskSch[runtime].set_task_id(tasks[curr_entry].get_id());       //Save the task ID to the schedule that is executing at the given time unit
      //Check to see if the current task finished execution - if so, free it from the waiting queue
      if(tasks[curr_entry].get_rem_extime() == 0)
      {
        //cout << "Task: " << tasks[curr_entry].get_id() << " has finished its execution!" << endl;
        tasks[curr_entry].set_rem_extime(tasks[curr_entry].get_extime());   //Reset the task execution time.
        waitingQueue[curr_entry] = -1;    //The current task is finished and no longer waiting
        curr_entry = -1;                  //There is no current task because it just finished
      }  
    }              
  }

  //Print the RM Schedule
  printRMSch(tasks, taskSch, simTime, numTasks);

  cout << "--- Ending the RM scheduling algorithm ---" << endl;
}

//Use Earliest Deadline First scheduling - This function schedules the task set
void edf_sch(Task *tasks, int numTasks, int simTime, Schedule *taskSch)
{
  cout << endl << "--- Beginning the EDF scheduling algorithm ---" << endl;
  
  //Sort the tasks in the Task array to be sorted in ascending order of their earliest deadline (EDF)
  sort(tasks, tasks + numTasks, deadline_cmp);
  //cout << "Tasks: " << tasks[0].get_id() << " Deadline: " << tasks[0].get_deadline() << endl;
  //cout << "Tasks: " << tasks[1].get_id() << " Deadline: " << tasks[1].get_deadline() << endl;
  //cout << "Tasks: " << tasks[2].get_id() << " Deadline: " << tasks[2].get_deadline() << endl;
  int curr_entry = 0;
  int waitingQueue[numTasks];

  //Run in a loop for the entire simulation time to create a schedule
  for(int runtime=0; runtime<simTime; runtime++)
  {
    //Add the current runtime to the schedule
    taskSch[runtime].set_curr_time(runtime);

    //Check to see if any deadlines were missed
    int q=0;
    bool e_list = false;      

    while(!e_list)
    {
      //See if the deadline was missed for the current task on the waiting queue
      if(waitingQueue[q] != -1)
      {
        //The deadline was missed
        if(runtime >= tasks[q].get_deadline())
        {
          //cout << "The deadline for task: " << tasks[q].get_id() << " was missed!" << endl;
          tasks[q].set_num_misses(tasks[q].get_num_misses()+1);
          waitingQueue[q] = -1;
          curr_entry = -1;
          taskSch[runtime].set_deadline_missed_task(tasks[q].get_id());
          tasks[q].set_rem_extime(tasks[q].get_extime());
        }
      }
      if(q == (numTasks-1))
      {
        e_list = true;  //The end of the queue was reached
      }
      else
        q++;
    }

    //Add items to the waitinqQueue that are being released at the current runtime
    for(int i=0; i<numTasks; i++)
    {
      if(runtime % tasks[i].get_period() == 0)
      {
        waitingQueue[i] = i;    
        tasks[i].set_deadline(runtime + tasks[i].get_period());     //Update the next deadline for the task
      }  
    }
  
    bool ready_for_execution = false;

    //There is no current task, see if the waiting queue has anything ready to execute
    if(curr_entry == -1)
    {    
      int m=0;
      bool found = false, endlist = false;      

      while(!found && !endlist)
      {
        if(waitingQueue[m] != -1)     //See if anything is on the waiting queue ready to be executed
        {
          curr_entry = m;
          ready_for_execution = true; //Some task is ready to be executed
          found = true;
        }
        else if(m == (numTasks-1))        //If nothing is found on the waiting queue then we have reached the end of the queue
        {
          //cout << "No task is running at this time unit!" << endl;
          taskSch[runtime].set_task_id(-1);
          endlist = true;
        }
        else
          m++;                        //Continue searching through the queue
      }
    }
    else      //The current task still needs execution time - Determine if a preemption will occur 
    {   
      //Determine preemptions
      bool preemption_occured = false;
      ready_for_execution = true; //Some task is ready for execution

      //Check to see if any other tasks are waiting that have higher priority. The tasks are ordered by priority so we only need to check up to the current task
      for(int index=0; index < numTasks; index++)
      {
        if((waitingQueue[index] != -1) && (tasks[curr_entry].get_deadline()>tasks[index].get_deadline())) //Preemption will occur
        {
          //cout << "Preemption occured! Task: " << tasks[curr_entry].get_id() << " was preempted!" << endl;        
          tasks[curr_entry].set_num_preemptions(tasks[curr_entry].get_num_preemptions()+1);
          taskSch[runtime].set_preempted_task(tasks[curr_entry].get_id());
          curr_entry = index;  
          preemption_occured = true;
        }
      }
      //Check to see if a preemption occured
      if(!preemption_occured)
      {
        //cout << "No Preemption!" << endl;
        taskSch[runtime].set_preempted_task(0);
      }      
    }

    //Execute for one time unit if we have a current task ready to execute
    if(ready_for_execution)
    {
      //cout << "Task: " << tasks[curr_entry].get_id() << " is executing!" << endl;
      tasks[curr_entry].set_rem_extime(tasks[curr_entry].get_rem_extime()-1);
      taskSch[runtime].set_task_id(tasks[curr_entry].get_id());       //Save the task ID to the schedule that is executing at the given time unit
      //Check to see if the current task finished execution - if so, free it from the waiting queue
      if(tasks[curr_entry].get_rem_extime() == 0)
      {
        //cout << "Task: " << tasks[curr_entry].get_id() << " has finished its execution!" << endl;
        tasks[curr_entry].set_rem_extime(tasks[curr_entry].get_extime());   //Reset the task execution time.
        waitingQueue[curr_entry] = -1;    //The current task is finished and no longer waiting
        curr_entry = -1;                  //There is no current task because it just finished
      }  
    }              
  }

  //Print the EDF schedule
  printEDFSch(tasks, taskSch, simTime, numTasks);

  cout << "--- Ending the EDF scheduling algorithm ---" << endl;
}

//Compare the periods of the different tasks to organize them in ascending order
bool period_cmp(const Task &a, const Task &b)
{
  return a.get_period() < b.get_period();
}

//Compare the deadlines of the different tasks to organize them in ascending order
bool deadline_cmp(const Task &a, const Task &b)
{
  return a.get_deadline() < b.get_deadline();
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

//Print the RM schedule to the console
void printRMSch(Task *tasks, Schedule *taskSch, int simTime, int numTasks)
{
  ofstream outfile;
  outfile.open("RM_Schedule.csv");

  //************PRINT THE SCHEDULE INFO TO THE CONSOLE AND FILE******************************//
  cout << endl << endl << "Time\t|Task ID\t|Preempted Task\t|Deadline Miss\t" << endl;
  outfile << "Time,Task ID,Preempted Task,Deadline Miss\n";
  cout << "-----------------------------------------------------------" << endl;

  for(int p=0; p<simTime; p++)
  {
    cout << taskSch[p].get_curr_time() << "\t|\t" << taskSch[p].get_task_id() << "\t|\t" << taskSch[p].get_preempted_task() << "\t|\t" << taskSch[p].get_deadline_missed_task() << endl;
    outfile << taskSch[p].get_curr_time() << "," << taskSch[p].get_task_id() << "," << taskSch[p].get_preempted_task() << "," << taskSch[p].get_deadline_missed_task() << "\n";
    cout << "-----------------------------------------------------------" << endl;
  }

  cout << endl << endl << "Task ID\t|Num Preempts\t|Num Misses" << endl;
  outfile << "\n\n" << "Task ID,Num Preempts,Num Misses\n";
  cout << "------------------------------------------" << endl;

  int tot_miss = 0, tot_preempt = 0;

  for(int b=0; b<numTasks; b++)
  {
    cout << tasks[b].get_id() << "\t|\t" << tasks[b].get_num_preemptions() << "\t|\t" << tasks[b].get_num_misses() << endl;
    outfile << tasks[b].get_id() << "," << tasks[b].get_num_preemptions() << "," << tasks[b].get_num_misses() << "\n";
    tot_miss += tasks[b].get_num_misses();
    tot_preempt += tasks[b].get_num_preemptions();
  }
  cout << "------------------------------------------" << endl;
  cout << "Total\t|\t" << tot_preempt << "\t|\t" << tot_miss << endl << endl;
  outfile << "Total," << tot_preempt << "," << tot_miss << "\n\n";
  //**********FINISHED PRINTING SCHEDULE INFO***************************************//
  outfile.close();
}

//Print the EDF schedule to the console
void printEDFSch(Task *tasks, Schedule *taskSch, int simTime, int numTasks)
{
  ofstream outfile;
  outfile.open("EDF_Schedule.csv");

  //************PRINT THE SCHEDULE INFO TO THE CONSOLE******************************//
  cout << endl << endl << "Time\t|Task ID\t|Preempted Task\t|Deadline Miss\t" << endl;
  outfile << "Time,Task ID,Preempted Task,Deadline Miss\n";
  cout << "-----------------------------------------------------------" << endl;

  for(int p=0; p<simTime; p++)
  {
    cout << taskSch[p].get_curr_time() << "\t|\t" << taskSch[p].get_task_id() << "\t|\t" << taskSch[p].get_preempted_task() << "\t|\t" << taskSch[p].get_deadline_missed_task() << endl;
    outfile << taskSch[p].get_curr_time() << "," << taskSch[p].get_task_id() << "," << taskSch[p].get_preempted_task() << "," << taskSch[p].get_deadline_missed_task() << "\n";
    cout << "-----------------------------------------------------------" << endl;
  }

  cout << endl << endl << "Task ID\t|Num Preempts\t|Num Misses" << endl;
  outfile << "\n\n" << "Task ID,Num Preempts,Num Misses\n";
  cout << "------------------------------------------" << endl;

  int tot_miss = 0, tot_preempt = 0;

  for(int b=0; b<numTasks; b++)
  {
    cout << tasks[b].get_id() << "\t|\t" << tasks[b].get_num_preemptions() << "\t|\t" << tasks[b].get_num_misses() << endl;
    outfile << tasks[b].get_id() << "," << tasks[b].get_num_preemptions() << "," << tasks[b].get_num_misses() << "\n";
    tot_miss += tasks[b].get_num_misses();
    tot_preempt += tasks[b].get_num_preemptions();
  }
  
  cout << "------------------------------------------" << endl;
  cout << "Total\t|\t" << tot_preempt << "\t|\t" << tot_miss << endl << endl;
  outfile << "Total," << tot_preempt << "," << tot_miss << "\n\n";
  //**********FINISHED PRINTING SCHEDULE INFO***************************************//
  outfile.close();
}