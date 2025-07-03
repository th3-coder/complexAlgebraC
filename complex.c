//change from polar to imaginary and vice versa

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

const double PI = 3.145296535;

void polarToImag(double *mag, double *angleDeg);
void imagToPolar(double *real, double *imag);
void clearWindow();


int main(){
    double mag, angleDeg, real, imag;
    char userChoice[6];
    int error = 0;
    clearWindow();

    //main loop
    while(1)
    {
        printf("P: Polar ---> Imag \tI:Imag ---> Polar \t(E or Exit): Exit\n");
        printf("Select: ");
        scanf("%s", &userChoice);
        printf("\n");
        if(strcmp(userChoice, "exit") == 0 || strcmp(userChoice, "Exit") == 0 || strcmp(userChoice, "e") == 0){
            break;
        }
        else if(strcmp(userChoice, "P") == 0 || strcmp(userChoice, "p") == 0){
            polarToImag(&mag, &angleDeg);
        }
        else if(strcmp(userChoice, "I") == 0|| strcmp(userChoice, "i") == 0){
            imagToPolar(&real, &imag);
        }
        else {
            printf("Invalid input, try again.\n");
        }
        // printf("Press any key to continue\n");
        // fflush(stdout);
        // scanf("%s", temp);
        
    }

    return 0;
}

void polarToImag(double *mag, double *angleDeg){
    //take input
    printf("Mag: ");
    scanf("%lf", &(*mag));

    printf("Angles (deg): ");
    scanf("%lf", &(*angleDeg));

    // printf("The angle is %.3f degrees\n", *angleDeg);
    // printf("The magnitude is %.3f \n", *mag);
    //calculations
    double angleRad = *angleDeg * PI/180;
    double real = cos(angleRad)*(*mag);
    double imag = sin(angleRad)*(*mag);
    

    if(real < 0.01) 
        real = 0;
    if (imag < 0.01)
        real = 0;
               
    clearWindow();
    
    printf("\nOriginal:\n"); // polar (old)
    printf("%.3f /_%.3f(deg)\n", *mag, *angleDeg);

    //make bold
    printf("\e[1m");
    printf("\nComplex Form:\n");
    //imag (new)
    printf("%.3f ", real);
    if(imag > 0){
        printf("+");
    } 
    else {
        printf("-");
    }
    printf(" j%.3f\n\n", imag);
    //clear bold formationg
    printf("\e[0m");
    return;

}

void imagToPolar(double *real, double *imag){
    //take input
    printf("Real: ");
    scanf("%lf", &(*real));

    printf("Imaginary: ");
    scanf("%lf", &(*imag));
    //calculations
    double mag = sqrt(pow((*real),2) + pow((*imag), 2));
    double angle = atan((*imag)/(*real));
    angle = angle*(180/PI);
    
    clearWindow();

    //imaginary (og)
    printf("\nOriginal:\n");
    printf("%.3f ", *real);
    if(imag > 0){
        printf("+");
    } 
    else {
        printf("-");
    }
    printf(" j%.3f\n", *imag);

    printf("\e[1m");
    printf("\nPolar Form:\n");
    
    //polar (new)
    printf("%.3f /_%.3f(deg)\n\n", mag, angle);
    printf("\e[0m");
    
    return;

}

void clearWindow(){
    #ifdef _WIN32 
        system("cls");
    #else 
        system("clear");
        #endif
}