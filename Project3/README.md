# CST-315
 
# README FILE
# By: Duc Vo, Ty Gehrke

## Documemtation

	The additional documentation is within "Project3_Improved UNIX_LINUX Command Line Interpreter.docx"
	One executable C++ file "shell.cpp"
	One text file including the batch code "batchfile.txt"
	
## Run the program

1) To run the program you can now run "code . &" in your linux client. This will install/open VS Code while allowing you
	to run commands in the terminal still.
	
2) Within VS Code, create a new C++ file named main.cpp and paste the main code into the file,
	then save the file.

3) Now run this command in the terminal, $ g++ shell.cpp -o run 

4) You then can run your program by running this command in the terminal with either:
			+ Interactive mode:	 $ ./run 
			--> In this mode the user will have to manually type in the command line 
			--> When this mode ran, it will give them the list of possible functions the users can do:
					+ Available commands:
							- quit - Exit the shell
							- history - Display command history 
									(the users can also use [!n] to run the n'th command in the list)
									Example: [!1] will run the 1st command in the list and [!2] will runt the 2nd command
							- setcolors - Change prompt colors
							- help - Display the available commandss
							- Any other valid shell commands
							- The user can use multiple command lines separated by ';'
								*** Warning: this would only work with shell commands, and it cannot be used for:
										+ history
										+ help
										+ setcolors
							

			--> The user can exit this mode by typing in 'quit' or 'CTRL + C'
			+ Batch mode: $ ./run 'batchfile.txt'
			--> In this mode, the program will read the 'batchfile.txt' and will run the commands within the file

## Setup Requirements

1) Install Linux.

2) Ensure that it is running the latest versions.

3) Use g++ to run programs.




