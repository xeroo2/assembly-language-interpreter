#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

// Constants for memory size, register count, flag count, and program size
const int MEMORY_SIZE = 64;
const int REGISTER_COUNT = 7;
const int FLAG_COUNT = 4;
const int PROGRAM_SIZE = 20;

// Arrays to represent memory and registers and flags too
int memory[MEMORY_SIZE] = {0};
int registers[REGISTER_COUNT] = {0};
int flags[FLAG_COUNT] = {0}; // Flags: OF, UF, CF, ZF
// Counter to keep track of the program execution
int programCounter = 0;

// Constant for memory store location
const int MEMORY_STORE_LOCATION = 11;

// Function prototypes for various operations
void MOV(int register_idx, int value);
void ADD(int dest_idx, int value1, int value2);
void ADD(int dest_idx, int value);
void SUB(int dest_idx, int value1, int value2);
void SUB(int dest_idx, int value1);
void MUL(int dest_idx, int value1, int value2);
void MUL(int dest_idx, int value1);
void DIV(int dest_idx, int value1, int value2);
void DIV(int dest_idx, int value1);
void INC(int dest_idx);
void DEC(int dest_idx);
void IN(int dest_idx);
void OUT(int dest_idx);
void STORE(int src_idx, int address);
void LOAD(int dest_idx, int address);
void ROR(int register_idx, int num_bits);
void ROL(int register_idx, int num_bits);
void SHL(int dest_idx, int num_bits);
void SHR(int dest_idx, int num_bits);

int updateFlags(int value1);
void setFlags(int flags[]);

void printFlags();
void printRegisters();
void printMemory();
int getNumberOfLines();
void extractValue(string src, int &value);
int getInteger(int value);
void toUppercase(string &str); // since c++ does not have built-in function to change string to uppercases, we have decided to write it our self
int readFile(string instructions[], int arraySize);
int writeToFile();

// Main function where the program execution begins
int main()
{
    int ARRAY_SIZE = getNumberOfLines();

    // Define the program instructions
    string program[ARRAY_SIZE];
    readFile(program, ARRAY_SIZE);
    // Process each instruction in the program
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        // should reset the flags after each inst
        setFlags(flags);
        // Parse the instruction
        stringstream ss(program[i]);
        string operation, dest, src1;
        ss >> operation >> dest >> src1;
        toUppercase(operation);
        // Execute the corresponding operation based on the parsed instruction
        if (operation == "MOV")
        {
            // Move operation
            int register_idx = stoi(dest.substr(1));
            int value = (src1[0] == 'R') ? registers[src1[1] - '0'] : stoi(src1);
            MOV(register_idx, value);
        }
        else if (operation == "IN")
        {
            int dest_idx = stoi(dest.substr(1));
            IN(dest_idx);
        }
        else if (operation == "OUT")
        {
            int src_idx = stoi(dest.substr(1));
            OUT(src_idx);
        }
        else if (operation == "ADD")
        {
            // Parse the destination register

            int dest_idx = stoi(dest.substr(1));

            // Determine the value of the first operand
            int value1;

            extractValue(src1, value1);
            int value2;
            // Check if there is a second operand
            if (ss >> src1)
            {
                // Determine the value of the second operand
                int value2;
                extractValue(src1, value2);

                // Execute the ADD operation with two operands
                ADD(dest_idx, value1, value2);
            }
            else
            {
                // Execute the ADD operation with one operand
                ADD(dest_idx, value1);
            }
        }
        else if (operation == "MUL")
        {
            int dest_idx = stoi(dest.substr(1));
            int value1;
            int value2 = 0;
            extractValue(src1, value1);
            if (ss >> src1)
            {
                int value2;
                extractValue(src1, value2);

                MUL(dest_idx, value1, value2);
            }
            else
            {
                MUL(dest_idx, value1);
            }
        }
        else if (operation == "SUB")
        {
            int dest_idx = stoi(dest.substr(1));
            int value1;
            extractValue(src1, value1);
            if (ss >> src1)
            {
                int value2;
                extractValue(src1, value2);
                SUB(dest_idx, value1, value2);
            }
            else
            {
                SUB(dest_idx, value1);
            }
        }
        else if (operation == "DIV")
        {
            int dest_idx = stoi(dest.substr(1));
            int value1;
            extractValue(src1, value1);

            if (ss >> src1)
            {
                int value2;
                extractValue(src1, value2);

                DIV(dest_idx, value1, value2);
            }
            else
            {
                DIV(dest_idx, value1);
            }
        }
        else if (operation == "INC")
        {
            int dest_idx = stoi(dest.substr(1));
            INC(dest_idx);
        }
        else if (operation == "DEC")
        {
            int dest_idx = stoi(dest.substr(1));
            DEC(dest_idx);
            // was here i think
        }

        else if (operation == "STORE")
        {
            int address, src_idx = stoi(dest.substr(1));
            if (src1[0] == '[') // Memory address in a register
            {
                address = registers[stoi(src1.substr(2, src1.size() - 3))];
            }
            else // Direct memory address
                address = stoi(src1);

            STORE(src_idx, address);
        }
        else if (operation == "LOAD")
        {
            int address, dest_idx = stoi(dest.substr(1));
            if (src1[0] == '[') // Memory address in a register
                address = registers[stoi(src1.substr(2, src1.size() - 3))];
            else // Direct memory address
                address = stoi(src1);

            LOAD(dest_idx, address);
        }
        else if (operation == "ROR")
        {
            int dest_idx = stoi(dest.substr(1));
            int num_bits = stoi(src1);
            ROR(dest_idx, num_bits);
        }
        else if (operation == "ROL")
        {
            int dest_idx = stoi(dest.substr(1));
            int num_bits = stoi(src1);
            ROL(dest_idx, num_bits);
        }
        else if (operation == "SHL")
        {
            int dest_idx = stoi(dest.substr(1));
            int num_bits = stoi(src1);
            SHL(dest_idx, num_bits);
        }
        else if (operation == "SHR")
        {
            int dest_idx = stoi(dest.substr(1));
            int num_bits = stoi(src1);
            SHR(dest_idx, num_bits);
        }
    }
    // Display memory content
    cout << "Final ";
    printMemory();

    // Writing to file
    writeToFile();
    return 0;
}

// To move a value to a specific register
void MOV(int register_idx, int value)
{
    registers[register_idx] = value;
    programCounter++;
    cout << "Instruction Number: " << programCounter << ". ";
    printRegisters();
    cout << "programCounter: " << programCounter << endl;
}

// Perform addition with two arguments and update flags accordingly
void ADD(int dest_idx, int value1, int value2)
{
    // result = updateFlags (value1, value2) ? registers[dest_idx] : value1 + value2;
    int result = updateFlags((value1 + value2)) ? registers[dest_idx] : (value1 + value2);
    registers[dest_idx] = result;
    programCounter++;
    cout << "Instruction Number: " << programCounter << ". ";
    printRegisters();
    printFlags();
    cout << "programCounter: " << programCounter << endl;
}

// Perform addition with one argument and the given register and update flags accordingly
void ADD(int dest_idx, int value)
{
    int operationResult = registers[dest_idx] + value;
    registers[dest_idx] = updateFlags(operationResult) ? registers[dest_idx] : (registers[dest_idx] + value);
    registers[dest_idx] = flags[3] == 1 ? registers[dest_idx] + value : registers[dest_idx];
    programCounter++;
    cout << "Instruction Number: " << programCounter << ". ";
    printRegisters();
    printFlags();
    cout << "programCounter: " << programCounter << endl;
}

// Perform subtraction with two arguments and update flags accordingly
void SUB(int dest_idx, int value1, int value2)
{
    int result = updateFlags((value1 - value2)) ? registers[dest_idx] : (value1 - value2);
    registers[dest_idx] = result;
    programCounter++;
    cout << "Instruction Number: " << programCounter << ". ";
    printRegisters();
    printFlags();
    cout << "programCounter: " << programCounter << endl;
}

// Perform subtraction with one argument and the given register and update flags accordingly
void SUB(int dest_idx, int value)
{
    int operationResult = registers[dest_idx] - value;
    registers[dest_idx] = updateFlags(operationResult) ? registers[dest_idx] : (registers[dest_idx] - value);
    registers[dest_idx] = flags[3] == 1 ? registers[dest_idx] - value : registers[dest_idx];
    programCounter++;
    cout << "Instruction Number: " << programCounter << ". ";
    printRegisters();
    printFlags();
    cout << "programCounter: " << programCounter << endl;
}

// Perform multiplication with two arguments and update flags accordingly
void MUL(int dest_idx, int value1, int value2)
{
    int result = updateFlags((value1 * value2)) ? registers[dest_idx] : (value1 * value2);
    registers[dest_idx] = result;
    programCounter++;
    cout << "Instruction Number: " << programCounter << ". ";
    printRegisters();
    printFlags();
    cout << "programCounter: " << programCounter << endl;
}

// Perform multiplication with one argument and the given register and update flags accordingly
void MUL(int dest_idx, int value)
{
    int operationResult = registers[dest_idx] * value;
    registers[dest_idx] = updateFlags(operationResult) ? registers[dest_idx] : (registers[dest_idx] * value);
    registers[dest_idx] = flags[3] == 1 ? registers[dest_idx] * value : registers[dest_idx];
    programCounter++;
    cout << "Instruction Number: " << programCounter << ". ";
    printRegisters();
    printFlags();
    cout << "programCounter: " << programCounter << endl;
}

// Perform division with two arguments and update flags accordingly
void DIV(int dest_idx, int value1, int value2)
{
    int result = updateFlags((value1 / value2)) ? registers[dest_idx] : (value1 / value2);
    registers[dest_idx] = result;
    programCounter++;
    cout << "Instruction Number: " << programCounter << ". ";
    printRegisters();
    printFlags();
    cout << "programCounter: " << programCounter << endl;
}

// Perform division with one argument and the given register and update flags accordingly
void DIV(int dest_idx, int value)
{
    int operationResult = registers[dest_idx] / value;
    registers[dest_idx] = updateFlags(operationResult) ? registers[dest_idx] : (registers[dest_idx] / value);
    registers[dest_idx] = flags[3] == 1 ? registers[dest_idx] / value : registers[dest_idx];
    programCounter++;
    cout << "Instruction Number: " << programCounter << ". ";
    printRegisters();
    printFlags();
    cout << "programCounter: " << programCounter << endl;
}

// Perform the increament on the given register
void INC(int dest_idx)
{
    ADD(dest_idx, 1);
}

// Perform the decreament on the given register
void DEC(int dest_idx)
{
    SUB(dest_idx, 1);
}

// Moving a value entered by the user to a specific register
void IN(int dest_idx)
{
    int inputValue;
    do
    {
        inputValue = getInteger(dest_idx);
    } while (inputValue < 0);
    MOV(dest_idx, inputValue);
}

// Getting a value from a specific register and display it on the terminal
void OUT(int src_idx)
{
    cout << "Value in register R" << src_idx << ": " << registers[src_idx] << endl;
    printRegisters();
    programCounter++;
    cout << "programCounter: " << programCounter << endl;
}

// Storing a in a register into a specific memory address
void STORE(int src_idx, int address)
{
    int previousValue = memory[address];
    memory[address] = registers[src_idx];
    cout << "The previous value of memory location: " << address << " was: " << previousValue << ". Now it's updated to: " << memory[address] << endl;
    printRegisters();
    programCounter++;
    cout << "programCounter: " << programCounter << endl;
    printMemory();
}

// Loading a value from a memory address to a specific register
void LOAD(int dest_idx, int address)
{
    registers[dest_idx] = memory[address];
    programCounter++;
    cout << "Loaded value " << registers[dest_idx] << " from memory location " << address << " to register R" << dest_idx << ". Registers Array: ";
    printRegisters();
    cout << "programCounter: " << programCounter << endl;
}

//
void ROR(int register_idx, int num_bits)
{
    unsigned int value = static_cast<unsigned int>(registers[register_idx]);

    num_bits %= sizeof(int) * 8; // Ensure num_bits is within valid range

    for (int i = 0; i < num_bits; ++i)
    {
        unsigned int lsb = value & 1;
        value = (value >> 1) | (lsb << (sizeof(int) * 8 - 1));
    }

    registers[register_idx] = static_cast<int>(value);
    cout << "ROR: Rotated R" << register_idx << " right by " << num_bits << " bits." << endl;
    cout << "Instruction Number: " << programCounter << ". ";

    printRegisters();
    programCounter++;
    cout << "programCounter: " << programCounter << endl;
}

//
void ROL(int register_idx, int num_bits)
{
    unsigned int value = static_cast<unsigned int>(registers[register_idx]);

    num_bits %= sizeof(int) * 8; // Ensure num_bits is within valid range

    for (int i = 0; i < num_bits; ++i)
    {
        unsigned int msb = (value >> (sizeof(int) * 8 - 1)) & 1;
        value = (value << 1) | msb;
    }

    registers[register_idx] = static_cast<int>(value);
    cout << "ROL: Rotated R" << register_idx << " left by " << num_bits << " bits." << endl;
    cout << "Instruction Number: " << programCounter << ". ";

    printRegisters();
    programCounter++;
    cout << "programCounter: " << programCounter << endl;
}

//
void SHL(int register_idx, int num_bits)
{
    int value = registers[register_idx];
    int shifted_value = value << num_bits;
    registers[register_idx] = shifted_value;
    cout << "SHL: Shifted R" << register_idx << " left by " << num_bits << " bits." << endl;
    cout << "Instruction Number: " << programCounter << ". ";

    printRegisters();
    programCounter++;
    cout << "programCounter: " << programCounter << endl;
}

//
void SHR(int register_idx, int num_bits)
{
    int value = registers[register_idx];
    int shifted_value = value >> num_bits;
    registers[register_idx] = shifted_value;
    cout << "SHR: Shifted R" << register_idx << " right by " << num_bits << " bits." << endl;
    cout << "Instruction Number: " << programCounter << ". ";

    printRegisters();
    programCounter++;
    cout << "programCounter: " << programCounter << endl;
}

// Updating the flags array
int updateFlags(int value1)
{
    int returnedValue = 0;
    if (value1 > 255)
    {
        flags[0] = 1;
        returnedValue++;
    }
    if (value1 < 0)
    {
        flags[1] = 1;
        returnedValue++;
    }
    if (value1 > 255)
    {
        flags[2] = 1;
        returnedValue++;
    }
    if (value1 == 0)
    {
        flags[3] = 1;
        returnedValue++;
    }
    return returnedValue;
}

// Resetting the flags array to zero after each operation
void setFlags(int flags[])
{
    for (int i = 0; i < FLAG_COUNT; i++)
        flags[i] = 0;
}

// Printing the flags array after each assembly instruction
void printFlags()
{
    cout << "Flags:     ";
    for (int i = 0; i < FLAG_COUNT; i++)
    {
        cout << flags[i] << " ";
    }
    cout << "#" << endl;
}

// Printing the registers array after each assembly instruction
void printRegisters()
{
    cout << "Registers: ";
    for (int i = 0; i < REGISTER_COUNT; i++)
    {
        cout << setw(2) << setfill('0') << registers[i] << " ";
    }
    cout << "#" << endl;
}

// Printing the memory array after each "STORE" instruction & at the program ending
void printMemory()
{
    cout << "Memory: " << endl;
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        cout << setw(2) << setfill('0') << memory[i] << " ";
        if ((i + 1) % 8 == 0)
        {
            cout << endl;
        }
    }
}

// Getting the number of instructions in the "instructions.asm" file, in order to know the "program" array size
int getNumberOfLines()
{
    ifstream inputFile("instructions.asm");
    if (!inputFile.is_open())
    {
        cout << "Failed to open the file." << endl;
        return -1;
    }

    int numberOfLines = 0;
    string line;

    while (getline(inputFile, line))
    {
        numberOfLines++;
    }
    return numberOfLines;
}

// Extracting a velue from a given register and updating the "value" variable due to reference passing type
void extractValue(string src, int &value)
{
    if (src[0] == 'R')
    {
        // getting the register number
        int register_number = stoi(src.substr(1));
        // getting the value in the passed register + // value = the extracted value
        value = registers[register_number];
    }
    else
        value = stoi(src);
}

// Getting a valid intger (not a String or a negative number) from the user, it is called when the "IN" instruction is executed (Error handling)
int getInteger(int dest_idx)
{
    string str;

    do
    {
        cout << "Enter the value of the register R" << dest_idx << ": ";
        getline(cin, str);
    } while (str.find_first_not_of("-0123456789") != string::npos);

    return atoi(str.c_str());
}

// Changing all the instructions to uppercase letters so the program is case in-sensitive
void toUppercase(string &str)
{
    for (int i = 0; i < sizeof(str); i++)
        str[i] = toupper(str[i]);
}

// Reading the instructions from the "instruction.asm" file
int readFile(string instructions[], int arraySize)
{
    ifstream inputFile("instructions.asm");
    if (!inputFile.is_open())
    {
        cout << "Failed to open the file." << endl;
        return -1;
    }

    vector<string> instructionsVector;
    string line;

    while (getline(inputFile, line))
    {
        instructionsVector.push_back(line);
        instructionsVector.back() += "\n";
    }
    copy(instructionsVector.begin(), instructionsVector.end(), instructions);
    return 0;
}

// Writing the final output into the "output.txt" file
int writeToFile()
{
    fstream textFile;
    textFile.open("output.txt", ios::out);
    if (!textFile.is_open())
    {
        cout << "Failed to Wrtie to File!!" << endl;
        return 0;
    }
    // textFile << printRegisters() << endl;
    textFile << "Registers: ";
    for (int i = 0; i < REGISTER_COUNT; i++)
    {
        textFile << setw(2) << setfill('0') << registers[i] << " ";
    }
    textFile << "#" << endl;

    // textFile << printFlags() << endl;
    textFile << "Flags	 : ";
    for (int i = 0; i < FLAG_COUNT; i++)
    {
        textFile << flags[i] << " ";
    }
    textFile << "#" << endl;

    textFile << "PC.      : " << programCounter << endl;
    // textFile << printMemory() << endl;
    textFile << "Memory: " << endl;
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        textFile << setw(2) << setfill('0') << memory[i] << " ";
        if ((i + 1) % 8 == 0)
        {
            textFile << endl;
        }
    }
    textFile << "#" << endl;
    return 0;
}