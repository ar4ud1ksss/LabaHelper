/*!
	\file
	Program is helper with laba 1.1.1 and 1.1.3
	
        
	//\details
	You choose number of laba and enter names of need files
*/


#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "Const.h"
#include "laba.h"
#include "laba1.h"
#include "laba2.h"

//=============================================================================

int main ()
    {
    printf ("#Laba 1.1.1 \n"
                "#(c) Kirill Shcherbina 2k!8\n\n");
    

    printf ("Choose number of laboratory work '1 / 2'\n");

    int NumberLabi = 0 ; 
    scanf ("%d", &NumberLabi);

    assert ((NumberLabi == 1) || (NumberLabi == 2));

    if (NumberLabi == 1)
        {
        Laba1 ();
        return 0;
        }
    
    if (NumberLabi == 2)
        {
        Laba2 (); 
        return 0;
        }
    
    }
