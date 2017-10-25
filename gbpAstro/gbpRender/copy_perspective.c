#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void copy_perspective(perspective_info *from, perspective_info *to) {
    SID_log("Copying perspective...", SID_LOG_OPEN);
    memcpy(to, from, sizeof(perspective_info));
    SID_log("Done.", SID_LOG_CLOSE);
}
