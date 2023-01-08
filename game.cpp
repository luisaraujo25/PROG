#include <iostream>
#include <fstream> //file i/o library
#include <string>
#include <vector>
#include <algorithm> //can we even use this?
#include <limits>

#define FILE_MAX_C 50000 //used for clearing winner files before writing in ascendent order

struct robo {
    int l;
    int c;
    bool alive = true;
} ;

struct player {
    int l;
    int c;
    bool alive = true;
};

struct winner_player{

    std::string first_name;
    std::string last_name;
    int score;
    int username_length;
};

//for every input checks if it is CTRL D OR CTRL Z and if it is the program ends
void ctrl_exit(){

    if(std::cin.eof()){
        std::cout << "CTRL Z/CTRL D" << std::endl;
        exit(1);
    }
}

bool read_int(int &anInt){

    std::cin >> anInt;
    ctrl_exit();
    if(std::cin.fail()){
        std::cin.clear();
        std::cin.ignore(1000,'\n');
        return false;
    }
    std::cin.ignore(1000,'\n');
    return true;
    // std::cout << anInt;
}


void clear_screen(bool exit){
    
    //se exit=true o jogo acaba e limpa tudo o que está acima
    //se exit=false então clear_screen é utilizado como um "update" do outuput do jogo
    if(exit){
        std::cout << "\033[2J\033[1;1H";
    }
    else{
        std::cout << "\033[2J";
    }
}

int get_robo_num (std::vector< std::vector<char> > map, const int lines, const int columns){
    
    int count = 0;

    for(int i = 0; i < lines; i++){ //leitura do mapa            
        for(int j = 0; j < columns; j++){
            if(map[i][j] == 'R')
                 count++; //contagem do numero de robos
        }
    }
    
    return count;
}

void get_pos( std::vector< std::vector<char> > map, std::vector<robo> &r, player &p, const int lines, int columns) {

    int num_robos = get_robo_num (map, lines, columns);
    
    
    robo aux;
    aux.alive = true;

    for(int i = 0; i < lines; i++){ //leitura do mapa            
        for(int j = 0; j < columns; j++){
            if(map[i][j] == 'R')
            {   
                aux.l = i; 
                aux.c = j;
                r.push_back(aux); //Pushing Robots position into vector
            }
            else if(map[i][j] == 'H')
            {
                p.l = i;
                p.c = j; //Saving Players position
            }
                  
        }
    }

    
}

void player_move( std::vector< std::vector<char> > &map, std::vector<robo> r, player &p, const int n_lines, int n_columns){
    
    char mov;
    std::cout << "Where do you want to move? (Q, W, E, D, C, X, Z, A, S)\n>>";
    std::cin >> mov;
    ctrl_exit();
    int linemov = 0, colmov = 0;
    
    //Verify input
    if(mov=='q' || mov=='Q'){
        linemov = -1;
        colmov = -1;
    }
    else if(mov=='w' || mov=='W'){
        linemov=-1;
    }
    else if(mov=='e' || mov=='E'){
        linemov = -1;
        colmov = 1;
    }
    else if(mov=='d' || mov=='D'){
        colmov=1;
    }
    else if(mov=='c' || mov=='C'){
        linemov = 1;
        colmov = 1;
    }
    else if(mov=='x' || mov=='X'){
        linemov=1;
    }
    else if(mov=='z' || mov=='Z'){
        linemov = 1;
        colmov = -1;
    }
    else if(mov=='a' || mov=='A'){
        colmov = -1;
    }
    else if(mov=='s' || mov=='S'){
        linemov = 0;
        colmov = 0;
    }
    else{
        clear_screen(true);

        //Draw map
        for(int i = 0; i < n_lines; i++){
                for(int j = 0; j < n_columns; j++){
                    std::cout << map[i][j];
                }
                std::cout << std::endl;
            }
        std::cout << "Letter not valid, try again\n";
        
        player_move(map, r, p, n_lines, n_columns);
    }

    //Movimento do jogador
    if(colmov != 0 || linemov != 0){
        if(map[p.l + linemov][p.c + colmov] == 'r'){
                    std::cout << "Invalid movement, try other direction\n>>";
                    player_move(map, r, p, n_lines, n_columns);
        }
        else if(map[p.l + linemov][p.c + colmov] == '*' || map[p.l + linemov][p.c + colmov] == 'R'){ //Condição em que jogador morre (vai contra robo ou fence/muro)
                    std::cout << "RIP\n";
                    p.alive = false;
                    map[p.l][p.c] = ' ';
                    map[p.l + linemov][p.c + colmov] = 'h';
                    p.l += linemov;
                    p.c +=  colmov;
        }
        else{
            map[p.l][p.c] = ' ';
            map[p.l + linemov][p.c + colmov] = 'H';
            p.l += linemov;
            p.c +=  colmov;
        }

    }

}

void robo_move (std::vector< std::vector<char> > &map, std::vector<robo> &r, player &p){
    // Primeiro desloca-se pelas linhas e depois pelas colunas
    for(int i = 0; i < r.size(); i++){
        if(r[i].alive){
            int l_des = r[i].l - p.l;
            int c_des = r[i].c - p.c;
        
        //Verificar se há robos sobrepostos e que ainda não foram atualizados
            for(int j = 0; j < r.size(); j++){
                for(int k = j+1; k < r.size(); k++){
                    if(r[j].l == r[k].l && r[j].c == r[k].c){
                        r[j].alive = false;
                        r[k].alive = false;
                        
                    }
            }
            }

        //c_des e l_des calculam o deslocamente do robô em relação ao jogador em relação às colunas e às linhas respetivamente
        //Se este valor for positivo significa que o robô está numa linha/coluna maior que a do jogador e então têm que andar uma linha/coluna para trás
        //O deslocamente dá-se primeiro pelas linhas e só depois pelas colunhas ,ou seja,
        //O robô só se desloca pelas colunas quando estiver na mesma linha que o jogador

        if(l_des > 0 && r[i].alive){ //Robo está numa linha abaixo do jogador
            if(map[r[i].l-1][r[i].c] == '*' || map[r[i].l-1][r[i].c] == 'r' || map[r[i].l-1][r[i].c] == 'R'){
                r[i].alive = false;                 //Movimento que causa com que o robô morra ou fique preso 
                map[r[i].l][r[i].c] = ' ';
                map[r[i].l-1][r[i].c] = 'r';
                r[i].l -= 1;
            }
            else if(map[r[i].l-1][r[i].c] == ' '){ //Movimento sem consequências para o robô
                map[r[i].l][r[i].c] = ' ';
                map[r[i].l-1][r[i].c] = 'R';
                r[i].l -= 1;
            }
            else if(map[r[i].l-1][r[i].c] == 'H'){
                map[r[i].l][r[i].c] = ' ';          //Movimento em que o robô apanha o jogador
                map[r[i].l-1][r[i].c] = 'h';
                r[i].l -= 1;
                p.alive = false;
            }
        }
        else if (l_des < 0 && r[i].alive){ //Robo está numa linha acima do jogador
            if(map[r[i].l+1][r[i].c] == '*' || map[r[i].l+1][r[i].c] == 'r' || map[r[i].l+1][r[i].c] == 'R'){
                r[i].alive = false;                 //Movimento que causa com que o robô morra ou fique preso 
                map[r[i].l][r[i].c] = ' ';
                map[r[i].l+1][r[i].c] = 'r';
                r[i].l += 1;
            }
            else if(map[r[i].l+1][r[i].c] == ' '){ //Movimento sem consequências para o robô
                map[r[i].l][r[i].c] = ' ';
                map[r[i].l+1][r[i].c] = 'R';
                r[i].l += 1;
            }
            else if(map[r[i].l+1][r[i].c] == 'H'){
                map[r[i].l][r[i].c] = ' ';          //Movimento em que o robô apanha o jogador
                map[r[i].l+1][r[i].c] = 'h';
                r[i].l += 1;
                p.alive = false;
            }
            
        }
        else if(r[i].l == p.l && r[i].alive){

            if(c_des > 0){ //Robo está numa coluna a direita do jogador
                if(map[r[i].l][r[i].c-1] == '*' || map[r[i].l][r[i].c-1] == 'r' || map[r[i].l][r[i].c-1] == 'R'){
                    r[i].alive = false;                 //Movimento que causa com que o robô morra ou fique preso 
                    map[r[i].l][r[i].c] = ' ';
                    map[r[i].l][r[i].c-1] = 'r';
                    r[i].c -= 1;
                }
            else if(map[r[i].l][r[i].c-1] == ' '){ //Movimento sem consequências para o robô
                map[r[i].l][r[i].c] = ' ';
                map[r[i].l][r[i].c-1] = 'R';
                r[i].c -= 1;
            }
            else if(map[r[i].l][r[i].c-1] == 'H'){
                map[r[i].l][r[i].c] = ' ';          //Movimento em que o robô apanha o jogador
                map[r[i].l][r[i].c-1] = 'h';
                r[i].c -= 1;
                p.alive = false;
            }
        }
        else if (c_des < 0 && r[i].alive){ //Robo está numa coluna à esquerda do jogador
            if(map[r[i].l][r[i].c+1] == '*' || map[r[i].l][r[i].c+1] == 'r' || map[r[i].l][r[i].c+1] == 'R'){
                r[i].alive = false;                 //Movimento que causa com que o robô morra ou fique preso 
                map[r[i].l][r[i].c] = ' ';
                map[r[i].l][r[i].c+1] = 'r';
                r[i].c += 1;
            }
            else if(map[r[i].l][r[i].c+1] == ' '){ //Movimento sem consequências para o robô
                map[r[i].l][r[i].c] = ' ';
                map[r[i].l][r[i].c+1] = 'R';
                r[i].c += 1;
            }
            else if(map[r[i].l][r[i].c+1] == 'H'){
                map[r[i].l][r[i].c] = ' ';          //Movimento em que o robô apanha o jogador
                map[r[i].l][r[i].c+1] = 'h';
                r[i].c += 1;
                p.alive = false;
            }
        }
            
        }
    }
    }
}

void winner_file(const int maze_num, const time_t score){
    
    std::string string_maze = std::to_string(maze_num);
    std::string name="MAZE_" + string_maze + "_WINNER.txt";

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
        
        size_t n1, n2, n3;
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
        /*
            1 -> Verificar se existe espaço sequer
            2 -> Verificar se sem todos os possíveis espaços adicionais o username é maior q o first_name
            3 -> Verificar se o username sem espaços adicionais (exceto o espaço que separa o primeiro
                e último nome) não ultrapassa o limite (15)
            4 -> Antes de retirar os espaços de username verifica se depois do primeiro nome só há
                mais um nome (quando há um espaço depois do segundo nome então temos mais que 2 nomes)
        */
    
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

    f.clear();
    f.seekg(0,std::ios::end); //moving to the end of file
    f << first_name << " " << last_name;

    for(int i=username.length()+1;i<15;i++){
        f << " ";
    }
    f << score;


    // ORGANIZAR DE FORMA ASCENDENTE

    int n_lines=0;
    std::vector<struct winner_player> pl;

    f.clear();    
    f.seekg(0, std::ios::beg);
    std::string first_line;
    std::getline(f,first_line);
    
    //reading all names (first and last) and scores to different arrays who share the same positions 
    while(!f.eof()){

        winner_player p;
        f >> p.first_name >> p.last_name >> p.score;       
        p.username_length=p.first_name.length()+p.last_name.length()+1;
        n_lines++;
        pl.push_back(p);
    }

    f.seekg(first_line.length()+2, std::ios::beg);
    for(int i=0;i<5000;i++){
        f << "";
    }

    if(!n_lines){
        n_lines=FILE_MAX_C;
    }
    int array[n_lines], pos;

    //coping scores to "array" because scores value can't be modified
    for(int m=0;m<n_lines;m++){

        array[m]=pl[m].score;
    }

    
    int n=n_lines;
    while(n>0){
        
        int min=FILE_MAX_C;
        for(int aux=0;aux<n_lines;aux++){
            
            if(array[aux]<min){
                
                min=pl[aux].score;
                pos=aux;
            }
        }
        //if find min then it replaces on "array" for a much bigger value so it can
        //find the next min in the next iteration
        array[pos]=FILE_MAX_C;
        n--;
        
        f << pl[pos].first_name << " " << pl[pos].last_name;
        for(int i=pl[pos].username_length;i<15;i++){
            f << " ";
        }
        f << min << std::endl;
        
    }


    std::cout << "Your score is now saved!" << std::endl;
    f.close();
}

void print_rules(){

    std::cout << "-In this game there are 3 elements inside of a labyrinth:" << std::endl;
    std::cout << "-The electric fence is represented by \"*\"," << std::endl;
    std::cout << "-The player (you) is represented by \"H\" while you are alive, and \"h\" if you die" << std::endl;
    std::cout << "-The robots are represented by \"R\" if they are alive, and \"r\" if they are dead or stuck" << std::endl;
    std::cout << "-Touching the fence will kill either the robots or the player" << std::endl;
    std::cout << "-If an alive robot touches a dead one, he gets stuck and won´t move more" << std::endl;
    std::cout << "-The game's main objective is to escape the robots and you win when all robots are dead" << std::endl;
    std::cout << std::endl;
    std::cout << "Basicly don't hit fences and run from robots until they no longer move!" << std::endl;

}

void menu(bool &play_game, bool &exit){

    bool repeat=true;
    exit=false, play_game=false;

    while(repeat){

        std::cout << "Welcome to the robot game. Choose your option (1,2,3):" << std::endl;
        std::cout << "1 - Rules\n2 - Play\n0 - Exit\n\n>>";
        int option;
        
        //Read option
        // Inputs como "1x", sendo x qualquer conjunto de letras, serão aceites
        while(!read_int(option)){
            std::cout << "Not a valid menu option\n>>";
        }

        ctrl_exit();
                
        if((option == 1 || option == 2 || option == 0) ){

        switch(option){
            case 1:
                std::cout << "*Rules*" << std::endl;
                print_rules();

                while(1){

                    std::cout << "Press Q to go back to menu." << std::endl << ">>";
                    std::string back_menu;
                    std::cin >> back_menu;
                    ctrl_exit();

                    
                    
                    if((back_menu.at(0)=='Q' || back_menu.at(0)=='q') && back_menu.length() == 1){
                        break;
                    }
                    else{
                        std::cout << "Not valid." << std::endl;
                    }
                }
                break;

            case 2:
                play_game=true;
                repeat=false;
                break;

            case 0:
                exit=true;
                repeat=false;
                break;

            default:
                break;

         }
        }
           
        else {
            std::cout << "Not a valid menu option \n" << std::endl;
        }
        
    }
}

bool file_check(std::vector< std::vector<char> > &map, int maze_num, int &lines, int &columns){

    bool ret=true;
    std::ifstream f;
    std::string string_maze = std::to_string(maze_num);
    std::string file_name ="MAZE_" + string_maze + ".txt";
    f.open(file_name, std::ios::in);

    if(f){
        std::cout << "Successful reading" << std::endl;
        //LER TUDO O QUE É PRECISO PARA UM VETOR OU WTV
        std::string m,n, line;
        std::getline(f,m,'x');
        std::getline(f,n,'\n');

        int num_lines = std::stoi(m), num_colum = std::stoi(n);
        lines = num_lines;
        columns = num_colum;

        //Rezise map vector
        for (int i = 0; i < num_lines; i++)
        {
            // construct a vector of chars with given default value
            std::vector<char> v;
            for (int j = 0; j < num_colum; j++)
                v.push_back(' ');
    
            // push back above one-dimensional vector
            map.push_back(v);
        }

        
        for(int i = 0; i < num_lines; i++){ //leitura do mapa
            std::getline(f, line);
            
            for(int j = 0; j < num_colum; j++){
                map[i][j] = line[j];
            }
        }
    }
    else ret=false;

    f.close();

    return ret;
}

void play(){

    time_t t=time(NULL);
    std::vector< std::vector<char> > map;
    std::vector<robo> robos;
    int n_lines, n_columns;
    player jogador;
    // std::string maze_num;
    int maze_num;
    bool winner = false, ret_value = true;

    // Inputs como "12x", sendo x qualquer conjunto de letras, serão aceites
    std::cout << "Insert the number of the maze you want to play." << std::endl << ">>";
    do{

        //read maze number
        while(!read_int(maze_num)){
            std::cout << "Invalid maze! Try again. (0 to exit)\n>>";
        }

        if(!maze_num){
            return;
        }
                
        ret_value = file_check(map, maze_num, n_lines, n_columns);
        if(!ret_value  ){
            std::cout << "Invalid maze! Try again. (0 to exit)\n>>";
        }
    }while(!ret_value);
    
    
    get_pos(map, robos, jogador, n_lines, n_columns); //Obtem posição dos jogadores e dos robos

        
    //Draw Map
    for(int i = 0; i < n_lines; i++){
            for(int j = 0; j < n_columns; j++){
                std::cout << map[i][j];
            }
            std::cout << std::endl;
        }



    while(1){
        player_move(map, robos, jogador, n_lines, n_columns);
        robo_move(map, robos, jogador);
        clear_screen(true);

        //Draw map
        for(int i = 0; i < n_lines; i++){
                for(int j = 0; j < n_columns; j++){
                    std::cout << map[i][j];
                }
                std::cout << std::endl;
            }

        //Verificações para o jogo terminar
        if(jogador.alive == false){ //Se o jogador morreu
            std::cout << "You lost! :(" << std::endl << std::endl;
            break;
        }

        bool all_robos_dead = true;

        for(int i = 0; i < robos.size(); i++){
            if(robos[i].alive == true) //Se os robos morrerem todos
                all_robos_dead = false;
        }

        if(all_robos_dead == true){
            winner = true;
            break;
        }
    }
    
    if(winner){

        time_t score=time(NULL)-t;
        winner_file(maze_num,score);
    }
}


int main(){

    bool main_loop = true, play_game=false, exit=false;

    while(main_loop){

        menu(play_game,exit);
        if(play_game){
            play();
        }
        if(exit){
            main_loop=false;
            clear_screen(exit);
        }
    }
    std::cout << "O jogo acabou" << std::endl;
}


