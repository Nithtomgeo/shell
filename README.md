# shell
Implements the unix shell commands


1. Type make to compile the shell.c and myls.c program.

2. Type ./unishell to run the shell.c program.

3. Internal commands:
exit 	- type exit to exit the program.
cd   	- type cd [directory path] to change the directory
  	- typing only cd will give “No parameters mentioned” message.
	- typing cd ..   will direct two directories back.
	eg: 
        cd 			-gives the message No arguments specified
	cd /home/grads/	-changes the current directory to absolute path /home/grads/
	cd ..			- take the shell directory to two directories before.
	cd user/projects	- will direct to the path specified, if that path is in the current working directory.
	Inorder to see the directory after cd command, type pwd

pwd	- type pwd to show the present working directory.

set	- type set MYPATH=directory path
Directory path should be replaced by the path myls executable is placed. Please make sure to give '/' after the directory name.
	eg: 
	if the myls is placed in /home/grads, please make sure to give the path as
	set MYPATH=/home/grads/
	if multiple paths are given and myls is in /home/grads/user then
	set MYPATH=/home/grads/:/home/grads/user/:/home/grads/user/projects/	
	The myls file should be in one of the paths which the user is inputting, otherwise it will prompt the user to input the correct path.
	Note: Please make sure there are no spaces. The format for spaces should be same as the example above.

4. CTRL-D	
Press CTRL + D to terminate the shell or program and come to the main shell.

5. myls command:
	myls.c file has been created and the .c file is placed in the tar files.
	Please set the MYPATH (location where the myls executable is placed) initially, otherwise it will ask the user to set the path. 
	Then type myls
	eg: set MYPATH=/home/grads/user/:/home/grads/
	To get the file details of a particular directory, go to that directory using cd and type myls.
	eg:
	If we want to see the files in directory /home/grads/ then	give:
	cd /home/grads/
	myls 

6. External commands:
	Inorder to execute external commands like ls, ls -l,ps etc, type the commands as such.
	More than 1 command can be executed in a line by giving ';'.
	eg: ls;ls -l;ps

7. Pipe Command:
	The shell works for a maximum of 2 pipes.
	eg: ls -l|sort|grep t or ls -1|head -3|tail -2 or ls|sort etc.

8. Redirection command:
	The shell works for redirections commands of the format, command1<file1 or command1>file1 or command1<file1>file2
	eg: sort<test1.c or ls -l>test1.c or grep t<test1.c>test2.c
	Please make sure to give valid file names.

9. Background command:
	A command followed by & makes the command a background process.Please make sure not to put space before the & operator.
	eg: sleep 100&		- will sleep for 100 seconds in the background and comes to the foreground
	Zombie background processes has been taken care of.
	
	Redirection and Background process works together.
	eg:grep t<test1.c>test2.c&

Note: If the background command is such that output comes to the foreground immediately after execution, then there are chances of the prompt getting 
distorted. In that case, press ENTER to run the shell normally.
If the background process is such that, output will not come to the foreground, then next prompt is displayed fine.

