#include <stdio.h>
#include <string.h>
#include "complex.h"

int main(){
    char userInput[256];
    int steps = 0, bP = 0;
    clearWindow();
    double buffMag, buffAngle;
    char showWork[64][256];
    int showsteps=0;
    //main loop
    while(1)
    {
        printf("Type in equation: p[mag, angle] i[real, imag]:\n");
        fgets(userInput, sizeof(userInput), stdin);
        
        int len_temp = strlen(userInput);
        if(len_temp > 0 && userInput[len_temp - 1] == '\n'){
            userInput[len_temp - 1] = '\0';
        }
        if(strcmp(userInput, "exit") == 0 || strcmp(userInput, "Exit") == 0 || strcmp(userInput, "e") == 0){
            break;
        }
        if(strcmp(userInput, "clear") == 0 || strcmp(userInput, "Clear") == 0 || strcmp(userInput, "c") == 0){
            clearWindow();
            break;
        }    
        printf("Eq. entered: %s\n\n", userInput);
        solveComplexEq(userInput, len_temp, &buffMag, &buffAngle, &bP, &steps, solution, showWork, &showsteps); 
    }

    return 0;
}
