#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void write_frame(render_info *render, int frame, int mode) {
    SID_log("Writing rendered frame...", SID_LOG_OPEN | SID_LOG_TIMER);

    // Create directory if needed
    char filename_out_dir_raw[SID_MAX_FILENAME_LENGTH];
    sprintf(filename_out_dir_raw, "%s/raw", render->filename_out_dir);
    mkdir(render->filename_out_dir, 02755);
    mkdir(filename_out_dir_raw, 02755);

    // Finalize images
    set_frame(render->camera);

    // Write left, right &/or mono images
    for(int i_set = 0; i_set < 3; i_set++) {
        // Set image set filesnames
        image_info **image_RGB         = NULL;
        image_info **image_RGBY        = NULL;
        image_info **image_RGBY_MARKED = NULL;
        image_info **image_Y           = NULL;
        image_info **image_RY          = NULL;
        image_info **image_GY          = NULL;
        image_info **image_BY          = NULL;
        int          stereo_offset_dir = 0;
        char         set_label[8];
        if(i_set == 0) {
            sprintf(set_label, "L");
            image_RGB         = render->camera->image_RGB_left;
            image_RGBY        = render->camera->image_RGBY_left;
            image_Y           = render->camera->image_Y_left;
            image_RY          = render->camera->image_RY_left;
            image_GY          = render->camera->image_GY_left;
            image_BY          = render->camera->image_BY_left;
            image_RGBY_MARKED = render->camera->image_RGBY_MARKED_left;
            stereo_offset_dir = -1;
        } else if(i_set == 1) {
            sprintf(set_label, "R");
            image_RGB         = render->camera->image_RGB_right;
            image_RGBY        = render->camera->image_RGBY_right;
            image_Y           = render->camera->image_Y_right;
            image_RY          = render->camera->image_RY_right;
            image_GY          = render->camera->image_GY_right;
            image_BY          = render->camera->image_BY_right;
            image_RGBY_MARKED = render->camera->image_RGBY_MARKED_right;
            stereo_offset_dir = 1;
        } else if(i_set == 2) {
            sprintf(set_label, "M");
            image_RGB         = render->camera->image_RGB;
            image_RGBY        = render->camera->image_RGBY;
            image_Y           = render->camera->image_Y;
            image_RY          = render->camera->image_RY;
            image_GY          = render->camera->image_GY;
            image_BY          = render->camera->image_BY;
            image_RGBY_MARKED = render->camera->image_RGBY_MARKED;
            stereo_offset_dir = 0;
        } else
            SID_exit_error("Undefined image set index in read_frame().", SID_ERROR_LOGIC);

        // Compute the depth for this stereo iteration
        set_camera_depths(render, stereo_offset_dir);

        // Write a set of images for each depth
        int flag_iteration_used = GBP_FALSE;
        for(int i_depth = 0; i_depth < render->camera->n_depth; i_depth++) {
            char filename_RGB[SID_MAX_FILENAME_LENGTH];
            char filename_Y[SID_MAX_FILENAME_LENGTH];
            char filename_RGBY[SID_MAX_FILENAME_LENGTH];
            char filename_RGBY_MARKED[SID_MAX_FILENAME_LENGTH];
            char filename_RY[SID_MAX_FILENAME_LENGTH];
            char filename_GY[SID_MAX_FILENAME_LENGTH];
            char filename_BY[SID_MAX_FILENAME_LENGTH];
            int  mode_png = mode;
            if(i_depth == 0) {
                sprintf(filename_RGB, "RGB_%s_%05d", set_label, frame);
                sprintf(filename_Y, "Y_%s_%05d", set_label, frame);
                sprintf(filename_RGBY, "RGBY_%s_%05d", set_label, frame);
                sprintf(filename_RY, "RY_%s_%05d", set_label, frame);
                sprintf(filename_GY, "GY_%s_%05d", set_label, frame);
                sprintf(filename_BY, "BY_%s_%05d", set_label, frame);
                sprintf(filename_RGBY_MARKED, "RGBY_MARKED_%s_%05d", set_label, frame);
            } else {
                sprintf(filename_RGB, "RGB_D_%02d_%s_%05d", i_depth, set_label, frame);
                sprintf(filename_Y, "Y_D_%02d_%s_%05d", i_depth, set_label, frame);
                sprintf(filename_RGBY, "RGBY_D_%02d_%s_%05d", i_depth, set_label, frame);
                sprintf(filename_RY, "RY_D_%02d_%s_%05d", i_depth, set_label, frame);
                sprintf(filename_GY, "GY_D_%02d_%s_%05d", i_depth, set_label, frame);
                sprintf(filename_BY, "BY_D_%02d_%s_%05d", i_depth, set_label, frame);
                sprintf(filename_RGBY_MARKED, "RGBY_D_%02d_MARKED_%s_%05d", i_depth, set_label, frame);
                mode_png = mode & (~WRITE_IMAGE_PNG);
            }
            if(image_RGB != NULL) {
                write_image(image_RGB[i_depth], render->filename_out_dir, filename_RGB, mode & (~WRITE_IMAGE_PNG));
                flag_iteration_used = GBP_TRUE;
            }
            if(image_Y != NULL) {
                write_image(image_Y[i_depth], render->filename_out_dir, filename_Y, mode & (~WRITE_IMAGE_PNG));
                flag_iteration_used = GBP_TRUE;
            }
            if(image_RGBY != NULL) {
                write_image(image_RGBY[i_depth], render->filename_out_dir, filename_RGBY, mode_png);
                flag_iteration_used = GBP_TRUE;
            }
            if(image_RY != NULL) {
                write_image(image_RY[i_depth], render->filename_out_dir, filename_RY, mode & (~WRITE_IMAGE_PNG));
                flag_iteration_used = GBP_TRUE;
            }
            if(image_GY != NULL) {
                write_image(image_GY[i_depth], render->filename_out_dir, filename_GY, mode & (~WRITE_IMAGE_PNG));
                flag_iteration_used = GBP_TRUE;
            }
            if(image_BY != NULL) {
                write_image(image_BY[i_depth], render->filename_out_dir, filename_BY, mode & (~WRITE_IMAGE_PNG));
                flag_iteration_used = GBP_TRUE;
            }
            if(image_RGBY_MARKED != NULL) {
                write_image(image_RGBY_MARKED[i_depth], render->filename_out_dir, filename_RGBY_MARKED, mode_png);
                flag_iteration_used = GBP_TRUE;
            }
        }

        // Write depth metadata (if iteration is used)
        if(flag_iteration_used && !SID_CHECK_BITFIELD_SWITCH(render->mode, SET_RENDER_RESCALE))
            write_frame_metadata(render, frame, set_label);
    }
    SID_log("Done.", SID_LOG_CLOSE);
}
