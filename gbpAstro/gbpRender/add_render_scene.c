#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void add_render_scene(render_info *render, int n_frames) {
    // Make sure all previous scences are sealed
    seal_scenes(render);

    scene_info *current_s = NULL;
    scene_info *last_s    = NULL;
    scene_info *next_s    = NULL;
    scene_info *new_s     = NULL;
    SID_log("Adding render scene...", SID_LOG_OPEN);

    // Create a new scene
    init_scene(&new_s, n_frames);

    // Attach it to the end of the linked list
    current_s = render->scenes;
    last_s    = current_s;
    while(current_s != NULL) {
        last_s    = current_s;
        current_s = current_s->next;
    }

    // Set first scene pointer
    if(last_s == NULL) {
        render->first_scene = new_s;
        render->scenes      = new_s;
        new_s->first_frame  = 0;
    }

    // Set last (ie last added) scene pointers
    //   (Carry last scene's last perspective as new starting perspective)
    else {
        copy_perspective(last_s->perspectives[last_s->n_frames - 1], new_s->perspectives[0]);
        new_s->first_frame = last_s->last_frame + 1;
        last_s->next       = new_s;
    }
    new_s->last_frame  = new_s->first_frame + (new_s->n_frames - 1);
    render->last_scene = new_s;
    SID_log("Done.", SID_LOG_CLOSE);
}
