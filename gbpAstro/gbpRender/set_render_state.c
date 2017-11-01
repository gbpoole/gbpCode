#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

// Return true if the new perspective is sucessfully set
int set_render_state(render_info *render, int frame, int mode) {
    // Make sure the render structure has been finalized
    (render->mode) = mode;
    if(!render->sealed)
        seal_render(render);

    // Copy the next perspective to the camera
    perspective_info *perspective   = render->camera->perspective;
    scene_info *      current_scene = render->scenes;
    int               r_val         = GBP_FALSE;
    int               i_frame       = 0;
    int               i_scene       = 0;
    int               frame_start   = 0;
    int               frame_stop    = 0;
    while(current_scene != NULL && !r_val) {
        if(i_scene == 0)
            frame_start = current_scene->first_frame;
        frame_stop = current_scene->last_frame;
        if(frame >= current_scene->first_frame && frame <= current_scene->last_frame) {
            copy_perspective(current_scene->perspectives[frame - current_scene->first_frame], perspective);
            r_val = GBP_TRUE;
        }
        current_scene = current_scene->next;
        i_scene++;
    }

    // Make sure we're requesting a valid frame
    if(!r_val)
        SID_trap_error("Invalid frame (%d) requested in set_render_state() (min/max=%d/%d).", SID_ERROR_LOGIC, frame, frame_start, frame_stop);

    // Perform snapshot and smooth-file reading
    int    i_snap, j_snap, snap_best;
    double snap_diff, snap_diff_best;
    if(!check_mode_for_flag(render->mode, SET_RENDER_RESCALE)) {
        int *snap_list = (int *)SID_malloc(sizeof(int) * render->n_interpolate);
        // Determine which snapshot(s) to use
        if(render->snap_a_list != NULL && render->n_snap_a_list > 0) {
            if(render->n_interpolate > 1)
                SID_log("Selecting snapshots for t=%lf...", SID_LOG_OPEN, perspective->time);
            else
                SID_log("Selecting snapshot for t=%lf...", SID_LOG_OPEN, perspective->time);
            pick_best_snap(perspective->time, render->snap_a_list, render->n_snap_a_list, &snap_best, &snap_diff_best);
            if(render->n_interpolate > 1) {
                // Determine the list of best snapshots to use
                if(snap_diff_best < 0)
                    snap_list[0] = GBP_MAX(0, snap_best - render->n_interpolate / 2);
                else
                    snap_list[0] = GBP_MAX(0, 1 + snap_best - render->n_interpolate / 2);
                for(i_snap = 1; i_snap < render->n_interpolate; i_snap++)
                    snap_list[i_snap] = snap_list[i_snap - 1] + 1;
                if(snap_list[render->n_interpolate - 1] >= render->n_snap_a_list) {
                    int j_snap;
                    for(i_snap = render->n_interpolate - 1, j_snap = 0; i_snap >= 0; i_snap--, j_snap++)
                        snap_list[i_snap] = render->n_snap_a_list - 1 - j_snap;
                }
                for(i_snap = 0; i_snap < render->n_interpolate; i_snap++)
                    SID_log("%03d (time=%8.6f)", SID_LOG_COMMENT, snap_list[i_snap], render->snap_a_list[snap_list[i_snap]]);
                // Set interpolation factor (a_i-a_0)/(a_1-a_0)
                // WARNING: THIS ASSUMES THAT n_interpolate==2
                render->f_interpolate =
                    (perspective->time - render->snap_a_list[snap_list[0]]) / (render->snap_a_list[snap_list[1]] - render->snap_a_list[snap_list[0]]);
                SID_log("f_interpolate=%le", SID_LOG_COMMENT, render->f_interpolate);
                if(render->n_interpolate != 2)
                    SID_trap_error("n_interpolate>2 not supported (yet).", SID_ERROR_NONE);

                SID_log("Done.", SID_LOG_CLOSE);
            } else if(render->n_interpolate <= 0)
                SID_trap_error("An invalid value for n_interpolate (%d) has been set.", SID_ERROR_LOGIC, render->n_interpolate);
            else {
                snap_list[0] = snap_best;
                SID_log("snap=%d is best with t=%lf...Done.", SID_LOG_CLOSE, snap_list[0], render->snap_a_list[snap_best]);
            }
        }

        // Initialize the array of pointers used to hold the snapshots
        if(render->plist_list == NULL) {
            SID_log("Initializing particle list...", SID_LOG_OPEN);
            render->plist_list = (plist_info **)SID_malloc(sizeof(plist_info *) * (render->n_interpolate));
            render->snap_list  = (int *)SID_calloc(sizeof(int) * (render->n_interpolate));
            for(i_snap = 0; i_snap < render->n_interpolate; i_snap++) {
                render->snap_list[i_snap]  = -1;
                render->plist_list[i_snap] = (plist_info *)SID_malloc(sizeof(plist_info));
                init_plist(render->plist_list[i_snap], NULL, GADGET_LENGTH, GADGET_MASS, GADGET_VELOCITY);
            }
            SID_log("Done.", SID_LOG_CLOSE);
        }

        // Move currently loaded snapshots to new locations in the list if necessary
        for(i_snap = 0; i_snap < render->n_interpolate; i_snap++) {
            if(snap_list[i_snap] != render->snap_list[i_snap]) {
                // Free plists we are done with before loading a new one to avoid
                //   unnecessarily doubling-up on RAM usage
                if(render->plist_list[i_snap] != NULL)
                    ADaPS_free(SID_FARG render->plist_list[i_snap]->data);
                render->snap_list[i_snap] = -1;
                for(j_snap = i_snap + 1; j_snap < render->n_interpolate; j_snap++) {
                    if(snap_list[i_snap] == render->snap_list[j_snap]) {
                        render->plist_list[i_snap]->data = render->plist_list[j_snap]->data;
                        render->snap_list[i_snap]        = render->snap_list[j_snap];
                        render->plist_list[j_snap]->data = NULL;
                        render->snap_list[j_snap]        = -1;
                    }
                }
            }
        }

        // Check (and if necessary, read) snapshots and smooth files here
        for(i_snap = 0; i_snap < render->n_interpolate; i_snap++) {
            if(render->snap_list[i_snap] < 0) {
                render->snap_list[i_snap] = snap_list[i_snap];
                read_gadget_binary_render(
                    render->snap_filename_root, render->snap_list[i_snap], render->plist_list[i_snap], READ_GADGET_RENDER_SCATTER);
                if(render->n_interpolate > 1)
                    read_smooth(render->plist_list[i_snap],
                                render->smooth_filename_root,
                                render->snap_list[i_snap],
                                SMOOTH_DEFAULT | READ_SMOOTH_LOG_SIGMA | READ_SMOOTH_LOG_RHO); // this is to speed-up logarythmic interpolation
                else
                    read_smooth(render->plist_list[i_snap], render->smooth_filename_root, render->snap_list[i_snap], SMOOTH_DEFAULT);
            }
        }
        SID_free(SID_FARG snap_list);

        // Fetch some info about the simulation that was stored in the Gadget files
        if(render->plist_list != NULL) {
            if(ADaPS_exist(render->plist_list[0]->data, "h_Hubble"))
                render->h_Hubble = ((double *)ADaPS_fetch(render->plist_list[0]->data, "h_Hubble"))[0];
            else
                render->h_Hubble = 1.;
            if(ADaPS_exist(render->plist_list[0]->data, "box_size_raw")) {
                render->box_size = ((double *)ADaPS_fetch(render->plist_list[0]->data, "box_size_raw"))[0];
            } else {
                render->box_size = 0.;
            }
        } else {
            render->h_Hubble = 1.;
            render->box_size = 0.;
        }

        // Mark particles
        perform_marking(render);
    } // if not rescale

    // This needs to be done near the end because
    //    it may depend on marking info (processed
    //    just above), etc.
    if(!render->camera->sealed)
        seal_camera(render);

    return (r_val);
}
