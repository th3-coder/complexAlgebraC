#ifndef COMPLEX_H
    #define COMPLEX_H

    void polarToImag(double mag, double angleDeg, double *real, double *imag);
    void imagToPolar(double real, double imag, double *mag, double *angle);
    void clearWindow();
    void solveComplexEq(char *equation, int eq_len, double *buffmag, double *buffangle, int *bP, int *steps, char *solution, char showWork[][256], int *showsteps);
    void addPolar(double mag[], double angle[], double *fmag, double *fangle, char op, char showWork[][256], int *steps);
    void multPolar(double mag[], double angle[], double *fmag, double *fangle, char op, char showWork[][256], int *steps);
    void addComplex(double real[], double imag[], double *fmag, double *fangle, char op, char showWork[][256], int *steps);
    void multComplex(double real[], double imag[], double *fmag, double *fangle, char op, char showWork[][256], int *steps);
    void multPolarImag(double mag[], double angle[], double real[], double imag[], int ccomplex, int ppolar, double *fmag, double *fangle, int lt, char op, char showWork[][256], int *steps);
    void addPolarImag(double mag[], double angle[], double real[], double imag[], int ccomplex, int ppolar, double *fmag, double *fangle, int lt, char op, char showWork[][256], int *steps);
    void formatEq(char *equation, char result[], int eq_len, int endPos, int startPos, int bP, int *steps);
    static const double PI = 3.14159265359;
    extern char userInput[256];
    extern char solution[256];

#endif
