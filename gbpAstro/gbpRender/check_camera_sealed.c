#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

int check_camera_sealed(camera_info *camera, int check_value) {
    if(!(camera->sealed == check_value)) {
        if(check_value)
            SID_trap_error("An unsealed camera is beig accessed when it is supposed to be sealed.", ERROR_LOGIC);
        else
            SID_trap_error("A sealed camera is beig accessed when it is supposed to be unsealed.", ERROR_LOGIC);
        return (FALSE);
    }
    return (TRUE);
}
