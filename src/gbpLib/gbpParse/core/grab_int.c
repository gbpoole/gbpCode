#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpSID.h>
#include <gbpParse_core.h>

int grab_int(char *line, int n, int *return_value) {
    char *word = NULL;
    scan_to_nth_word(line, n, &word);
    sscanf(word, "%d", return_value);
    return (SID_ERROR_NONE);
}
