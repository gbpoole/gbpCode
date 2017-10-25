#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void seal_render(render_info *render) {
    if(!render->sealed) {
        SID_log("Sealing render...", SID_LOG_OPEN);

        // Read expansion factor list if the file's been given
        if(strcmp(render->snap_a_list_filename, "none")) {
            FILE *fp_list = NULL;
            if(SID.I_am_Master) {
                if((fp_list = fopen(render->snap_a_list_filename, "r")) == NULL)
                    SID_trap_error("Could not open snapshot a_list {%s}", ERROR_IO_OPEN, render->snap_a_list_filename);
                render->n_snap_a_list = count_lines_data(fp_list);
            }
            SID_Bcast(&(render->n_snap_a_list), 1, SID_INT, SID.COMM_WORLD, MASTER_RANK);
            render->snap_a_list = (double *)SID_malloc(sizeof(double) * render->n_snap_a_list);
            if(SID.I_am_Master) {
                char * line        = NULL;
                size_t line_length = 0;
                for(int i = 0; i < render->n_snap_a_list; i++) {
                    grab_next_line_data(fp_list, &line, &line_length);
                    grab_double(line, 1, &(render->snap_a_list[i]));
                }
                fclose(fp_list);
                SID_free(SID_FARG line);
            }
            SID_Bcast(render->snap_a_list, render->n_snap_a_list, SID_DOUBLE, SID.COMM_WORLD, MASTER_RANK);
        }

        // Seal the scenes
        seal_scenes(render);

        // Set the total number of frames in the render
        render->n_frames = render->last_scene->last_frame + 1;

        // Set the flag saying that render has been sealed
        render->sealed = TRUE;
    }

    SID_log("Done.", SID_LOG_CLOSE);
}
