#include <gbpLib.h>
#include <gbpRender.h>
#include <gd.h>

void set_image_RGBY(image_info *image_RGBY_in,
                    image_info *image_RGB_in,
                    image_info *image_Y_in,
                    double      RGB_min_in,
                    double      RGB_max_in,
                    double      Y_min_in,
                    double      Y_max_in,
                    int         flag_log_RGB,
                    int         flag_log_Y) {
    double Y_min = Y_min_in;
    double Y_max = Y_max_in;
    if(flag_log_Y) {
        Y_min = take_log10(Y_min);
        Y_max = take_log10(Y_max);
    }
    double RGB_min = RGB_min_in;
    double RGB_max = RGB_max_in;
    if(flag_log_RGB) {
        RGB_min = take_log10(RGB_min);
        RGB_max = take_log10(RGB_max);
    }
    double *image_RGBY = image_RGBY_in->values;
    double *image_RGB  = image_RGB_in->values;
    double *image_Y    = image_Y_in->values;
    int     n_colours  = image_RGB_in->n_colours - 1;
    double  RGB_range  = RGB_max - RGB_min;
    double  Y_range    = Y_max - Y_min;
    for(int i_x = 0, i_pixel = 0; i_x < image_RGBY_in->width; i_x++) {
        for(int i_y = 0; i_y < image_RGBY_in->height; i_y++, i_pixel++) {
            // Set the pixel colour
            double image_RGB_i = image_RGB[i_pixel];
            if(flag_log_RGB)
                image_RGB_i = take_log10(image_RGB_i);
            int pixel_value = (int)(n_colours * (image_RGB_i - RGB_min) / RGB_range);
            pixel_value     = GBP_MAX(0, GBP_MIN(pixel_value, n_colours));
            int red         = (int)image_RGB_in->colour_table[0][pixel_value];
            int green       = (int)image_RGB_in->colour_table[1][pixel_value];
            int blue        = (int)image_RGB_in->colour_table[2][pixel_value];
            int alpha       = gdAlphaOpaque;

            // Compute the brightness of the pixel [0.->1.]
            double image_Y_i = image_Y[i_pixel];
            if(flag_log_Y)
                image_Y_i = take_log10(image_Y_i);
            double brightness = (image_Y_i - Y_min) / Y_range;
            brightness        = GBP_MAX(0., GBP_MIN(brightness, 1.));

            // Scale the pixel colours by the brightness
            red   = (int)(brightness * (double)red);
            green = (int)(brightness * (double)green);
            blue  = (int)(brightness * (double)blue);

            gdImageSetPixel(image_RGBY_in->gd_ptr, i_x, i_y, gdTrueColorAlpha(red, green, blue, alpha));
        }
    }
}
