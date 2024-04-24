// Coders: Christian Hubner, Jacob Toxtle-Garcia
// Class: COP 2006-13969, Spring 2024
// Date: March 28, 2024
// Description: Group Project, write a C++ program for a battleship game with AI, solo, and two player options


//Known errors: Poor error handling (In-Game)
// ship strike error in single player (2 as well)
//poor user-friendly in 2 player (spacers)


#include <iostream>
//Library to Seed the Random Number generator to give  actually random numbers
#include <ctime>
//This library includes the random number generator we use for this program
#include <cstdlib>
#include <string>
//These libraries refer to important containers to be used
#include <vector>
#include <stack>

//We declare a class of type boat
class Boat
{
private:
//Boats have names,
    std::string name;

//These values are the same and are seperated to cause headaches
    int health;
    int length;

    //Direction is used to store the orientation fo the boat
    //Down or Right
    char dirc;

    //These vectors will store the spaces of the boat
    std::vector<int> shipCol;
    std::vector<int> shipRow;

public:

    //Basic constructor to create and later handle boats
    Boat(std::string Ship_Name,int HP, int Ship_Size, char Direction, std::vector<int>Col,std::vector<int>Row)
    {
        name=Ship_Name;

        health=HP;
        length=Ship_Size;

        dirc=Direction;

        shipCol=Col;
        shipRow=Row;
    }
    //Member function to return a boat's name
    std::string Boat_Name()
    {
        return name;
    }
    //This function will decrease a ship's health
    void Record_Hits()
    {
        health--;
    }
    //This function returns a ship's current health
    int Check_Ship_Health()
    {
        return health;
    }

    //This function is used by Play AI and returns the boat on a space
    std::string GetBoat(int r, int c)
    {
        for(int i=0; i<length; i++)
        {
            //if a ship's position matches the user's input space, return the name of the ship
            if((shipRow[i]==r) && (shipCol[i]==c))
            {
                return name;
            }
        }
        return "";
    }

};

//------------------------------------------------------------------------------------------------

//Here we have all user-defined function initialized and declared beforehand,
//This saves some time and keeps us organized, Some variable names are reused to avoid
//headaches,

//Note: Size and Length mean the same thing. D, Direc, Dirct, etc. refer to orientation

//an essential function intended to manipulate a board and create boats,
//Literally set up the game
void Set_up_1(int grid[][10], std::vector<Boat> &Boat_list10);

//Fill the board with 0 in all spaces, signifying empty
void ReSet_Board(int Board[][10]);

//This function processes each boat's postion and
void Establish(int size, int Board[][10], std::stack<int> &Coordinates);

//Get random direction
char direction(int Number);
//random location
int Location(int col, int &row, int size, char d);
//check if spaces are empty
int Is_Empty(int grid[][10], int col, int row, int length,char dirc);
//place the boat on the array/board
void Edit_Grid(int grid[][10], int col, int row, int length, char dirct);
//Pack up the important information about the ship into a four-digit number
int Pack_UP_Coordinates(int col, int row, int length, char direct);

//Self-explanatory function
void Print_Board_Ships(int grid[][10]);

//Create the boat objects by unpacking the info from the four-digit number.
void Create_Boats(std::vector<Boat> &Boat_list_123,std::stack<int>&Coordinates);
void UnPack_Coordinates(int &col, int &row, int &size, int Coor, char &Direc);

//Play alone
void Play_Alone(int grid[][10],std::vector<Boat>&Boat_list_1234);
//play against the computer
void Play_AI(int playerGrid[][10], int aiGrid[][10], std::vector<Boat>& playerBoats, std::vector<Boat>& aiBoats);

//print our game board with the boats hidden until strucked
void Print_GameBoard(int grid[][10]);
//get space on array from user inputs
int Get_Space(int grid[][10], int row, int col);

//Set up 2 will allow two players to create their own boards to play together
void Set_up_2(int grid[][10],int grid2[][10],std::vector<Boat>&Ship_list_1p,std::vector<Boat>Ship_list_2p);
//Play with my nonexistent friends
void Play_Wth_Friends(int grid[][10],int grid2[][10],std::vector<Boat>&Ship_list_1p,std::vector<Boat>Ship_list_2p);
//A version of Print_Gameboard which prints two boards, One with the player's ships, another hiding the enemy's ships
void Print_2_GameBoard(int grid[][10],int grid2[][10]);
//------------------------------------------------------------------------------------------------
//important variables to refer to a ship's health and size more easily
enum Warship {DESTROYER=2, CRUISER=3, BATTLESHIP=4, CARRIER=5};

//main function
int main()
{
    //Two arrays to contain a board each are initialized
    int Gameboard[10][10],Gameboard2[10][10];
    //Two vectors are our containers to handle referring to and manipulating our boat objects
    std::vector<Boat>Warships;
    std::vector<Boat>Warshipz;

    //Christian's variables for Play AI
    int playerGrid[10][10], aiGrid[10][10], hitGrid[10][10];
    std::vector<Boat> playerBoats;
    std::vector<Boat> aiBoats;

    //welcome the user and ask if they want to play and choose a game mode using switch cases
    std::cout<<"Welcome to Battleship. Input [K] to Play! Any Other inputs will exit the program"<<std::endl;
    char Choice1;
    int Choice2;
    std::cin>>Choice1;
    if (Choice1=='K'||Choice1=='k')
    {
        std::cout<<"Please input a single character for the appropriate choice"
                   "\n[1] For Single Player"
                   "\n[2] For Single Player vs Computer"
                   "\n[4] For Two Player, Pass-and-Play"<<std::endl;
        std::cin>>Choice2;
        switch (Choice2)
        {
            case 1:
                //Play alone only uses 1 board and 1 vector
                Set_up_1(Gameboard, Warships);
                Play_Alone(Gameboard,Warships);
                break;
            case 2:
                //Play Ai will use 2 boards and 2 vectors
                std::cout<<"Please set up your board.";
                Set_up_1(playerGrid, playerBoats); // Set up the player's board
                std::cout<<"Please set up the AI's board.";
                Set_up_1(aiGrid, aiBoats); // Set up the AI's board

                Play_AI(playerGrid, aiGrid, playerBoats, aiBoats);
                break;
            case 4:
                //Play with friends will also use 2 array boards and 2 vectors with ships
                Set_up_2(Gameboard,Gameboard2,Warships,Warshipz);
                Play_Wth_Friends(Gameboard,Gameboard2,Warships,Warshipz);
                break;
        }

    }
    //most other inputs lead to quitting the game
    return 0;
}
//------------------------------------------------------------------------------------------------

void Play_AI(int playerGrid[][10], int aiGrid[][10], std::vector<Boat>& playerBoats, std::vector<Boat>& aiBoats)//defines the function "Play_AI" as needing a player grid, ai grid, and two seperate lists of the same pre-defined boats
{
    bool playerTurn = true; //starts the game with the player's turn
    bool gameRunning = true; //variable that tracks if the game is active or over, starts as active
    int playerGuesses = 0, aiGuesses = 0; //Initializes variables that track the player's and ai's total turns
    int playerTotalHits = 0, aiTotalHits = 0; //Initializes variables that track the player's and ai's hits
    int playerMisses = 0, aiMisses = 0; //Initializes variables that track the player's and ai's misses

    while (gameRunning)//This is the main part of the custom function which says that while the game has not ended the player and ai should keep taking turns
    {
        if (playerTurn)//When it is the player's turn
        {
            Print_GameBoard(playerGrid);//The grid the player is using is printed
            std::cout << "Player's turn. "; //Displays that it is the player's turn
            int row1=13; //Initializes row input variable for user input and defines it so it will trigger the input while loop
            int col1=13; //Initializes column input variable for user input and defines it so it will trigger the input while loop
            while ((row1 <= 0 || row1 >= 9)&&(col1 <= 0 || col1 >=9)) //This while loop will run until the user enters a valid coordinate as a selection
            {
                std::cout << "Enter row to attack: "; // Outputs that the user should select a row
                std::cin >> row1; // Stores the user input in their row variable
                std::cout << "Enter column to attack: "; // Outputs that the user should select a column
                std::cin >> col1; // Stores the user input in their column variable
                if ((row1 <= 0 || row1 >= 9)&&(col1 <= 0 || col1 >=9)) //If the user does not input a valid grid coordinate
                {
                    std::cout<<"Incorrect input, please try again\n"; //Outputs that an incorrect input has been entered
                }
            }

            int space = Get_Space(aiGrid, row1, col1); // initializes a variable called "space" that checks if the coordinate on the grid is empty, has a ship, or has already been hit
            if (space == 0) // If the space is functionally empty for the game
            {
                std::cout << "Player missed!" << std::endl; //Outputs that the player has missed
                aiGrid[row1][col1] = 9; //Marks a miss on the displayed "aiGrid"
                playerMisses++; //Adds a miss to the player's total
            }
            else if (space != 0 && space != 9) //If the space is not empty and the player has not missed
            {
                std::cout << "Player hit AI's ship!" << std::endl; // Then it displays that the player has hit a ship
                aiGrid[row1][col1] = 1; //Marks a hit on the displayed "aiGrid"
                playerTotalHits++; //Adds a hit to the player's total
                for (auto& boat : aiBoats) // Checks throughout the "aiBoats"
                {
                    if (boat.GetBoat(row1, col1) != "") //If the space contains a ship
                    {
                        std::cout << "Player has struck the " << boat.Boat_Name() << "!" << std::endl; //Displays the name of the ship the player has hit
                        boat.Record_Hits(); //tracks that the boat has been hit by the player
                        if (boat.Check_Ship_Health() == 0) // If the boat has been hit on all of its spaces
                        {
                            std::cout << "Player has sunk the " << boat.Boat_Name() << "!" << std::endl; //Display that the boat has been destroyed
                        }
                        break;
                    }
                }
            }
            else if (space == 9) //If the space has already been hit
            {
                std::cout << "Player wasted a guess!" << std::endl;//Tell the player they have wasted a guess
            }

            playerGuesses++; //Increase the number of turns the player has taken
            if (aiTotalHits == 17) //If the ai has hit 17 times
            {
                std::cout << "AI wins!" << std::endl; //Display that the ai has won
                gameRunning = false; //Stop taking turns
            }

            playerTurn = false;// Ends the players turb
        }
        else
        {
            Print_GameBoard(aiGrid);// Prints the board the computer opponent is using
            int row2 = rand() % 10; //Uses a random number to pick a row
            int col2 = rand() % 10; //Uses a random number to pick a column

            int space = Get_Space(playerGrid, row2, col2); // initializes a variable called "space" that checks if the coordinate on the grid is empty, has a ship, or has already been hit
            if (space == 0) //If the space is functionally empty for the game
            {
                std::cout << "AI missed at row " << row2 << " and column " << col2 << "!" << std::endl; //Display that the computer opponent has missed
                playerGrid[row2][col2] = 9; //Marks a miss on the displayed "playerGrid"
                aiMisses++; //Increases the count of misses for the computer opponent
            }
            else if (space != 0 && space != 9) //If the space is not empty or a previous miss
            {
                std::cout << "AI hit player's ship at row " << row2 << " and column " << col2 << "!" << std::endl; //Display that the computer opponent has hit the player's ship
                playerGrid[row2][col2] = 1; // marks a hit on the displayed "playerGrid"
                aiTotalHits++; // Increases the count of hits for the computer opponent
                for (auto& boat : playerBoats) //Searches through "playerBoats"
                {
                    if (boat.GetBoat(row2, col2) != "") //If the space contains a ship
                    {
                        std::cout << "AI has struck the " << boat.Boat_Name() << "!" << std::endl; //Displays which ship the computer opponent has hit
                        boat.Record_Hits(); //tracks that the computer opponent has hit the ship
                        if (boat.Check_Ship_Health() == 0) //If the boat has been hit in all of its spaces
                        {
                            std::cout << "AI has sunk the " << boat.Boat_Name() << "!" << std::endl; //Display that the computer opponent has sunk the ship
                        }
                        break;
                    }
                }
            }
            else if (space == 9) // If the space has previously been chosen
            {
                std::cout << "AI wasted a guess!" << std::endl; //Display that the computer opponent wasted a turn
            }

            aiGuesses++; //Increase the count of turns for the computer opponent
            if (playerTotalHits == 17) // If the payer has destroyed all of the computer's ships
            {
                std::cout << "Player Wins!" << std::endl; //Display that the player has won
                gameRunning = false; //Stops taking turns for the game
            }

            playerTurn = true; //makes it the player's turn again
        }
    }

    std::cout << "Player made " << playerGuesses << " guesses with " << playerMisses << " misses." << std::endl; // Displays the player's guesses and misses
    std::cout << "AI made " << aiGuesses << " guesses with " << aiMisses << " misses." << std::endl; //Displays the computer opponent's guesses and misses
}

//------------------------------------------------------------------------------------------------

void Set_up_1(int grid[][10], std::vector<Boat> &Boat_list1)
{
    //init and dec important variables for the user
    char Choice3, Choice4;
    //a stack to handle the important numbers related to our boats
    std::stack<int> Coordinates;
    //allows our do-while to run properly
    bool ReRole=true;

    //The purpose of this loop is to allow the user the option to re-roll his or her ship positions
    do
    {
        //Clean the board
        ReSet_Board(grid);

        //Place "Ship type" on the "Game board" and record it's "position"
        Establish(DESTROYER, grid, Coordinates);
        Establish(CRUISER, grid, Coordinates);
        Establish(CRUISER, grid, Coordinates);
        Establish(BATTLESHIP, grid, Coordinates);
        Establish(CARRIER, grid, Coordinates);

        //ask the user if they want to see the board
        std::cout<<" Would you like to see the Board with ships?"
                   "\n [y] to see the complete board with ships!"
                   "\n [n] to continue!"<<std::endl;
        std::cin>>Choice4;

        switch (Choice4)
        {
            case 'y':
            case 'Y':
                Print_Board_Ships(grid);
                //ask if the user likes the board if not then reshuffle the board
                std::cout << "Would You like to play this Board?"
                             "\n [y] to start the game!"
                             "\n [n] to reshuffle the board!" << std::endl;
                std::cin >> Choice3;
                switch (Choice3)
                {
                    case 'y':
                    case 'Y':
                        ReRole = false;
                        break;
                    case 'n':
                    case 'N':
                        continue;
                }
                break;
            case 'n':
            case 'N':
                ReRole=false;
                break;
        }
    } while (ReRole);
    //if the board/array is good, then create the boat objects using the stack from earlier
    Create_Boats(Boat_list1, Coordinates);
}

void ReSet_Board(int Board[][10])
{
    //fill each and every space of the array with a 0 to mean empty
    for(int col=0; col<10; col++) //Outer column loop
    {
        for(int row=0; row<10; row++) //Inner row loop
        {
            Board[col][row]=0;
        }
    }
}
void Establish(int size, int Board[][10], std::stack<int> &Coordinates)
{
    //seed the number generator so results are a bit random
    srand(time(0));

    //some important variables to handle placing and recording the ship's position
    int Free_Real_Estate, row=0, col=0, Cereal_Num=0;
    char dirc;


    dirc=direction(rand()%10);  //randomly pick which direction to place boat
    col=Location(col, row, size, dirc);//Random Location

    //This loop will 'loop' if a fail condition is met (boat overlap)
    while(true)
    {
        //Horizontal direction or going right
        if (dirc == 'h')
        {
            //check if the space we generated is empty or not
            Free_Real_Estate=Is_Empty(Board, col, row, size, dirc);
            if (Free_Real_Estate==1)//if the boat overlaps, generate another random column, row and direction and start the loop again
            {
                //Get another random location and direction
                dirc = direction(rand() % 10);
                col = Location(col, row, size, dirc);
                //loop back around
                continue;
            }
            //if the spaces are empty then edit the board (uses the ship's size as the
            // indicating number)
            Edit_Grid(Board, col, row, size, dirc);//place the boat on the grid
            //pack up the important info about the ship for later
            Cereal_Num=Pack_UP_Coordinates(col,row,size,dirc);
            //place the coordinates of the ship in the stack
            Coordinates.push(Cereal_Num);
            break;
        }
            //vertical direction or going down
        else if (dirc == 'v')
        {
            //check if the space we generated is empty or not
            Free_Real_Estate = Is_Empty(Board, col, row, size, dirc);
            if (Free_Real_Estate==1)
            {
                //Get another random location and direction
                dirc=direction(rand() % 10);
                col=Location(col, row, size, dirc);
                //loop back around
                continue;
            }
            //if the spaces are empty then edit the board with the ship (uses the ship's size as the
            // indicating number)
            Edit_Grid(Board, col, row, size, dirc);
            //pack up the important info about the ship for later
            Cereal_Num=Pack_UP_Coordinates(col,row,size,dirc);
            //place the coordinates of the ship in the stack
            Coordinates.push(Cereal_Num);
            break;
        }
    }
}
void Create_Boats(std::vector<Boat> &Boat_list_123, std::stack<int>&Coordinates)
{
    //Init and declare important vars about for the boat objects
    int COL, ROW, LENGTH;
    char DIRECTION;

    //Begin to unpack the stack and create boats based on the numbers inside
    while (!Coordinates.empty())
    {
        int COORDINATES = Coordinates.top();
        Coordinates.pop();
        //Unpack the four-digit code to process the details about the boat
        UnPack_Coordinates(COL, ROW, LENGTH, COORDINATES, DIRECTION);

        switch (LENGTH)
        {
            //All cases accomplish the same function to create a ship
            //Case for the destroyer
            case 2:
            {
                //Check direction of the ship
                if(DIRECTION=='h')
                {
                    //Init a vector for rows
                    std::vector<int> r1 (LENGTH);
                    //for loop to fill the vector with the row number the boat is in
                    for (int i=0; i<(int)r1.size(); ++i)
                    {
                        r1.at(i)=ROW;
                    }
                    //init vector for boat columns
                    std::vector<int> c1 (LENGTH);
                    //for loop to fill the vector with the columns the boat is found in
                    for (int i=0; i<(int)c1.size(); ++i)
                    {
                        c1.at(i)=COL;
                        COL++;
                    }
                    //Finally finalize the details and create an object for this boat
                    Boat Destroyer("Destroyer",LENGTH,LENGTH,DIRECTION,c1,r1);
                    //place boat in a container for later reference
                    Boat_list_123.push_back(Destroyer);
                }
                //if the boat is vertical...
                else if(DIRECTION=='v')
                {
                    //Our vector will have mutliple rows to be in
                    std::vector<int> r1 (LENGTH);
                    //for loop to place row numbers in vector
                    //also ++i is a thing, pretty cool right? learned that from a friend
                    for (int i=0; i<(int)r1.size(); ++i)
                    {
                        r1.at(i)=ROW;
                        ROW++;
                    }
                    //we only need one space for the column, but it still needs to be in a vector
                    std::vector<int> c1 (LENGTH);
                    for (int i=0; i<(int)c1.size(); ++i)
                    {
                        c1.at(i)=COL;
                    }
                    //Create a boat object called destroyer with every important detail
                    Boat Destroyer("Destroyer",LENGTH,LENGTH,DIRECTION,c1,r1);
                    //place boat in container for later reference
                    Boat_list_123.push_back(Destroyer);
                }
                break;
            }
            //case for sub and cruisers
            case 3:
            {
                // the sub is the second boat to be created, so the container should only have
                // 1 ship inside currently, then create the sub as the second ship
                //note may need to fix later this if else ladder, (was it 2 or 3?)
                if (Boat_list_123.size()<2)
                {
                    if (DIRECTION == 'h') {
                        std::vector<int> r1(LENGTH);
                        for (int i = 0; i < (int) r1.size(); ++i) {
                            r1.at(i) = ROW;
                        }
                        std::vector<int> c1(LENGTH);
                        for (int i = 0; i < (int) c1.size(); ++i) {
                            c1.at(i) = COL;
                            COL++;
                        }

                        Boat Submarine("Submarine", LENGTH, LENGTH, DIRECTION, c1, r1);
                        Boat_list_123.push_back(Submarine);
                    }
                    else if (DIRECTION == 'v') {
                        std::vector<int> r1(LENGTH);
                        for (int i = 0; i < (int) r1.size(); ++i) {
                            r1.at(i) = ROW;
                            ROW++;
                        }
                        std::vector<int> c1(LENGTH);
                        for (int i = 0; i < (int) c1.size(); ++i) {
                            c1.at(i) = COL;
                        }
                        Boat Submarine("Submarine", LENGTH, LENGTH, DIRECTION, c1, r1);
                        Boat_list_123.push_back(Submarine);
                    }
                }
                //else create the cruiser as the third ship
                else
                {
                    if (DIRECTION == 'h')
                    {
                        std::vector<int> r1(LENGTH);
                        for (int i = 0; i < (int) r1.size(); ++i)
                        {
                            r1.at(i) = ROW;
                        }
                        std::vector<int> c1(LENGTH);
                        for (int i = 0; i < (int) c1.size(); ++i)
                        {
                            c1.at(i) = COL;
                            COL++;
                        }
                        Boat Cruiser("Cruiser", LENGTH, LENGTH, DIRECTION, c1, r1);
                        Boat_list_123.push_back(Cruiser);

                    }
                    else if (DIRECTION == 'v')
                    {
                        std::vector<int> r1(LENGTH);
                        for (int i = 0; i < (int) r1.size(); ++i)
                        {
                            r1.at(i) = ROW;
                            ROW++;
                        }
                        std::vector<int> c1(LENGTH);
                        for (int i = 0; i < (int) c1.size(); ++i)
                        {
                            c1.at(i) = COL;
                        }
                        Boat Cruiser("Cruiser", LENGTH, LENGTH, DIRECTION, c1, r1);
                        Boat_list_123.push_back(Cruiser);
                    }
                }
                break;
            }
            //case for battleship
            case 4:
            {
                if (DIRECTION == 'h')
                {
                    std::vector<int> r1(LENGTH);
                    for (int i = 0; i < (int) r1.size(); ++i)
                    {
                        r1.at(i) = ROW;
                    }
                    std::vector<int> c1(LENGTH);
                    for (int i = 0; i < (int) c1.size(); ++i)
                    {
                        c1.at(i) = COL;
                        COL++;
                    }
                    Boat Battleship("Battleship", LENGTH, LENGTH, DIRECTION, c1, r1);
                    Boat_list_123.push_back(Battleship);

                }
                else if (DIRECTION == 'v')
                {
                    std::vector<int> r1(LENGTH);
                    for (int i = 0; i < (int) r1.size(); ++i)
                    {
                        r1.at(i) = ROW;
                        ROW++;
                    }
                    std::vector<int> c1(LENGTH);
                    for (int i = 0; i < (int) c1.size(); ++i)
                    {
                        c1.at(i) = COL;
                    }
                    Boat Battleship("Battleship", LENGTH, LENGTH, DIRECTION, c1, r1);
                    Boat_list_123.push_back(Battleship);
                }
                break;
            }
            //case for aircraft carrier
            case 5:
            {
                if (DIRECTION == 'h')
                {
                    std::vector<int> r1(LENGTH);
                    for (int i = 0; i < (int) r1.size(); ++i)
                    {
                        r1.at(i) = ROW;
                    }
                    std::vector<int> c1(LENGTH);
                    for (int i = 0; i < (int) c1.size(); ++i)
                    {
                        c1.at(i) = COL;
                        COL++;
                    }
                    Boat Carrier("Aircraft Carrier", LENGTH, LENGTH, DIRECTION, c1, r1);
                    Boat_list_123.push_back(Carrier);

                }
                else if (DIRECTION == 'v')
                {
                    std::vector<int> r1(LENGTH);
                    for (int i = 0; i < (int) r1.size(); ++i)
                    {
                        r1.at(i) = ROW;
                        ROW++;
                    }
                    std::vector<int> c1(LENGTH);
                    for (int i = 0; i < (int) c1.size(); ++i)
                    {
                        c1.at(i) = COL;
                    }
                    Boat Carrier("Aircraft Carrier", LENGTH, LENGTH, DIRECTION, c1, r1);
                    Boat_list_123.push_back(Carrier);
                }
                break;
            }
        }
    }
}
//--------------------------------------------------------------------------------------------------
//Establish();
int Location(int col, int &row, int size, char d)
{
    switch(size)
    {
        //generate random numbers for the ships position
        //this value depends on the ships location to determine a place that WILL
        //NOT accidentally go 'OVERBOARD'
        //   0 1 2 3 4 5 6 7 8 9
        //  |_|_|_|_|_|_|_|_|_|4|4 4 4
        //Whoops now the battleship will never be hit!

        case DESTROYER:
            if(d=='h')
            {
                col=rand()%8;
                row=rand()%10;
            }
            else
            {
                col=rand()%10;
                row=rand()%8;
            }
            break;
        case CRUISER:
            if(d=='h')
            {
                col=rand()%7;
                row=rand()%10;
            }
            else
            {
                col=rand()%10;
                row=rand()%7;
            }
            break;
        case BATTLESHIP:
            if(d=='h')
            {
                col=rand()%6;
                row=rand()%10;
            }
            else
            {
                col=rand()%10;
                row=rand()%6;
            }
            break;
        case CARRIER:
            if(d=='h')
            {
                col=rand()%5;
                row=rand()%10;
            }
            else
            {
                col=rand()%10;
                row=rand()%5;
            }
    }
    //Check If needed
    return col;
}
char direction(int Number)
{
    // if random number is greater than 4 then we go right
    if(Number>4)
    {
        return 'h';  //randomly pick which direction to place boat
    }
    //else we go down
    else
    {
        return 'v';
    }
}
int Is_Empty(int grid[][10], int col, int row, int length, char dirc)
{
    int check;
    //depending on ship direction
    if(dirc=='h')
    {//a for loop to explore the length of ship on the board
        for(int i=col; i<col+length; i++)
        {
            //check if spaces are 0 or empty
            check=grid[row][i];
            if(check!=0)
            {
                //if all is well return 1 or true
                return 1;
            }
        }
        //return 0 or false
        return 0;
    }
    //same process as before for vertical orientation
    else
    {
        for(int i=row; i<row+length; i++)
        {
            check=grid[i][col];
            if(check!=0)
            {
                return 1;

            }
        }
        return 0;
    }
}
void Edit_Grid(int grid[][10], int col, int row, int length, char dirct)
{
    //using the direction and length of the ship we will mark the array
    //where the ship is
    if(dirct=='h')
    {
        for(int i=0; i<length; i++)
        {
            grid[row][col]=length;
            col++;
        }
    }
    //vertical
    else if(dirct=='v')
    {
        for(int i=0; i<length; i++)
        {
            grid[row][col]=length;
            row++;
        }
    }
}

//---------------------------------------------------------------------------------------------------
//General Code:
// #_#_#_#  ==   Direction,Size,Row,Col

int Pack_UP_Coordinates(int col, int row, int length, char direct)
{
    //we will store the ship's beginning point, direction and size in one number
    int Result;
    Result=(length*100);
    Result+=(row*10);
    Result+=(col);
    //debugging area - "THIS failed 12 times, so this places exist to remind
    // me of my nightmares"
    int Debug_Flag=Result;

    switch(direct)
    {
        case 'h':
        {
            Result += 1000;
            break;
        }
        case 'v':
        {
            Result += 2000;
            break;
        }
    }
    return Result;
}
void UnPack_Coordinates(int &col, int &row, int &size, int Coor, char &Direc)
{
    //Simple number deconstructer to append ptrs with valid information about the ships
    //idea from reverse a number project
    Direc = (Coor >= 2000) ? 'v' : 'h';

    size = (Coor / 100) % 10;

    col = Coor % 10;

    row = Coor / 10 % 10;
}
//Get space from the board and return what's innit
int Get_Space(int grid[][10], int row, int col)
{
    //get space from user inputs
    int space = grid[row][col];
    return space;
}

//PRINT PLAYING BOARD == TO HIT!
void Print_GameBoard(int grid[][10])
{

    std::cout << "   0|1|2|3|4|5|6|7|8|9" << std::endl;
    for(int i = 0; i < 10; i++)  // column loop
    {
        for(int j = 0; j < 10; j++)  // row loop
        {
            if(j == 0)
            {
                std::cout << i << " |"; // print row number and spaces before new row
            }
            if(grid[i][j] == 1)  // if the space is a hit, print 'X|'
            {
                std::cout << "X|";
            }
            else if(grid[i][j] == 9)  // if the space is a miss, print '0|'
            {
                std::cout << "0|";
            }
            else // if the space is empty, print '_'
            {
                std::cout << "_|";
            }
        }
        std::cout << std::endl; // new line at end of column
    }
    std::cout << std::endl;
}
//PRINT PLAYER BOARD == DEFEND!
void Print_Board_Ships(int grid[][10])
{
    std::cout << "  |0|1|2|3|4|5|6|7|8|9|" << std::endl;
    for(int i=0; i<10; i++)  //column loop
    {
        for(int j=0; j<10; j++)  //row loop
        {
            if(j==0)
            {
                std::cout << i << " |" ; //print row number and spaces before new row
            }
            if (grid[i][j]==0)//empty space
            {
                std::cout<<"_|";
            }
            else if (grid[i][j]!=0)//print the number related to the boat
            {
                std::cout << grid[i][j]<<"|";
            }
        }
        std::cout << std::endl; //new line at end of column
    }
    std::cout << std::endl;
}
//PINT A BOARD DEFENDING AND ATTACKING BOARD
void Print_2_GameBoard(int grid[][10], int grid2[][10]) {
    //print player board
    std::cout << "  |0|1|2|3|4|5|6|7|8|9|" << std::endl;
    for (int i = 0; i < 10; i++)  //column loop
    {
        for (int j = 0; j < 10; j++)  //row loop
        {
            if (j == 0) {
                std::cout << i << " |"; //print row number and spaces before new row
            }
            if (grid[i][j] == 0) //empty space
            {
                std::cout << "_|";
            } else if (grid[i][j] != 0) //print ship position
            {
                std::cout << grid[i][j] << "|";
            }
        }
        std::cout << std::endl; //new line at end of column
    }
    std::cout << std::endl;
    //-------------------
    std::cout << "   0|1|2|3|4|5|6|7|8|9" << std::endl;
    for (int i = 0; i < 10; i++)  //column loop
    {
        for (int j = 0; j < 10; j++)  //row loop
        {
            if (j == 0) {
                std::cout << i << " |"; //print row number and spaces before new row
            }
            if (grid2[i][j] == 1)  //if the space is a hit, print it
            {
                std::cout << "X|";
            } else if (grid2[i][j] == 9)  //if the space is a miss, print it
            {
                std::cout << "0|";
            } else {
                std::cout << "_|";  //otherwise, just print a 0
            }
        }
        std::cout << std::endl; //new line at end of column
    }
}
//---------------------------------------------------------------------------------------------------
//SinglePlayer NO Computer
void Play_Alone(int grid[][10],std::vector<Boat>&Boat_list_1234)
{
    //NOTE: I like switch cases, they haven't failed me yet... I was blind to easier ways

    //This part of the code is similar to inspiration code with a few adjustments

    //all needed vars to run this game properly
    bool Run_Game=true;
    int row=0,col=0;

    //These vars explain their purposes pretty well
    int Guesses=0,Total_Hits=0,Misses=0,Space=0;

    //Store boat name for the game
    std::string Boat_Name;

    //run the game in a while loop
    while(Run_Game)
    {
        //print board
        Print_GameBoard(grid);

        //ask user to input a space to strike
        //(will add some error handing here) 0-9
        std::cout << "Enter a row coordinate: ";
        std::cin >> row;
        std::cout << "Enter a column coordinate: ";
        std::cin >> col;
        std::cout << std::endl;

        //add to guesses made and get the space from array
        Guesses++;
        Space=Get_Space(grid, row, col);


        switch(Space) {
            case 0://the space is empty record miss
                std::cout << "You have missed" << std::endl;
                grid[row][col] = 9;
                Misses++;
                break;

            case 1://space has been hit, user wasted a guess
            case 9:
                std::cout << "This space has already been hit. You have wasted a guess!" << std::endl;
                break;


            case 2://strike on the destroyer
                grid[row][col] = 1;
                Total_Hits++;

                //retrieve info on struck ship using the container/vector
                std::cout << "You have struck the " << Boat_list_1234[4].Boat_Name() << "!" << std::endl;
                Boat_list_1234[4].Record_Hits();

                //check if ship has been sunk yet
                if (Boat_list_1234[4].Check_Ship_Health() == 0) {
                    std::cout << "You have sunk the "<<Boat_list_1234[4].Boat_Name() << "!" << std::endl;
                }
                break;

            case 3:
                //strike on cuiser or sub
                grid[row][col] = 1;
                Total_Hits++;

                //potential critical error: wrong ship stiked
                //replace with get boat to avoid wrong boat, remedy by max hits == 17
                Boat_Name=Boat_list_1234[3].Boat_Name();
                if (Boat_Name=="Cruiser")
                {
                    std::cout << "You have struck the " << Boat_list_1234[3].Boat_Name() << "!" << std::endl;
                    Boat_list_1234[3].Record_Hits();
                    if (Boat_list_1234[3].Check_Ship_Health() == 0) {
                        std::cout << "You have sunk the "<<Boat_list_1234[3].Boat_Name() << "!" << std::endl;
                    }
                }
                else
                {
                    std::cout << "You have struck the " << Boat_list_1234[2].Boat_Name() << "!" << std::endl;
                    Boat_list_1234[2].Record_Hits();
                    if (Boat_list_1234[2].Check_Ship_Health() == 0) {
                        std::cout << "You have sunk the "<<Boat_list_1234[2].Boat_Name() << "!" << std::endl;
                    }
                }
                Boat_Name.clear();
                break;
            case 4:
                //stike battleship
                grid[row][col] = 1;
                Total_Hits++;
                std::cout << "You have struck the " << Boat_list_1234[1].Boat_Name() << "!" << std::endl;
                Boat_list_1234[1].Record_Hits();
                if (Boat_list_1234[1].Check_Ship_Health() == 0) {
                    std::cout << "You have sunk the "<<Boat_list_1234[1].Boat_Name() << "!" << std::endl;
                }
                break;
            case 5:
                //strike aircraft carrier
                grid[row][col] = 1;
                Total_Hits++;
                std::cout << "You have struck the " << Boat_list_1234[0].Boat_Name() << "!" << std::endl;
                Boat_list_1234[0].Record_Hits();
                if (Boat_list_1234[0].Check_Ship_Health() == 0) {
                    std::cout << "You have sunk the "<<Boat_list_1234[0].Boat_Name() << "!" << std::endl;
                }
                break;
        }

        //if the total hits reaches a specified number end the game
        // 2+3+3+4+5 = 17 or all ship health combined
        if(Total_Hits==17)
        {
            Run_Game=false;
        }
    }//end of while game is playing loop

    //end the game
    std::cout<<"Thank you for playing!  You have sunk all the Warships in " << Guesses << " moves!" << std::endl;
    std::cout<<"Congrats on your Victory Commander, Input any key to exit the program";
    int Choice_3;
    std::cin>>Choice_3;
}
//---------------------------------------------------------------------------------------------------

void Set_up_2(int grid[][10],int grid2[][10],std::vector<Boat>&Ship_list_1p,std::vector<Boat>Ship_list_2p)
{
    //potential idea was to add security to game by having users input a code to start their turn
    //std::string Launch_codes[2];
    //set up 2 is similar to set up 1, except each player will set up a board they are
    //satisfied with

    std::cout << "\nWelcome to Two Player Battleship!";

    std::cout << "\nPlayer 1,Let's Place your ships!" << std::endl;

    char Choice4;
    std::stack<int> Coordinates;
    bool ReRole = true;
    do {
        ReSet_Board(grid);
        Establish(DESTROYER, grid, Coordinates);
        Establish(CRUISER, grid, Coordinates);
        Establish(CRUISER, grid, Coordinates);
        Establish(BATTLESHIP, grid, Coordinates);
        Establish(CARRIER, grid, Coordinates);

        Print_Board_Ships(grid);

        std::cout << "Would You like to play this Board?"
                     "\n [y] to Continue!"
                     "\n [n] to reshuffle the board!" << std::endl;
        std::cin >> Choice4;
        switch (Choice4) {
            case 'y':
            case 'Y':
                ReRole = false;
                break;
            case 'n':
            case 'N':
                continue;
        }

    } while (ReRole);
    Create_Boats(Ship_list_1p, Coordinates);

    std::cout << "Thank You, Now You Have Finished setting up Your Ships";

    std::cout << "Please pass the device to Player 2!" << std::endl;
    //a spacer to prevent peeking
    //This where those launch codes would be useful
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

    std::cout << "\nPlayer 2,  Let's Place your ships!" << std::endl;

    //------------------------------------------------------------------------

    char Choice5;
    bool ReRole2 = true;
    do
    {
        //Notice we use grid2 to modify player two's board
        ReSet_Board(grid2);
        Establish(DESTROYER, grid2, Coordinates);
        Establish(CRUISER, grid2, Coordinates);
        Establish(CRUISER, grid2, Coordinates);
        Establish(BATTLESHIP, grid2, Coordinates);
        Establish(CARRIER, grid2, Coordinates);

        Print_Board_Ships(grid2);

        std::cout << "Would You like to play this Board?"
                     "\n [y] to Continue!"
                     "\n [n] to reshuffle the board!" << std::endl;
        std::cin >> Choice5;
        switch (Choice5) {
            case 'y':
            case 'Y':
                ReRole2 = false;
                break;
            case 'n':
            case 'N':
                continue;
        }

    } while (ReRole2);
    Create_Boats(Ship_list_2p, Coordinates);

    //another inconvenient spacer
    //we should really learn sfml
    std::cout << "Thank You, Now You Have Finished setting up Your Ships"<<std::endl;
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

}
void Play_Wth_Friends(int grid[][10],int grid2[][10],std::vector<Boat>&Ship_list_1p,std::vector<Boat>Ship_list_2p)
{
    // play with friends is similar in design to play alone with some exceptions
    //more grids and vectors, mean more variables to handle each players' turn

    bool Run_Game = true;

    //Player 1's vars
    int p1_row = 0, p1_col = 0;
    int p1_Guesses = 0, p1_Total_Hits = 0, p1_Misses = 0, p1_Space = 0;
    std::string msg1="Nothing Happened... Yet";
    //Player 2's vars
    int p2_row = 0, p2_col = 0;
    int p2_Guesses = 0, p2_Total_Hits = 0, p2_Misses = 0, p2_Space = 0;
    std::string msg2="Nothing Happened... Yet";
    //msg is important for concise turn summary

    int turn = 1;

    //store boat name
    std::string Boat_Name;

    do
    {
        //spacer plague this program
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        std::cout << "turn " << turn<<std::endl;
        //instructions to interpret the board
        std::cout<<"\n9 == The Enemy Missed!"
                   "\n1 == Hits on our Ships!"<<std::endl;

        //print the boards
        Print_2_GameBoard(grid, grid2);

        //output what has occurred on the board recently
        std::cout<<"On the Previous turn: "<<msg1<<std::endl;
        msg1.clear();

        //input space
        std::cout << "Enter a row coordinate: ";
        std::cin >> p1_row;
        std::cout << "Enter a column coordinate: ";
        std::cin >> p1_col;
        std::cout << std::endl;

        p1_Guesses++;
        p1_Space = Get_Space(grid2, p1_row, p1_col);

        //after each case we will store the result to be outputted during the next turn
        //This makes the game more like red october, since we can't see what happened until after the turn ends

        switch (p1_Space) {
            case 0://player 1 misses
                msg1="You have missed";
                grid2[p1_row][p1_col] = 9;
                p1_Misses++;
                break;

            case 1:
            case 9://A repeat input
                msg1="This space has already been hit. You have wasted a guess!";
                break;


            case 2:// striked the destoryer
                grid2[p1_row][p1_col] = 1;
                p2_Total_Hits++;

                msg1="You have struck the Destroyer!";
                Ship_list_1p[4].Record_Hits();
                //check sunk
                if (Ship_list_1p[4].Check_Ship_Health() == 0) {
                    msg1="You have sunk the Destroyer!";
                }
                break;

            case 3:
                //strike sub or cuiser
                grid2[p1_row][p1_col] = 1;
                p2_Total_Hits++;

                Boat_Name = Ship_list_1p[3].Boat_Name();
                if (Boat_Name == "Cruiser") {
                    msg1="You have struck the Cruiser!";
                    Ship_list_1p[3].Record_Hits();
                    if (Ship_list_1p[3].Check_Ship_Health() == 0) {
                        msg1="You have sunk the Cruiser!";
                    }
                    Boat_Name.clear();
                } else {
                    msg1="You have struck the Submarine!";
                    Ship_list_1p[2].Record_Hits();
                    if (Ship_list_1p[2].Check_Ship_Health() == 0) {
                        msg1="You have sunk the Submarine!";
                    }
                    Boat_Name.clear();
                }
                Boat_Name.clear();
                break;
                //strike battleship
            case 4:
                grid2[p1_row][p1_col] = 1;
                p2_Total_Hits++;
                msg1="You have struck the Battleship!";
                Ship_list_1p[1].Record_Hits();
                if (Ship_list_1p[1].Check_Ship_Health() == 0) {
                    msg1="You have sunk the Battleship!";
                }
                break;
                //strike aircraft carrier
            case 5:
                grid2[p1_row][p1_col] = 1;
                p2_Total_Hits++;
                msg1="You have struck the Aircraft Carrier!";
                Ship_list_1p[0].Record_Hits();
                if (Ship_list_1p[0].Check_Ship_Health() == 0) {
                    msg1="You have sunk the Aircraft Carrier!";
                }
                break;
        }
        //if player 2 has taken 17 hits they lose the game, and player 1 wins
        if (p2_Total_Hits == 17)
        {
            std::cout << "Player 1 You have WON, You have sunk all the Enemy Warships in " << p1_Guesses << " moves!"
                      << std::endl;
            Run_Game = false;
        }
        //-----------------------------------------------------------------------------------------------------
                //WHY SPACERS?
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

        //---------------------------------

        //the exact previous process is repeated for player 2
        std::cout << "turn " << turn<<std::endl;
        turn++;

        std::cout<<"\n9 == The Enemy Missed!"
                   "\n1 == Hits on our Ships!"<<std::endl;

        Print_2_GameBoard(grid2, grid);

        std::cout<<"On the Previous turn: "<<msg2<<std::endl;
        msg2.clear();

        std::cout << "Enter a row coordinate: ";
        std::cin >> p2_row;
        std::cout << "Enter a column coordinate: ";
        std::cin >> p2_col;
        std::cout << std::endl;

        p2_Guesses++;
        p2_Space = Get_Space(grid, p2_row, p2_col);

        switch (p2_Space) {
            case 0:
                msg2="You have missed";
                grid[p2_row][p2_col] = 9;
                p2_Misses++;
                break;

            case 1:
                msg2="This space was already been hit. You have wasted a guess!";
                break;

            case 2:
                grid[p2_row][p2_col] = 1;
                p1_Total_Hits++;

                msg2="You have struck the Destroyer!";
                Ship_list_2p[4].Record_Hits();
                if (Ship_list_2p[4].Check_Ship_Health() == 0) {
                    msg2="You have sunk the Destroyer!";
                }
                break;

            case 3:
                grid[p2_row][p2_col] = 1;
                p1_Total_Hits++;

                Boat_Name = Ship_list_2p[3].Boat_Name();
                if (Boat_Name == "Cruiser") {
                    msg2="You have struck the Cruiser!";
                    Ship_list_2p[3].Record_Hits();
                    if (Ship_list_2p[3].Check_Ship_Health() == 0) {
                        msg2="You have sunk the Cruiser!";
                    }
                    Boat_Name.clear();
                } else {
                    msg2="You have struck the Submarine!";
                    Ship_list_2p[2].Record_Hits();
                    if (Ship_list_2p[2].Check_Ship_Health() == 0) {
                        msg2="You have sunk the Submarine!";
                    }
                    Boat_Name.clear();
                }
                Boat_Name.clear();
                break;
            case 4:
                grid[p2_row][p2_col] = 1;
                p1_Total_Hits++;
                msg2="You have struck the Battleship!";
                Ship_list_2p[1].Record_Hits();
                if (Ship_list_2p[1].Check_Ship_Health() == 0) {
                    msg2="You have sunk the Battleship!";
                }
                break;
            case 5:
                grid[p2_row][p2_col] = 1;
                p1_Total_Hits++;
                msg2="You have struck the Aircraft Carrier!";
                Ship_list_2p[0].Record_Hits();
                if (Ship_list_2p[0].Check_Ship_Health() == 0)
                {
                    msg2="You have sunk the Aircraft Carrier";
                }
                break;
        }
        if (p1_Total_Hits == 17)
        {
            std::cout << "Player 2 You have WON, You have sunk all the Enemy Warships in " << p2_Guesses << " moves!"
                      << std::endl;
            Run_Game = false;
        }

    }
    while (Run_Game);//end of while game is playing loop
    std::cout<<"Congrats on your Victory Commander, Input any key to exit the program";
    int Choice_3;
    std::cin>>Choice_3;

    //end the program
}

//----------------------------------------------------------------------------------------------------------