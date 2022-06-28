#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <pthread.h>
// Initializing global variables
int SI; // Switch case variable to execute READ,WRITE and TERMINATE functions.
int IC, C;
char M[100][4];   // Storing the content of the line into an array
char buffer[41];  // For temporary storage
char IR[4], R[4]; // For job instruction such as GD,PD,...
int i = 0, t = 0, a = 0, loc, j, k;
FILE *ph, *pho; // File pointers to accessing input and output files
void INIT()     // initializing the array M[][] with $
{
    int i, j;
    for (i = 0; i < 100; i++)
    {
        for (j = 0; j < 4; j++)
        {
            M[i][j] = '$';
        }
    }
    for (i = 0; i < 4; i++)
    {
        IR[i] = R[i] = '$';
    }
}
void LOAD()
{
    int t = 0;
    int i, j, k;
    while (getc(ph) != EOF) // Gets all characters from a file and checks the file contents
    {
        fseek(ph, -1, SEEK_CUR); // Sets the file position of the stream to the given of seek_cur for Current position of the file pointer
        memset(buffer, 0, 42);   // Copies the character c to the first n characters of the string pointed to
        fgets(buffer, 42, ph);   // 42 characters at max will be copied from the file pointer into the buffer
        printf("\nBuffer contents:\n");
        for (i = 0; i < 41; i++)
        {
            printf("%c", buffer[i]); // Printing buffer contents
        }
        printf("\n");
        // This block checks the instructions that are given in the file Eg.: $AMJ,$DTA,$END
        if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J') // Check for the first instruction $AMJ*
        {
            // Program starts executing the instruction
            t = 0;
            INIT(); // init function is called.
        }
        // This block checks for next instruction $DTA
        else if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A') // if the the buffer[0:3]=$DTA start read the content and apply job function
        {
            START();
        }
        // This block checks the end of the instruction $END
        else if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
            continue; // If the program is at the end of the instruction, jump and check other instructions which start with $AMJ
        else          // If the content of the line dose not start with any instruction
        {
            for (int k = t; k < t + 10; k++) // Print the content into the memory
            {
                printf("M[%d]:", k);
                for (int i = 0; i < 4; i++)
                {
                    M[k][i] = buffer[a]; // a=0
                    a++;
                    printf("%c", M[k][i]);
                }
                printf("\n");
            }
            t = t + 10;
            a = 0;
        }
    }
    // Closing the file
    fclose(ph);
    fclose(pho);
}
void EXECUTEUSERPROGRAM() // Performs GD,PD,LR,SR
{
    for (i = 0; i < 4; i++)
    {
        IR[i] = M[IC][i];
    }
    loc = ((int)IR[2] - 48) * 10 + ((int)IR[3] - 48);
    IC++;
    printf("\n\n%d\n\n", loc);
    if (IR[0] == 'G' && IR[1] == 'D')
    {
        // SI=1 to start reading
        SI = 1;
        printf("\nIN GD job\n");
        MOS();
    }
    else if (IR[0] == 'P' && IR[1] == 'D')
    {
        // SI=2 to start writing
        SI = 2;
        printf("\nIN PD job\n");
        for (int i = loc; i < loc + 10; i++)
        {
            printf("M[%d] : ", i);
            for (j = 0; j < 4; j++)
            {
                printf("%c", M[i][j]);
            }
            printf("\n");
        }
        MOS();
    }
    else if (IR[0] == 'H')
    {
        // SI=3, program will terminate
        SI = 3;
        printf("Program Terminated.");
        MOS(); // call mos function
    }
    else if (IR[0] == 'L' && IR[1] == 'R')
    { // This block will perform LR job
        printf("\nIn LR job\n");
        for (i = 0; i < 4; i++)
        {
            R[i] = M[loc][i];
            printf("%c", R[i]);
        }
        EXECUTEUSERPROGRAM();
    }
    else if (IR[0] == 'S' && IR[1] == 'R')
    { // This block will perform SR job
        printf("\nIN SR job\n");
        printf("\nmemory stored with data:");
        for (i = 0; i < 4; i++)
        {
            M[loc][i] = R[i];
            printf("%c", M[loc][i]);
        }
        EXECUTEUSERPROGRAM();
    }
    else if (IR[0] == 'C' && IR[1] == 'R')
    {
        for (i = 0; i < 4; i++)
        {
            if (R[i] == M[loc][i])
            {
                printf("\nStrings Match!\n");
                C = 1;
            }
            else
            {
                C = 0;
                printf("\nStrings Don't Match!\n");
                break;
            }
        }
        EXECUTEUSERPROGRAM();
    }
    else if (IR[0] == 'B' && IR[1] == 'T')
    {
        if (C == 1)
        {
            IC = loc;
            printf("\nValue of IC (instruction counter) %d", IC); // Prints pointer location between 0-99
        }
        EXECUTEUSERPROGRAM();
    }
}
void READ()
{
    int i, j, k;
    if (fgets(buffer, 41, ph) != NULL)
    {
        k = 0;
        for (i = loc; i < loc + 10; i++) // Current string exists for 10 positions even if not occupied.
        {
            printf("M[%d] : ", i);
            for (j = 0; j < 4 && buffer[k] != '\0'; j++)
            {
                M[i][j] = buffer[k];
                k++;
                printf("%c", M[i][j]); // Printing the characters in input file 4 at a time
            }
            printf("\n");
        }
    }
    EXECUTEUSERPROGRAM();
}
deletebuffer()
{
    for (int asd = 0; asd < 41; asd++)
    {
        buffer[asd] = NULL;
    }
}
void WRITE()
{
    int i, j, k;
    IR[3] = '0';
    k = 0;
    printf("\n");
    deletebuffer();
    for (i = loc; i < loc + 10; i++)
    {
        for (j = 0; j < 4 && M[i][j] != '$'; j++)
        {
            if (M[i][j] == '\n')
            {
                buffer[k] = ' ';
            }
            else
                buffer[k] = M[i][j];
            printf("%c", buffer[k]);
            k++;
        }
    }
    buffer[k] = '\n';
    printf("\n strings that given in input file is \n");
    for (int as = 0; as < 41; as++)
    {
        printf("%c", buffer[as]);
    }
    fputs(buffer, pho);
    EXECUTEUSERPROGRAM();
}
void TERMINATE() // Terminate function will execute if SI=3
{
    memset(buffer, 0, 41); // Copies the character c to the first n characters of the string pointed to
    // buffer[0] = '\n';
    // buffer[1] = '\n';
    fputs(buffer, pho);
}
void MOS()
{
    switch (SI)
    {
    case 1:
        READ();
        break;
    case 2:
        WRITE();
        break;
    case 3:
        TERMINATE();
        break;
    default:
        SI = 0;
    }
}
void START()
{
    IC = 0; // Instruction Counter
    printf("\n Process Running ... \n");
    EXECUTEUSERPROGRAM();
}
void tSync(void)
{
    // Opens read and write files
    for (int i = 1; i <= 4; i++)
    {
        char file[64];
        char fileo[64];
        sprintf(file, "%d.txt", i);
        sprintf(fileo, "%dop.txt", i);
        ph = fopen(file, "r");   // Open file with read mode
        pho = fopen(fileo, "w"); // Open file with write mode
        if (ph == NULL)          // Check if the file exists
        {
            printf("It is not exist");
        }
        else
        {
            LOAD();
        }
        printf("Process Done.\n");
        if (i < 4)
        {
            system("pause");
        }
    }
}
int main() // Main Function
{
    pthread_t thread; // Thread to run the processes
    pthread_create(&thread, NULL, tSync, NULL);
    pthread_exit(NULL);
    return 0;
}
