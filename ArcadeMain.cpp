//6x7 grid
//6 rows, 7 columns
//c w blue arcade 4

#include <iostream> //cin and cout
using namespace std;
#include "C4.h"

int main()
{
    char choice;
    srand(time(NULL));
    
	if (pickMode())//decide pvp (true) or pve (false)
		PvP(); //Start PvP
	else
		PvE(); //Start PvE
    
    //playagain loop
    do
    {
        cout<<"\nWould you like to play again? (y/n) ";
        cin>>choice;
    }
    //validate
    while (!(choice == 'y' || choice == 'n' || choice == 'Y' || choice == 'N')); 
    //start again if they choose yes, otherwise, thanks for playing
    if (choice=='y'||choice=='Y')
        main();
    else
        cout<<"Thanks for playing! \v-William and Greeley\n"; 
    
	return 0;
}
