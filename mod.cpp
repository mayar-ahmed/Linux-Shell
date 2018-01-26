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
int pid = -1;
bool flag = 0;
char machine[HOST_NAME_MAX];
 char user[LOGIN_NAME_MAX];  
 char cwd[1024];

string trim(string str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
	if(first!=last) //string is not just spaces
    	return str.substr(first, (last-first+1));
	else
		return "";
}
void handler(int sig_num)
{
return;
}

void chandler(int sig_num)
{
//   int status; 
  // waitpid(-1 , &status, WNOHANG);
while(1)
{   
	int status;
	pid_t pid = waitpid(-1,&status,WNOHANG);
	if(pid<=0) //child still alive
	{
	break;
	}
}
}
char** parse_command(string s)
{
	char ** args = new char * [s.size()+1];
	stringstream ss;
	ss<<s;
	string t; 
	int i=0;
	while (ss>>t)
	{	
		args[i] =new char [t.size()+1];
		copy(t.begin()  , t.end() , args[i]);
		args[i][t.size()] = '\0';
		//cout<<args[i]<<endl;
		i++;
		
	}
	args[i] =NULL;
	return args;	
}

int main()
{

uid_t id = getuid();
struct passwd *pwd = getpwuid(id);
char *name = pwd->pw_name;
//char *dir = pwd->pw_dir;
//printf("%s %s:\n",name,dir);

 signal(SIGINT,handler);
signal (SIGCHLD ,chandler);
 int res;
 
 string s;
 stringstream ss;
 char tmp[1000];


 gethostname(machine,HOST_NAME_MAX);
 getlogin_r(user,LOGIN_NAME_MAX);

while(1)	
{
	 if(getcwd(cwd,sizeof(cwd)) != NULL)
	   {  
		 printf("%s@%s:%s ",name,machine,cwd);
	   }
	 else
	  perror("getcwd() error");

	 getline(std::cin,s);
	 s = trim(s);
	 
	 if(s == "")
	 {
	
		continue;
	 }
	 
	if(s[0] == 'c' && s[1] == 'd')
	{
		stringstream ss2;
		 
		 ss2 << s;
		ss2 >> tmp;
		
		 ss2 >> tmp;
		
	
	  if(strcmp(tmp, "cd") == 0)
		 res=chdir("/home");
	  else
	  	res = chdir(tmp); 
		
	  if(res == -1)
	  {
		perror("MyTerminal error");
	  }
	}
	else{


if (s.find("&") != std::string::npos) 
	{	 flag=1;
		 s = s.substr(0,s.size()-1);
	}
	else
		flag=0;

	  char ** arguments = parse_command(s);
      	
	 
		
	 pid = fork();
        usleep(100000);

	  if (pid == -1)
		perror("error in fork");

	  else if (pid == 0)
		{ 
			//child
			//printf("\nI am the child, my pid = %d and my parent's pid = %d\n\n", getpid(), getppid());	
			
			
			if (execvp (arguments[0] , arguments) <0 )
				printf("no such program\n");
			exit(errno);
		}   
	  else 	
		{
			if(!flag)
			{
				int status;
				waitpid(pid , &status, 0);
				//flag = 0;
			}

			//=0;
//		    printf("\nI am the parent, my pid = %d and my parent's pid = %d\n\n", getpid(), getppid());	 
			//pid = -1;
		 }
	 
		

	}
    
}
return 0;

}
