#include <iostream>
#include <fstream> //file i/o library
#include <string>
#include <vector>
#include <algorithm> //can we even use this?
#include <limits>
#include <time.h>

#define IGNORE_NUM 1000 //used in readInt function
#define FILE_MAX_C 10000 //used for clearing winner files before writing in ascendent order
#define ARRAY_SIZE 1000

// PARTIAL DEFINITION OF SOME CLASSES/STRUCTS FOR THE DEVELOPMENT OF THE 2ND PROJECT (ROBOT GAME)
///////////////////////////////////////////////////////////////////////////////
struct Movement
{
    int dRow, dCol; // displacement, taking into account the chosen movement
};
///////////////////////////////////////////////////////////////////////////////
struct Position
{
    int row, col;
};

/* --------CTRL_EXIT---------
->Checks if the stream has reached the end of file (CTRL+D/CTRL+Z)
->For every cin call we have we also call this functions so it can be checked if CTRL+Z/CTRL+D was inserted
*/
void ctrl_exit()
{

    if (std::cin.eof())
    {
        std::cout << "CTRL Z/CTRL D" << std::endl;
        exit(1);
    }
}

/*--------CLEAR_SCREEN---------
->this functions just clears the terminal "screen"
*/
void clear_screen(bool exit)
{

    //se exit=true o jogo acaba e limpa tudo o que está acima
    //se exit=false então clear_screen é utilizado como um "update" do outuput do jogo
    if (exit)
    {
        std::cout << "\033[2J\033[1;1H";
    }
    else
    {
        std::cout << "\033[2J";
    }
}

/*----------READ_INT----------
->This function is called whenever we need to read an integer number, so it can be checked whether what
the user inserted is a valid integer or not.
->In case that what is inserted isn't valid then the function returns false;
*/
bool read_int(int &anInt)
{

    std::cout << ">>";
    std::cin >> anInt;
    ctrl_exit();
    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(IGNORE_NUM, '\n');
        std::cout << "Invalid integer\n";
        return false;
    }
    std::cin.ignore(IGNORE_NUM, '\n');
    return true;
    // std::cout << anInt;
}

bool file_check(int maze_num)
{

    bool ret = true;
    std::ifstream f;
    std::string file_name;
    if(maze_num<10){
        file_name="MAZE_"+std::to_string(0)+std::to_string(maze_num)+".txt";
    }
    else file_name="MAZE_"+std::to_string(maze_num)+".txt";
    f.open(file_name, std::ios::in);

    if (f)
    {
        std::cout << "Successful reading" << std::endl;
    }
    else
        ret = false;

    f.close();

    return ret;
}

// bool isMovValid(Movement mov, std::vector<Robot> &r, std::vector<Post> &p){

// }

///////////////////////////////////////////////////////////////////////////////
class Player
{
public:
    Player();
    Player(int row, int col);
    int getRow() const;
    int getCol() const;
    char getSymbol() const;
    bool isAlive() const;
    void setAsDead();
    bool move(Movement delta);
    void setInitialTime();
    void setScore();
    int getScore() const;

private:
    int row, col;
    bool alive;
    char symbol;
    time_t ini, score;
};
///////////////////////////////////////////////////////////////////////////////
class Robot
{
public:
    enum State
    {
        ALIVE,
        STUCK,
        DEAD
    };
    Robot(Position pos);
    int getID() const;
    char getSymbol() const; // get char representation of robot (R if alive, r if dead)
    int getRow() const;
    int getCol() const;
    Position getPosition() const;
    bool isAlive() const;
    void setRow(int x);
    void setCol(int y);
    void setPosition(const Position &pos);
    void setAsDead();
    void roboMove(const Player p);
    //other methods
private:
    static int robotCounter; //used to attribute automatically the id to the robots
    int id;
    int row, col;
    bool alive;
    // other attributes (?)
};
///////////////////////////////////////////////////////////////////////////////
class Post
{
public:
    Post(Position pos, char type);
    int getRow() const;
    int getCol() const;
    char getSymbol() const; // get char representation of Post
    bool isElectrified() const;
    //other methods
private:
    int row, col;
    char type; // '*' - electrified; '+'- non-electrified
               // other attributes (?)
               // could also have a state, like the robot(?)
};
///////////////////////////////////////////////////////////////////////////////
class Maze
{
public:
    Maze();
    Maze(const std::string &filename);
    bool addPost(const Post &post);
    bool addRobot(const Robot &robo);
    bool addPlayer(const Player &player);
    bool addExit(Position pos);
    int getnumRows() const;
    int getnumCols() const;
    void setRows(int rows);
    void setCols(int cols);
    bool readMap(const std::string &filename);
    std::vector<Robot> getRobo();
    std::vector<Post> getPost();
    Player getPlayer();
    Position getExit();
    void setRobo(std::vector<Robot> r); //used to update Robo vec
    void setPost(std::vector<Post> p); // used to update Post vec
    void setPlayer(Player player); // used to update Player
    void setExit(Position exit); // used to update Exit
    //  std::vector< std::vector<char> > getMap ();
    // other methods
private:
    int numRows, numCols;
    std::vector<Robot> robotVec;
    std::vector<Post> postVec;
    Player player;
    Position exit;
    // data structure that represents the posts
};
///////////////////////////////////////////////////////////////////////////////
class Game
{
public:
    Game(const std::string &filename);
    // This constructor should initialize the Maze, the vector of Robots, and the Player,
    // using the chars read from the file
    bool play(int &score); // implements the game loop; returns true if player wins, false otherwise
    bool isValid();

private:
    void showGameDisplay(Maze maze1) const;
    bool collide(const Robot &robot, const Post &post) const;     // check if robot collided with post (and possibly set it as dead)
    bool collide(const Robot &robot, const Player &player) const; // check if human and robot collided (and possibly set human as dead)
    bool collide(const Robot &robot, const Robot &robot2) const;
    bool collide(const Post &post, const Player &player) const;
    // other methods, for example:
    // to check if player is trying to move to a valid place
    // to apply a valid play and check collisions
    // to check if two robots collide (and possibly set them as dead)
    // etc.
    // private:
    Maze maze;
    //  Player player;
    //  std::vector<Robot> robotVe;

    //other attributes
};

class Winner    
{
public:
    Winner();
    Winner(const int score);
    int getScore() const;
    void setName(const std::string first, const std::string last);
    std::string getFirstName() const;
    std::string getLastName() const;
    void setNameLength();
    int getNameLength() const;

private:
    int score, name_length;
    std::string first_name, last_name;
};

/////////////////
/////    PLAYER

Player::Player()
{
}

Player::Player(int row, int col)
{
    this->row = row;
    this->col = col;
    this->alive = true;
}

int Player::getRow() const
{
    return row;
}
int Player::getCol() const
{
    return col;
}
char Player::getSymbol() const
{
    if (isAlive())
        return 'H';
    else
        return 'h';
}
bool Player::isAlive() const
{
    return alive;
}
void Player::setAsDead()
{
    this->alive = false;
    this->symbol = 'h';
}
bool Player::move(Movement delta)
{
    this->row += delta.dRow;
    this->col += delta.dCol;
    return true;
}
void Player::setInitialTime(){
    this->ini=time(NULL);
}

void Player::setScore(){
    this->score=time(NULL)-this->ini;
}

int Player::getScore() const{
    return this->score;
}

/////////////////
////// ROBO

int Robot::robotCounter = 0;

Robot::Robot(Position pos)
{
    this->id = robotCounter;
    this->robotCounter++;
    this->row = pos.row;
    this->col = pos.col;
    this->alive = true;
}
int Robot::getID() const
{
    return this->id;
}

char Robot::getSymbol() const // get char representation of robot (R if alive, r if dead)
{
    if (this->alive)
        return 'R';
    else
        return 'r';
}

int Robot::getRow() const
{
    return this->row;
}
int Robot::getCol() const
{
    return this->col;
}

Position Robot::getPosition() const
{
    Position pos;
    pos.col = this->col;
    pos.row = this->row;
    return pos;
}

bool Robot::isAlive() const
{
    return this->alive;
}

void Robot::setRow(int x)
{
    this->row = x;
}

void Robot::setCol(int y)
{
    this->col = y;
}

void Robot::setPosition(const Position &pos)
{
    this->row = pos.row;
    this->col = pos.col;
}

void Robot::setAsDead()
{
    this->alive = false;
}

void Robot::roboMove(const Player p)
{

    int dx = this->col - p.getCol();
    int dy = this->row - p.getRow();

    if (dx != 0)
        dx = dx / abs(dx);
    if (dy != 0)
        dy = dy / abs(dy);

    if (this->alive)
    {
        this->row -= dy;
        this->col -= dx;
    }
}

//////////////
///     POST

Post::Post(Position pos, char type)
{
    this->row = pos.row;
    this->col = pos.col;
    this->type = type;
}

int Post::getRow() const
{
    return this->row;
}

int Post::getCol() const
{
    return this->col;
}

char Post::getSymbol() const // get char representation of Post
{
    return this->type;
}

bool Post::isElectrified() const
{
    if (this->type == '*')
        return true;
    else
        return false;
}

///////////
//      MAZE

Maze::Maze()
{
}

Maze::Maze(const std::string &filename)
{
    this->player = Player(10, 10);
    readMap(filename);
}

bool Maze::addPost(const Post &post)
{

    this->postVec.push_back(post);

    return true;
}

bool Maze::addRobot(const Robot &robo)
{

    this->robotVec.push_back(robo);
    return true;
}

bool Maze::addExit(Position pos)
{

    this->exit = pos;
    return true;
}

int Maze::getnumRows() const
{
    return this->numRows;
}
int Maze::getnumCols() const
{
    return this->numCols;
}

void Maze::setRows(int rows)
{
    this->numRows = rows;
}
void Maze::setCols(int cols)
{
    this->numCols = cols;
}

std::vector<Robot> Maze::getRobo()
{
    return this->robotVec;
}
std::vector<Post> Maze::getPost()
{
    return this->postVec;
}
Player Maze::getPlayer()
{
    return this->player;
}
Position Maze::getExit()
{
    return this->exit;
}

bool Maze::addPlayer(const Player &player)
{
    this->player = player;
    return true;
}

void Maze::setRobo(std::vector<Robot> r)
{
    this->robotVec = r;
}
void Maze::setPost(std::vector<Post> p)
{
    this->postVec = p;
}
void Maze::setPlayer(Player player)
{
    this->player = player;
}
void Maze::setExit(Position exit)
{
    this->exit = exit;
}

bool Maze::readMap(const std::string &filename)
{
    bool ret = false;
    std::ifstream f;
    f.open(filename, std::ios::in);

    std::string m, n, line;
    std::getline(f, m, 'x'); //rows
    std::getline(f, n, '\n'); //columns

    int num_lines = std::stoi(m), num_colum = std::stoi(n);
    setRows(num_lines);
    setCols(num_colum);


    // i- lines | j- columns
        for (int i = 0; i < num_lines; i++)
        { //leitura do mapa
            std::getline(f, line);

            for (int j = 0; j < num_colum; j++)
            {
                Position pos;
                pos.row = i;
                pos.col = j;

                switch (line[j])
                {

                case '*':
                {
                    if (j != 0 && j != num_colum - 1 && i != 0 && i != num_lines - 1) //Map limits not are Posts
                        addPost(Post(pos, '*'));
                    break;
                }
                case '+':
                {
                    addPost(Post(pos, '+'));
                    break;
                }
                case 'O':
                {
                    addExit(pos);
                    break;
                }
                case 'H':
                {
                    addPlayer(Player(i, j));
                    break;
                }
                case 'R':
                {
                    addRobot(Robot(pos));
                    break;
                }
                }
            }
        }
    ret = true;

    f.close();
    return ret;
}

////////////////

Game::Game(const std::string &filename)
{
    this->maze = Maze(filename);
}
// This constructor should initialize the Maze, the vector of Robots, and the Player,
// using the chars read from the file
bool Game::play(int &score)
{
    bool readMov = false;
    char mov;
    Movement movP;

    std::vector<Robot> r = maze.getRobo();
    std::vector<Post> p = maze.getPost();
    Player player = maze.getPlayer();
    Position exit = maze.getExit();

    player.setInitialTime();

    showGameDisplay(maze);


    //Verify input
    while (player.isAlive())
    { //Over is when player wins by exiting MAze
        readMov = false;
        bool MovValid = true;
        std::cout << "Where do you want to move? (Q, W, E, D, C, X, Z, A, S)\n>>";
        std::cin >> mov;
        ctrl_exit();
        movP.dRow = 0;
        movP.dCol = 0;

        if (mov == 'q' || mov == 'Q')
        {
            movP.dRow = -1;
            movP.dCol = -1;
            readMov = true;
        }
        else if (mov == 'w' || mov == 'W')
        {
            movP.dRow = -1;
            readMov = true;
        }
        else if (mov == 'e' || mov == 'E')
        {
            movP.dRow = -1;
            movP.dCol = 1;
            readMov = true;
        }
        else if (mov == 'd' || mov == 'D')
        {
            movP.dCol = 1;
            readMov = true;
        }
        else if (mov == 'c' || mov == 'C')
        {
            movP.dRow = 1;
            movP.dCol = 1;
            readMov = true;
        }
        else if (mov == 'x' || mov == 'X')
        {
            movP.dRow = 1;
            readMov = true;
        }
        else if (mov == 'z' || mov == 'Z')
        {
            movP.dRow = 1;
            movP.dCol = -1;
            readMov = true;
        }
        else if (mov == 'a' || mov == 'A')
        {
            movP.dCol = -1;
            readMov = true;
        }
        else if (mov == 's' || mov == 'S')
        {
            movP.dRow = 0;
            movP.dCol = 0;
            readMov = true;
        }
        else
        {
            std::cout << "Invalid movement input\n";
            readMov = false;
        }

        if (readMov)
        {

            //IS MOVE VALID
            /*The player should not be allowed to move to cells 
            occupied by destroyed/stuck robots or by nonelectrified posts; 
            if he/she tries to 
            do so, he/she must be informed that the movement is invalid and
            asked for a new movement.*/

            Position playerNMov;
            playerNMov.col = player.getCol() + movP.dCol;
            playerNMov.row = player.getRow() + movP.dRow;

            for (unsigned int i = 0; i < r.size(); i++)
            { //ALL ROBOTS
                if (r[i].getSymbol() == 'r' && r[i].getCol() == playerNMov.col && r[i].getRow() == playerNMov.row)
                {
                    MovValid = false;
                    std::cout << "Movement Invalid, Cell is occupied by a destroyed/stuck robot\n";
                }
            }

            for (unsigned int j = 0; j < p.size(); j++)
            { // ALL POSTS
                if (p[j].getSymbol() == '+' && p[j].getCol() == playerNMov.col && p[j].getRow() == playerNMov.row)
                {
                    MovValid = false;
                    std::cout << "Movement Invalid, Cell is occupied by a non-eletrified post\n";
                }
            }

            if ((playerNMov.col == maze.getnumCols() - 1 || playerNMov.row == maze.getnumRows() - 1 || playerNMov.col == 0 || playerNMov.row == 0) && !(playerNMov.col == exit.col && playerNMov.row == exit.row))
            {
                MovValid = false;
                std::cout << "Movement Invalid, Cell is part of the Wall\n";
            }

            //

            if (MovValid)
            { //Movement must be valid, cant move to positions occupied by non eletrifed posts or destroyed/stuck robots

                player.move(movP);

                if (player.getRow() == exit.row && player.getCol() == exit.col)
                {
                    //Player reaches exit
                    
                    break; 
                }

                for (unsigned int i = 0; i < r.size(); i++)
                { //ALL ROBOTS
                    r[i].roboMove(player);

                    //ROBOS crash
                    for (unsigned int k = 0; k < r.size(); k++)
                    { //ALL ROBOS
                        if (k != i)
                        { //Dont compare with itself
                            if (collide(r[i], r[k]))
                            {
                                r[i].setAsDead();
                                r[k].setAsDead();
                            }
                        }
                    }

                    //VERIFICAÇÕES SE TERMINOU

                    //ROBO crash with post
                    for (unsigned int j = 0; j < p.size(); j++)
                    { // ALL POSTS
                        if (collide(r[i], p[j]) /*&& p[j].getSymbol() == '*'*/)
                        {
                            r[i].setAsDead();
                        }
                        //Player crash with post or robot
                        if (collide(r[i], player))
                        {
                            player.setAsDead();
                        }

                        //Player collide with eletrified post
                        if ((collide(p[j], player) && p[j].getSymbol() == '*'))
                        {
                            movP.dCol = -movP.dCol;
                            movP.dRow = -movP.dRow;
                            player.move(movP);
                            player.setAsDead();
                        }
                    }
                }

                maze.setRobo(r);
                maze.setPost(p);
                maze.setPlayer(player);

                printf("a\n");
                showGameDisplay(maze);

                
            }
        }
    }

    if (player.isAlive()){
        player.setScore();
        score=player.getScore();
        return true;
    }
    else
        return false;
}

bool Game::isValid()
{ 
    return true;
}
//private
void Game::showGameDisplay(Maze maze1) const
{

    std::vector<Robot> r = maze1.getRobo();
    std::vector<Post> p = maze1.getPost();
    Player player = maze1.getPlayer();
    Position exit = maze1.getExit();
    bool printed = false;
    int largura = maze1.getnumRows() - 1;
    int altura = maze1.getnumCols() - 1;
    bool printP = false;
    bool overlapRobo = false;

    clear_screen(true);

    for (int i = 0; i < maze1.getnumRows(); i++)
    { //lines

        for ( int j = 0; j < maze1.getnumCols(); j++)
        { // cols

            if (i == 0 || j == 0 || i == largura || j == altura)
            { //Edges of the maze
                if (exit.row == i && exit.col == j)
                {
                    std::cout << 'O';
                    // printed = true;
                }
                else
                    std::cout << "*";
            }
            // else if(i == 0 || j == 0 || i == largura || j == altura && ){

            // }
            else
            {
                //ROBOS need to check if robos dont overlap or it will print twice
                for (unsigned int rint = 0; rint < r.size(); rint++)
                {
                    overlapRobo = false;
                    

                    if (r[rint].getRow() == i && r[rint].getCol() == j)     //Robo in position being analyzed
                    {   
                        if(collide(r.at(rint), player))
                            overlapRobo = true;

                        // printf("\nRobo pos - %d  %d, P: %d - %d \n", r.at(rint).getCol(), r.at(rint).getRow(), player.getCol(), player.getRow());
                        for (int rint2 = rint - 1; rint2 >= 0; rint2--) //Robos that havent been printed
                        {
                            
                            //Checks if any robot that was printed is overlaping with the current one or is overlaping with the player
                            if (collide(r.at(rint), r.at(rint2)) ){
                                
                                overlapRobo = true;
                            }
                        }
                        if (!overlapRobo)
                        { //If current robot doesnt overlap with any other, then it prints
                            std::cout << r[rint].getSymbol();
                            printed = true;
                        }
                    }
                }

                //POSTS
                for (unsigned int pint = 0; pint < p.size(); pint++)
                {
                    printP = true; //variable that defines if post should be printed

                    if (p[pint].getRow() == i && p[pint].getCol() == j)
                    {
                        for (unsigned int rint = 0; rint < r.size(); rint++)
                        {

                            if (collide(r.at(rint), p.at(pint)) || collide(p.at(pint), player))
                            { //If post is overlaped by a robot or player do not print it, print only robot or player
                                printP = false;
                            }
                        }
                    }
                    else
                        printP = false;

                    if (printP)
                    {
                        std::cout << p[pint].getSymbol();
                        printed = true;
                    }
                }

                //JOGADOR
                if (player.getRow() == i && player.getCol() == j)
                {
                    std::cout << player.getSymbol();
                    printed = true;
                }
                
                if (exit.row == i && exit.col == j)
                {
                    std::cout << 'O';
                    printed = true;
                }

                if (!printed)
                { //If nothing was printed in the current position, print " "
                    std::cout << " ";
                }
            }
            printed = false;
        }
        std::cout << std::endl;
    }
}
bool Game::collide(const Robot &robot, const Post &post) const // check if robot collided with post (and possibly set it as dead)
{
    if (robot.getCol() == post.getCol() && robot.getRow() == post.getRow())
        return true;
    else
        return false;
}
bool Game::collide(const Robot &robot, const Player &player) const // check if human and robot collided (and possibly set human as dead)
{   
    // printf(" COl Robo pos - %d  %d, P: %d - %d \n", robot.getCol(), robot.getRow(), player.getCol(), player.getRow());
    if (robot.getCol() == player.getCol() && robot.getRow() == player.getRow())
        return true;
    else
        return false;
}
bool Game::collide(const Robot &robot, const Robot &robot2) const
{
    if (robot.getCol() == robot2.getCol() && robot.getRow() == robot2.getRow())
        return true;
    else
        return false;
}
bool Game::collide(const Post &post, const Player &player) const
{
    if (player.getCol() == post.getCol() && player.getRow() == post.getRow())
        return true;
    else
        return false;
}

Winner::Winner(){
}

Winner::Winner(const int score){
    this->score=score;
}

int Winner::getScore() const{
    return this->score;
}

void Winner::setName(const std::string first, const std::string last){
    this->first_name=first;
    this->last_name=last;
}

std::string Winner::getFirstName() const{
    return this->first_name;
}

std::string Winner::getLastName() const{
    return this->last_name;
}

void Winner::setNameLength(){
    this->name_length=this->first_name.length()+this->last_name.length()+1;
}

int Winner::getNameLength() const{
    return this->name_length;
}

/////////////////////////////////////////////////////////

/*
--------PRINT_RULES--------
->This function opens the file "Rules.txt" where the rules of the game are saved and then prints them.
*/
int print_rules()
{

    std::ifstream f(("RULES.txt"));
    std::string rules;
    if (f.is_open())
    {
        //f.seekg(0,f.beg);
        // std::cout << "Hello\n";

        while (getline(f, rules))
        {
            std::cout << rules << std::endl;
        }
    }
    else
    {
        return -1;
    }
    f.close();
    return 0;
}

/*--------PRESS_Q--------
->This function was only created to give the user time to read the rules and/or read the winners' list
->It is only used to go back to menu once Q is pressed, after the user ends reading the rules/winners' list. 
*/
void press_q()
{

    while (1)
    {

        std::cout << "Press Q to go back to menu." << std::endl
                  << ">>";
        std::string back_menu;
        std::cin >> back_menu;
        ctrl_exit();

        if ((back_menu.at(0) == 'Q' || back_menu.at(0) == 'q') && back_menu.length() == 1)
        {
            break;
        }
        else
        {
            std::cout << "Not valid." << std::endl;
        }
    }
}

/*-------WINNERS_LIST--------
->Asks the maze's number the user wants to see the winners
->In case that maze exists it prints the winners
->In case that maze doesn't exist prints empty list 
*/
void winners_list()
{

    std::cout << "What maze? (press 0 to go back to the menu)\n";
    int maze_number;
    bool invalid_maze = true;
    while (invalid_maze)
    {
        if (!read_int(maze_number))
        {
            std::cout << "Not a valid integer, try again.\n";
            continue;
        }
        if (!maze_number)
            break;
        std::string maze_name = "MAZE_" + std::to_string(maze_number) + "_WINNER.txt";
        std::ifstream f(maze_name);
        if (f.is_open())
        {
            invalid_maze = false;
            // std::cout << "Existing maze\n";
            std::cout << "***WINNERS***\n";
            std::string winner;
            int aux = 0;
            while (getline(f, winner))
            {
                if (aux > 1)
                {
                    std::cout << winner << std::endl;
                }
                aux++;
            }
            std::cout << std::endl;
            press_q();
            f.close();
        }
        else
        {
            std::cout << "Empty list\n";
            std::cout << "\nTo go back to the menu press 0\n";
        }
    }
}

/*
-----------MENU----------
->play_game - if true when the program returns to main() function the game starts
->exit-> if true when the program returns to main() it ends
->if the user chooses option number 1 (rules) then it will call the print_rules() function and after
reading the rules the user need to insert q to go back to the menu.
*/
void menu(bool &play_game, bool &exit)
{

    bool repeat = true;
    exit = false, play_game = false;

    while (repeat)
    {

        std::cout << "Welcome to the robot game. Choose your option (1,2,3):" << std::endl;
        std::cout << "1 - Rules\n2 - Play\n3 - Winners\n0 - Exit\n\n";
        int option;
        //Read option
        // Inputs como "1x", sendo x qualquer conjunto de letras, serão aceites
        while (!read_int(option))
        {
            std::cout << "Not a valid menu option\n>>";
        }
        //ctrl_exit();

        switch (option)
        {
        case 1:
            std::cout << "*Rules*" << std::endl;
            print_rules();
            press_q();
            break;

        case 2:
            play_game = true;
            repeat = false;
            break;

        case 3:
            winners_list();
            break;

        case 0:
            exit = true;
            repeat = false;
            break;

        default:
            std::cout << "Not a valid menu option \n" << std::endl;
            break;
        }
    }
}


/*
--------MAZE_NAME--------
->Asks the user the maze number that he/she wants to play
->The program will keep asking the number until is valid
->If the number is 0 then the function returns false which means that the game is going back
to the menu (see this in main function)
->It returns (true) when the user finally inserts a valid (existing) maze number.
*/
bool maze_name(std::string &name, int &maze_num){

    bool valid_maze=false;
    std::cout << "Insert the number of the maze you want to play." << std::endl;
    do
    {
        //read maze number
        while (!read_int(maze_num))
        {
            std::cout << "Invalid integer! Try again.\n";
        }
        
        if (!maze_num)
        {
            return false;
        }

        valid_maze = file_check(maze_num);
        if (!valid_maze)
        {
            std::cout << "Invalid maze! Try again. (0 to exit)\n";
        }
    } while (!valid_maze);
    if(maze_num<10){
        name="MAZE_"+std::to_string(0)+std::to_string(maze_num)+".txt";
    }
    else name="MAZE_"+std::to_string(maze_num)+".txt";

    return true;
}

/*
--------WINNER_FILE--------
->First it checks if the winner file of that maze (maze_num) exists, if it doesn't then it creates it and writes
the first 2 lines of the file
->After that it chekcs if the name is valid and, in case it is, it writes the name of the winner in the file
->In the end, it sorts the players by ascendent score
*/
void winner_file(const int maze_num, time_t score){
    
    std::string name;
    if(maze_num<10){
        name="MAZE_"+std::to_string(0)+std::to_string(maze_num)+"_WINNER.txt";
    }
    else name="MAZE_"+std::to_string(maze_num)+"_WINNER.txt";
    Winner win(score);

    std::cout << "Score: " << score<< std::endl;

    //if the file doesnt exist already winner_file creates it and writes the first line
    if (!std::fstream(name)){
        std::fstream f;
        f.open(name, std::ios::out);
        std::cout << "File created" << std::endl;
        f << "Player      - Time" << std::endl << std::endl;
        f.close();
    }

    std::fstream f;
    f.open(name);
    std::cout << "You won! Insert your name." << std::endl << ">>";
    
    std::string username, first_name, last_name, aux2;

    //"first" is used to delete a bug (returning that the username is not valid before the user's input)
    bool first=true;

    do{

        std::getline(std::cin, username);
        ctrl_exit();
        
        int n1, n3;
        n1 = username.find_first_of(" "); // same as using .find()
        
        //a string aux 2 é apenas auxiliar para testar se o nome cumpre todas as condições 
        aux2 = username.substr(n1+1, username.length());
        n3 = aux2.find_first_of(" ");
        first_name = username.substr(0,n1);
        username.erase(remove(username.begin(), username.end(), ' '), username.end());

        if(first){
            first=false;
            continue;
        }
        
        // 1 -> Verificar se existe espaço sequer
        // 2 -> Verificar se sem todos os possíveis espaços adicionais o username é maior q o first_name
        // 3 -> Verificar se o username sem espaços adicionais (exceto o espaço que separa o primeiro
        // e último nome) não ultrapassa o limite (15)
        // 4 -> Antes de retirar os espaços de username verifica se depois do primeiro nome só há
        // mais um nome (quando há um espaço depois do segundo nome então temos mais que 2 nomes)
        
    
        if(n1 == -1 || first_name.length() == username.length() || username.length()+1>15 || n3 != -1){

            std::cout << "The username requires 2 names and 15 is the max lenght." << std::endl;
            std::cout << "Try to insert your name again!" << std::endl;
            continue;
        }
        else{

            last_name = username.substr(n1,username.length());
            break;
        }

    }while(1);

    win.setName(first_name,last_name);
    win.setNameLength();

    f.clear();
    f.seekg(0,std::ios::end); //moving to the end of file
    f << win.getFirstName() << " " << win.getLastName();

    for(int i=win.getNameLength();i<15;i++){
        f << " ";
    }
    f << win.getScore();

    // ORGANIZAR DE FORMA ASCENDENTE

    int n_lines=0;
    std::vector <Winner> pl;

    f.clear();    
    f.seekg(0, std::ios::beg);
    std::string first_line;
    std::getline(f,first_line);
    
    //reading all names (first and last) and scores to different arrays who share the same positions 
    while(!f.eof()){

        f >> first_name >> last_name >> score;
        Winner p(score);
        p.setName(first_name, last_name);
        p.setNameLength();
        n_lines++;
        pl.push_back(p);
    }

    f.seekg(first_line.length()+2, std::ios::beg);
    for(int i=0;i<FILE_MAX_C;i++){
        f << "";
    }

    int array[ARRAY_SIZE], pos;

    //coping scores to "array" because scores value can't be modified
    for(int m=0;m<n_lines;m++){

        array[m]=pl[m].getScore();
    }

    
    int n=n_lines;
    while(n>0){
        
        int min=FILE_MAX_C;
        for(int aux=0;aux<n_lines;aux++){
            
            if(array[aux]<min){
                
                min=pl[aux].getScore();
                pos=aux;
            }
        }
        //if find min then it replaces on "array" for a much bigger value so it can
        //find the next min in the next iteration
        array[pos]=FILE_MAX_C;
        n--;
        
        f << pl[pos].getFirstName() << " " << pl[pos].getLastName();
        for(int i=pl[pos].getNameLength();i<15;i++){
            f << " ";
        }
        f << min << std::endl;
        
    }


    std::cout << "Your score is now saved!" << std::endl;
    f.close();
}

int main()
{

    bool exit = false, play_game = false;
    //in case some strange error happens in the function maze_name, the name of the maze by default is:
    std::string name="MAZE_12.txt";
    int maze_num; 
    int score=-1;
    while (!exit)
    {
        menu(play_game, exit);
        if (play_game){
            if(!maze_name(name, maze_num)){ // maze_name returns false when the user inserts 0 as the maze number
                play_game=false;
                continue;
            }
            Game g = Game(name);
            if (g.play(score)){
                std::cout << "U WON!\n";
                winner_file(maze_num,score);
            }
            else{
                std::cout << "U LOST!\n";
            }
        }
    }
    clear_screen(exit);
    std::cout << "O jogo acabou" << std::endl;
}

