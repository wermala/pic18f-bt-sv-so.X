#include <xc.h>
#include <stdio.h>
#include "signal.h"

char *tableauSelectionne;
char nEchantillon = 0;

char triangle[] =
{
    16, 19, 22, 25, 28, 31, 28,
    25, 22, 19, 16, 13, 10, 7,
    4, 1, 4, 7, 10, 13
};
     
char sinus[] =
{
    16, 21, 25, 28, 30, 31, 30,
    28, 25, 21, 16, 11, 7, 4,
    2, 1, 2, 4, 7, 11
};
    
char carre[] =
{
    31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};

void selectionneTableau(char tableau){
    switch (tableau){
        case 0:
            tableauSelectionne = sinus;
            break;
        
        case 1:   
            tableauSelectionne = triangle;
            break;
            
        case 2:   
            tableauSelectionne = carre;
            break;
          
        default:
            break;
    }
}

unsigned char echantillon(){
    char vEchantillon;
    if(nEchantillon > 20) {
        nEchantillon = 0;
    }
    vEchantillon = tableauSelectionne[nEchantillon++]; 
    return vEchantillon;
}
