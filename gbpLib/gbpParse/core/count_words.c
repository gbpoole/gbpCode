#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpSID.h>
#include <gbpParse_core.h>

int count_words(char *line) {
    int n_words   = 0;
    int flag_last = GBP_FALSE;
    int flag_new  = GBP_FALSE;
    for(size_t i_line = 0; i_line < strlen(line); i_line++) {
        flag_new = !check_space(line + i_line);
        if(flag_new && !flag_last)
            n_words++;
        flag_last = flag_new;
    }
    return (n_words);
}
