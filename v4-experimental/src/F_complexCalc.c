#include "complex.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void polarToImag(double mag, double angleDeg, double *real, double *imag){
    //take input
    printf("Mag: ");
    scanf("%lf", &(mag));

    printf("Angles (deg): ");
    scanf("%lf", &(angleDeg));

    //printf("The angle is %.3f degrees\n", *angleDeg);
    //printf("The magnitude is %.3f \n", *mag);
    
    //calculations
    double angleRad = angleDeg * PI/180;
    *real = cos(angleRad)*(mag);
    *imag = sin(angleRad)*(mag);
               
    clearWindow();
    
    printf("\nOriginal:\n"); // polar (old)
    printf("%.3f /_%.3f(deg)\n", mag, angleDeg);

    //make bold
    printf("\e[1m");
    printf("\nComplex Form:\n");
    //imag (new)
    printf("%.3f ", *real);
    if(imag > 0){
        printf("+");
    } 
    else {
        printf("-");
    }
    printf(" j%.3f\n\n", *imag);
    //clear bold formationg
    printf("\e[0m");
    return;

}

void imagToPolar(double real, double imag, double *mag, double *angle){
    //take input
    printf("Real: ");
    scanf("%lf", &(real));

    printf("Imaginary: ");
    scanf("%lf", &(imag));
    //calculations
    *mag = sqrt(pow((real),2) + pow((imag), 2));
    *angle = atan((imag)/(real));
    *angle = *angle*(180/PI);
    
    clearWindow();

    //imaginary (og)
    printf("\nOriginal:\n");
    printf("%.3f ", real);
    if(imag > 0){
        printf("+");
    } 
    else {
        printf("-");
    }
    printf(" j%.3f\n", imag);

    printf("\e[1m");
    printf("\nPolar Form:\n");
    
    //polar (new)
    printf("%.3f /_%.3f(deg)\n\n", *mag, *angle);
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