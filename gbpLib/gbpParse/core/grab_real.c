#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpSID.h>
#include <gbpParse_core.h>

int grab_real(char *line, int n, GBPREAL *return_value) {
    char *word = NULL;
    scan_to_nth_word(line, n, &word);
#if USE_DOUBLE
    sscanf(word, "%llf", return_value);
#else
    sscanf(word, "%f", return_value);
#endif
    return (ERROR_NONE);
}
