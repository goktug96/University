#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

void readFile ( const char *filename ) {
    printf ( "Reading the file : %s \n" , filename ) ;
    FILE *filepointer ;
    int c;
    filepointer = fopen(filename, "r");
    if ( filepointer == NULL) {
        printf ( "Error : %s \n" , strerror ( errno ) ) ;
        return ;
    }
    while (( c=fgetc ( filepointer )) != EOF)
        printf ( "%c " , (char) c ) ;
    fclose ( filepointer );
    return ;
}

int main ( int argc , char *argv [ ] ) {
    const char protectedfile [ ] = "mysecretfile.txt";

    char *fn = argv [ 1 ] ;
    char *temp = fn;

    while(*temp){ //convert to lower characters
        *temp = tolower((unsigned char)*temp);
        temp++;
    }

    if(strstr(fn, protectedfile)==NULL){ //if private file does not exist in given path
        if (strcmp (fn , protectedfile) != 0) {
            readFile ( fn ) ;
        }
    }
    else{
        printf ( "Access to the file specified is not permitted!  \n" ) ;
        return 0;
    }
    return 0;
}
