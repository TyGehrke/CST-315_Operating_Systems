#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


// setting up table entry
class PageTableEntry {
public:
    int frame_number;           // physical frame number associated with the virtual page
    bool valid;                 

    // innitializing the class
    PageTableEntry() : frame_number(-1), valid(false) {}
};


// class representing PageTable
class PageTable {
public:
    std::vector<PageTableEntry> entries;        // vector to store entries

    PageTable(int size) : entries(size) {}

    // setting up the entries, first come first serve
    void setMapping(int page_number, int frame_number) {
        if (page_number >= 0 && page_number < entries.size() && frame_number >= 0) {
            entries[page_number].frame_number = frame_number;
            entries[page_number].valid = true;
        }
    }
};

// setting up the memoryManager class
class MemoryManager {
public:
    int physical_memory_size;
    int virtual_memory_size;
    int page_size;
    int page_table_size;
    PageTable page_table;
    std::vector<std::string> physical_memory;
    std::vector<std::string> virtual_memory;

    // constructor initializing the class
    MemoryManager(int physical_size, int virtual_size, int page_sz)
        : physical_memory_size(physical_size),
          virtual_memory_size(virtual_size),
          page_size(page_sz),
          page_table_size(virtual_size / page_sz),
          page_table(page_table_size),
          physical_memory(physical_size / page_sz),
          virtual_memory(virtual_size / page_sz) {

        // setting up the mapping    
        for (int i = 0; i < 6; ++i) {
            page_table.setMapping(i, i);
        }
    }

    // method to allocate memory, they are first come first serve
    void allocateMemory(int process_id, int required_memory) {
        // calculate the number of pages needed
        int num_pages_needed = (required_memory + page_size - 1) / page_size;
        for (int i = 0; i < page_table_size; ++i) {
            if (page_table.entries[i].valid) {
                int frame_number = findFreeFrame();
                if (frame_number != -1) {
                    page_table.entries[i].frame_number = frame_number;
                    page_table.entries[i].valid = true;

                    // printing the according maps of page into physical frame
                    std::cout << "Mapping: Virtual Page " << i << " -> Physical Frame " << frame_number << '\n';
                    physical_memory[frame_number] = virtual_memory[i];
                }
                --num_pages_needed;
            }
            // check if all pages are allocated
            if (num_pages_needed == 0) {
                break;
            }
        }
    }

    // method to find a free frame in the physical memory in case the current frame is occupied
    int findFreeFrame() {
        for (int i = 0; i < physical_memory.size(); ++i) {
            if (physical_memory[i].empty()) {
                return i;
            }
        }
        return -1;
    }


    // load the input from a txt file
    void loadProgram(const std::string& filename) {
        std::ifstream inputFile(filename);
        std::string word;
        int page_index = 0;

        while (inputFile >> word && page_index < virtual_memory.size()) {
            virtual_memory[page_index++] = word;
        }
    }

    // printing method to display virtual mem
    void printVirtualMemory(std::ostream& output = std::cout) const {
        output << "\nVirtual Memory:\n";
        for (int i = 0; i < virtual_memory.size(); ++i) {
            output << "Page " << i << ": " << virtual_memory[i] << '\n';
        }
    }

    // printing method to display physical mem
    void printPhysicalMemory(std::ostream& output = std::cout) const {
        output << "\nPhysical Memory:\n";
        for (int i = 0; i < physical_memory.size(); ++i) {
            output << "Frame " << i << ": " << physical_memory[i] << '\n';
        }
    }

    // printing method for PageTable
    void printPageTable(std::ostream& output = std::cout) const {
        output << "\nPage Table:\n";
        for (int i = 0; i < page_table.entries.size(); ++i) {
            if (page_table.entries[i].valid) {
                output << "Page " << i << " -> Frame " << page_table.entries[i].frame_number << '\n';
            }
        }
    }

    // Saving the output into file
    void saveMemoryToFile(const std::string& filename) const {
        std::ofstream outputFile(filename);
        if (outputFile.is_open()) {
            printPageTable(outputFile);
            printVirtualMemory(outputFile);
            printPhysicalMemory(outputFile);
            std::cout << "Memory contents saved to '" << filename << "'.\n";
        } else {
            std::cerr << "Unable to open file '" << filename << "' for writing.\n";
        }
    }
};

int main() {
    // Initialize memory manager with sizes (physical memory: 640 bytes, virtual memory: 320 bytes, page size: 64 bytes)
    MemoryManager memoryManager(10 * 64, 5 * 64, 64);

    // Load data from a file into virtual memory
    memoryManager.loadProgram("data1.txt");

    // Allocate memory for a process (Process 2) requiring 256 bytes
    memoryManager.allocateMemory(2, 256);

    // Print the contents to console
    memoryManager.printPageTable();
    memoryManager.printVirtualMemory();
    memoryManager.printPhysicalMemory();

    // Save the contents to a file
    memoryManager.saveMemoryToFile("output1.txt");

    std::cout << "\n------------------ SECOND ITERATION ------------------\n\n";

    // SECOND ITERATION

    // Load data from a file into virtual memory
    memoryManager.loadProgram("data2.txt");

    // Allocate memory for a process (Process 2) requiring 256 bytes
    memoryManager.allocateMemory(2, 256);

    // Print the contents to console
    memoryManager.printPageTable();
    memoryManager.printVirtualMemory();
    memoryManager.printPhysicalMemory();

    // Save the contents to a file
    memoryManager.saveMemoryToFile("output2.txt");

    return 0;
}
