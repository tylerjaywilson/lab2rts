#include <iostream>
#include <fstream>
#include "task.hpp"
#include <string>
#include <stdlib.h>
#include <sstream>
#include <algorithm> 

using namespace std;

int main()
{
  ifstream systeminfo ("input_file.txt");
  int num_of_tasks = -1;		//The total number of tasks to be scheduled - Determined by an input file.
  int sim_time = -1;        //The total simulation time of the system - Determined by an input file.
  string input_line;        //Store each line from the input file to allow for parsing.
  Task *taskset;
  
  /*******************************************************************************************************************/
  /**************** This section handles the parsing of the input file ***********************************************/
  if (systeminfo.is_open())
  {
    getline(systeminfo, input_line);              //Determine the total number of tasks to be scheduled.
    istringstream(input_line) >> num_of_tasks; 
    getline(systeminfo, input_line);              //Determine the total simulation time.
    istringstream(input_line) >> sim_time;
    taskset = new Task[num_of_tasks];             //Allocate memory for the total number of tasks.

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
      taskset[i].set_extime(temp);    //Set the current task's execution time

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
  
  

  for (int i=0; i<num_of_tasks; i++)
  {
    cout<<"ID: "<<taskset[i].get_id()<<endl;
  }


  delete [] taskset;    //Free up the allocated space for the taskset pointer.
  return 1;
}
