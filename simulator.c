/**
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 * 
 OH questions:
    - how to extract regA:
        - bit shifts and masking, how do they work
    - convertNum
    - Does branching mean changing pc and hitting continue in a loop?
    - Writing tests
    - What to do with fill
    - test every opcode
    - test branching forward and backward
    - check out the homeworkds/maybe discussion
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int);

int
main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    for (int i = 0; i < NUMREGS; i++)
    {
        state.reg[i] = 0;
    }
    

    state.pc = 0;
    int done = 0;
    int count = 0;
    printf("\n");

    while(done == 0)
    {
        printState(&state);
        count++;
        // printf("%d \n", state.mem[state.pc]);
        
        // do bit shifts and operations and extract opcode
        int inst = state.mem[state.pc];
        int regA, regB;
        int opcode = inst >> 22;
        // int temp = 0b0000000 000 000 000 0000000000000 000;
        regA = inst >> 19;
        regA = regA & 0b0000000000111;
        regB = inst >> 16;
        regB = regB & 0b0000000000000111;

        // printf("Opcode is %d \n", opcode);
        // printf("RegA is %d \n", regA);
        // printf("RegB is %d \n", regB);
        // if it's R-type
        if (opcode == 0 || opcode == 1)
        {
            int dest = inst & 0b00000000000000000000000000000111;
            // printf("Dest is %d \n", dest);

            // Add contents of regA with contents of regB
            // , store results in destReg.
            if (opcode == 0)
            {
                state.reg[dest] = state.reg[regA] + state.reg[regB];
            }
            // Nor contents of regA with contents of regB,
            // store results in destReg. This is a bitwise
            // nor; each bit is treated independently.
            else
            {
                int temp = ~(state.reg[regA] | state.reg[regB]);
                state.reg[dest] = temp;
            }
            
        }
        // if it's I-type
        else if (opcode == 2 || opcode == 3 || opcode == 4)
        {
            // do smth on offset with convertNum
            int offset = inst & 0b00000000000000001111111111111111;
            offset = convertNum(offset);
            // printf("Offset is %d \n", offset);

            // lw - Load regB from memory. 
            // Memory address is formed by adding 
            // offsetField with the contents of regA.
            if (opcode == 2)
            {
                state.reg[regB] = state.mem[state.reg[regA] + offset];
            }
            // sw - Store regB into memory. 
            // Memory address is formed by adding 
            // offsetField with the contents of regA.
            else if (opcode == 3)
            {
                state.mem[state.reg[regA] + offset] = state.reg[regB];
            }
            // beq - If the contents of regA and regB 
            // are the same, then branch to the address 
            // PC+1+offsetField, where PC is the address 
            // of this beq instruction.
            else
            {
                if (state.reg[regA] == state.reg[regB])
                {
                    state.pc = state.pc + 1 + offset;
                    continue;
                }
            }
        }
        // if it's J-type
        // jalr - First store PC+1 into regB, 
        // where PC is the address of this jalr 
        // instruction. Then branch to the address 
        // contained in regA. Note that this implies 
        // if regA and regB refer to the same register, 
        // the net effect will be jumping to PC+1.
        else if (opcode == 5)
        {
            // do smth on offset with convertNum
            int offset = inst;

            state.reg[regB] = state.pc + 1;
            state.pc = state.reg[regA];
            continue;

        }
        // if it's O-type
        else if (opcode == 6 || opcode == 7)
        {
            if (opcode == 6)
            {
                done = 1;
                printf("machine halted\n");
                printf("total of %d instructions executed\n", count);
                printf("final state of machine:\n");
                state.pc++;
                printState(&state);
                break;
            }
            else 
            {
                // do nothing
            }
        }
        state.pc++;
        
    }

    return(0);
}

void
printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
              printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
              printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int
convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1<<15) ) {
        num -= (1<<16);
    }
    return(num);
}

