//header test
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "complex.h"

//const double PI = 3.14159265359;

int main(){
    double mag, angleDeg, real, imag;
    char userInput[1000];
    int error = 0;
    clearWindow();

    //main loop
    while(1)
    {
        printf("Type in equation: p(mag, angle) i(real, imag):\n");
        fgets(userInput, sizeof(userInput), stdin);
        
        int len_temp = strlen(userInput);
        if(len_temp > 0 && userInput[len_temp - 1] == '\n'){
            userInput[len_temp - 1] = '\0';
        }
        if(strcmp(userInput, "exit") == 0 || strcmp(userInput, "Exit") == 0 || strcmp(userInput, "e") == 0){
            break;
        }

        printf("Eq. entered: %s\n", userInput);

        solveComplexEq(userInput, len_temp);   
    }

    return 0;
}

void solveComplexEq(char *equation, int eq_len){
    //find first operator
    char op_ptr;
    int op_pos = -1;
    char op;
    int ccomplex = 0, ppolar = 0;

    for (int i = 0; i < eq_len; i++){
        if (equation[i] == 'p'){
            for(int j = i; j < eq_len; j++){
                if(equation[j] == ')'){
                    i = j-2;
                    break;
                }
            }
        }
        if(equation[i] == '*' || equation[i] == '/' || equation[i] == '+' || equation[i] == '-'){

            op = equation[i];
            op_pos = i;
            break;
        }
    }
    if(op_pos == -1){
        return;
    }

    char leftTerm[15], rightTerm[15];
    int lt, rt, endPos;
    double mag[2], angle[2], real[2], imag[2];

    //find left term
    for(int j = op_pos - 1; j > 0; j--){
        if (sscanf(&equation[j-1], "p(%lf,%lf)", &mag[ppolar], &angle[ppolar])){
            ppolar++;
            lt = 1;
            break;
        }
        else if (sscanf(&equation[j-1], "i(%lf,%lf)", &real[ccomplex], &imag[ccomplex])){
            ccomplex++;
            lt = -1;
            break;
        }
    }
    if(lt == 1){
        printf("Left Term: %.3f<%.3f\n", mag[ppolar-1], angle[ppolar-1]);
    }
    else if(lt == -1){
        printf("Left Term: %.3f<%.3f\n", real[ccomplex-1], imag[ccomplex-1]);
    }
    printf("Operator: %c\n", op);
    
    //find right term
    for(int j = op_pos; j > 0; j++){
        if (sscanf(&equation[j], "p(%lf,%lf)", &mag[ppolar], &angle[ppolar])){
            rt = 1;
            ppolar++;
            for (int k = j-1; k < eq_len; k++){
               if(equation[k] == ')'){
                     endPos = k;
                     break;
                 }
             }
            break;
        }
        else if(sscanf(&equation[j], "i(%lf,%lf)", &real[ccomplex], &imag[ccomplex])){
            rt = -1;
            ccomplex++;
            for (int k = j-1; k < eq_len; k++){
               if(equation[k] == ')'){
                     endPos = k;
                     break;
                 }
             }
            break;
        }
    }
    if(rt == 1){
        printf("Right Term: %.3f<%.3f\n", mag[ppolar-1], angle[ppolar-1]);
    }
    else if(rt == -1){
        printf("Right Term: %.3f<%.3f\n", real[ccomplex-1], imag[ccomplex-1]);
    }
    // printf("LT %i\n", lt);
    // printf("RT %i\n", rt);
    // printf("Start Pos: %i\n", startPos);
    // printf("End Pos: %i\n", endPos);
    
    double fmag, fangle;

    //determine types and simplify
    if(op == '*' || op == '/'){
        if(lt == 1 && rt == 1){
            multPolar(mag, angle, &fmag, &fangle, op);
        }
        else if (lt == -1 && rt == -1){
            //call multiply complex function 
            multComplex(real, imag, &fmag, &fangle, op);
        }
        else{
            //call multiply polar and complex function
            multPolarImag(mag, angle, real, imag, ccomplex, ppolar, &fmag, &fangle, lt, op);
        }
    }
    else if(op == '+' || op == '-'){
        if(lt == 1 && rt == 1){
            //add polar eq function
            addPolar(mag, angle, &fmag, &fangle, op);
        }
        else if (lt == -1 && rt == -1){
            //add complex function
            addComplex(real, imag, &fmag, &fangle, op);
        }
        else{
            //add polar and complex function
            addPolarImag(mag, angle, real, imag, ccomplex, ppolar, &fmag, &fangle, lt, op);
        }
    }
    char result[30];
    sprintf(result, "p(%.3f,%.3f)", fmag, fangle);
    printf("Simplified version: %s\n", result);
    formatEq(equation, result, eq_len, endPos);
    printf("\nNew Equation:\n %s\n\n", equation);
    //recurvively call function
    eq_len = strlen(equation);
    solveComplexEq(equation, eq_len);

    return;
}

void addPolar(double mag[], double angle[], double *fmag, double *fangle, char op){
    double real, imag;
    angle[0] *= (180/PI);
    angle[1] *= (180/PI);
    
    if(op != '-'){
        real = mag[1]*(cos(angle[0])) + mag[1]*(cos(angle[1]));
        imag = mag[0]*(sin(angle[0])) + mag[1]*(sin(angle[1]));
    } 
    else{
        real = mag[1]*(cos(angle[0])) - mag[1]*(cos(angle[1]));
        imag = mag[0]*(sin(angle[0])) - mag[1]*(sin(angle[1]));
    }

    *fmag = sqrt(pow(real,2) + pow(imag,2));
    *fangle = atan(imag/real);
    return;
}

void multPolar(double mag[], double angle[], double *fmag, double *fangle, char op){
    if(op != '/'){
        *fmag = (mag[0])*(mag[1]);
        *fangle = angle[0] + angle[1];
    }
    else {
        *fmag = (mag[0])/(mag[1]);
        *fangle = angle[0] - angle[1];
    }
    return;
}

void addComplex(double real[], double imag[], double *fmag, double *fangle, char op){
    double freal, fimag;
    if(op != '-'){
        freal = real[0] + real[1];
        fimag = imag[0] + imag[1];
    }
    else{
        freal = real[0] - real[1];
        fimag = imag[0] - imag[1];
    }

    *fmag = sqrt(pow(freal,2)+pow(fimag,2));
    *fangle = (180/PI)*(atan(fimag/freal));
    return;
}

void multComplex(double real[], double imag[], double *fmag, double *fangle, char op){
    double tempMag[2], tempAngle[2];
    if(op != '/'){
        *fmag = sqrt(pow(real[0],2)+pow(imag[0],2))*sqrt(pow(real[1],2)+pow(imag[1],2));
        *fangle = (180/PI)*(atan(imag[0]/real[0]) + atan(imag[1]/real[1]));
    }
    else{
        *fmag = sqrt(pow(real[0],2)+pow(imag[0],2))/sqrt(pow(real[1],2)+pow(imag[1],2));
        *fangle = (180/PI)*(atan(imag[0]/real[0]) - atan(imag[1]/real[1]));
    }
}

void multPolarImag(double mag[], double angle[], double real[], double imag[], int ccomplex, int ppolar, double *fmag, double *fangle, int lt, char op){
    double tempAngle = (180/PI)*(atan(imag[ccomplex-1]/real[ccomplex-1]));
    if(op != '/'){
    *fmag = mag[ppolar-1]*sqrt(pow(imag[ccomplex-1],2)+pow(real[ccomplex-1],2));
    *fangle = angle[ppolar-1] + tempAngle;
    }
    else{
        if(lt == 1){
            *fmag = mag[ppolar-1]/sqrt(pow(imag[ccomplex-1],2)+pow(real[ccomplex-1],2));
            *fangle = angle[ppolar-1] - tempAngle;
        }
        else if (lt == -1){
            *fmag = sqrt(pow(imag[0],2)+pow(real[0],2))/mag[0];
            *fangle = tempAngle - angle[ppolar-1];    
        }
    }
}

void addPolarImag(double mag[], double angle[], double real[], double imag[], int ccomplex, int ppolar, double *fmag, double *fangle, int lt, char op){
    double angleRad = (PI/180)*angle[0];
    double tempReal, tempImag;
    ppolar -= 1;
    ppolar -= 1;

    if(op != '-'){
    tempReal = real[ccomplex] + mag[ppolar]*cos(angleRad);
    tempImag = imag[ccomplex] + mag[ppolar]*sin(angleRad);
    }
    else{
        if(lt == -1){
            tempReal = real[ccomplex] - mag[ppolar]*cos(angleRad);
            tempImag = imag[ccomplex] - mag[ppolar]*sin(angleRad);
        }
        else if (lt == 1){
            tempReal = mag[0]*cos(angleRad) - real[ccomplex];
            tempImag = mag[ppolar]*sin(angleRad) - imag[ccomplex];
        
        }

    }
    *fmag = sqrt(pow(tempReal,2)+pow(tempImag,2));
    *fangle = (180/PI)*(atan(tempImag/tempReal));
}

void formatEq(char *equation, char result[], int eq_len, int endPos){
    int resultLen = strlen(result);
    // Copy the result to the beginning of equation
    memcpy(equation, result, resultLen);
    // Copy the rest of the equation after endPos
    int i = resultLen;
    int j = endPos + 1;
    while (j < eq_len && equation[j] != '\0') {
        equation[i++] = equation[j++];
    }
    equation[i] = '\0';
    return;
}