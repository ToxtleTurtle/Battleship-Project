// Coders: Christian Hubner, Jacob Toxtle-Garcia
// Class: COP 2006-13969, Spring 2024
// Date: March 28, 2024
// Description: Group Project, write a C++ program for a battleship game with AI, solo, and two player options

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

#include <vector>
#include <stack>


class Boat
{
private:

    std::string name;

    int health;
    int length;

    char dirc;

    std::vector<int> shipCol;
    std::vector<int> shipRow;

public:

    Boat(std::string Ship_Name,int HP, int Ship_Size, char Direction, std::vector<int>Col,std::vector<int>Row)
    {
        name=Ship_Name;

        health=HP;
        length=Ship_Size;

        dirc=Direction;

        shipCol=Col;
        shipRow=Row;
    }
    std::string Boat_Name()
    {
        return name;
    }
    void Record_Hits()
    {
        health--;
    }
    int Check_Ship_Health()
    {
        return health;
    }
    std::string GetBoat(int r, int c)
    {
        for(int i=0; i<length; i++)
        {
            if((shipRow[i]==r) && (shipCol[i]==c))
            {
                return name;
            }
        }
        return "";
    }

};

void Set_up_1(int grid[][10], std::vector<Boat> &Boat_list10);

void ReSet_Board(int Board[][10]);
void Establish(int size, int Board[][10], std::stack<int> &Coordinates);

char direction(int Number);
int Location(int col, int &row, int size, char d);
int Is_Empty(int grid[][10], int col, int row, int length,char dirc);
void Edit_Grid(int grid[][10], int col, int row, int length, char dirct);
int Pack_UP_Coordinates(int col, int row, int length, char direct);

void Print_Board_Ships(int grid[][10]);

void Create_Boats(std::vector<Boat> &Boat_list_123,std::stack<int>&Coordinates);
void UnPack_Coordinates(int &col, int &row, int &size, int Coor, char &Direc);


void Play_Alone(int grid[][10],std::vector<Boat>&Boat_list_1234);
void Play_AI(int playerGrid[][10], int aiGrid[][10], std::vector<Boat>& playerBoats, std::vector<Boat>& aiBoats);

void Print_GameBoard(int grid[][10]);

int Get_Space(int grid[][10], int row, int col);

void Set_up_2(int grid[][10],int grid2[][10],std::vector<Boat>&Ship_list_1p,std::vector<Boat>Ship_list_2p);
void Play_Wth_Friends(int grid[][10],int grid2[][10],std::vector<Boat>&Ship_list_1p,std::vector<Boat>Ship_list_2p);
void Print_2_GameBoard(int grid[][10],int grid2[][10]);

/*
General Functions
void Play_AI();
void Play_Wth_Friends();

More general functions
int AI();
void Score();
*/


enum Warship {DESTROYER=2, CRUISER=3, BATTLESHIP=4, CARRIER=5};

int main()
{
    int Gameboard[10][10],Gameboard2[10][10];
    std::vector<Boat>Warships;
    std::vector<Boat>Warshipz;

    int playerGrid[10][10], aiGrid[10][10], hitGrid[10][10];
    std::vector<Boat> playerBoats;
    std::vector<Boat> aiBoats;

    std::cout<<"Welcome to Battleship. Input [K] to Play!"<<std::endl;
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
                Set_up_1(Gameboard, Warships);
                Play_Alone(Gameboard,Warships);
                break;
            case 2:
                std::cout<<"Please set up your board.";
                Set_up_1(playerGrid, playerBoats); // Set up the player's board
                std::cout<<"Please set up the AI's board.";
                Set_up_1(aiGrid, aiBoats); // Set up the AI's board

                Play_AI(playerGrid, aiGrid, playerBoats, aiBoats);
                break;
            case 4:
                Set_up_2(Gameboard,Gameboard2,Warships,Warshipz);
                Play_Wth_Friends(Gameboard,Gameboard2,Warships,Warshipz);
                break;
        }

    }
    return 0;
}
//------------------------------------------------------------------------------------------------

void Play_AI(int playerGrid[][10], int aiGrid[][10], std::vector<Boat>& playerBoats, std::vector<Boat>& aiBoats)
{
    bool playerTurn = true;
    bool gameRunning = true;
    int playerGuesses = 0, aiGuesses = 0;
    int playerTotalHits = 0, aiTotalHits = 0;
    int playerMisses = 0, aiMisses = 0;

    while (gameRunning)
    {
        if (playerTurn)
        {
            // Player's turn
            Print_GameBoard(playerGrid);
            std::cout << "Player's turn. ";
            int row1, col1;
            std::cout << "Enter row to attack: ";
            std::cin >> row1;
            std::cout << "Enter column to attack: ";
            std::cin >> col1;

            int space = Get_Space(aiGrid, row1, col1);
            if (space == 0)
            {
                std::cout << "Player missed!" << std::endl;
                aiGrid[row1][col1] = 9;
                playerMisses++;
            }
            else if (space != 0)
            {
                std::cout << "Player hit AI's ship!" << std::endl;
                aiGrid[row1][col1] = 1;
                playerTotalHits++;
                for (auto& boat : aiBoats)
                {
                    if (boat.GetBoat(row1, col1) != "")
                    {
                        std::cout << "Player has struck the " << boat.Boat_Name() << "!" << std::endl;
                        boat.Record_Hits();
                        if (boat.Check_Ship_Health() == 0)
                        {
                            std::cout << "Player has sunk the " << boat.Boat_Name() << "!" << std::endl;
                        }
                        break;
                    }
                }
            }
            else if (space == 9)
            {
                std::cout << "Player wasted a guess!" << std::endl;
            }

            playerGuesses++;
            if (aiTotalHits == 17)
            {
                std::cout << "AI wins!" << std::endl;
                gameRunning = false;
            }

            playerTurn = false;
        }
        else
        {
            // AI's turn
            Print_GameBoard(aiGrid);
            int row2 = rand() % 10;
            int col2 = rand() % 10;

            int space = Get_Space(playerGrid, row2, col2);
            if (space == 0)
            {
                std::cout << "AI missed at row " << row2 << " and column " << col2 << "!" << std::endl;
                playerGrid[row2][col2] = 9;
                aiMisses++;
            }
            else if (space != 0)
            {
                std::cout << "AI hit player's ship at row " << row2 << " and column " << col2 << "!" << std::endl;
                playerGrid[row2][col2] = 1;
                aiTotalHits++;
                for (auto& boat : playerBoats)
                {
                    if (boat.GetBoat(row2, col2) != "")
                    {
                        std::cout << "AI has struck the " << boat.Boat_Name() << "!" << std::endl;
                        boat.Record_Hits();
                        if (boat.Check_Ship_Health() == 0)
                        {
                            std::cout << "AI has sunk the " << boat.Boat_Name() << "!" << std::endl;
                        }
                        break;
                    }
                }
            }
            else if (space == 9)
            {
                std::cout << "AI wasted a guess!" << std::endl;
            }

            aiGuesses++;
            if (playerTotalHits == 17)
            {
                std::cout << "Player Wins!" << std::endl;
                gameRunning = false;
            }

            playerTurn = true;
        }
    }

    std::cout << "Player made " << playerGuesses << " guesses with " << playerMisses << " misses." << std::endl;
    std::cout << "AI made " << aiGuesses << " guesses with " << aiMisses << " misses." << std::endl;
}

//------------------------------------------------------------------------------------------------
void Set_up_1(int grid[][10], std::vector<Boat> &Boat_list1)
{
    char Choice3, Choice4;
    std::stack<int> Coordinates;
    bool ReRole=true;
    do
    {
        ReSet_Board(grid);
        Establish(DESTROYER, grid, Coordinates);
        Establish(CRUISER, grid, Coordinates);
        Establish(CRUISER, grid, Coordinates);
        Establish(BATTLESHIP, grid, Coordinates);
        Establish(CARRIER, grid, Coordinates);


        std::cout<<" Would you like to see the Board with ships?"
                   "\n [y] to see the complete board with ships!"
                   "\n [n] to start then game blind!"<<std::endl;
        std::cin>>Choice4;

        switch (Choice4)
        {
            case 'y':
            case 'Y':
                Print_Board_Ships(grid);

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
    Create_Boats(Boat_list1, Coordinates);
}

void ReSet_Board(int Board[][10])
{
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
    srand(time(0));

    int Free_Real_Estate, row=0, col=0, Cereal_Num=0;
    char dirc;

    dirc=direction(rand()%10);  //randomly pick which direction to place boat
    col=Location(col, row, size, dirc);//Random Location

    while(true)
    {
        if (dirc == 'h')
        {
            Free_Real_Estate=Is_Empty(Board, col, row, size, dirc);
            if (Free_Real_Estate==1)//if the boat overlaps, generate another random column, row and direction and start the loop again
            {
                dirc = direction(rand() % 10);
                col = Location(col, row, size, dirc);
                continue;
            }
            Edit_Grid(Board, col, row, size, dirc);//place the boat on the grid
            Cereal_Num=Pack_UP_Coordinates(col,row,size,dirc);
            Coordinates.push(Cereal_Num);
            break;
        }

        else if (dirc == 'v')
        {
            Free_Real_Estate = Is_Empty(Board, col, row, size, dirc);
            if (Free_Real_Estate==1)
            {
                dirc=direction(rand() % 10);
                col=Location(col, row, size, dirc);
                continue;
            }
            Edit_Grid(Board, col, row, size, dirc);
            Cereal_Num=Pack_UP_Coordinates(col,row,size,dirc);
            Coordinates.push(Cereal_Num);
            break;
        }
    }
}
void Create_Boats(std::vector<Boat> &Boat_list_123, std::stack<int>&Coordinates)
{
    int COL, ROW, LENGTH;
    char DIRECTION;

    while (!Coordinates.empty())
    {
        int COORDINATES = Coordinates.top();
        Coordinates.pop();

        UnPack_Coordinates(COL, ROW, LENGTH, COORDINATES, DIRECTION);

        switch (LENGTH)
        {
            case 2:
            {
                if(DIRECTION=='h')
                {
                    std::vector<int> r1 (LENGTH);
                    for (int i=0; i<(int)r1.size(); ++i)
                    {
                        r1.at(i)=ROW;
                    }
                    std::vector<int> c1 (LENGTH);
                    for (int i=0; i<(int)c1.size(); ++i)
                    {
                        c1.at(i)=COL;
                        COL++;
                    }

                    Boat Destroyer("Destroyer",LENGTH,LENGTH,DIRECTION,c1,r1);
                    Boat_list_123.push_back(Destroyer);
                }
                else if(DIRECTION=='v')
                {
                    std::vector<int> r1 (LENGTH);
                    for (int i=0; i<(int)r1.size(); ++i)
                    {
                        r1.at(i)=ROW;
                        ROW++;
                    }
                    std::vector<int> c1 (LENGTH);
                    for (int i=0; i<(int)c1.size(); ++i)
                    {
                        c1.at(i)=COL;
                    }
                    Boat Destroyer("Destroyer",LENGTH,LENGTH,DIRECTION,c1,r1);
                    Boat_list_123.push_back(Destroyer);
                }
                break;
            }
            case 3:
            {
                if (Boat_list_123.size()<3)
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
//-----------------------------------------------------------------------------------
//Establish();
int Location(int col, int &row, int size, char d)
{
    switch(size)
    {
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
    if(Number>4)
    {
        return 'h';  //randomly pick which direction to place boat
    }
    else
    {
        return 'v';
    }
}
int Is_Empty(int grid[][10], int col, int row, int length, char dirc)
{
    int check;
    if(dirc=='h')
    {
        for(int i=col; i<col+length; i++)
        {
            check=grid[row][i];
            if(check!=0)
            {
                return 1;
            }
        }

        return 0;
    }
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
    if(dirct=='h')
    {
        for(int i=0; i<length; i++)
        {
            grid[row][col]=length;
            col++;
        }
    }
    else if(dirct=='v')
    {
        for(int i=0; i<length; i++)
        {
            grid[row][col]=length;
            row++;
        }
    }
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
            if (grid[i][j]==0)
            {
                std::cout<<"_|";
            }
            else if (grid[i][j]!=0)
            {
                std::cout << grid[i][j]<<"|";
            }
        }
        std::cout << std::endl; //new line at end of column
    }
    std::cout << std::endl;
}
//------------------------------------------------------------------------------------------------
//General Code;
// #_#_#_#  ==   Direction,Size,Row,Col
int Pack_UP_Coordinates(int col, int row, int length, char direct)
{
    int Result;
    Result=(length*100);
    Result+=(row*10);
    Result+=(col);
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
    Direc = (Coor >= 2000) ? 'v' : 'h';

    size = (Coor / 100) % 10;

    col = Coor % 10;

    row = Coor / 10 % 10;
}
//---------------------------------------------------------------------------------------------------
//SinglePlayer NO AI
void Play_Alone(int grid[][10],std::vector<Boat>&Boat_list_1234)
{
    bool Run_Game=true;
    int row=0,col=0;
    int Guesses=0,Total_Hits=0,Misses=0,Space=0;
    std::string Boat_Name;

    while(Run_Game)
    {
        Print_GameBoard(grid);

        std::cout << "Enter a row coordinate: ";
        std::cin >> row;
        std::cout << "Enter a column coordinate: ";
        std::cin >> col;
        std::cout << std::endl;

        Guesses++;
        Space=Get_Space(grid, row, col);

        switch(Space) {
            case 0:
                std::cout << "You have missed" << std::endl;
                grid[row][col] = 9;
                Misses++;
                break;

            case 1:
                std::cout << "This space has already been hit. You have wasted a guess!" << std::endl;
                break;


            case 2:
                grid[row][col] = 1;
                Total_Hits++;

                std::cout << "You have struck the " << Boat_list_1234[4].Boat_Name() << "!" << std::endl;
                Boat_list_1234[4].Record_Hits();
                if (Boat_list_1234[4].Check_Ship_Health() == 0) {
                    std::cout << "You have sunk the "<<Boat_list_1234[4].Boat_Name() << "!" << std::endl;
                }
                break;

            case 3:
                grid[row][col] = 1;
                Total_Hits++;

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
                grid[row][col] = 1;
                Total_Hits++;
                std::cout << "You have struck the " << Boat_list_1234[1].Boat_Name() << "!" << std::endl;
                Boat_list_1234[1].Record_Hits();
                if (Boat_list_1234[1].Check_Ship_Health() == 0) {
                    std::cout << "You have sunk the "<<Boat_list_1234[1].Boat_Name() << "!" << std::endl;
                }
                break;
            case 5:
                grid[row][col] = 1;
                Total_Hits++;
                std::cout << "You have struck the " << Boat_list_1234[0].Boat_Name() << "!" << std::endl;
                Boat_list_1234[0].Record_Hits();
                if (Boat_list_1234[0].Check_Ship_Health() == 0) {
                    std::cout << "You have sunk the "<<Boat_list_1234[0].Boat_Name() << "!" << std::endl;
                }
                break;
        }

        if(Total_Hits==17)
        {
            Run_Game=false;
        }
    }//end of while game is playing loop

    std::cout<<"Thank you for playing!  You have sunk all the Warships in " << Guesses << " moves!" << std::endl;
}

//-----------------------------------------------------------------------------------------------------------------------

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
            if(grid[i][j] == 1)  // if the space is a hit, print 'X'
            {
                std::cout << "X|";
            }
            else if(grid[i][j] == 9)  // if the space is a miss, print '0'
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
    int Get_Space(int grid[][10], int row, int col)
{
    int space = grid[row][col];
    return space;
}

//old code
/*
void Print_GameBoard(int grid[][10])
{
    std::cout << "   0|1|2|3|4|5|6|7|8|9" << std::endl;
    for(int i=0; i<10; i++)  //column loop
    {
        for(int j=0; j<10; j++)  //row loop
        {
            if(j==0)
            {
                std::cout << i << " |" ; //print row number and spaces before new row
            }
            if(grid[i][j]==1)  //if the space is a hit, print it
            {
                std::cout << "X|";
            } else if(grid[i][j]==9)  //if the space is a miss, print it
            {
                std::cout << "0|";
            } else
            {
                std:: cout << "_|";  //otherwise, just print a 0
            }
        }
        std::cout << std::endl; //new line at end of column
    }
    std::cout << std::endl;
}
    int Get_Space(int grid[][10], int row, int col)
{
    int space=grid[row][col];
    return space;
}*/
//---------------------------------------------------------------------------------------------------

void Set_up_2(int grid[][10],int grid2[][10],std::vector<Boat>&Ship_list_1p,std::vector<Boat>Ship_list_2p)
{
    //std::string Launch_codes[2];

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

    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

    std::cout << "\nPlayer 2,  Let's Place your ships!" << std::endl;

    //------------------------------------------------------------------------

    char Choice5;
    bool ReRole2 = true;
    do {
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

    std::cout << "Thank You, Now You Have Finished setting up Your Ships"<<std::endl;
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

}

void Play_Wth_Friends(int grid[][10],int grid2[][10],std::vector<Boat>&Ship_list_1p,std::vector<Boat>Ship_list_2p)
{
    bool Run_Game = true;
    int p1_row = 0, p1_col = 0;
    int p1_Guesses = 0, p1_Total_Hits = 0, p1_Misses = 0, p1_Space = 0;
    std::string msg1="Nothing Happened... Yet";

    int p2_row = 0, p2_col = 0;
    int p2_Guesses = 0, p2_Total_Hits = 0, p2_Misses = 0, p2_Space = 0;
    std::string msg2="Nothing Happened... Yet";

    int turn = 1;

    std::string Boat_Name;

    do
    {
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        std::cout << "turn " << turn<<std::endl;

            std::cout<<"\n9 == The Enemy Missed!"
                       "\n1 == Hits on our Ships!"<<std::endl;

        Print_2_GameBoard(grid, grid2);

        std::cout<<"On the Previous turn: "<<msg1<<std::endl;
        msg1.clear();

        std::cout << "Enter a row coordinate: ";
        std::cin >> p1_row;
        std::cout << "Enter a column coordinate: ";
        std::cin >> p1_col;
        std::cout << std::endl;

        p1_Guesses++;
        p1_Space = Get_Space(grid2, p1_row, p1_col);

        switch (p1_Space) {
            case 0:
                msg1="You have missed";
                grid2[p1_row][p1_col] = 9;
                p1_Misses++;
                break;

            case 1:
                msg1="This space has already been hit. You have wasted a guess!";
                break;


            case 2:
                grid2[p1_row][p1_col] = 1;
                p2_Total_Hits++;

                msg1="You have struck the Destroyer!";
                Ship_list_1p[4].Record_Hits();
                if (Ship_list_1p[4].Check_Ship_Health() == 0) {
                    msg1="You have sunk the Destroyer!";
                }
                break;

            case 3:
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
            case 4:
                grid2[p1_row][p1_col] = 1;
                p2_Total_Hits++;
                msg1="You have struck the Battleship!";
                Ship_list_1p[1].Record_Hits();
                if (Ship_list_1p[1].Check_Ship_Health() == 0) {
                    msg1="You have sunk the Battleship!";
                }
                break;
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
        if (p2_Total_Hits == 17)
        {
            Run_Game = false;
        }
        //-----------------------------------------------------------------------------------------------------

        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

        //---------------------------------
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
            Run_Game = false;
        }

    }
    while (Run_Game);//end of while game is playing loop

    //Didn't work, game finished before message displayed
    if (p1_Total_Hits == 17) {
        std::cout << "Player 2 You have WON, You have sunk all the Enemy Warships in " << p2_Guesses << " moves!"
                  << std::endl;
    } else if (p2_Total_Hits == 17) {
        std::cout << "Player 1 You have WON, You have sunk all the Enemy Warships in " << p1_Guesses << " moves!"
                  << std::endl;
    }
}
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
            if (grid[i][j] == 0) {
                std::cout << "_|";
            } else if (grid[i][j] != 0) {
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
//----------------------------------------------------------------------------------------------------------