/*****
* Project 02: Grid game
* COSC 208, Introduction to Computer Systems, Spring 2024
*****/








#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
void validated_input(WINDOW* input, WINDOW* description, char *toReturn); // This is a function handle
void get_input(WINDOW* input, WINDOW* description, char *toReturn);
char get_direction(WINDOW* input); // WINDOW* description


const char *ship_tags[] = {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};
const char carrier[]= {'S', 'S', 'S', 'S', 'S','\0'};
const char carrier_tag[] = "Carrier";
const char battleship[] = {'S', 'S', 'S', 'S','\0'};
const char battle_tag[] = "Battleship";
const char cruiser[] = {'S', 'S', 'S','\0'};
const char cruiser_tag[] = "Cruiser";
 const char submarine[] = {'S', 'S', 'S','\0'};
const char submarine_tag[] = "Submarine";
const char destroyer[] = {'S', 'S','\0'};
 const char destroyer_tag[] = "Destroyer";
const char *ships[] = {carrier, battleship, cruiser, submarine, destroyer};
const char *tags[] = {carrier_tag, battle_tag, cruiser_tag, submarine_tag, destroyer_tag};
const char hit = 'X';
const char miss = 'M';
const char b_space = 'O';
const char s_part = 'S';
const int b_size = 10;
const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
             






void windisplay( WINDOW* win, char **matrix, int rows, int cols){
   // This function is meant to display the contents of the given matrix to a given window
   // It does this be going through every element in the matrix and placing the info into the window
   int col = 4;
   int row = 2;
   for (int r = 0; r < rows; r++) {
       col = 4;
       for (int c = 0; c < cols; c++) {
            if (matrix[r][c] == 'S'){
                wattron(win,COLOR_PAIR(3));
                mvwprintw(win, row,col, "%c ", matrix[r][c]);
                wattroff(win,COLOR_PAIR(3));
            }
            else if (matrix[r][c] == 'M'){
                wattron(win,COLOR_PAIR(1));
                mvwprintw(win, row,col, "%c ", matrix[r][c]);
                wattroff(win,COLOR_PAIR(3));            }
              else if (matrix[r][c] == 'X'){
                wattron(win,COLOR_PAIR(2));
                mvwprintw(win, row,col, "%c ", matrix[r][c]);
                wattroff(win,COLOR_PAIR(2));            }
            else{
                mvwprintw(win, row,col, "%c ", matrix[r][c]);


            }
           col += 2;
       }
       row +=1;
   }
   wrefresh(win);
}


int overlap(char *board[], int row, int col, int dir, int size){
       // Overlap is meant to scan the area around dir and see if the place has any S, it returns 1 signifing we hit another ship
       // This function returns 0 of the desired ship doesnt overlab with any other ships
   if(dir == 0){
       for(int y =0; y< size; y++){
           if((row-y) <0||board[row-y][col] ==s_part){
               return 1;
           }
       }
   } else if(dir ==1){
       for(int y =0; y< size; y++){
           if((col+y) >9||board[row][col+y] ==s_part){
               return 1;
           }
       }
     
   } else if(dir ==2){
       for(int y =0; y< size; y++){
           if((row+y) >9 ||board[row+y][col] ==s_part){
               return 1;
           }
       }
   } else if(dir ==3){
       for(int y =0; y< size; y++){
           if((col-y) <0 || board[row][col-y] ==s_part){
               return 1;
           }
       }
   }
   return 0;
}


char **ai_assign_ships( char *ai_board[], char *ai_ship_locations[]){
   // This funciton is meant to assign the ai thier random ship board
   srand((unsigned int) time(NULL));
   // int rand(0);
   char carrier[] = {'S', 'S', 'S', 'S', 'S', '\0'};
   char battleship[] = {'S', 'S', 'S', 'S','\0'};
   char cruiser[] = {'S', 'S', 'S','\0'};
   char submarine[] = {'S', 'S', 'S','\0'};
   char destroyer[] = {'S', 'S','\0'};
   char *ships[] = {carrier, battleship, cruiser, submarine, destroyer};
   for(int i =0; i< 5; i++){
       int row = rand() % 9; int col = rand() % 9;
       ai_ship_locations[i][0] = '0' + row;
       ai_ship_locations[i][1] = '0'+ col;
       int chosen = rand() % 4; //0 = N, 1 = E, 2= S, W = 3
       ai_ship_locations[i][3] = '0' +  strlen(ships[i]);//-('0'-strlen(ships[i]));
       if(chosen == 0 && row -strlen(ships[i]) > 0&& overlap(ai_board, row, col, chosen,strlen(ships[i]))==0){
           for(int y= 0;y < strlen(*(ships +i)); y++){
               ai_board[row-y][col] = s_part;
               ai_ship_locations[i][2] = 'N';
           }
       }else if(chosen ==1 && (col+strlen(ships[i])) <9  && overlap(ai_board, row, col, chosen,strlen(ships[i]))==0){
           for(int y= 0;y < strlen(*(ships +i)); y++){
               ai_board[row][col+y] = s_part;
               ai_ship_locations[i][2] = 'E';
           }
       }else if(chosen == 2 && (row + strlen(ships[i])) < 9 && overlap(ai_board, row, col, chosen,strlen(ships[i]))==0){
           for(int y= 0;y < strlen(*(ships +i)); y++){
               ai_board[row+y][col] = s_part;
               ai_ship_locations[i][2] = 'S';
           }
       }else if(chosen ==3 && (col - strlen(ships[i])) > 0  && overlap(ai_board, row, col, chosen,strlen(ships[i]))==0){//change+-5 to strlen of the ship
           for(int y= 0;y < strlen(*(ships +i)); y++){
               ai_board[row][col-y] = s_part;
               ai_ship_locations[i][2] = 'W';
           }
       }else{
           i--;
       }
 
   }
   return ai_board;
}


char **gen_battle_boardai(char *ai_ship_locations[]){
       // This function is meant to gen a display board for the ai that shows all the places we hit, this is different from the Ai_Location
   char **ai_board = malloc(sizeof(char *) *10); //check if it has to be size 11 for null
   for(int i =0; i< 10; i++){
       *(ai_board +i) = malloc(sizeof(char) *10);
       for(int y=0; y< 10; y++){
           ai_board[i][y] = b_space;
       }
   }
   ai_board = ai_assign_ships(ai_board, ai_ship_locations);
   return ai_board;
}


char **gen_battle_boardplayer(){
       // This function is meant to gen a display board for the player that shows all the places we hit, this is different from the player_Location
   char **player_board = malloc(sizeof(char *) *10); //check if it has to be size 11 for null
   for(int i =0; i< 10; i++){
       *(player_board +i) = malloc(sizeof(char) *10);
       for(int y=0; y< 10; y++){
           player_board[i][y] = b_space;
       }
   }
   return player_board;
}




void get_input(WINDOW* input, WINDOW* description, char *toReturn ){
   // prints the players current input and returns the input in an array to be later used
   // This function also validates player input making sure the player inputed cols and rows are valid
  wclear(input);
  box(input,0,0);
  mvwprintw(input,1,4,"Player Input");
  mvwprintw(input,2,4,"Row (A - J)");
  mvwprintw(input,3,4,"Col (0 - 9)");
  mvwprintw(input,5,4,"Chosen Row: ");
  mvwprintw(input,6,4,"Chosen Col: ");
  wrefresh(input);
  wmove(input,5,16);
  char row = wgetch(input);
  mvwprintw(input,5,16,"%c",row);
  wmove(input,6,16);
  char col = wgetch(input);
  mvwprintw(input,6,16,"%c",col);
  wrefresh(input);
  toReturn[0] = row;
  toReturn[1] = col;
  toReturn[2] = '\0';
  validated_input(input,description,toReturn);
}


// This fucntion asked the player for the desired location to put their ship and direction they want, It validates if their input
   // Is correct and if it isnt it promts error messages and askes the player for a new row and col until a valid one is provided
void player_ship_select(WINDOW* input, WINDOW* description, WINDOW* win, char **player_board, char **ship_locations){ // This will display the probelms found in the code
  wclear(description);
  box(description,0,0);
  char cRow; char cCol;
  mvwprintw(description,1,2 ,"Select where you want your ships to go:");
  for (int i = 0; i< 5; i++){
      int SHIP_SIZE = strlen(ships[i]);
      char to_Return[3];
      mvwprintw(description,3,2 ,"                                                             ");
      mvwprintw(description,4,2 ,"                                                             ");
      mvwprintw(description,5,2 ,"                                                             ");
      mvwprintw(description,4,2 ,"                                                               ");
      mvwprintw(description,6,2 ,"                                                               ");
      mvwprintw(description,2,2 ,"Select where you want to put your %s of size %d. (EX: A0)",tags[i],SHIP_SIZE);
      wrefresh(description);  get_input(input, description, to_Return); wrefresh(description);
      cRow = to_Return[0];  cCol = to_Return[1];
      int row = cRow -'A'; int col = cCol -'0';
      ship_locations[i][0] = cRow-17;  ship_locations[i][1] = cCol;
      int s_p = 0;
      // "1 = East, 2 = South, 3 = West 4 = Go Back)"
      while (s_p == 0){
          mvwprintw(description,3,2 ,"Which direction do you want your ship to face? (0 = North,");
          mvwprintw(description,4,2 , "W = West E = East, S = South, N = North 4 = Go Back)");
          wrefresh(description);
          char chosen = get_direction(input); wrefresh(description);
          if((chosen == 'N' || chosen == 'n')  && row+1 >= SHIP_SIZE ){
                 if(overlap(player_board, row, col, NORTH ,SHIP_SIZE)==0){
                     ship_locations[i][2] = 'N';
                     ship_locations[i][3] = SHIP_SIZE+'0';
                     for(int y= 0; y < strlen(*(ships +i)); y++){
                         player_board[row-y][col] = ships[i][y];
                         s_p++;
                     }
                 }else{
                     mvwprintw(description,6,2 ,"                                        ");
                     mvwprintw(description,6,2 ," Overlaps Try Again");
                 }
             } else if((chosen == 'E' || chosen == 'e') &&  col + SHIP_SIZE -1 < 10){
                 if(overlap(player_board, row, col,EAST,SHIP_SIZE)==0){
                     ship_locations[i][2] = 'E';
                     ship_locations[i][3] = SHIP_SIZE + '0';
                     for(int y= 0;y < strlen(*(ships +i)); y++){
                         player_board[row][col+y] = ships[i][y];
                         s_p++;
                     }
                 }else{
                     mvwprintw(description,6,2 ,"                                        ");
                     mvwprintw(description,6,2 ," Overlaps Try Again");
                 }
             }else if((chosen == 'S' || chosen == 's') && row + SHIP_SIZE -1 < 10){
                 if(overlap(player_board, row, col, SOUTH,SHIP_SIZE)==0){
                     ship_locations[i][2] = 'S';
                     ship_locations[i][3] = SHIP_SIZE + '0';
                     for(int y= 0;y < strlen(*(ships +i)); y++){
                         player_board[row+y][col] = ships[i][y];
                         s_p++;
                     }
                 }else{
                     mvwprintw(description,6,2 ,"                                        ");
                     mvwprintw(description,6,2 ," Overlaps Try Again");
                 }
             }else if((chosen == 'W' || chosen == 'w') && (col - SHIP_SIZE)  + 1 >= 0){
                 if(overlap(player_board, row, col, WEST,SHIP_SIZE)==0){
                         ship_locations[i][2] = 'W';
                         ship_locations[i][3] = SHIP_SIZE + '0';
                         for(int y= 0;y < strlen(*(ships +i)); y++){
                             player_board[row][col-y] = ships[i][y];
                             s_p++;
                         }
                 } else{
                     mvwprintw(description,6,2 ,"                                        ");
                     mvwprintw(description,6,2 ," Overlaps Try Again");
                 }
             }  else if (chosen == '4'){
                 i--;
                 break;
             }else{
                 mvwprintw(description,3,2 ,"                                                       ");
                 mvwprintw(description,4,2 ,"                                                       ");
                 mvwprintw(description,5,2 ,"Incorrect Input / Goes Out Of Bound");
             }
         wrefresh(win);
         windisplay(win,player_board,10,10);
     }
 }
}


  // This function is used to display the status of the given player displaying how many ships have been destoryed
void ship_status(WINDOW* description, char character,char **ship_locations, char **board, char **visual){ //this builds off when something is hit, which will be another function.
    wclear(description);
    box(description,0,0);
    wattron(description,COLOR_PAIR(3));
    mvwprintw(description,1,1,"Events (press any button to continue)");
    wattroff(description,COLOR_PAIR(3));
  int wrow = 2;
  for(int i=0; i < 5; i++){
       if(ship_locations[i][0] != hit){//!strcmp(ship_locations[i],"XXXX")
           int row = ship_locations[i][0]-'0';
           int col = ship_locations[i][1]-'0';
           char dir = ship_locations[i][2];
           int size = ship_locations[i][3]-'0';
           int isDestroyed =1;
            for(int y =0; y< size; y++){
               if(dir =='N'){
                   if(hit !=board[row-y][col]){
                       isDestroyed=0;
                       break;
                   }
               }
               else if(dir =='E'){
                   if(hit !=board[row][col+y]){
                       isDestroyed=0;
                       break;
                   }
               } else if(dir =='S'){
                   if(hit !=board[row+y][col]){
                       isDestroyed=0;
                       break;
                   }
               } else if(dir =='W'){
                   if(hit !=board[row][col-y]){
                       isDestroyed=0;
                       break;
                   }
               }
           }
           if(isDestroyed ==1){
               if(dir =='N'){
                   for(int y =0; y< size; y++){
                       visual[row-y][col] =s_part;
                   }
               } else if(dir =='E'){
                   for(int y =0; y< size; y++){
                       visual[row][col+y] =s_part;
                   }
               } else if(dir =='S'){
                   for(int y =0; y< size; y++){
                       visual[row+y][col] =s_part;
                   }
               } else if(dir =='W'){
                   for(int y =0; y< size; y++){
                       visual[row][col-y] =s_part;
                   }
               }
               for(int y =0; y< strlen(ship_locations[i]); y++){
                   ship_locations[i][y] = hit;
               }
               if(character == 'a'){
                   mvwprintw(description,wrow,2,"Your opponents %s was sunk!", ship_tags[i]);
               }else{
                   mvwprintw(description,wrow,2,"Your %s was sunk!", ship_tags[i]);
               }
               wrow++;
           }
       }
   }
    wrefresh(description);




}


int checkLoser(char *shipLocations[]){
 int y= 0;
for(int i =0; i < 5; i++){
    if(shipLocations[i][0]== hit){
        y++;
    }
}
if(y==5){
 return 1;
}
return 0;
}


void ai_hit( WINDOW* description, WINDOW* win, char **player_visual, char**player_board){
   // This function is meant to allow the ai  to choose a location they want to hit
//int turn_done = 0;
//while(turn_done == 0){
    wclear(description);
    box(description,0,0);
    int row = rand() %10;
    int col = rand()%10;
    //read the whole array for X's, and pick where to hit.
    //base function:
    int rT =0; int cT = 0;
    int targetrtfound =0;
    int targetctfound =0;
    int swab =0;
   for(int i =0; i<b_size; i++){
        for(int y =0; y< b_size; y++){
            if(player_visual[i][y]==hit && (targetrtfound ==0 && targetctfound ==0) &&swab==0){
                rT =i; cT=y;
                swab =1;
            }
            if(!targetrtfound && !targetctfound && swab >0){
                 if(rT!=0 && player_visual[rT-1][cT] == b_space){
                     rT--;
                     targetrtfound++;
                     targetctfound++;
                 }else if(rT!=9 && player_visual[rT+1][cT] == b_space){
                     rT++;
                     targetrtfound++;
                     targetctfound++;
                 }else if((cT!=0 && player_visual[rT][cT-1] == b_space) ){
                     cT--;
                     targetrtfound++;
                     targetctfound++;
                 }
                 else if((cT!=9 && player_visual[rT][cT+1] == b_space)){
                     cT++;
                     targetrtfound++;
                     targetctfound++;
                 }
             }
            }
        if(i==9 && swab ==1){
            row = rT;
            col = cT;
            break;
        }
   }
   if(player_visual[row][col] == s_part|| player_visual[row][col] == miss || player_visual[row][col] == hit){
        while(player_visual[row][col]== s_part || player_visual[row][col] == miss||  player_visual[row][col] == hit ){
            row = rand() %10;col = rand()%10;
        }
   }
    if(player_board[row][col]==s_part){
        player_visual[row][col] = hit;//switch out with s when it hits a ship;
        player_board[row][col] = hit;
          mvwprintw(description,1,2,"Your opponent hit one of your ships! (press any button to continue)");
    }else{
        player_visual[row][col] = miss;//switch out with s when it hits a ship;
        player_board[row][col] = miss;
          mvwprintw(description,1,2,"Your opponent missed you (press any button to continue)!");
    }
    wrefresh(description);
    wgetch(description);
}




void player_hit(WINDOW* description,  WINDOW* input,  WINDOW* aiwin, char **ai_visual, char**ai_board){
   // This function is meant to allow the player to choose a location they want to hit, using valid_input this function
   // valiates if the player chose a valid location and marks the location from player board
  wclear(description);
  box(description,0,0);
  wrefresh(description);
  char cRow;
  char cCol;
  char arr[3];
  int turn_done = 0;
  mvwprintw(description,1,1,"Select coords to hit their ship.\n");
  wrefresh(description);
   while(turn_done==0){
       get_input(input, description, arr);
       cRow = arr[0];
       cCol = arr[1];
       int row = cRow -'A';
       int col = cCol -'0';
       if(ai_visual[row][col] != 'S' && ai_board[row][col] == 'S'){
           ai_visual[row][col] ='X';
           ai_board[row][col] ='X';
           turn_done++;
           mvwprintw(description,2,1,"Hit Good Job. (press any button to continue)");
       }else if(ai_visual[row][col] == 'X' || ai_visual[row][col] == 'M' || ai_visual[row][col] == 'S'   ){
           mvwprintw(description,2,1,"You hit that spot already. (press any button to continue)");
       }else{
           ai_visual[row][col] ='M';
           ai_board[row][col] ='M';
           mvwprintw(description,2,1,"You missed! (press any button to continue)");
           turn_done++;
       }
        wgetch(description);  


        wrefresh(description);
   }
}


char get_direction(WINDOW* input){ // WINDOW* description
  wclear(input);
  box(input,0,0);
  mvwprintw(input,1,4,"Player Input");
  mvwprintw(input,2,1,"N = North, E = East");
  mvwprintw(input,3,1,"S = South, W = West");
  mvwprintw(input,4,1,"4 = Back: ");
  mvwprintw(input,6,4,"Chosen Input: ");
  char dir = wgetch(input);
  mvwprintw(input,6,17,"%c",dir);
  wrefresh(input);
  return dir;
}


void validated_input(WINDOW* input, WINDOW* description , char *toReturn){
    wclear(description);
    box(description,0,0);
    if( (toReturn[0] >= 'a' && toReturn[0] <='j') ){
    toReturn[0] = toReturn[0] - 32;
    }
    char cRow = toReturn[0];
    char cCol = toReturn[1];
    if ((cRow < 'A' || cRow > 'J') || (cCol < '0' || cCol > '9')){
        mvwprintw(description,3,3, "Please Stay in the Grid Remember Row(A -J) and COL(0 - 9) " );
        mvwprintw(description,4,3, "Please Try Again :)" );
        wrefresh(description);
        get_input (input, description, toReturn );
    }
}




void print_description(WINDOW* description){
    // This function is meant to display the inital directions of the game, is the first function that is called
    wclear(description);
    box(description,0,0);
    wattron(description,COLOR_PAIR(3));
    mvwprintw(description,1,2 ,"                    Welcome To Battle Ship");
    mvwprintw(description,2,2, "Each player deploys his ships (of lengths varying from 2 to 5 squares)");
    mvwprintw(description,3,2,"secretly on a square grid. Then each player shoots at the other's grid ");
    mvwprintw(description,4,2,"by calling a location. The defender responds by Hit! or Miss!. You try ");
    mvwprintw(description,5,2,"to deduce where the enemy ships are and sink them.");
    mvwprintw(description,6,2,"To Continue Press Any Button");
    wattroff(description,COLOR_PAIR(3));
    wrefresh(description);


}




void free_my_mans(char** input, int rows){
    for(int i =0; i< rows;i++){
        free(input[i]);
    }
    free(input);
}
//begins the game and runs all the necessary functions for the game to operate.
void initiate_game(char *player_board[], char *ai_board[], char *player_ship_locations[], char*ai_ship_locations[], char** player_visual, char **ai_visual){
    initscr(); // Sets up memory and clears the screen
    start_color(); // Enable color
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    cbreak(); noecho(); nodelay(stdscr, TRUE);
    /*Player SCREEN SIZE*/
    int height = 13; int width = 25;
    int start_x = 1 ; int start_y = 8;
    WINDOW *description = newwin(8,75,0, 0);
    box(description,0,0);
 
    WINDOW *input = newwin(8,22, 0, 80);
    wclear(input);
    box(input,0,0);
    wrefresh(description);
    WINDOW *win = newwin(height,width, start_y, start_x);
    refresh();
    box(win,0,0);
    mvwprintw(win,0,0, "----------Player---------");
    mvwprintw(win, 1,3, " 0 1 2 3 4 5 6 7 8 9");
    int idx = 65;
    for (int i = 2; i < 12; i++){
        mvwprintw(win, i,1, "%c",idx);
        idx++;
    }
    windisplay(win,player_board,10,10);
    int aistart_x = 50; int aistart_y = 8;
    WINDOW *aiwin = newwin(height,width,aistart_y, aistart_x);
    box(aiwin,0,0);
    mvwprintw(aiwin,0,0, "------------AI-----------");
    mvwprintw(aiwin, 1,3, " 0 1 2 3 4 5 6 7 8 9");
    int aiidx = 65;
    for (int i = 2; i < 12; i++){ // initallize the border of the thing
        mvwprintw(aiwin, i,1, "%c",aiidx);
        aiidx++;
    }
    windisplay(aiwin,ai_visual,10,10);
    wrefresh(aiwin);
    print_description(description);
    wgetch(win);
    player_ship_select(input, description, win, player_board, player_ship_locations);
    wclear(description);
    box(description,0,0);
    wrefresh(description);
    char playerChar = 'p';
    char aiChar= 'a';
    int loserchecked = 0;
    while(loserchecked==0){//probably a better way to do this
        player_hit(description,input,aiwin, ai_visual, ai_board);
        ship_status(description, aiChar, ai_ship_locations, ai_board, ai_visual);
        windisplay(aiwin,ai_visual,10,10);
        wgetch(description);  
        loserchecked = checkLoser(ai_ship_locations);
        if (loserchecked == 1){
            wclear(description);
            box(description,0,0);
            mvwprintw(description,3,5, "You Won! Congrats");
            mvwprintw(description,4,5, "Press Enter to Quit");
            wrefresh(description);
            break;
        }
        ai_hit(description, win, player_visual, player_board);
        ship_status(description, playerChar, player_ship_locations, player_board, player_visual);
        windisplay(win,player_board,10,10);
        wgetch(description);
        loserchecked = checkLoser(player_ship_locations);
        if(loserchecked ==1){
            wclear(description);
            box(description,0,0);
            mvwprintw(aiwin,3,5, "You Lost! Ai Took Over :(");
            mvwprintw(aiwin,4,5, "Press Enter to Quit");
            wrefresh(description);
            break;
        }
    }    
    while(1){
    int choice = wgetch(win);
        if(choice == 10){
            break;
        }
    }
    free_my_mans(ai_visual, 10);
    free_my_mans(player_visual,10 );
}






int main() {
    int GridWidth = 4;
    char **ai_ship_locations = malloc(sizeof(char *) * (GridWidth+1));
    char **player_ship_locations  = malloc(sizeof(char *) *(GridWidth+1));
   
    for(int i =0; i < GridWidth+1; i++){
        player_ship_locations[i] = malloc(sizeof(char) *(GridWidth + 1));
        ai_ship_locations[i] = malloc(sizeof(char) * (GridWidth + 1));
        for(int y=0; y< GridWidth +1; y++){
            player_ship_locations[i][y]='0';
            ai_ship_locations[i][y] ='0';
        }
        player_ship_locations[i][GridWidth]='\0';
        ai_ship_locations[i][GridWidth] ='\0';
    }
    char **player_board  = gen_battle_boardplayer();
    char **ai_board  = gen_battle_boardai(ai_ship_locations);
    char **player_visual = malloc(sizeof(char *) *11);
    char **ai_visual = malloc(sizeof(char *) *11);
    for(int i=0; i< b_size; i++){
        player_visual[i]= malloc(sizeof(char)*11);
        ai_visual[i] = malloc(sizeof(char) *11);
        for(int y=0; y< b_size; y++){
            player_visual[i][y] =b_space;
            ai_visual[i][y] = b_space;
        }
        player_visual[i][b_size] = '\0';
        ai_visual[i][b_size] = '\0';
    }
    initiate_game(player_board, ai_board, player_ship_locations, ai_ship_locations, player_visual, ai_visual);
    /* NCURSES START*/
    free_my_mans(ai_board, 10);
    free_my_mans(player_board,10 );
    free_my_mans(ai_ship_locations, 5);
    free_my_mans(player_ship_locations,5 );
    endwin();
    return 0;
}















