#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpSID.h>
#include <gbpParse_core.h>

int grab_float(char *line, int n, float *return_value) {
    char *word = NULL;
    scan_to_nth_word(line, n, &word);
    sscanf(word, "%f", return_value);
    return (ERROR_NONE);
}
