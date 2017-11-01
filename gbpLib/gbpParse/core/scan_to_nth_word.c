#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpSID.h>
#include <gbpParse_core.h>

int scan_to_nth_word(char *line, int n, char **word) {
    if(n <= 0)
        SID_trap_error("Invalid word index (%d) requested in scan_to_nth_word().", SID_ERROR_LOGIC, n);
    int    n_words   = 0;
    int    flag_last = GBP_FALSE;
    int    flag_new  = GBP_FALSE;
    size_t i_line    = 0;
    for(; i_line < strlen(line); i_line++) {
        flag_new = !check_space(line + i_line);
        if(flag_new && !flag_last)
            n_words++;
        if(n_words == n)
            break;
        flag_last = flag_new;
    }
    if(i_line >= strlen(line))
        SID_trap_error("End of line {%s} reached before finding n_word=%d", ERROR_LINE_TOO_SHORT, line, n);
    (*word) = line + i_line;
    return (n_words == n);
}
