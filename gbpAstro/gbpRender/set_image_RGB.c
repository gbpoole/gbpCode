#include <gbpLib.h>
#include <gbpRender.h>
#include <gd.h>

void set_image_RGB(image_info *image_RGB_in, double RGB_min_in, double RGB_max_in, int flag_log_RGB) {
    double *image_RGB = image_RGB_in->values;
    int     n_colours = image_RGB_in->n_colours - 1;
    double  RGB_min   = RGB_min_in;
    double  RGB_max   = RGB_max_in;
    if(flag_log_RGB) {
        RGB_min = take_log10(RGB_min);
        RGB_max = take_log10(RGB_max);
    }
    double RGB_range = RGB_max - RGB_min;
    for(int i_x = 0, i_pixel = 0; i_x < image_RGB_in->width; i_x++) {
        for(int i_y = 0; i_y < image_RGB_in->height; i_y++, i_pixel++) {
            double image_RGB_i = image_RGB[i_pixel];
            if(flag_log_RGB)
                image_RGB_i = take_log10(image_RGB_i);
            int pixel_value = (int)((double)n_colours * (image_RGB_i - RGB_min) / RGB_range);
            pixel_value     = MAX(0, MIN(pixel_value, n_colours));
            int red         = (int)image_RGB_in->colour_table[0][pixel_value];
            int green       = (int)image_RGB_in->colour_table[1][pixel_value];
            int blue        = (int)image_RGB_in->colour_table[2][pixel_value];
            int alpha       = gdAlphaOpaque;
            gdImageSetPixel(image_RGB_in->gd_ptr, i_x, i_y, gdTrueColorAlpha(red, green, blue, alpha));
        }
    }
}
