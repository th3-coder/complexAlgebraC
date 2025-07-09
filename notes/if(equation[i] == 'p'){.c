if(equation[i] == 'p'){
            // Expecting format: p(mag,angle)
            if (sscanf(&equation[i], "p(%lf,%lf)", &mag, &angle) == 2) {
                polarTerms[pCounter++] = mag;
                polarTerms[pCounter++] = angle;
            }
        }
        if(equation[i] == 'i'){
            // Expecting format: i(real,imag)
            if (sscanf(&equation[i], "i(%lf,%lf)", &real, &imag) == 2) {
                complexTerms[cCounter++] = real;
                complexTerms[cCounter++] = imag;