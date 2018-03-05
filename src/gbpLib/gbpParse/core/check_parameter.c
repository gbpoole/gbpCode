#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpSID.h>
#include <gbpParse_core.h>

int check_parameter(char *line) {
    int line_length   = strlen(line);
    int r_val         = GBP_FALSE;
    int flag_continue = GBP_TRUE;
    if(count_words(line) > 0) {
        for(int i_line = 0; i_line < line_length && flag_continue; i_line++) {
            if(!check_space(line + i_line)) {
                flag_continue = GBP_FALSE;
                if(!strncmp(line + i_line, GBPPARSE_PARAMETER_CHARACTER, 1))
                    r_val = GBP_TRUE;
            }
        }
    } else
        r_val = GBP_TRUE;
    return (r_val);
}
