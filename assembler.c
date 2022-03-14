/**
 * Project 1 
 * Assembler code fragment for LC-2K 
 */


/*
Outline:
    - Converting from decimal to binary and then back to decimal
    - Offset two's complement
    - discussion 1


OH question:
    - Converting to 16 bits
    - using negative numbers?
    - Test cases
    - values init bound
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *, int, char[10][7], int);
int isNumber(char *);

int main(int argc, char *argv[])
{
    printf("Here in main \n");
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    char values[100][7];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */

    char notFinished = 'f';

    int counter = 0;

    while (1 == 1)
    {
        char line[MAXLINELENGTH];
        if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
            /* reached end of file */
            printf("\n exited loop \n");
            break;
        }
        // printf("%s", line);

        /* check for line too long (by looking for a \n) */
        if (strchr(line, '\n') == NULL) {
            /* line too long */
            printf("error: line too long\n");
            exit(1);
        }
        char *ptr = line;
        if (sscanf(ptr, "%[^\t\n\r ]", label)) {
            /* successfully read label; advance pointer over the label */
            ptr += strlen(label);
            // printf("Here in label \n");
            // printf("%s\n", label);
            strcpy(values[counter], label);
        }
        // printf("\n counter is %d \n", counter);
        counter++;
    }


    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);

    counter = 0;
    int final = 0;

    while (1 == 1)
    {
        if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2, counter, values, final))
        {
            break;
        }
        int num = atoi(opcode);
        fprintf(outFilePtr,"%d \n", num);
        counter++;
    }
    if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2, counter, values, final) ) {
        /* reached end of file */
        return 0;
    }

    /* after doing a readAndParse, you may want to do the following to test the
        opcode */
    if (!strcmp(opcode, "add") != 0) {
        /* do whatever you need to do for opcode "add" */
    }

    return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if successfully read
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
        char *arg1, char *arg2, int pc, char values[10][7], int final)
{
    char line[MAXLINELENGTH];

    printf("Here in readandparse \n");

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
        /* reached end of file */
        return(0);
    }
    printf("%s", line);

    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
        printf("error: line too long\n");
        exit(1);
    }

    /* printf("\n Labels are \n");
    for(size_t i = 0; i < 10; i++)
    {
        printf("\n Element is %s", values[i]);
    }
    printf("\n"); */

    /* is there a label? */
    char *ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label)) {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
        // printf("Label is %s \n", label);
    }
    
    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);
    
    // Need branches depending on instr type and opcode
    // R-Type
    if (!strcmp(opcode, "add") != 0 || !strcmp(opcode, "nor") != 0)
    {
        if (!strcmp(opcode, "add") != 0)
        {
            strcpy(opcode, "0");
        }
        else
        {
            strcpy(opcode, "1");
        }
        int temp = atoi(opcode);
        int temp0 = atoi(arg0);
        int temp1 = atoi(arg1);
        temp = temp << 22;
        temp0 = temp0 << 19;
        temp1 = temp1 << 16;
        int temp2 = atoi(arg2);
        final = temp + temp0 + temp1 + temp2;
        sprintf(opcode, "%d", final);
        printf("%d %d %d %d \n", temp, temp0, temp1, temp2);
    }
    else if (!strcmp(opcode, "lw") != 0 || !strcmp(opcode, "sw") != 0 || !strcmp(opcode, "beq") != 0)
    {
        if (!strcmp(opcode, "lw") != 0)
        {
            strcpy(opcode, "2");
        }
        else if (!strcmp(opcode, "sw") != 0)
        {
            strcpy(opcode, "3");
        }
        else
        {
            strcpy(opcode, "4");
        }
        char undef = 'f';
        int offset = 0;
        if (!isNumber(arg2))
        {
            for (size_t i = 0; i < 100; i++)
            {
                if (!strcmp(arg2, values[i]) != 0)
                {
                    undef = 't';
                    sprintf(arg2, "%ld", i);
                    break;
                }
            }
            if (undef == 'f')
            {
                exit(1);
            }
            if (!strcmp(opcode, "2") != 0)
            {
                offset = atoi(arg2);
            }
            else if (!strcmp(opcode, "3") != 0)
            {
                offset = atoi(arg2);
            }
            else
            {
                offset = atoi(arg2) - pc - 1;
            }
        }
        else {
            offset = atoi(arg2);
        }
        int temp = atoi(opcode);
        int temp0 = atoi(arg0);
        int temp1 = atoi(arg1);
        int temp2 = offset;

        if (temp2 > 32767 || temp2 < -32768)
        {
            exit(1);
        }
        
        printf("Before: %d \n", temp2);
        
        temp2 = temp2 & 0xFFFF;
        printf("After: %d \n", temp2);
        temp = temp << 22;
        temp0 = temp0 << 19;
        temp1 = temp1 << 16;
        final = temp + temp0 + temp1 + temp2;
        sprintf(opcode, "%d", final);
        printf("%d %d %d %d \n", temp, temp0, temp1, temp2);
    }
    else if (!strcmp(opcode, "halt") != 0 || !strcmp(opcode, "noop") != 0)
    {
        if (!strcmp(opcode, "halt") != 0)
        {
            strcpy(opcode, "6");
        }
        else
        {
            strcpy(opcode, "7");
        }
        strcpy(arg0, "0");
        strcpy(arg1, "0");
        strcpy(arg2, "0");
        int temp = atoi(opcode);
        int temp0 = atoi(arg0);
        int temp1 = atoi(arg1);
        temp = temp << 22;
        temp0 = temp0 << 19;
        temp1 = temp1 << 16;
        final = temp + temp0 + temp1;
        sprintf(opcode, "%d", final);
        printf("%d %d %d \n", temp, temp0, temp1);
    }
    else if (!strcmp(opcode, "jalr") != 0)
    {
        strcpy(opcode, "5");
        int temp = atoi(opcode);
        int temp0 = atoi(arg0);
        int temp1 = atoi(arg1);
        temp = temp << 22;
        temp0 = temp0 << 19;
        temp1 = temp1 << 16;
        final = temp + temp0 + temp1;
        sprintf(opcode, "%d", final);
        printf("%d \n", final);
    }
    else if (!strcmp(opcode, ".fill") != 0)
    {
        if (!isNumber(arg0))
        {
            char undef = 'f';
            for (size_t i = 0; i < 100; i++)
            {
                if (!strcmp(arg0, values[i]) != 0)
                {
                    undef = 't';
                    sprintf(arg0, "%ld", i);
                    break;
                }
            }
            if (undef == 'f')
            {
                exit(1);
            }
        }
        strcpy(opcode, arg0);
        printf("%d \n", final);
    }
    else
    {
        exit(1);
    }

    return(1);
}

int isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}


