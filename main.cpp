#include <iostream>
#include <fstream>
#include "task.hpp"
#include <string>
#include <stdlib.h>

using namespace std;

int main()
{
  ifstream systeminfo ("input_file.txt");
  int num_of_tasks = -1;		//The total number of tasks to be scheduled - Determined by an input file.
  int sim_time = -1;        //The total simulation time of the system - Determined by an input file.
  string input_line;        //Store each line from the input file to allow for parsing.
  Task *taskset;

  if (systeminfo.is_open())
  {
    getline(systeminfo, input_line);   //Determine the total number of tasks to be scheduled.
    getline(systeminfo, input_line);       //Determine the total simulation time.
    taskset = new Task[num_of_tasks];                 //Allocate memory for the total number of tasks.

    for (int i=0; i < num_of_tasks; i++)       // Continue to read lines from the input until we reach the end of the file.
    {

    }
    systeminfo.close();
  }
  else
  {
  	cout << "Unable to open the input file!";
  	return 0;
  }
  
  
  delete [] taskset;    //Free up the allocated space for the taskset pointer.
  return 1;
}
