#include <iostream> //cin and cout
#include <string> //strings and stuff
#include <ctime> //srand(time(NULL));
#include <stdlib.h> 
#include <unistd.h> //usleep for animation
#include <fstream> //scoreboard
#include <iomanip> //fill

using namespace std;

#include "C4.h"

//decide to pve or pvp
bool pickMode(){
	char pick;
    //Choose a mode
	do{
		cout<<"Pick P to play against a player or C to play against the computer. [P/C] ";
		cin>>pick;
    //validate
	}while(!(pick=='p'||pick=='P'|| pick=='c'||pick=='C'));
    
	//pvp is true, pve is false, return as such
    if (pick=='p'||pick=='P')
		return true;
	return false;
}
//PvP
void PvP(){
	string appleJuice[ROWS][COLS];//initialize
	initGrid(appleJuice);//this function sets the board to empty slots
	
	int row, col, movesMade=0;
	string player1,color1,player2,color2;//player picks
	
	//player one picks
	cout<<"What is player 1's name? ";
	cin>>player1;
	do{
		//these colors are the escape sequence options
		cout<<"Pick a color to represent " << player1 <<". The options are red, yellow, green, cyan, blue and magenta. ";
		cin>>color1;
	}while(!(color1=="red"||color1=="yellow"||color1=="green"||color1=="cyan"||color1=="blue"||color1=="magenta"));
	
	//player two choices
	cout<<"What is player 2's name? ";
	cin>>player2;
	do{//this is input validation loop
		do{//this is no double colors loop
			cout<<"Pick a color to represent " << player2 <<". The options are red, yellow, green, cyan, blue and magenta. ";
			cin>>color2;

			if (color1==color2)
				cout<<"Sorry you can't both have the same color. Pick another color "<< player2 << ". \n";
        //validate no double colors
		}while(color1==color2);
    //validate choice
	}while(!(color2=="red"||color2=="yellow"||color2=="green"||color2=="cyan"||color2=="blue"||color2=="magenta"));
	
	color1=setColor(color1);//this should convert colors to escape strings
	color2=setColor(color2);
	
   
   	bool winner1=false,winner2=false;//for the gameplay loop to know who wins

	//actual gameplay loop
	while(!winner1 && !winner2 && movesMade<42){ 
        //Turn counter, using prefix operator
		cout<<"Turn: "<< ++movesMade<<endl<<endl;
        if (movesMade%2==1){
            col=getChoice(appleJuice,player1);//given the grid, and player name, will return the column they choose once that column is acceptable
            row=updateGrid(appleJuice,col,color1);//takes the column from above, then udpates grid with correct token color
       		winner1=findWinner(appleJuice,color1,row,col);//returns true if theres a winner

        }
        else{
            col=getChoice(appleJuice,player2);//same as player one, but with player two's info
            row=updateGrid(appleJuice,col,color2);
       		winner2=findWinner(appleJuice,color2,row,col);
        }
	}
    //If player one wins, say so, likewise with player two. Otherwise, the board filled and it must be a tie
    if (winner1)
        cout<<"Congrats "<<player1<<"! You won!\n";
    else if (winner2)
        cout<<"Congrats "<<player2<<"! You won!\n";
    else
        cout<<"Tie game!\n";
}
//returns escape sequence color, given color word
string setColor(string color){
    if(color=="red")
		color="\033[1;101m";
    if(color=="yellow")
		color="\033[1;103m";
    if(color=="green")
		color="\033[1;102m";
    if(color=="cyan")
		color="\033[1;106m";
	if(color=="blue")
		color="\033[1;104m";
    if(color=="magenta")
		color="\033[1;105m";
	return color;
}
//Player vs Computer or Arcade 
void PvE(){
    string appleJuice[ROWS][COLS]; 
	initGrid(appleJuice); //create and inititalize game board
	int row,col, movesMade=0;
   	bool playerWin=false,cpuWin=false;//for the gameplay loop
    string player,color,cpucolor,mode;//player picks
	
	//player picks
	cout<<"What is the player's name? ";
	cin>>player;
	do{
		//these colors are the escape sequence options
		cout<<"Pick a color to represent " << player <<". The options are red, yellow, green, cyan, blue and magenta. ";
		cin>>color;
	}while(!(color=="red"||color=="yellow"||color=="green"||color=="cyan"||color=="blue"||color=="magenta"));
    
    //set up the cpu color, which is red unless the player is already
    if (color=="red")
        cpucolor="yellow";
    else
        cpucolor="red";

    do{
        cout<<"Choose your game mode: normal or arcade\n";
        cin>>mode;
    }
    while (mode!="normal" && mode!="arcade");
    
    //Convert to usable escape sequences for updateGrid
    color=setColor(color);
    cpucolor=setColor(cpucolor);
    
    //If in arcade, give player instructions
    if(mode=="arcade")
        //instructions
        cout<<"\nInstructions: Try to fill up the board without the computer winning.\n"
            <<"You earn 100 points for every chip in a row of 4 or more.\n";
    
    int score=0;
    
    //start the gameplay loop
    while(!playerWin && !cpuWin && movesMade<42){
        //cout the turn with prefix operator
        cout<<"Turn: "<< ++movesMade<<endl<<endl;
        
        //If normal mode, and odd turn aka player turn, run normal player associated functions
        if (movesMade%2==1 && mode=="normal"){
            col=getChoice(appleJuice,player);//getChoice gets the column
            row=updateGrid(appleJuice,col,color); //updateGrid fills the top spot of that column, and returns that spot
       		playerWin=findWinner(appleJuice,color,row,col); //pass the inputs into findWinner 
        }
        //If arcade mode, and odd turn aka player turn, run arcade player associated functions
        else if (movesMade%2==1 && mode=="arcade"){
            col=getChoice(appleJuice,player); //getChoice gets the column
            row=updateGrid(appleJuice,col,color); //updateGrid fills the top spot of that column, and returns that spot 
            int round=ArcadeScore(appleJuice,color,row,col); //ArcadeScore checks points earned per move and saves that to round 
            score+=round; //add round to running total
            cout<<"\nTotal for this round: " << round<<endl; 
            cout<<"Your total score: "<<score<<endl;
            //this loop counts down from three, waiting a second between
            for (int i=3;i>0;i--){
                cout<<"Next round starting in: "<<i;
                cout.flush();
                usleep(1000000);
                cout<<"\r";    
            }
            cout<<endl;
        }
        //else it's the computer's turn
        else{
            //if it's normal or it's arcade and the computer's first turn, run random pick
            if (mode == "normal"||(mode=="arcade"&&movesMade==2))
                col=getCPUChoice(appleJuice);
            //else mode == "arcade" by default
            else 
                col=getArcadeMode(appleJuice, cpucolor);
            
            row=updateGrid(appleJuice,col,cpucolor); //after getting column, pass them into update and check for winning
       		cpuWin=findWinner(appleJuice,cpucolor,row,col);
        }
	}
    
    //postgame messages
    if(mode=="normal"){
        if (playerWin)
            cout<<"Congrats "<<player<<"! You won!\n";
        else if (cpuWin){
            cout<<"Sorry, "<<player<<", the computer beat you.\n";
        }
        else
            cout<<"Tie game!\n";
    }
    else{
        if (cpuWin)
            cout<<"Sorry, "<<player<<", the computer beat you.\n";
        else{
            cout<<"You win! Congratulations!\n";
            cout<<"You have earned 750 points for winning!\n";
            score+=750;
        } 
        cout<<"Your score: "<<score<<endl;
        updateScoreboard(score); 
    }
}
//Determine the Winner of the game
bool findWinner(string appleJuice[][COLS], string color,int row, int col){
    
    string chip = " "+color+"0"+"\033[0m ";
    //horizontal
        //move in one direction until a value that isn't chip
            int inARow=0,count=0;
            while (chip==appleJuice[row][(col+count)]){ //while chip matches, continue to the right
                count++;
                inARow++;
                if (col+count>COLS-1) //Check bounds so it doesn't go too far
                    break;
            }
            count=1;//set it to one as to not double count the initial spot
			if (col-count>=0){//to check bounds so while loop can access that location
				while (chip==appleJuice[row][(col-count)]){ //while chip matches, continue to the left
					count++;
					inARow++;
					if (col-count<0)//Check bounds so it doesn't go too far
						break;
				}
            }
            if (inARow>=4){
                cout<<"Horizontal Win!\n";
                return true;
            }
			
			
    //vertical
        //check three below
            count=0;
			inARow=0;
			while(chip==appleJuice[row+count][col]){
				count++;
				inARow++;
				if (row+count<0)
					break;
			}
			if(inARow>=4){
				cout<<"Vertical Win!\n";
				return true;
			}
    //diagonal
        inARow=0; //reset these to avoid carry over
        count=0;
        // (/) up to the right and down to the left
            while (chip==appleJuice[row-count][(col+count)]){ //while chip matches, continue up and to the right
                count++;
                inARow++;
                if (col+count>COLS-1||row-count<0)//Check bounds so it doesn't go too far
                    break;
            }

            count=1;
            if (row+count<ROWS&&col-count>=0){
                while (chip==appleJuice[row+count][(col-count)]){ //while chip matches, continue down and to the left
                    count++;
                    inARow++;
                    if (col-count<0||row+count>ROWS-1)//Check bounds so it doesn't go too far
                        break;
                }
            }
            if (inARow>=4){
                cout<<"Diagonal (/) Win!\n";
                return true;
            }

        // (\) down to the right and up to the left 
            inARow=0; //reset these as to not get any carry over
            count=0;
                while (chip==appleJuice[row+count][(col+count)]){ //while chip matches, continue down and to the right
                    count++;
                    inARow++;
                    if (col+count>COLS-1||row+count>ROWS-1)//Check bounds so it doesn't go too far
                        break;
                }

                count=1;
                if (col-count>=0&&row-count>=0){
                    while (chip==appleJuice[row-count][(col-count)]){ //while chip matches, continue up and to the left
                        count++;
                        inARow++;
                        if (col-count<0||row-count<0)//Check bounds so it doesn't go too far
                            break;
                    }
                }
            if (inARow>=4){
                cout<<"Diagonal(\\) Win!\n";
                return true;
            }

    return false;
}
//ARCADE ONLY: Award the player points
int ArcadeScore(string appleJuice[][COLS], string color,int row, int col){ 
    string chip = " "+color+"0"+"\033[0m"+" ";
    int score=0;
    //horizontal
        //move in one direction until a value that isn't chip
            int inARow=0,count=0;
            while (chip==appleJuice[row][(col+count)]){ //while chip matches, continue to the right
                count++;
                inARow++;
                if (col+count>COLS-1)//Check bounds so it doesn't go too far
                    break;
            }
            count=1;//set it to one as to not double count the initial spot
			if (col-count>=0){//to check bounds so while loop can access that location
				while (chip==appleJuice[row][(col-count)]){ //while chip matches, continue to the left
					count++;
					inARow++;
					if (col-count<0)//Check bounds so it doesn't go too far
						break;
				}
            }
            if (inARow>=4){
                cout<<"Horizontal Connection!\n";
                cout<<"400 points!";
                if(inARow>4)
                    cout<<"\nBonus points: "<<(inARow*100)-400<<endl;
                score+=100*inARow;
            }
			
			
    //vertical
        //check three below
            count=0;
			inARow=0;
			while(chip==appleJuice[row+count][col]){
				count++;
				inARow++;
				if (row+count<0)
					break;
			}
			if (inARow>=4){
                cout<<"Vertical Connection!\n";
                cout<<"400 points!\n";
                if(inARow>4)
                    cout<<"Bonus points: "<<(inARow*100)-400<<endl;
                score+=100*inARow;
            }
			
    //diagonal
        inARow=0; //reset these as to not get any carry over
        count=0;
        // (/) up to the right and down to the left
            while (chip==appleJuice[row-count][(col+count)]){ //while chip matches, continue up and to the right
                count++;
                inARow++;
                if (col+count>COLS-1||row-count<0)//Check bounds so it doesn't go too far
                    break;
            }

            count=1;
            if(col-count>=0&&row+count<ROWS){
                while (chip==appleJuice[row+count][(col-count)]){ //while chip matches, continue down and to the left
                    count++;
                    inARow++;
                    if (col-count<0||row+count>ROWS-1)//Check bounds so it doesn't go too far
                        break;
                }
            }
            if (inARow>=4){
                cout<<"Diagonal (/) Connection!\n";
                cout<<"400 points!\n";
                if(inARow>4)
                    cout<<"Bonus points: "<<(inARow*100)-400<<endl;
                score+=100*inARow;
            }

        // (\) down to the right and up to the left
            inARow=0; //reset these to avoid any carry over
            count=0;
            while (chip==appleJuice[row+count][(col+count)]){ //while chip matches, continue down and to the right
                count++;
                inARow++;
                if (col+count>COLS-1||row+count>ROWS-1)//Check bounds so it doesn't go too far
                    break;
            }

            count=1;
            if (row-count>=0&&col-count>=0){
                while (chip==appleJuice[row-count][(col-count)]){ //while chip matches, continue up and to the left
                    count++;
                    inARow++;
                    if (col-count<0||row-count<0)//Check bounds so it doesn't go too far
                        break;
                }
            }
            if (inARow>=4){
                cout<<"Diagonal(\\) Connection!\n";
                cout<<"400 points!\n";
                if(inARow>4)
                    cout<<"Bonus points: "<<(inARow*100)-400<<endl;
                score+=100*inARow;
            }
        
    return score; 
}
//ARCADE ONLY: Check if scoreboard file exists, if not creates one 
void checkScoreboard(){
    ifstream iFile("Scoreboard.txt");
    if(iFile.is_open()){
        return;
    }
    else{
        ofstream oFile("Scoreboard.txt");
		oFile<<"GBL"<<endl<<7950<<endl<<"WCP"<<endl<<7750<<endl;
        for (int i=0;i<8;i++){
            oFile<<"---"<<endl<<0000<<endl;
        }
    }
}
//ARCADE ONLY: Draw out the high scoreboard
void drawScoreboard(string names[], int scores[])
{
    cout<<"********************\n     Scoreboard\n********************\n";
    cout.fill('0');
    for (int i = 0; i< 10; i++)
        cout<<i+1<<". "<<names[i]<<"\t\t"<<setw(4)<<scores[i]<<endl;
}
//ARCADE ONLY: Add the player to scoreboard
void updateScoreboard(int score)
{
    checkScoreboard();//check if file exists  
    ifstream iFile("Scoreboard.txt");//open the file
    //check if the iFile exists
    if(iFile.is_open()){
        //take in the file contents and store in array
        string names[10];
        int points[10];
        string x;
        int index=-1,y;
        //alternates taking in the names and the scores
        for (int i=0; i<10; i++){ 
            iFile>>x>>y;
            names[i]=x;
            points[i]=y;
        }
        
        //find where on the scoreboard score goes
        for (int i=0;i<10;i++){
            if (score>points[i]){
                index=i;
                break;
            }
        }
        //if the score does not make it on the scoreboard
        if (index==-1){
            drawScoreboard(names,points);
            return;
        }
        
        string intials;
        cout<<endl;
        cout<<"Congratulations! You did well enough to place on the scoreboard. You are #"<<index+1<<".\n";
        cout<<"Enter your 3 character initials to save your high score: ";
        cin>>intials;
        while (intials.length()!=3){
            cout<<"Your initials need to be exactly three characters. Try again. ";
            cin>>intials;
        }
        
        
        int smallPoints[10]; //array of scores that will be pushed below the new score
        string smallNames[10]; //array of names that will be pushed below the new score
        int j=0;
        for (int i = index; i<10;i++){ //read big array into small array
            smallPoints[j]=points[i];
            smallNames[j]=names[i];
            j++;
        }
        
        points[index]=score;
        names[index]=intials;
        
        j=0;
        for (int i = index+1; i<10;i++){ //read small array back into big array below index
            points[i]=smallPoints[j];
            names[i]=smallNames[j];
            j++;
        }
        
        ofstream oFile ("Scoreboard.txt"); //output updated scoreboard array into Scoreboard.txt
        for (int i = 0; i<10; i++)
            oFile<<names[i]<<endl<<points[i]<<endl;
         
         iFile.close();
         oFile.close();
         cout<<endl<<endl;
         drawScoreboard(names, points);
    }

}   
//
//Gameboard functions
//
//initializes grid
void initGrid(string appleJuice[ROWS][COLS]){
    for (int i=0;i<ROWS;i++)
        for (int j = 0; j<COLS; j++)
            appleJuice[i][j] = " O ";
    drawGrid(appleJuice);
}

//prints out the game board
void drawGrid(string array[][COLS]){
    cout<<" ";
    for (int i = 1; i<8; i++)
        cout<<" "<<i<<" ";
    cout<<endl<<"┌─────────────────────┐"<<endl;
    for (int i=0;i<ROWS;i++){
        cout<<"├";
        for(int j=0;j<COLS;j++){
               cout<<array[i][j];
        }
        cout<<"┤"<<endl;
    }
    cout<<"└─────────────────────┘"<<endl;
}

//updates grid after a move
int updateGrid(string appleJuice[][COLS], int col, string color){
	int spot=ROWS-1; //spot is the place where the next token goes, so start it at the bottom
	//decrementing for loop to find the next spot from the bottom that is free
	for (int i = ROWS-1; i>=0; i--){
		//cout<<"i: "<<i<<endl;
        if (appleJuice[i][col] == " O ")
        {	
			spot=i;
			//cout<<spot<<endl;
			break;	
        }
	}
    
	//count down from the top until the spot is reached
	for(int i=0;i<ROWS;i++){
		appleJuice[i][col]=" "+color+"0"+"\033[0m ";//change it to filled in with users color
		system("clear");//clear out the screen
		drawGrid(appleJuice);//show the new grid
		cout.flush();
		usleep(200000);//pause

		//once spot is reached, play a noise and end
		if(i==spot){
			cout<<"\a";
			break;
		}
		//reset to empty slot
		appleJuice[i][col]= " O ";
	}
	
	system("clear"); //clear and show the grid one more time
	drawGrid(appleJuice);
    return spot; //returns the row to be used in findWinner
}

//gets a player's choice
int getChoice(string appleJuice[][COLS],string player)//gets users choice
{
    int choice;
    cout<<"It's your turn, "<<player<<". ";
    cout<<"Which column will you put your piece in? ";
    cin>>choice;
	choice--;
    while (choice < 0 || choice > COLS-1 || appleJuice[0][choice]!=" O ")
    {
        cout<<"Column is either invalid or full. Please choose another column: ";
        cin>>choice; //lets not forget to ask them for input again
        //I got distracted by the thought of a full column and just moved on
        //is that a tally? Yes.
		choice--;
    }
    return choice;
}

//cpu choice
int getCPUChoice(string appleJuice[][COLS])
{
    int cpu;
    do
    {
        cpu= rand()%(COLS-1);
    }
    while (appleJuice[0][cpu]!=" O ");
    return cpu;
}

int getArcadeMode(string appleJuice[][COLS], string color)
{   
        int max = 0, maxCol = 0;
        int maxArray[7]={ };
        int row;
        string chip = " "+color+"0"+"\033[0m"+" ";

           //for each column, find the top row, and check around that top spot
            for (int col = 0; col<COLS; col++){
				row=-1;
				max=0;
                //decrementing for loop to find the next spot from the bottom that is free
                for (int i = ROWS-1; i>=0; i--){
		            if (appleJuice[i][col] == " O "){	
		                row=i;
						//cout<<"Col is: "<<col<<" Row is: "<<row<<endl; //debugging
						break;
					}
				}
                //if the column is full
                if (row==-1){
                    maxArray[col]=-1;
                    continue;
                }
                //horizontal
                    //move in one direction until a value that isn't chip
                    int inARow=0,count=1;
                    if (col+count<COLS){ //to check bounds so the while loop doesn't cause issues trying to access that location
                        while (chip==appleJuice[row][(col+count)]){ //while chip matches, continue to the right
                            count++;
                            inARow++;
                            if (col+count>COLS-1)//Check bounds so it doesn't go too far
                                break;
                        }
                    }

                    count=1;//set it to one so as to not double count the initial spot
                    if (col-count>=0){//to check bounds so the while loop doesn't cause issues trying to access that location
                        while (chip==appleJuice[row][(col-count)]){ //while chip matches, continue to the left
                            count++;
                            inARow++;
                            if (col-count<0)//Check bounds so it doesn't go too far
                                break;
                        }
                    }
                    if (inARow>=max){
                        max = inARow;   
                    }

						
			    //vertical
			        //check three below
                    count=1;
                    inARow=0;
                    if (row+count<ROWS){
                        while(chip==appleJuice[row+count][col]){
                            count++;
                            inARow++;
                            if (row+count<0)
                                break;
                        }
                    }
                    if (inARow>=max){
                        max = inARow;  
                    }
                
			    //diagonal
                    inARow=0; //reset to avoid carry over 
                    count=1;
                    //up to the right and down to the left
                    if (col+count<COLS&&row-count>=0){
                        //while chip matches, continue up and to the right
                        while (chip==appleJuice[row-count][(col+count)]){ 
                            count++;
                            inARow++;
                            if (col+count>COLS-1||row-count<0)//Check bounds so it doesn't go too far
                                break;
                        }
                    }

                        count=1;
                    if (col-count>=0&&row+count<ROWS){
                        //while chip matches, continue down and to the left
                        while (chip==appleJuice[row+count][(col-count)]){ 
                            count++;
                            inARow++;
                            if (col-count<0||row+count>ROWS-1)//Check bounds so it doesn't go too far
                                break;
                        }
                    }
                        if (inARow>=max){
                            max = inARow;   
                        }

                    //down to the right and up to the left
                        inARow=0; //reset to avoid carry over
                        count=1;
                     if (col+count<COLS&&row+count<ROWS){
                        //while chip matches, continue down and to the right
                        while (chip==appleJuice[row+count][(col+count)]){ 
                            count++;
                            inARow++;
                            //Check bounds so it doesn't go too far
                            if (col+count>COLS-1||row+count>ROWS-1)
                                break;
                            }
                     }

                    count=1;
                    if (col-count>=0&&row-count>=0){ 
                            //while chip matches, continue up and to the left
                            while (chip==appleJuice[row-count][(col-count)]){ 
                                count++;
                                inARow++;
                                //Check bounds so it doesn't go too far
                                if (col-count<0||row-count<0)
                                    break;
                            }
                    }
                    if (inARow>=max)
                        max = inARow;   

                    maxArray[col] = max;
                }
                

/*             for (int i = 0; i<COLS; i++) //debugging */            
           do{
               //looks for maxCol (the column with the most amount of chips in a row)
                for (int i = 0; i<COLS; i++){ 
                    if (maxArray[i]>maxArray[maxCol])
                        maxCol = i;     
                }
                //if the maxCol is full, use next best option
                if (appleJuice[0][maxCol]!=" O "){ 
                    maxArray[maxCol] = -1;//makes column invalid
                }
            }
            while (appleJuice[0][maxCol]!=" O ");
                
               
        return maxCol;
}