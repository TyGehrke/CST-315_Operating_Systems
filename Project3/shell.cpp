#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <limits>  // Add this include for numeric_limits
#include <sys/wait.h>
#include <vector>

const int MAX_COMMAND_LENGTH = 1000;

// ANSI color escape codes
std::string ANSI_COLOR_RESET = "\033[0m";
std::string ANSI_COLOR_GREEN = "\033[32m";
std::string ANSI_COLOR_CYAN = "\033[36m";

void executeCommand(const char* command) {
    // Fork to create a child process
    pid_t pid = fork();

    if (pid < 0) {
        // handling errors 
        std::cerr << "Error forking process." << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        // Execute the command using the shell (sh)
        execl("/bin/sh", "sh", "-c", command, NULL);
        // If execl fails, exit the child process
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        // Wait for the child process to finish
        waitpid(pid, NULL, 0);
    }
}

void setPromptColors() {
    // Prompt the user to choose colors
    std::cout << "Choose a color for the command prompt:" << std::endl;
    std::cout << "1. Green (default)" << std::endl;
    std::cout << "2. Blue" << std::endl;
    std::cout << "3. Red" << std::endl;
    std::cout << "4. Yellow" << std::endl;
    std::cout << "5. Magenta" << std::endl;

    int colorChoice;
    std::cout << "Enter the number of your choice: ";

    // Check if the input is a valid integer
    if (!(std::cin >> colorChoice)) {
        std::cin.clear();  // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
        std::cout << "Invalid choice. Colors is not updated. You can change them again." << std::endl;
        return;
    }

    // Clear the input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (colorChoice) {
        case 1:
            ANSI_COLOR_GREEN = "\033[32m";
            std::cout << "Colors updated. You can change them again anytime." << std::endl;
            break;
        case 2:
            ANSI_COLOR_GREEN = "\033[34m";  // Blue
            std::cout << "Colors updated. You can change them again anytime." << std::endl;
            break;
        case 3:
            ANSI_COLOR_GREEN = "\033[31m";  // Red
            std::cout << "Colors updated. You can change them again anytime." << std::endl;
            break;
        case 4:
            ANSI_COLOR_GREEN = "\033[33m";  // Yellow
            std::cout << "Colors updated. You can change them again anytime." << std::endl;
            break;
        case 5:
            ANSI_COLOR_GREEN = "\033[35m";  // Magenta
            std::cout << "Colors updated. You can change them again anytime." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Colors is not updated. You can change them again." << std::endl;
            break;
    }
}



// display the available functions that the user can use
void displayHelp() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "1. quit - Exit the shell" << std::endl;
    std::cout << "2. history - Display command history" << std::endl;
    std::cout << "3. setcolors - Change prompt colors" << std::endl;
    std::cout << "4. help - Display this help message" << std::endl;
    std::cout << "5. Any other valid shell commands" << std::endl;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> commandHistory;  // Command history storage

    // Display help message
    displayHelp();

    if (argc == 2) {
        // Batch mode
        // Open the batch file for reading
        std::ifstream batchFile(argv[1]);
        if (!batchFile.is_open()) {
            // Display an error message if the file cannot be opened
            std::cerr << "Error opening batch file." << std::endl;
            return EXIT_FAILURE;
        }

        std::string command;
        // Read commands from the batch file, separated by semicolons
        while (std::getline(batchFile, command, ';')) {
            if (!command.empty()) {
                // Display the command and execute it
                std::cout << "$lopeShell: " << command << std::endl;
                executeCommand(command.c_str());
            }
        }

        batchFile.close();

    } else {
        // Interactive mode
        std::string userInput;

        std::cout << ANSI_COLOR_GREEN << "$lopeShell" << ANSI_COLOR_CYAN << ": " << ANSI_COLOR_RESET;
        while (std::getline(std::cin, userInput)) {
            
            

            if (userInput.find('!') == 0) {
                // Handle history commands
                // find the user selection
                size_t index = std::stoi(userInput.substr(1)) - 1;

                // run the selected command
                if (index < commandHistory.size()) {
                    userInput = commandHistory[index];
                    std::cout << ANSI_COLOR_GREEN << "$lopeShell" << ANSI_COLOR_CYAN << ": " << ANSI_COLOR_RESET << userInput << std::endl;
                } else {
                    std::cout << "Command not found in history." << std::endl;
                    std::cout << ANSI_COLOR_GREEN << "$lopeShell" << ANSI_COLOR_CYAN << ": " << ANSI_COLOR_RESET;
                    continue;
                }
            }

            if (userInput == "quit") {
                // Break out of the loop if the user types "quit"
                break;
            }

            if (userInput == "history") {
                // Display command history
                for (size_t i = 0; i < commandHistory.size(); ++i) {
                    std::cout << ANSI_COLOR_CYAN << i + 1 << ": " << ANSI_COLOR_RESET << commandHistory[i] << std::endl;
                }
                std::cout << ANSI_COLOR_GREEN << "$lopeShell" << ANSI_COLOR_CYAN << ": " << ANSI_COLOR_RESET;
                continue;
            }

            if (userInput == "setcolors") {
                // Allow users to change prompt colors
                setPromptColors();
                commandHistory.push_back(userInput);
                std::cout << ANSI_COLOR_GREEN << "$lopeShell" << ANSI_COLOR_CYAN << ": " << ANSI_COLOR_RESET;
                continue;
            }

            if (userInput == "help") {
                // Display help message
                displayHelp();
                commandHistory.push_back(userInput);

                std::cout << ANSI_COLOR_GREEN << "$lopeShell" << ANSI_COLOR_CYAN << ": " << ANSI_COLOR_RESET;
                continue;
            }

            

            if(userInput.find(';') != std::string::npos)
            {
                // Handle multiple commands separated by semicolons
                size_t pos = 0;
                std::string token;
                while ((pos = userInput.find(';')) != std::string::npos) {
                    token = userInput.substr(0, pos);
                    executeCommand(token.c_str());
                    // save to the history command
                    commandHistory.push_back(token);
                    userInput.erase(0, pos + 1);
                }
                // Execute the last command
                executeCommand(userInput.c_str());
                commandHistory.push_back(userInput);
            }
            else
            {
                executeCommand(userInput.c_str());
                // Add the command to history
                commandHistory.push_back(userInput);
            }

            std::cout << ANSI_COLOR_GREEN << "$lopeShell" << ANSI_COLOR_CYAN << ": " << ANSI_COLOR_RESET;

            

        }
    }

    

    return EXIT_SUCCESS;
}
