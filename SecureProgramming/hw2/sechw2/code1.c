#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void processFile ( const char *path ) {
    printf ( "Process file executes \n" );
    char samplecommandtoexecute [100]={" ls -la "};
    strcat ( samplecommandtoexecute, path ) ;
    int status = system( samplecommandtoexecute );
    return;
}

bool checkEscape(char *path){
    if(strstr(path, "..") != NULL || strstr(path, ".") != NULL){
        return false;
    }
    return true;
}

int main ( int argc , char *argv [ ] ) {
    const char *safepath = "/home" ;
    size_t spl = strlen ( safepath ) ;
    char *fn = argv [ 1 ] ;
    if(checkEscape(fn)){
        if ( !strncmp ( fn , safepath , spl ) ) {
            processFile ( fn ) ;
        }
    }
    else{
        printf ( "Path specified is not valid !  \n" ) ;
        return 0;
    }
    return 0;
}
