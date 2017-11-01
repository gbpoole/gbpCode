#include <gbpLib.h>
#include <gbpRender.h>
#include <gd.h>

void set_image_RGBY_MARKED(image_info *image_RGBY_in,
                           image_info *image_RY_in,
                           image_info *image_GY_in,
                           image_info *image_BY_in,
                           image_info *image_Y_in,
                           double      Y_min_in,
                           double      Y_max_in,
                           int         flag_log_Y) {
    double *image_RY  = image_RY_in->values;
    double *image_GY  = image_GY_in->values;
    double *image_BY  = image_BY_in->values;
    double *image_Y   = image_Y_in->values;
    int     n_colours = image_RGBY_in->n_colours - 1;
    double  Y_min     = Y_min_in;
    double  Y_max     = Y_max_in;
    if(flag_log_Y) {
        Y_min = take_log10(Y_min);
        Y_max = take_log10(Y_max);
    }
    double Y_range = Y_max - Y_min;
    for(int i_x = 0, i_pixel = 0; i_x < image_RGBY_in->width; i_x++) {
        for(int i_y = 0; i_y < image_RGBY_in->height; i_y++, i_pixel++) {
            // Draw colours from the RY, GY and BY images
            int red   = GBP_MAX(0, GBP_MIN((int)(((double)n_colours) * image_RY[i_pixel]), n_colours));
            int green = GBP_MAX(0, GBP_MIN((int)(((double)n_colours) * image_GY[i_pixel]), n_colours));
            int blue  = GBP_MAX(0, GBP_MIN((int)(((double)n_colours) * image_BY[i_pixel]), n_colours));
            int alpha = gdAlphaOpaque;

            // Compute the brightness of the pixel [0.->1.]
            double image_Y_i = image_Y[i_pixel];
            if(flag_log_Y)
                image_Y_i = take_log10(image_Y_i);
            double brightness;
            brightness = (image_Y_i - Y_min) / Y_range;
            brightness = GBP_MAX(0., GBP_MIN(brightness, 1.));

            // Scale the pixel colours by the brightness
            red   = (int)(brightness * (double)red);
            green = (int)(brightness * (double)green);
            blue  = (int)(brightness * (double)blue);

            gdImageSetPixel(image_RGBY_in->gd_ptr, i_x, i_y, gdTrueColorAlpha(red, green, blue, alpha));
        }
    }
}
