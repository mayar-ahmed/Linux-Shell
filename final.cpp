#include<iostream>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <sstream>
#include <string>
#include <cstring>
#include<signal.h>
using namespace std;
int pid = 0;
bool flag = 0;

//----------signal handlers----------------//

void handler(int sig_num) // for SIGINT , do nothing
{
	return;
}

void chandler(int sig_num) //for SIGCHILD , wait for terminated children
{
	while(1) // to catch sigchild of multiple children send it at the same time
	{   
		int status;
		pid_t pid = waitpid(-1,&status,WNOHANG);
		if(pid<=0) //child still alive
		{
			break;
		}
	}
}

//-------------string manipulation-----------------------//

string trim(string str) // remove extra spaces from beginning and end of input before processing it
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
	if(first!=last) //string is not just spaces
    	return str.substr(first, (last-first+1));
	else
		return "";
}

char** parse_command(string s) //takes input string and converts it to an array of arguments
{
	char ** args = new char * [s.size()+1];
	cout<<s.size()+1;
	stringstream ss;
	ss<<s;
	string t; 
	int i=0;
	while (ss>>t)
	{	
		args[i] =new char [t.size()+1];
		copy(t.begin()  , t.end() , args[i]);
		args[i][t.size()] = '\0';
		i++;
		
	}
	args[i] =NULL;
	return args;	
}

int main()
{
	//override signal handlers
	signal(SIGINT,handler);
	signal (SIGCHLD ,chandler);
	
	char machine[HOST_NAME_MAX];				//Variable to store the machine name
	char cwd[1024];						//variable to store the Current Working Directory
	int res;						//Result of the change directory command to check on it
	string input;						//input string from the user 
	char nd[1000];         					//character array of the new directory
	
    // get machine name,  user
	uid_t id = getuid();					//Get the user ID	
	struct passwd *pwd = getpwuid(id);			//Get the struct of the users' info
	char *name = pwd->pw_name;				//Get the user's name
	gethostname(machine,HOST_NAME_MAX);			//Get the nachine name
	

	while(1)	
	{
		 //print current directory
		 if(getcwd(cwd,sizeof(cwd)) != NULL)
		   {  
			 printf("%s@%s:%s$",name,machine,cwd);
		   }
		 else
		  perror("getcwd() error");
		//Getting the commands from the user
		 getline(std::cin,input);
		 //removing extra spaces from the command
		 input = trim(input);
		 //if the user presses Enter only
		 if(input == "")
		 {
			continue;
		 }
		cout<<endl;
			cout<<input<< " "<<input.size();
		 //if command is cd -> change the directory
		if( (int)input.size() >1 && (input[0] == 'c' && input[1] == 'd'))
		{
			stringstream ss;	
			 ss << input;
			 ss >> nd;
			 ss >> nd;
			
		//The default directory is Home unless otherwise is stated
		    if(strcmp(nd, "cd") == 0)
				res=chdir("/home");
		    else
				res = chdir(nd); 
			
		    if(res == -1)
		    {
				perror("MyTerminal error");
		    }
		}
		
		else{ //program or command
			
			//check if it's a background process
			if (input.find("&") != std::string::npos) 
				{	 flag=1;
					 input = input.substr(0,input.size()-1);
				}
				else
					flag=0;

			//parse my input into a list of arguments
			
			char ** arguments = parse_command(input);
					
			pid = fork();
			usleep(100000);

			if (pid == -1)
				perror("error in fork");

		   	else if (pid == 0)
				{ 
					//child ,  change it to the program that I want to execute
					if (execvp (arguments[0] , arguments) <0 )
						printf("no such program or command \n");
					
					exit(errno); //if program doesn't exist termnate child
				}   
			else 	
				{ 
					//parent (my bash)
					if(!flag) // if it's a foreground process wait for it to terminate
					{
						int status;
						waitpid(pid , &status, 0);
					}
				}
		}
		
	}
	return 0;

}
