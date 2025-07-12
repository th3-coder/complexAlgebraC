#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include "complex.h"

char userInput[256] = {0};
char solution[256] = {0};

void solveComplexEq(char *equation, int eq_len, double *buffMag, double *buffAngle, int *bP, int *steps, char *solution){
    //find first operator
    char op_ptr;
    int op_pos = -1;
    char op;
    char result[256];
    double fmag, fangle; //result (passed by ref into functions)
    int startPos = -1, endPos;
    //scan for parenthesis call solve function using inside parenthesis only using output to finish [update]
    char temp[256];
    for(int i = 0; i < eq_len; i++){
        if(equation[i] == '(' && startPos == -1){
            startPos = i+1;
        }
        else if(equation[i] == ')'){
            endPos = i-1;
            break;
        }
    }
    if(startPos != -1){
        for(int i = 0; i < eq_len; i++){
            temp[i] = equation[i+startPos];
        }
        *bP = 1;
        temp[(endPos-startPos)+1] = '\0';
        //printf("Inside parenth: %s\n", temp);
        solveComplexEq(temp, strlen(temp), buffMag, buffAngle, bP, steps, solution);
        sprintf(result, "p[%.3f,%.3f]", *buffMag, *buffAngle);
        fmag = *buffMag;
        fangle = *buffAngle;
        //printf("Result: %s: \n", result);
        formatEq(equation, result, eq_len, startPos-1, endPos+1, *bP, steps);
        eq_len = strlen(equation);
        *bP = 0;
        solveComplexEq(equation, eq_len, buffMag, buffAngle, bP, steps, solution);
        return;
    }


    //check for operators using pemdas
    for (int i = 0; i < eq_len; i++){
        if(equation[i] == '*' || equation[i] == '/'){
            op = equation[i];
            op_pos = i;
            break;
        }
    }
    if(op_pos == -1){
        for (int i = 0; i < eq_len; i++){
            if (equation[i] == '+' || equation[i] == '-') {
                op = equation[i];
                op_pos = i;
                break;
            }
        }
    }

    //check if any operator found, convert single term if not [update]
    if(op_pos == -1){
        if((equation[0] == 'i' || equation[0] == 'p') && !bP){
            printf("\n\nFinal Solution:\n%s\n\n", equation);
            *steps = 0;
        }
        return;
    }

    if(*steps == 0){
        printf("%d   Solving\t%s\n", *steps, equation);
    }
    else{
        printf("%d    . . .\t%s\n", *steps, equation);
    }
    (*steps)++;

    //strings for left and right term 
    char leftTerm[15], rightTerm[15];
    //lt, rt (-1 for imag, 1 for polar) //startPos = index where left term starts //endPos = index where right term ends
    int lt, rt;
    int ccomplex = 0, ppolar = 0;
    double mag[2], angle[2], real[2], imag[2]; //arrays to store possible values 
    startPos = 0; endPos = 0;
    
    //find left term
    for(int j = op_pos - 1; j >= 0; j--){
        //check for polar term
        if (sscanf(&equation[j-1], "p[%lf,%lf]", &mag[0], &angle[0])){
            ppolar++;
            lt = 1;
            for(int k = j; k >= 0; k--){
                if(equation[k] == 'p'){
                    startPos = k;
                    break;
                }
            }
            break;
        }
        //check for imag term
        else if (sscanf(&equation[j-1], "i[%lf,%lf]", &real[0], &imag[0])){
            ccomplex++;
            lt = -1;
            for(int k = j; k >= 0; k--){
                if(equation[k] == 'i'){
                    startPos = k;
                    break;
                }
            }
            break;
        }    
    }
    
    //find right term
    for(int j = op_pos; j > 0; j++){
        if (sscanf(&equation[j], "p[%lf,%lf]", &mag[1], &angle[1])){
            rt = 1;
            ppolar++;
            for (int k = j-1; k < eq_len; k++){
               if(equation[k] == ']'){
                     endPos = k;
                     break;
                 }
             }
            break;
        }
        else if(sscanf(&equation[j], "i[%lf,%lf]", &real[1], &imag[1])){
            rt = -1;
            ccomplex++;
            for (int k = j-1; k < eq_len; k++){
               if(equation[k] == ']'){
                     endPos = k;
                     break;
                 }
             }
            break;
        }
    }

    // testing //
    // if(lt == 1){
    //     printf("Left Term: %.3f<%.3f\n", mag[0], angle[0]);
    // }
    // else if(lt == -1){
    //     printf("Left Term: %.3f<%.3f\n", real[0], imag[0]);
    // }
    // printf("Operator: %c\n", op);
    // if(rt == 1){
    //     printf("Right Term: %.3f<%.3f\n", mag[1], angle[1]);
    // }
    // else if(rt == -1){
    //     printf("Right Term: %.3f<%.3f\n", real[1], imag[1]);
    // }
    // // printf("LT %i\tRT %i\n", lt, rt);
    //printf("Start Pos: %i\tEnd Pos: %i\n", startPos, endPos);
    // printf("Operator Position: %i\n", op_pos);
    // printf("\nSolving: %s\n\n", equation);
    
    fmag = 0;
    fangle = 0; 

    //determine types and call functions to simplify equation
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
    result[0] = '\0';

    *buffMag = fmag;
    *buffAngle = fangle;
    //output result
    sprintf(result, "p[%.3f,%.3f]", fmag, fangle);
    //printf("Simplified version: %s\n", result);
    formatEq(equation, result, eq_len, startPos, endPos, *bP, steps);
    //recurvively call function
    eq_len = strlen(equation); //get update length of equation after formatting 
    solveComplexEq(equation, eq_len, buffMag, buffAngle, bP, steps, solution);

    equation[0] = '\0';

    return;
}

void addPolar(double mag[], double angle[], double *fmag, double *fangle, char op){
    double real, imag;
    angle[0] *= (180/PI);
    angle[1] *= (180/PI);
    
    if(op != '-'){
        real = mag[0]*(cos(angle[0])) + mag[1]*(cos(angle[1]));
        imag = mag[0]*(sin(angle[0])) + mag[1]*(sin(angle[1]));
    } 
    else{
        real = mag[0]*(cos(angle[0])) - mag[1]*(cos(angle[1]));
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
    double tempAngle;
    if(op != '/'){
        if(lt == 1){
            tempAngle = (180/PI)*(atan(imag[1]/real[1]));
            *fmag = mag[0]*sqrt(pow(imag[1],2)+pow(real[1],2));
            *fangle = angle[0] + tempAngle;
        }
        else if(lt == -1){
            tempAngle = (180/PI)*(atan(imag[0]/real[0]));
            *fmag = mag[1]*sqrt(pow(imag[0],2)+pow(real[0],2));
            *fangle = angle[1] + tempAngle;
        }
    }
    else{
        if(lt == 1){
            *fmag = mag[0]/sqrt(pow(imag[1],2)+pow(real[1],2));
            *fangle = angle[0] - tempAngle;
        }
        else if (lt == -1){
            *fmag = sqrt(pow(imag[0],2)+pow(real[0],2))/mag[1];
            *fangle = tempAngle - angle[1];    
        }
    }
}

void addPolarImag(double mag[], double angle[], double real[], double imag[], int ccomplex, int ppolar, double *fmag, double *fangle, int lt, char op){
    double angleRad = (PI/180)*angle[0];
    double tempReal, tempImag;
    ppolar -= 1;
    ppolar -= 1;

    if(lt = 1){
        ppolar = 0;
        ccomplex = 1;
    }
    else if(lt = -1){
        ppolar = 1;
        ccomplex = 1;
    }

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
            tempReal = mag[ppolar]*cos(angleRad) - real[ccomplex];
            tempImag = mag[ppolar]*sin(angleRad) - imag[ccomplex];
        }
    }
    *fmag = sqrt(pow(tempReal,2)+pow(tempImag,2));
    *fangle = (180/PI)*(atan(tempImag/tempReal));
}

//format equation and simplfying terms (resultant equation, result from simplification of two terms, length of equation, index of start of left hand term, index of start of right hand term)
void formatEq(char *equation, char result[], int eq_len, int startPos, int endPos, int bP, int *steps){
    char leftH[500], rightH[500];
    int resultLen = strlen(result);
    int rht = 0; 
    //find lht
    if(startPos){
        strncpy(leftH, equation, startPos);
        leftH[startPos+1] = '\0';
    }
    else{
        leftH[0] = '\0';
    }
    //printf("Equation length: %i\n", eq_len);

    int j = endPos;
    //check if op after endPos to determine if there is a rht
    while(j < eq_len-3){
        // if(equation[j] = '\0'){
        //     break;
        // }
        if(equation[j] == '*' || equation[j] == '/' || equation[j] == '+' || equation[j] == '-'){
            rht = 1;
            break;
        }
        j++;
    }
    j = 0;
    //find rht
    if(rht){
    for(int i = endPos + 1; i < eq_len; i++){
        if(equation[i] != '\0'){
            rightH[j++] = equation[i];
        }
        else{
            break;
        }
    }
    rightH[j] = '\0';
    }
    else{
        rightH[0] = '\0';
    }

    j = 0;
    equation[0] = '\0';
    while(j < strlen(leftH)){
        equation[j] = leftH[j];
        j++;
    }

    int i = 0;
    while(i < resultLen){
        equation[j++] = result[i++];
    }
    i = 0;
    while(i < strlen(rightH)){
        equation[j++] = rightH[i++];
    }
    equation[j] = '\0'; //add null op to end of string
    
    // concantanate strings into new equation
    // strncat(equation, leftH, strlen(leftH));
    // strncat(equation, result, resultLen);
    // strncat(equation, rightH, strlen(rightH));
    // Build new equation
    // snprintf(equation, 1000, "%s%s%s", leftH, result, rightH);

    // testing // 
    // printf("LeftH Length: %i\n", strlen(leftH));
    // printf("RightH Length: %i\n", strlen(rightH));
    // printf("Results length: %i\n", resultLen);
    //printf("Left hand: %s\tRight Hand: %s\tResult: %s\n", leftH, rightH, result);
    //printf("\nSolution: \n%s\n\n", equation);
    
    // old method to check if final solution //
    int leftH_len = strlen(leftH);
    int rightH_len = strlen(rightH);
    if(!leftH_len && !rightH_len && !bP){
        printf("\nSolution: \n%s\n\n", equation);
        *steps = 0;
    }
    //clear buff
    leftH[0] = '\0';
    rightH[0] = '\0';
    //result = '\0';
    return;
}
