# CST-315
 
# README FILE
# By: Duc Vo, Ty Gehrke

## Documemtation

	The additional documentation is within "Deadlock.docx"
	3 executable C++ file:
		"DeadlockCase1.c" -  simulating the deadlock
		"DeadlockCase2.c" -  simulating the simple version of the solution
		"DeadlockCase3.c" -  simulating the full solution for deadlock
	
## Run the program

1) To run the program you can now run "code . &" in your linux client. This will install/open VS Code while allowing you
	to run commands in the terminal still.
	
2) Within VS Code, create a new C++ file named main.cpp and paste the main code into the file,
	then save the file.

3) Now run this command in the terminal:
			 $ gcc pthread DeadlockCase1.c -o run1
			 $ gcc pthread DeadlockCase2.c -o run2
			 $ gcc pthread DeadlockCase3.c -o run3

4) You then can run your program by running this command in the terminal:
			 $ ./run1
			 $ ./run2
			 $ ./run3
			 
	You can stop the running program by Ctrl + C

## Setup Requirements

1) Install Linux.

2) Ensure that it is running the latest versions.

3) Use g++ to run programs.

4) In case the environment doesn't include pthreads, it can be done as follow:
			sudo apt-get install libpthread-stubs0-dev


