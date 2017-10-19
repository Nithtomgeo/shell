
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<signal.h>
#include<errno.h>
#include<dirent.h>

/*funtion declarations*/
void builtin(char*,char*);
void pipefn(char**,int,int*,int*);
void fork_external(char**,int,int*,int*,char*,char*,int,char*);
void redirection(char**,int,char*,char*,int,char*);
void listexternal(char**,int);
void background(char**,int);

/*variable declarations*/
char pat[100];
char words[80][80],words1[80][80],temp[80],temp1[80],words2[80][80],words3[80][80],words4[80][80],words5[80][80],path[100],envimt[100][100];
char *input2,*input3,*input10,*buffer;
long capacity;
int c,i,q,y;
int pipeval,forkcount;
int f2;
int child_status;
int bakflag,fla;
pid_t child,child1,co,bakground=-1;

extern char ** environ;

int main(int argv,char *args[],char *envp[])
{
	char command[80],ten[80],*argument[80],*argument1[80],*argument2[80];
	char *input1,*input,*input4,*input5,*input6,*input7,*input8,*input9;
	int c,p,t,r,o,k,x,e;
	int loop,pipflag,extflag,redflag1,redflag2,length;
	int fd[2],fd1[2],f1;
	char let[10];
	while(1)
	{	pipflag = 0;extflag = 0;pipeval =0,bakflag = 0;fla =1;				//setting flag variables to 0
		redflag1=0,redflag2=0;strcpy(temp1,"");
		forkcount =0;child = 0;
		for(loop =0 ;loop<10;loop++)
		{
			strcpy(words[loop],"");
			strcpy(words1[loop],"");
			argument[loop]=NULL;
		}
		strcpy(command,"");
		fflush(stdin);
		printf("($) ");
		fgets(command,sizeof(command),stdin);					//accepting input from the user
		if(feof(stdin))
			exit(0);

		input1 = strtok(command,"\n");
		input4 = strtok(command,";");
		p = 0;
		while(input4 != NULL)							//separating commands given by commas
		{
			strcpy(words[p],input4);
			p++;

			if(p == 2)
				extflag = 1;
			input4 = strtok(NULL,";");
		}
		q = 0;
		input5 = strtok(command,"|");
		while(input5 !=NULL)							//separating commands given by pipes
		{
			strcpy(words1[q],input5);
			q++;

			if(q == 2)
				pipflag = 1;
			input5 = strtok(NULL,"|");
		}
		if((pipflag != 1)&&(extflag !=1))
		{
			length = strlen(command);
			strcpy(ten,command);
			if(command[length-1]=='&')
			{
				bakflag = 1;
				strcpy(ten,command);
				strtok(ten,"&");
			}

			o = 0;
                        input7 = strtok(ten,"<");
                        while(input7 != NULL)
                        {
                                strcpy(words3[o],input7);
                                o++;
                                if(o == 2)
                                {let[0] = '<',redflag2 = 1;				//separating commands given by redirection operators
                                        input8 = strtok(words3[o-1],">");
                                        k = 0;
                                        while(input8 != NULL)
                                        {
                                                strcpy(words4[k],input8);
                                                k++;
                                                input8 = strtok(NULL,">");

						if(k == 2)
                                                	strcpy(temp1,words4[1]);
                                        }
                                }
                                input7 = strtok(NULL,"<");
                        }
			if(strcmp(temp1,"")==0)
			{
			r = 0;
			input6 = strtok(ten,">");
			while(input6 != NULL)
			{
				strcpy(words2[r],input6);
				r++;
				if(r == 2)
				{
					let[0] = '>';
					redflag1 = 1;
				}
				input6 = strtok(NULL,">");
			}
			}
		}

		length = strlen(command);
		if(command[length-1]=='&')						//for background commands, seaprating & from them.
		{
			input9 = strtok(command,"&");
			x=0;
		while(input9 != NULL)
		{
			strcpy(words5[x],input9);
			x++;
			if(x==1)
				bakflag = 1;
			input9=strtok(NULL,"&");
		}
		}

		for(t=0;t<p||t<q||t<r||t<o||t<x;t++)
		{
		if(pipflag == 1)
			input  = strtok(words1[t]," ");					//removing the extra spaces from the commands
		else if(redflag1 == 1)
			input = strtok(words2[t]," ");
		else if(redflag2 == 1)
			input = strtok(words3[t]," ");
		else if(bakflag == 1)
			input = strtok(words5[t]," ");
		else
			input  = strtok(words[t]," ");
		c = 0;

		if(input == NULL)
		break;

		while(input!=NULL)
		{
			argument[c]=input;
			c++;
			input = strtok(NULL," ");
		}argument[c] = NULL;

		// checking  for internal commands exit, cd, pwd, set and calling the builtin function
		if((strcmp(argument[0],"exit")==0)||(strcmp(argument[0],"cd")==0)||(strcmp(argument[0],"pwd")==0)||(strcmp(argument[0],"set")==0))
		{
			if(strcmp(argument[0],"cd")==0)
			{
				if(argument[1] == 0)
				{
					fprintf(stderr,"No arguments specified\n");
					break;
				}
			}
			builtin(argument[0],argument[1]);
		}
		else if(pipflag == 1)				//calling the pipefn fucntion for pipe commands
		{
			pipefn(argument,2,fd,fd1);
		}
		else if((redflag1 == 1) || (redflag2 == 1))
		{
			break;
		}
		else if(strcmp(argument[0],"myls")==0)		//calling the listexternal function for myls command.
		{
			listexternal(argument,4);
		}
		else if(bakflag == 1)				//calling the background function.
		{
			background(argument,5);
		}
		else
					//for other external commands calling the fork_external function.
		{
			fork_external(argument,1,NULL,NULL,NULL,NULL,0,NULL);
		}
		}
		if(redflag1 == 1)
		{
			strcpy(temp,words2[1]);strtok(temp," ");redirection(argument,3,temp,let,f1,temp1);
		}
		else if(redflag2 == 1)
		{
			strcpy(temp,words3[1]);strtok(temp1," ");redirection(argument,3,temp,let,f1,temp1);
		}
	}
	return 0;
}

/*fucntion for built in commands starts here*/
void builtin(char *commd,char *argmt)
{
	if(strcmp(commd,"exit")==0)				//implementing exit command
		exit(0);

	else if(strcmp(commd,"cd")==0)				//implementing cd command
	{
		strcpy(path,argmt);
		if(chdir(argmt)==-1)
		{
			perror("Invalid argument");
		}
		else if(argmt[0]!='/')
		{
			capacity = pathconf(".",_PC_PATH_MAX);
			if((buffer = (char *)malloc((size_t)capacity))!=NULL)
			{
				if(!getcwd(buffer,(size_t)capacity))
					perror("Error");
				strcat(buffer,"/");
				strcat(buffer,path);
				chdir(buffer);
			}
		}
		else
		{
			chdir(path);
		}
	}

	else if(strcmp(commd,"pwd")==0)				//implementing pwd command
	{
		capacity = pathconf(".", _PC_PATH_MAX);
		if((buffer = (char *)malloc((size_t)capacity))!=NULL)
		{
			if(!getcwd(buffer,(size_t)capacity))
				perror("Error");
			fprintf(stderr,"%s\n",buffer);
		}
	}

	else if(strcmp(commd,"set")==0)				//implementing set command for MYPATH variable
	{
		input3 = strtok(argmt,"=");
		c = 0;
		while(input3!=NULL)
		{
			strcpy(words1[c],input3);
			c++;
			input3 = strtok(NULL,"=");
		}
				i = 0;
		setenv(words1[0],words1[1],1);
		fprintf(stderr,"%s=%s\n",words1[0],getenv(words1[0]));
		strcpy(pat,getenv(words1[0]));
	}
}

/*forking processes, main functionalities of all commands and execvp operations are done here
  status =1 -> normal external command
  status =2 -> pipes command implmentation
  status =3 -> redirection operation implementation.
  status =4 -> myls operation implementation.
  status =5 -> background operation implementation.
*/
void fork_external(char ** argum,int  status,int *fd,int *fd1,char *argum1, char *let,int f1,char *temp1)
{
	if(status != 1)
		forkcount ++;

	child = fork();
	if(child == 0)
	{
		if(status == 1)					//for normal external command 
		{
			execvp(argum[0],argum);
			fprintf(stderr,"Enter a command/Invalid command\n");
			exit(1);
		}
		else if(status == 2)				//pipe command  implementation.
		{
			if((forkcount % 2) != 0)
			{
				if(forkcount == 1)
				{
					close(fd[0]);
					close(STDOUT_FILENO);dup2(fd[1],1);
					if((execvp(argum[0],argum))==-1)
						perror("Unable to execue command at 1");
					exit(1);
				}
				else
				{
					close(fd1[1]);
					close(STDIN_FILENO);dup2(fd1[0],0);
					if((execvp(argum[0],argum))== -1)
						perror("Unable to execute command");
					exit(1);
				}
			}
			else
			{
				close(fd[1]);
				close(STDIN_FILENO);dup2(fd[0],0);
				if((fd1[0]>0)&&(fd1[1]>0))
				{
					close(fd1[0]);
					close(STDOUT_FILENO);dup2(fd1[1],1);
				}
				execvp(argum[0],argum);
				perror("Unable to execute command");
				exit(1);
			}
		}
		else if(status  == 3)					//redirection operation implementation.
		{
			if(let[0]=='>')
			{
				close(STDOUT_FILENO);
				f1 = open(argum1,O_RDWR|O_CREAT|O_TRUNC,0777);
				dup(f1);
				execvp(argum[0],argum);
				perror("Unable to execute command");
				exit(1);
			}
			else if(let[0]=='<')
			{
				f1 = open(argum1,O_RDONLY);
				dup2(f1,STDIN_FILENO);
				close(f1);
				if(strcmp(temp1,"")!=0)
				{
					f2=open(temp1,O_WRONLY|O_CREAT,0666);
					dup2(f2,STDOUT_FILENO);
					close(f2);
				}
			execvp(argum[0],argum);
			perror("Unable to execute command");
			exit(1);
			}
		}
		else if(status == 4)					//myls command implementation.
		{
			capacity = pathconf(".", _PC_PATH_MAX);
                	if((buffer = (char *)malloc((size_t)capacity))!=NULL)
                	{
                        	if(!getcwd(buffer,(size_t)capacity))
                                	perror("Error");

			argum[1] = buffer;
			argum[2] = NULL;
			y = 0;
			input10 = strtok(pat,":");
                	while(input10 !=NULL)                                                    //separating commands given by pipes
                	{
                        	strcpy(envimt[y],input10);
				strcat(envimt[y],"myls");
				if(access(envimt[y],F_OK)==0)
				{
					fla = 0;
					argum[0] = envimt[y];
					break;
                		}
				else
				{
					fla = 1;
				}
			y++;
			input10 = strtok(NULL,":");
			}
			if(fla == 1)
			{	fprintf(stderr,"No file as such in the path provided/or no path provided. Please set the environment variable correctly.\n");
				exit(1);
			}
			}
			execvp(argum[0],argum);
		}
		else if(status == 5)					//background process implementation.
		{
			fclose(stdin);
			fopen("/dev/null","r");
			execvp(argum[0],argum);
			exit(1);
		}
	}
	else if(child < 0)
	{
		perror("Child process could not be created");
	}
	else
	{
	if(status == 1)
	{
		waitpid(child,&child_status,0);
	}
	}

}

/* pipe creation anc calling fork_external for forking process*/
void pipefn(char **argum,int status,int *fd,int *fd1)
{
	pipeval++;
	if((pipeval % 2) != 0)
	{
		if(pipeval ==1)
			pipe(fd);				//pipe creation
		else
		{
			if(q!=pipeval)
			{
				pipe(fd);
			}
		}

		fork_external(argum,status,fd,fd1,NULL,NULL,0,NULL);
		close(fd1[0]);
		close(fd1[1]);
	}
	else if((pipeval > 1)&&((pipeval % 2)==0))
	{
		fd1[0] = -1;fd1[1]= -1;
		if(q!=pipeval)
		{
			pipe(fd1);
		}
		fork_external(argum,status,fd,fd1,NULL,NULL,0,NULL);
		close(fd[0]);
		close(fd[1]);
	}
	waitpid(child,&child_status,0);
}

/*redirection function to call fork_external*/
void redirection(char **argum,int status,char *argum1,char *let,int f1,char *temp1)
{
		if(bakflag == 1)
		{
			struct sigaction sb;
			sb.sa_handler = SIG_IGN;
			sigemptyset(&sb.sa_mask);
			sb.sa_flags = 0;
			if(sigaction(SIGCHLD,&sb,0)==-1)
			{
				perror(0);
				exit(1);
			}
			fork_external(argum,status,NULL,NULL,argum1,let,f1,temp1);
		}
		else
		{
			fork_external(argum,status,NULL,NULL,argum1,let,f1,temp1);
			waitpid(child,&child_status,0);
		}
}

/*external function to call fork_external*/
void listexternal(char **argum,int status)
{
		fork_external(argum,status,NULL,NULL,NULL,NULL,0,NULL);
		waitpid(child,&child_status,0);
}

/*background function to call fork_external*/
void background(char **argum,int status)
{
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		if(sigaction(SIGCHLD,&sa,0)==-1)
		{
			perror(0);
			exit(1);
		}
		fork_external(argum,status,NULL,NULL,NULL,NULL,0,NULL);
}

