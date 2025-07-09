#include <stdio.h>
#include <string.h>

int main() {

    char test[30];
    printf("Type full name: \n");
    //wrong !!!
    //scanf("%s", &test);

    //get full line
    fgets(test, sizeof(test), stdin);
    printf("Hello %s ", test);
    
    return 0;
}