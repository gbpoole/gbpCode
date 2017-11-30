#define _MAIN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpLib.h>
#include <gbpSPH.h>

int main(int argc, char *argv[]) {
    SID_Init(&argc, &argv, NULL);

    // Parse command line
    char filename_in[256];
    int  snapshot;
    if(argc != 3) {
        SID_exit_error("Syntax: %s filename_in_root snapshot\n", SID_ERROR_SYNTAX, argv[0]);
    }
    strcpy(filename_in, argv[1]);
    snapshot = atoi(argv[2]);

    SID_log("Displaying header for GADGET file {%s}...", SID_LOG_OPEN, filename_in);

    // Display header
    plist_info plist;
    init_plist(&plist, NULL, GADGET_LENGTH, GADGET_MASS, GADGET_VELOCITY);
    read_gadget_binary_header(filename_in, snapshot, &plist);
    display_gadget_header(&plist);

    // Clean-up
    free_plist(&plist);
    SID_log("Done.", SID_LOG_CLOSE);
    SID_Finalize();
}
