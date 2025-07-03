#include <stdio.h>
#include <math.h>

double PI = 3.145926535;

int main() {
    double userInput;
    printf("Enter value to calculate arch tangent: ");
    scanf("%lf", &userInput);
    double thetaRad = atan(userInput);

    printf("The arch tangent of %.3f is %.2f rads\n", userInput, thetaRad);

    double thetaDeg = thetaRad*(180/PI);
    printf("The arch tangent of %.3f is %.2f degrees\n", userInput, thetaDeg);

    if((thetaDeg < 90 && thetaDeg > 0) || (thetaDeg > -360 && thetaDeg < -270)){
        printf("In quadrant 1\n");
    }
    else if(thetaDeg < 180 && thetaDeg > 90 || (thetaDeg < -90 && thetaDeg > -180)){
        printf("In quadrant 2\n");
    } 
    else if(thetaDeg < 270 && thetaDeg > 180|| (thetaDeg < -180 && thetaDeg > -270)){
        printf("In quadrant 3\n");
    } 
    else if(thetaDeg < 270 || thetaDeg > -270){
        printf("In quadrant 4\n");
    }


    return 0;
}