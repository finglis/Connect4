// Connect 4 game written by Fiona Inglis (40509175) for Algorithms and Data Structure course in 2021

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Global variables
struct stack
{
    int *array;
    int top;
};

//Standard mode functions
int gameMode();
void initBoard();
int standardMainLoop(int x_max, int y_max, int inARow);
char *getBoardLoc(char *board, int x, int y, int x_max, int y_max);
void displayBoard(char *board, int x_max, int y_max);
char determinePlayer();
int getInput();
int getUndoRedoInput(int maxValue);
int addPiece(char *board, int input, int x_max, int y_max, int *turnCounter, char player, int inARow);
int removePiece(char *board, int input, int x_max, int y_max, int *turnCounter, char player, int inARow);
int checkWin(char *board, int inARow, int input, int y, char player, int x_max, int y_max);
int matchingInDirection(char *board, int inARowToWin, int input, int y, char player, int xStride, int yStride, int x_max, int y_max);
void saveMove(int *recordedGame, int turnCounter, int x);
void saveGame(char *fileName, int *recordedGame, int arraySize);
void openGame(int x_max, int y_max, int inARow);
void delay();

//Bot mode functions
int botMainLoop(int x_max, int y_max, int inARow);
int getRandomNumber(int x_max);

//3 Player mode funcitons
int threePlayerMainLoop(int x_max, int y_max, int inARow);
char determine3Player(int turnCounter);

//Advanced mode funcitons
int altMainLoop(int x_max, int y_max, int inARow);
void init_stack(struct stack *undoRedoStack, int maxTurns);
int undoSave(struct stack *undoRedoStack, int *recordedGame, int *turnCounter, int maxTurns);
void push(struct stack *undoRedoStack, int items, int maxTurns);
int *pop(struct stack *undoRedoStack);
int checkStack(struct stack *undoRedoStack);
void clearStack(struct stack *undoRedoStack);
void freeStack(struct stack *undoRedoStack);
void printStack(struct stack *undoRedoStack);

int main(void)
{
    char gameChoice = gameMode();
    if (gameChoice == 1) //Enter standard 4 in a row game with board size 7*6
    {
        int inARow = 4;
        int x_max = 7;
        int y_max = 6;
        standardMainLoop(x_max, y_max, inARow);
    }
    if (gameChoice == 2) //Enter standard 4 in a row game with board size 7*6 and a bot 
    {
        int inARow = 4;
        int x_max = 7;
        int y_max = 6;
        botMainLoop(x_max, y_max, inARow);
    }
    if (gameChoice == 3) //Enter 3 player mode on a standard board but with 3 "in a row" to win
    {
        int inARow = 3;
        int x_max = 7;
        int y_max = 6;
        threePlayerMainLoop(x_max, y_max, inARow);
    }
    if (gameChoice == 4) //Enter game with variable board size and choose your own "in a row" to win
    {
        int maxInARowValue = 20;
        printf("How many in a row to win? ");
        int inARow = getInput(maxInARowValue);
        int maxXYValue = 25;
        printf("Enter the x value you would like to use: ");
        int x_max = getInput(maxXYValue);
        printf("Enter the y value you would like to use: ");
        int y_max = getInput(maxXYValue);
        if ((x_max < inARow) && (y_max < inARow))
        {
            printf("It would be impossible to win with these values, please restart the game and try again.");
        }
        else
        {
            altMainLoop(x_max, y_max, inARow);
        }
    }
    if (gameChoice == 5) //Enter the mode to replay a previous standard game
    {
        int inARow = 4;
        int x_max = 7;
        int y_max = 6;
        openGame(x_max, y_max, inARow);
    }
}

int gameMode() //Determine the game mode the user would like to play
{
    int gameChoice;
    while (1)
    {
        printf("Press 1 to play 1v1 on a standard board.\n");
        printf("Press 2 to play against a bot.\n");
        printf("Press 3 to play 3 player mode on a standard board.\n");
        printf("Press 4 to play 1v1 on your choice of board size/how many 'in a row' to win (This includes undo/redo funcitonality).\n");
        printf("Press 5 to replay a previous 1v1 on a standard board.\n");
        printf("Enter your game choice: ");
        scanf("%d", &gameChoice);
        if (gameChoice >= 1 && gameChoice <= 5)
        {
            break;
        }
        else
        {
            printf("Please enter a valid gamemode, between 1 and 5\n");
        }
    }
    return gameChoice;
}

int standardMainLoop(int x_max, int y_max, int inARow) //Standard 4 in a row game mode including save game when win occurs
{
    int turnCounter = 0; //Keeps track of each turn and determines who is to play
    char board[x_max][y_max]; //The game board
    int maxTurns = x_max * y_max; //Determine the max number of moves that can be played to determine the recorded array size
    int recordedGame[maxTurns]; //Array that contains each move played and is then exported
    int result;
    char time_string[20];
    char fileName[24];

    //Timing variables
    // clock_t start, end;
    // double cpu_time_used;
    // double totalTime = 0;
    // double averageTime = 0;

    //Set up Procedures
    initBoard(&board[0][0], x_max, y_max);
    displayBoard(&board[0][0], x_max, y_max);

    //Main game cycle
    while (result != 1)
    {
        char player = determinePlayer(turnCounter);
        printf("\nThis is turn number: %d", (turnCounter + 1));
        printf("\nEnter your column choice: ");
        int input = getInput(x_max);
        //start = clock(); //start timing the standard game loop after the input till after the check win
        int x = input - 1;
        int y = addPiece(&board[0][0], x, x_max, y_max, &turnCounter, player, inARow);
        displayBoard(&board[0][0], x_max, y_max);
        saveMove(recordedGame, turnCounter, x); 
        result = checkWin(&board[0][0], inARow, x, y, player, x_max, y_max);
        // end = clock(); //end timing the standard game loop after the input till after the check win
        // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        // printf("It took %f seconds (%f miliseconds) from the value being entered to the check win function being run", cpu_time_used, cpu_time_used * 1000);
        // totalTime = totalTime + cpu_time_used; 
        // printf("\ntotal time used: %f", totalTime);
    }
    // averageTime = totalTime/turnCounter;
    // printf("\nAverage time used: %f", averageTime);
    time_t t;
    time(&t);
    strftime(time_string, 20, "%Y-%m-%d-%H-%M-%S", localtime(&t));
    sprintf(fileName, "%s.txt", time_string);
    saveGame(fileName, recordedGame, turnCounter);
    return 0;
}

char *getBoardLoc(char *board, int x, int y, int x_max, int y_max) //Determine the location in a 2D array, returns the equivalent of array[x][y]
{
    if (x < 0 || x > x_max || y < 0 || y > y_max)
    {
        return NULL;
    }
    return ((board + (x * y_max)) + y);
}

void initBoard(char *board, int x_max, int y_max) // Create the board for variable x and y sizes
{
    int z = 0;
    for (int x = 0; x < x_max; x++)
    {
        for (int y = 0; y < y_max; y++)
        {
            //*((board+(i*y_max))+j)
            *getBoardLoc(board, x, y, x_max, y_max) = ' ';
            z++;
        }
    }
}

void displayBoard(char *board, int x_max, int y_max) //Print the board incuding adaptation to various x and y entries
{
    printf("\n");
    for (int y = y_max - 1; y >= 0; y--)
    {
        for (int x = 0; x < x_max; x++)
        {
            //printf("| %d,%d ", x,y); //This will print the location for each position on the array
            printf("| %c", *getBoardLoc(board, x, y, x_max, y_max)); //aka print(&board[0][0], x_max, y_max);
            if (x == x_max - 1)
            {
                printf("|\n");
            }
        }
    }
    for (int i = 1; i <= x_max; i++)
    {
        printf("---");
    }
    printf("-\n ");
    for (int j = 1; j <= x_max; j++)
    {
        if (j < 10)
        {
            printf(" %d ", j);
        }
        else
        {
            printf("%d ", j);
        }
    }
    printf("\n");
}

char determinePlayer(int turnCounter) //Identify the player using the turn counter
{
    int turn;
    char player;
    turn = turnCounter % 2;
    if (turn == 0)
    {
        printf("\nIt is X to play");
        player = 'X';
    }
    else
    {
        printf("\nIt is O to play");
        player = 'O';
    }
    return player;
}

int getInput(int maxValue) //Get user input including and verifying within required range
{
    int input = 0;
    scanf("%d", &input);
    while (!((input >= 1) && (input <= maxValue)))
    {
        printf("Error! Your number is out of board range, please enter a number between 1 and %d: ", maxValue);
        scanf("%d", &input);
    }
    return input;
}

int addPiece(char *board, int input, int x_max, int y_max, int *turnCounter, char player, int inARow) //Add a piece to the correct column and stack on previous entries to that column
{
    int y;
    for (y = 0; y <= y_max; y++)
    {
        if ((y <= y_max - 1) && *getBoardLoc(board, input, y, x_max, y_max) == ' ')
        {
            //printf("The board has space, entering x_max = %d, y_max = %d\n", input, y);
            *getBoardLoc(board, input, y, x_max, y_max) = player;
            (*turnCounter)++;
            return y;
        }
    }
    printf("There is no more space in this column, please choose another entry.\n");
    return -1;
}

int checkWin(char *board, int inARowToWin, int x, int y, char player, int x_max, int y_max) //Check the last play for a "win" for x amount along 4 axes - horizonal, vertical, forSlash, backSlash using matchingInDirection function
{
    int inARow = 0;

    //vertical check
    inARow = matchingInDirection(board, inARowToWin, x, y, player, 0, 1, x_max, y_max);

    if (inARow >= inARowToWin)
    {
        printf("\n%c Wins!", player);
        return 1;
    }

    //horizontal check
    inARow = matchingInDirection(board, inARowToWin, x, y, player, 1, 0, x_max, y_max);

    if (inARow >= inARowToWin)
    {
        printf("\n%c Wins!", player);
        return 1;
    }

    //forward slash diagonal check
    inARow = matchingInDirection(board, inARowToWin, x, y, player, 1, 1, x_max, y_max);

    if (inARow >= inARowToWin)
    {
        printf("\n%c Wins!", player);
        return 1;
    }

    //backward slash diagonal check
    inARow = matchingInDirection(board, inARowToWin, x, y, player, -1, 1, x_max, y_max);

    if (inARow >= inARowToWin)
    {
        printf("%c Wins!", player);
        return 1;
    }
    return 0;
}

int matchingInDirection(char *board, int inARowToWin, int x, int y, char player, int xStride, int yStride, int x_max, int y_max) //Check win loops
{
    //printf("\nchecking for win from %c at x: %i, y: %i", player, x, y);
    int inARow = 0;

    for (int i = 0; i < inARowToWin; i++) //This looks up to 3 values "up" from the piece coordinates to check to see if the player matches.
    {
        char *loc = getBoardLoc(board, x + xStride * i, y + yStride * i, x_max, y_max);
        if (loc == NULL)
            break;
        if (*loc == player)
        {
            inARow++;
        }
        else
        {
            break; //if there is nothing in this direction, break and then check in opposite direction
        }
    }

    for (int i = 1; i < inARowToWin; i++) //This looks up to 3 values away from the piece coordinates to check to see if the player matches.
    {
        char *loc = getBoardLoc(board, x - xStride * i, y - yStride * i, x_max, y_max);
        if (loc == NULL)
            break;
        if (*loc == player)
        {
            inARow++;
        }
        else
        {
            break; //if there is nothing in this direction, break and return the inARow counter
        }
    }
    return inARow;
}

void saveMove(int *recordedGame, int turnCounter, int x) //Saving each move made by storing the input value into recordedGame array
{
    int adaptedTurnCounter = turnCounter - 1;
    recordedGame[adaptedTurnCounter] = x;
}

void saveGame(char *fileName, int *recordedGame, int arraySize) //Exporting the recordedGame array to .txt file and used at the end of a game
{
    printf("\nSaving game as: %s", fileName);
    FILE *fp = fopen(fileName, "w+");
    for (int i = 0; i < arraySize; i++)
    {
        fprintf(fp, "%d,", recordedGame[i]);
    }
    fclose(fp);
}

void openGame(int x_max, int y_max, int inARow) //Opens a saved game .txt file and displays each move
{
    char fileName[29];
    int maxTurns = x_max * y_max;
    int recordedGame[maxTurns]; //Array to be filled with contents of txt file
    char board[x_max][y_max];
    char result;
    int replayCounter = 0;

    //Set up Procedures for import
    initBoard(&board[0][0], x_max, y_max);
    displayBoard(&board[0][0], x_max, y_max);

    printf("Please enter the name of the game you would like to replay: ");
    scanf("%28s", fileName);
    FILE * fp = fopen(fileName, "r");
    while (fp == NULL)
    {
        printf("\nError! Unable to load %s, please enter a valid game name\n", fileName);
        scanf("%28s", fileName);
    }
    int sucessfulReads;
    for ( sucessfulReads = 0; sucessfulReads < maxTurns; sucessfulReads++) //Fill array with contents of the txt file
    {
        if (fscanf(fp, "%d,", &recordedGame[sucessfulReads]) != 1)
        {
            break;
        }
        
    }
    fclose (fp);

    //Main game cycle
    while (replayCounter < sucessfulReads)
    {
        char player = determinePlayer(replayCounter); //Determine who is to play
        printf("\nTurn number: %d", replayCounter);
        int printValue = recordedGame[replayCounter];
        printf("\nEnter column choice: %d ", printValue + 1);
        delay();
        int x = recordedGame[replayCounter];
        addPiece(&board[0][0], x, x_max, y_max, &replayCounter, player, inARow);
        displayBoard(&board[0][0], x_max, y_max);     
    } 
    result = determinePlayer(sucessfulReads+1);
    printf("\n%c Wins!", result);
}

void delay()
{
    int milliSeconds = 2000;
    clock_t start_time = clock();
    while (clock() < start_time + milliSeconds);
}

/* The following functions are for the bot game mode only */

int botMainLoop(int x_max, int y_max, int inARow) //Standard 4 in a row game mode including save game when win occurs against a simple bot 
{
    int turnCounter = 0; //Keeps track of each turn and determines who is to play
    char board[x_max][y_max]; //The game board
    int maxTurns = x_max * y_max; //Determine the max number of moves that can be played to determine the recorded array size
    int recordedGame[maxTurns]; //Array that contains each move played and is then exported
    int result;

    //Set up Procedures
    initBoard(&board[0][0], x_max, y_max);
    displayBoard(&board[0][0], x_max, y_max);

    //Main game cycle
    while (result != 1)
    {
        int input;
        char player = determinePlayer(turnCounter); //Determine who is to play. Bot plays 2nd, "O".
        if (player == 'O')
        {
            printf("\nThis is turn number: %d", (turnCounter + 1));
            printf("\nThe bot will enter its choice: ");
            input = getRandomNumber(x_max);

        }
        if (player == 'X')
        {
            printf("\nThis is turn number: %d", (turnCounter + 1));
            printf("\nEnter your column choice: ");
            input = getInput(x_max); //Get user input
        }
        int x = input - 1;
        int y = addPiece(&board[0][0], x, x_max, y_max, &turnCounter, player, inARow); 
        displayBoard(&board[0][0], x_max, y_max); 
        saveMove(recordedGame, turnCounter, x);
        result = checkWin(&board[0][0], inARow, x, y, player, x_max, y_max); 
    }
    time_t t;
    time(&t);
    char time_string[20];
    strftime(time_string, 20, "%Y-%m-%d-%H-%M-%S", localtime(&t));
    char fileName[24];
    sprintf(fileName, "%s.txt", time_string);
    saveGame(fileName, recordedGame, turnCounter);
    return 0;
}

int getRandomNumber(int x_max)
{
    srand(time(NULL));
    int input = rand() % x_max + 1;
    return input;
}

/*The following functions are for the 3 player mode only*/

int threePlayerMainLoop(int x_max, int y_max, int inARow)
{
    int turnCounter = 0;
    char board[x_max][y_max];
    int result;

    //Set up Procedures
    initBoard(&board[0][0], x_max, y_max);
    displayBoard(&board[0][0], x_max, y_max);

    //Main game cycle
    while (result != 1)
    {
        char player = determine3Player(turnCounter);
        printf("\nThis is turn number: %d", (turnCounter + 1));
        printf("\nEnter your column choice: ");
        int input = getInput(x_max);
        int x = input - 1;
        int y = addPiece(&board[0][0], x, x_max, y_max, &turnCounter, player, inARow); 
        displayBoard(&board[0][0], x_max, y_max); 
        result = checkWin(&board[0][0], inARow, x, y, player, x_max, y_max);
    }
    return 0;
}

char determine3Player(int turnCounter) //Identify the player using the turn counter
{
    int turn;
    char player;
    turn = turnCounter % 3;
    if (turn == 0)
    {
        printf("\nIt is X to play");
        player = 'X';
    }
    if (turn == 1)
    {
        printf("\nIt is O to play");
        player = 'O';
    }
    if (turn == 2)
    {
        printf("\nIt is S to play");
        player = 'S';
    }
    return player;
}

/* The following functions are for the advanced game mode only */

int altMainLoop(int x_max, int y_max, int inARow) //Advanced "x in a row" game mode including variable board size, "in a row" length and undo and redo features
{
    int turnCounter = 0; //Keeps track of each turn and determines who is to play
    char board[x_max][y_max]; //The game board
    int maxTurns = x_max * y_max; //Determine the max number of moves that can be played to determine the recorded array size
    int y;
    int result;
    int recordedGame[maxTurns]; //Array that contains each move played and is then exported
    int inputChar;
    char time_string[20];
    char fileName[24];

    //Timing variables
    // clock_t start, end;
    // double cpu_time_used;
    // double totalTime = 0;
    // double averageTime = 0;
    
    //Set up Procedures
    initBoard(&board[0][0], x_max, y_max);
    displayBoard(&board[0][0], x_max, y_max);
    struct stack undoRedoStack; //create the stack variable
    init_stack(&undoRedoStack, maxTurns);

    //Main game cycle
    while (result != 1)
    {
        char player = determinePlayer(turnCounter);
        printf("\nThis is turn number: %d", (turnCounter + 1));
        printf("\nEnter your column choice OR choose 0 to undo or redo: ");
        int input = getUndoRedoInput(x_max);
        if (input != 0)
        {
            clearStack(&undoRedoStack);
            int x = input - 1;
            y = addPiece(&board[0][0], x, x_max, y_max, &turnCounter, player, inARow); 
            //start = clock(); //start timer for display board
            displayBoard(&board[0][0], x_max, y_max);    
            //end = clock(); //end timer for display board                                  
            saveMove(recordedGame, turnCounter, x);
            //start = clock(); //start timer for check win function
            result = checkWin(&board[0][0], inARow, x, y, player, x_max, y_max); 
            //end = clock(); //end timer for check win function
            // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            // printf("It took %f seconds (%f miliseconds) for the check win function to run", cpu_time_used, cpu_time_used * 1000);
            // totalTime = totalTime + cpu_time_used; 
            // printf("\ntotal time used: %f", totalTime);
        }
        if (input == 0) 
        {
            printf("Please enter '-1' to undo or '1' to redo: ");
            scanf( "%d", &inputChar);
            while(inputChar != -1 && inputChar != 1)
            {
                printf("Please enter '-1' to undo or '1' to redo: ");
                scanf("%d", &inputChar);
            }
            if (inputChar == -1) //UNDO
            {
                input = undoSave(&undoRedoStack, recordedGame, &turnCounter, maxTurns); //Remove the save record for the last turn and add it to undo/redo stack
                removePiece(*board, input, x_max, y_max, &turnCounter, player, inARow); //Remove the last turn from the board
                displayBoard(&board[0][0], x_max, y_max);
            }
            if (inputChar == 1) //REDO
            {
                int x;
                int spaceCheck = checkStack(&undoRedoStack);
                if(spaceCheck != -1) //If the undo/redo stack is not empty proceed
                {
                    x = *pop(&undoRedoStack);
                    y = addPiece(&board[0][0], x, x_max, y_max, &turnCounter, player, inARow);
                    displayBoard(&board[0][0], x_max, y_max);                                      
                    saveMove(recordedGame, turnCounter, x);
                    result = checkWin(&board[0][0], inARow, x, y, player, x_max, y_max);
                }
            }
            if ((inputChar != 1) && (inputChar != -1))
            {
                printf("There is no plays to undo or plays saved to 'redo'\n");
            }
        }
    }
    // averageTime = totalTime/turnCounter;
    // printf("\nAverage time used: %f", averageTime);
    time_t t;
    time(&t);
    freeStack(&undoRedoStack);
    strftime(time_string, 20, "%Y-%m-%d-%H-%M-%S", localtime(&t));
    sprintf(fileName, "%s.txt", time_string);
    saveGame(fileName, recordedGame, turnCounter);
    return 0;
}

void init_stack(struct stack *undoRedoStack, int maxTurns) //Init the stack to store the undo moves to allow for redo functionality
{
    undoRedoStack->array = malloc(sizeof (int) * maxTurns);
    undoRedoStack->top = -1;
}

int getUndoRedoInput(int maxValue) //Get user input including and verifying within required range - includes undo/redo options
{
    int intInput = 0;
    scanf("%d", &intInput);
    while (!((intInput <= maxValue) && (intInput >= 0)))
    {
        printf("Error! your number is out of board range, please enter a number between 1 and %d OR 0 to undo/redo: ", maxValue);
        scanf("%d", &intInput);
    }
    return intInput;
}

int undoSave(struct stack *undoRedoStack, int *recordedGame, int *turnCounter, int maxTurns) //Removes previous moves from the save record and adds them to the undo/redo stack
{
    int stackValue = recordedGame[(*turnCounter) -1];
    push(undoRedoStack, stackValue, maxTurns);
    recordedGame[*turnCounter] = 0;
    return stackValue;
}

int removePiece(char *board, int input, int x_max, int y_max, int *turnCounter, char player, int inARow) //Removes previous moves from the board 
{
    int y;
    for (y = y_max; y >= 0; y--) //Reverse search through the X column for a space that is not empty
    {
        if ((y <= y_max - 1) && *getBoardLoc(board, input, y, x_max, y_max) != ' ')
        {
            *getBoardLoc(board, input, y, x_max, y_max) = ' ';
            (*turnCounter)--; 
            return y;
        }
    }
    printf("There is no entries in this column\n");
    return -1; 
}

void push(struct stack *undoRedoStack, int item, int maxTurns) //Adds the removed move to the stack for storage if redo is required
{
    if (undoRedoStack->top == maxTurns -1)
    {
        printf("stack is full, cannot push to stack");
        return;
    }
    undoRedoStack->top++;
    undoRedoStack->array[undoRedoStack->top] = item;
}

int *pop(struct stack *undoRedoStack) //Remove the top move from the stack
{
    int *data;
    if(undoRedoStack->top == -1)
    {
        printf("The stack is empty");
        return NULL;
    }
    data = &undoRedoStack->array[undoRedoStack->top];
    printf("Replaying move: %d", (*data) + 1);
    undoRedoStack->top--;
    return data;
}

int checkStack(struct stack *undoRedoStack) //Check to see if there is contents within the undo/redo stack
{
    if(undoRedoStack->top == -1)
    {
        printf("The stack is empty! Nothing to redo, lets try that again...\n");
        return -1;
    }
    return 1;
}

void clearStack(struct stack *undoRedoStack) //Clear the contents of the stack and occurs if the user moves using a normal 1-x input
{
    if (undoRedoStack) 
    { 
        undoRedoStack->top = -1; 
    }     
}

void freeStack(struct stack *undoRedoStack) //If a play is done while the playStack is full then it must be cleared
{
    if (undoRedoStack) 
    {  
        free(undoRedoStack->array); 
    } 
}

void printStack(struct stack *undoRedoStack) //Print the contents of the stack - used for debugging
{
    printf("top is: %i\n", undoRedoStack->top);
    if (undoRedoStack->top == -1){
        printf("nothing in the stack\n");
        return;
    }
    for (int x =0; x <= undoRedoStack->top; x++){
        printf("%i |", undoRedoStack->array[x]);
    }
    printf("\n");
}
