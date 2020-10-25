#ifndef C4_H
#define C4_H

const int ROWS=6;
const int COLS=7;

//Get choices
int getChoice(string[][COLS], string); //gets user's choice of column
int getCPUChoice(string[][COLS]); //generates a choice for the computer

//Find a Winner
bool findWinner(string[][COLS],string,int,int); //determines the winner of the game

//Pick your game mode
bool pickMode();
void PvP();//player vs. player
	string setColor(string);//to set the color of each player, change blue to escape string blue for example
void PvE();//player vs. environment
    //Arcade
    int getArcadeMode(string[][COLS], string); //generates a smarter choice for the computer-->not just a random column
    int ArcadeScore(string[][COLS], string,int,int); //returns Arcade Mode score
    void updateScoreboard(int);//takes in score after arcade and displays top score
    void checkScoreboard(); //checks if Scoreboard.txt file exists and creates one if it doesn't
    void drawScoreboard(string[], int[]); //draws the scoreboard

//Gameboard functions
void initGrid(string[][COLS]); //set grid first time
int updateGrid(string[][COLS], int, string);//updates the grid after a move has been made
void drawGrid(string[][COLS]);//draws out the grid in ascii
#endif