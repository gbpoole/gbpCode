#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpSID.h>
#include <gbpParse_parameter_files.h>

int remove_parameter_character(char *line) {
    char temp_char[2];
    int  j, flag = GBP_TRUE, rval = GBP_FALSE;
    for(j = 0; j < strlen(line) && flag; j++) {
        sprintf(temp_char, "%c", line[j]);
        if(strcmp(temp_char, " ")) {
            if(!strcmp(temp_char, GBPPARSE_PARAMETER_CHARACTER)) {
                char replace[2];
                sprintf(replace, " ");
                strncpy(&(line[j]), replace, 1);
                rval = GBP_TRUE;
                flag = GBP_FALSE;
            }
        }
    }
    return (rval);
}
