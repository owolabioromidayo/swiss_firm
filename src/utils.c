#include "utils.h"

char* read_tls_cert(){
    char * buffer = 0;
    long length;
    FILE * f = fopen( "./server_cert.pem", "r");

    if (f){
        printf("PROCESSING FILE \n");
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer){
            fread (buffer, 1, length, f);
        }
        fclose (f);
    }

    return buffer;
}