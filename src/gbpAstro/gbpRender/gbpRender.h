#ifndef RENDER_AWAKE
#define RENDER_AWAKE
#include <stdint.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpSPH.h>
#include <gbpTrees.h>
#include <gd.h>
#if USE_FFMPEG
#include <avformat.h>
#include <swscale.h>
#endif

#define MAKE_MAP_DEFAULT 0
#define MAKE_MAP_APPLY_DIMMING SID_TTTP01
#define MAKE_MAP_COLOUR SID_TTTP02
#define MAKE_MAP_LUMINOSITY SID_TTTP03
#define MAKE_MAP_NO_WEIGHTING SID_TTTP04
#define MAKE_MAP_MODE_RHO SID_TTTP05
#define MAKE_MAP_MODE_SIGMA SID_TTTP06
#define MAKE_MAP_INV_SIGMA SID_TTTP07

#define N_KERNEL_TABLE 20000
#define SPH_KERNEL_2D SID_TTTP01
#define SPH_KERNEL_GADGET SID_TTTP02
#define SPH_KERNEL_GASOLINE SID_TTTP03
#define SPH_KERNEL_GAUSSIAN SID_TTTP04

#define CAMERA_MONO SID_TTTP00
#define CAMERA_STEREO SID_TTTP01
#define CAMERA_PLANE_PARALLEL SID_TTTP02
#define CAMERA_LOG_RGB SID_TTTP03
#define CAMERA_LOG_Y SID_TTTP04
#define CAMERA_DEFAULT CAMERA_MONO

#define CAMERA_RGB_MODE_1CHANNEL SID_TTTP01
#define CAMERA_RGB_MODE_MARKED SID_TTTP02
#define CAMERA_RGB_MODE_DEFAULT CAMERA_RGB_MODE_1CHANNEL

#define RENDER_INIT_PERSPECTIVE SID_TTTP01
#define RENDER_INIT_EVOLVE SID_TTTP02
#define RENDER_INIT_DEFAULT RENDER_INIT_PERSPECTIVE

#define SET_RENDER_DEFAULT SID_TTTP01
#define SET_RENDER_RESCALE SID_TTTP02
#define SET_RENDER_GADGET SID_TTTP03

#define WRITE_FRAME_DEFAULT SID_TTTP01
#define WRITE_FRAME_PNG_ONLY SID_TTTP02

#define RENDER_SWS_FLAGS SWS_BICUBIC
#define RENDER_FORMAT_DEFAULT "mpeg"
#define RENDER_AV_LOG_LEVEL -1
#define RENDER_GOP_SIZE 2
#define RENDER_QSCALE 1

#define WRITE_IMAGE_PNG SID_TTTP01
#define WRITE_IMAGE_RAW SID_TTTP02
#define WRITE_IMAGE_LOG SID_TTTP03
#define WRITE_IMAGE_DEFAULT (WRITE_IMAGE_PNG | WRITE_IMAGE_RAW)

#define READ_GADGET_RENDER_SCATTER SID_TTTP01
#define READ_GADGET_RENDER_ID_ORDERED SID_TTTP02
#define READ_GADGET_RENDER_DEFAULT READ_GADGET_RENDER_SCATTER

#define RENDER_INVALID_SSIMPL_DIR ":%* invalid directory *%:"

// Data structure which holds all info about an image
typedef struct image_info image_info;
struct image_info {
    gdImagePtr gd_ptr;
    int        width;
    int        height;
    int        n_pixels;
    double     range[2];
    int **     colour_table;
    double *   values;
    char       colourmapselect[SID_MAX_FILENAME_LENGTH];
    int        n_colours;
};

// Data structure which holds all info about a movie
typedef struct movie_info movie_info;
struct movie_info {
#if USE_FFMPEG
    AVStream *       video_stream;
    AVFormatContext *video_context;
    AVFrame *        picture;
    AVFrame *        temp_picture;
#endif
    uint8_t *video_outbuf;
    size_t   video_outbuf_size;
    int      width;
    int      height;
    int      n_pixels;
    int      n_frames;
    int      frame_count;
    int      frame_rate; // frames per second
    double   total_duration;
};

typedef struct perspective_info perspective_info;
struct perspective_info {
    double p_o[3];        // Object position
    double p_c[3];        // Camera position; computed from p_o, radius zeta and theta
    double d_o;           // Separation between object and camera
    double radius;        // Separation between object and camera modulo phi
    double theta;         // Rotation angle (azimuthal)   of camera position about x,y,z_o (after c-o transformation)
    double zeta;          // Rotation angle (altitudinal) of camera position about x,y,z_o (after c-o transformation)
    double phi;           // Zoom factor
    double FOV;           // Minimum field of view at object position
    double time;          // Used for time-evolving movies
    double focus_shift_x; // After all transformations are applied, this shifts everything in the image-frame.  Useful
    double focus_shift_y; //    for cases where you want flag_comoving=GBP_FALSE but don't want (0,0,0) in the image-centre.
    // The following things are ancillary parameters that don't need to be
    //    set by the user ... they are set when the scene is sealed.
    double d_near_field;       // Distance to the near-field cut-off
    double d_taper_field;      // Distance to the point where tapering starts.  Must be >= d_near_field.
    double d_image_plane;      // Distance to the image plane
    double stereo_offset;      // This is the lateral displacement that gets applied to the camera position for stereo renderings
    double FOV_x_object_plane; // Field of view for the rendered frame in the object plane
    double FOV_y_object_plane; // Field of view for the rendered frame in the object plane
    double FOV_x_image_plane;  // Field of view for the rendered frame in the image  plane
    double FOV_y_image_plane;  // Field of view for the rendered frame in the image  plane
};

typedef struct scene_info scene_info;
struct scene_info {
    int                n_frames;
    int                first_frame;
    int                last_frame;
    perspective_info **perspectives;
    int                sealed; // Set to GBP_TRUE only if scene initialization has been finalized
    int                flag_time_set;
    int                flag_p_o_set;
    int                flag_radius_set;
    scene_info *       next;
};

#define GBPRENDER_DEPTH_ARRAY_ID_SIZE 32
typedef struct camera_info camera_info;
struct camera_info {
    int               sealed; // Set to GBP_TRUE if camera initialization has been finalized
    int               camera_mode;
    char              colour_table[SID_MAX_FILENAME_LENGTH];
    int               flag_velocity_space;
    int               width;
    int               height;
    int               n_depth;
    int               n_depth_alloc;
    int               flag_depth_init;
    char **           depth_array_identifier; // each will be allocated with size GBPRENDER_DEPTH_ARRAY_ID_SIZE
    double *          depth_array;
    double *          depth_array_x;
    double *          depth_array_y;
    double *          depth_array_FOV_x;
    double *          depth_array_FOV_y;
    double *          depth_array_f_stretch;
    double            stereo_ratio;
    double            f_near_field;
    double            f_taper_field;
    double            f_image_plane;
    perspective_info *perspective; // Present perspective state of camera
    image_info **     image_RGBY;
    image_info **     image_RGBY_left;
    image_info **     image_RGBY_right;
    char *            mask_RGBY;
    char *            mask_RGBY_left;
    char *            mask_RGBY_right;
    image_info **     image_RGBY_MARKED;
    image_info **     image_RGBY_MARKED_left;
    image_info **     image_RGBY_MARKED_right;
    char *            mask_RGBY_MARKED;
    char *            mask_RGBY_MARKED_left;
    char *            mask_RGBY_MARKED_right;
    int               RGB_mode;
    char              RGB_param[64];
    double            RGB_range[2];
    interp_info *     RGB_gamma;
    ADaPS *           transfer_list;
    image_info **     image_RGB;
    image_info **     image_RGB_left;
    image_info **     image_RGB_right;
    char *            mask_RGB;
    char *            mask_RGB_left;
    char *            mask_RGB_right;
    int               Y_mode;
    char              Y_param[64];
    double            Y_range[2];
    interp_info *     Y_gamma;
    image_info **     image_Y;
    image_info **     image_Y_left;
    image_info **     image_Y_right;
    char *            mask_Y;
    char *            mask_Y_left;
    char *            mask_Y_right;
    int               Z_mode;
    double            Z_range[2];
    interp_info *     Z_gamma;
    // These images are used when colours can not be set with a colour table
    image_info **image_RY;
    image_info **image_RY_left;
    image_info **image_RY_right;
    image_info **image_GY;
    image_info **image_GY_left;
    image_info **image_GY_right;
    image_info **image_BY;
    image_info **image_BY_left;
    image_info **image_BY_right;
};

typedef struct mark_arg_info mark_arg_info;
struct mark_arg_info {
    char           species[32];
    char           type[32];
    double         dval[8];
    int            ival[8];
    char           value;
    int            flag_keep_properties;
    int            marked_n_particles;
    mark_arg_info *next;
};

typedef struct process_halo_info process_halo_info;
struct process_halo_info {
    int                  n_particles;
    size_t               offset;
    int                  id;
    int                  tree_case;
    int                  descendant_id;
    int                  tree_id;
    int                  file_offset;
    int                  file_index;
    int                  n_subgroups;
    int                  n_particles_peak;
    halo_properties_info properties;
    void *               ids;
};

typedef struct render_info render_info;
struct render_info {
    int          mode;
    double *     kernel_radius;
    double *     kernel_table;
    double *     kernel_table_3d;
    double       kernel_table_avg;
    camera_info *camera;
    scene_info * scenes;
    scene_info * first_scene;
    scene_info * last_scene;
    double       f_interpolate;
    int          n_interpolate;
    int          n_frames;
    char         filename_SSimPL_root[256];
    char         filename_halos_version[256];
    char         filename_trees_version[256];
    char         filename_out_dir[256];
    char         snap_filename_root[256];
    char         mark_filename_root[256];
    char         smooth_filename_root[256];
    char         snap_a_list_filename[256];
    int          n_snap_a_list;
    int          snap_number;
    double *     snap_a_list;
    int *        snap_list;
    int          flag_comoving;
    int          flag_fade;
    double       alpha_fade;
    int          flag_force_periodic;
    int          flag_read_marked;
    int          flag_add_absorption;
    plist_info **plist_list;
    tree_info *  trees;

    // Marking info
    mark_arg_info *       mark_arg_first;
    mark_arg_info *       mark_arg_last;
    int                   n_mark_args;
    int                   n_mark_properties;
    halo_properties_info *mark_properties;
    size_t *              mark_n_particles;

    double h_Hubble;
    double box_size;
    double f_absorption;
    int    w_mode;
    int    v_mode;

    // Colour info
    int     n_colour_list;
    char ** colour_name;
    int **  colour_RGB;
    float **colour_f_RGB;
    int     colour_index_black;
    int     colour_index_white;

    int sealed; // GBP_TRUE if the render is fully initialized
};

typedef struct map_quantities_info map_quantities_info;
struct map_quantities_info {
    int          flag_weigh;
    int          flag_line_integral;
    int *        ptype_used;
    size_t       n_particles;
    GBPREAL **   x;
    GBPREAL **   y;
    GBPREAL **   z;
    float **     h_smooth;
    float **     rho;
    float **     sigma;
    double       mass_array;
    interp_info *transfer_rho;
    int          flag_transfer_rho_log;
    interp_info *transfer_sigma;
    int          flag_transfer_sigma_log;
    int          flag_comoving;
    double       inv_expansion_factor_cubed;
    int          v_mode;
    int          w_mode;
};

// Function definitions
#ifdef __cplusplus
extern "C" {
#endif

double RGB_lookup(render_info *render, char colour, int channel);
char   fetch_render_colour_index(render_info *render, const char *name);

void init_perspective(perspective_info **perspective);
void free_perspective(perspective_info **perspective);
void copy_perspective(perspective_info *from, perspective_info *to);
void init_scene(scene_info **scene, int n_frames);
void seal_scenes(render_info *render);
void free_scenes(scene_info **scene);
void init_camera(camera_info **camera, int mode);
void seal_camera(render_info *render);
void free_camera(camera_info **camera);
void init_render(render_info **render);
void free_render(render_info **render);
void add_render_scene(render_info *render, int n_frames);
void seal_render(render_info *render);
int  check_camera_sealed(camera_info *camera, int check_value);
int  set_render_state(render_info *render, int frame, int mode);
int  set_render_depths(render_info *render,
                       double       x_hat,
                       double       y_hat,
                       double       z_hat,
                       double       theta,
                       double       theta_roll,
                       double       box_size,
                       double       expansion_factor);
void parse_render_file(render_info **render, char *filename);
void parse_set_scene_quantity(camera_info *camera, scene_info *scene, const char *quantity, char *line, int i_word_in);
void write_frame(render_info *render, int frame, int mode);
void write_frame_metadata(render_info *render, int frame, const char *set_label);
void read_frame(render_info *render, int frame);
void set_frame(camera_info *camera);
void set_render_scale(render_info *render, double RGB_min, double RGB_max, double Y_min, double Y_max, double Z_min, double Z_max);
int  set_transfer_function(char *line, int i_word, interp_info **return_interp);

void  rotate_particle(double x_hat, double y_hat, double z_hat, double theta, GBPREAL *x_i, GBPREAL *y_i, GBPREAL *z_i);
void  set_pixel_space(float   h_i,
                      float   x_i,
                      float   y_i,
                      float   f_i,
                      double  xmin,
                      double  ymin,
                      double  FOV_x,
                      double  FOV_y,
                      double  pixel_size_x,
                      double  pixel_size_y,
                      double  radius_kernel_max,
                      double *radius2_norm,
                      double *radius_kernel,
                      double *part_pos_x,
                      double *part_pos_y,
                      int *   kx_min,
                      int *   kx_max,
                      int *   ky_min,
                      int *   ky_max);
void  transform_particle(GBPREAL *x_i,
                         GBPREAL *y_i,
                         GBPREAL *z_i,
                         double   x_o,
                         double   y_o,
                         double   z_o,
                         double   x_hat,
                         double   y_hat,
                         double   z_hat,
                         double   d_o,
                         double   stereo_offset,
                         double   theta,
                         double   theta_roll,
                         double   box_size,
                         double   expansion_factor,
                         double   focus_shift_x,
                         double   focus_shift_y,
                         int      flag_comoving,
                         int      flag_force_periodic);
int   set_camera_depths(render_info *render, int flag_stereo_offset);
int   free_camera_depths(camera_info *camera);
void  free_particle_map_quantities(map_quantities_info *mq);
void  init_particle_map_quantities(map_quantities_info *mq, render_info *render, ADaPS *transfer_list, int flag_comoving, double expansion_factor);
void  set_particle_map_quantities(render_info *        render,
                                  map_quantities_info *mq,
                                  int                  mode,
                                  size_t               i_particle,
                                  float                box_size,
                                  float                half_box,
                                  float *              x_i,
                                  float *              y_i,
                                  float *              z_i,
                                  float *              h_i,
                                  float *              v_i,
                                  float *              w_i);
float compute_f_stretch(double d_image_plane, float z_i, int flag_plane_parallel);
void  compute_perspective_transformation(render_info *render,
                                         int          flag_stereo_offset,
                                         double *     FOV_x,
                                         double *     FOV_y,
                                         double *     d_o,
                                         double *     x_o_out,
                                         double *     y_o_out,
                                         double *     z_o_out,
                                         double *     x_c_out,
                                         double *     y_c_out,
                                         double *     z_c_out,
                                         double *     x_hat,
                                         double *     y_hat,
                                         double *     z_hat,
                                         double *     theta,
                                         double *     theta_roll);
int   check_if_particle_marked(char **mark, int i_species, size_t i_particle, char *c_i);
void  init_make_map_noabs(render_info *render,
                          int          stereo_offset_dir,
                          double       unit_factor,
                          const char * unit_text,
                          double       f_image_plane,
                          double       box_size,
                          double       FOV_x_in,
                          double       FOV_y_in,
                          double       xmin,
                          double       ymin,
                          double       pixel_size_x,
                          double       pixel_size_y,
                          double       radius_kernel_max,
                          int          nx,
                          int          ny,
                          double       expansion_factor,
                          double       focus_shift_x,
                          double       focus_shift_y,
                          double       d_near_field,
                          double       stereo_offset,
                          int          flag_comoving,
                          int          flag_force_periodic,
                          int          camera_mode,
                          int *        flag_weigh,
                          int *        flag_line_integral,
                          float **     x,
                          float **     y,
                          float **     z,
                          float **     h_smooth,
                          float **     f_stretch,
                          float **     value,
                          float **     weight,
                          char **      colour,
                          size_t **    z_index,
                          int *        i_x_min_local_return,
                          int *        i_x_max_local_return,
                          size_t *     n_particles);
void  init_make_map_abs(render_info *render,
                        int          stereo_offset_dir,
                        double       unit_factor,
                        const char * unit_text,
                        double       f_image_plane,
                        double       box_size,
                        double       FOV_x_in,
                        double       FOV_y_in,
                        double       xmin,
                        double       ymin,
                        double       pixel_size_x,
                        double       pixel_size_y,
                        double       radius_kernel_max,
                        int          nx,
                        int          ny,
                        double       expansion_factor,
                        double       focus_shift_x,
                        double       focus_shift_y,
                        double       d_near_field,
                        double       stereo_offset,
                        int          flag_comoving,
                        int          flag_force_periodic,
                        int          camera_mode,
                        int *        flag_weigh,
                        int *        flag_line_integral,
                        float **     x,
                        float **     y,
                        float **     z,
                        float **     h_smooth,
                        float **     f_stretch,
                        float **     value,
                        float **     weight,
                        char **      colour,
                        size_t **    z_index,
                        int *        i_x_min_local_return,
                        int *        i_x_max_local_return,
                        size_t *     n_particles);
void  fetch_image_array(image_info **image, int i_depth, double **values);
void  render_frame(render_info *render);

void open_movie(char *filename, int width, int height, int n_frames, int rate, movie_info *movie);
void write_image_to_movie(image_info *image, movie_info *movie);
void close_movie(movie_info *movie);
void init_image(int width, int height, const char *colourmapselect, image_info **image);
void free_image(image_info **image);
void create_colour_table(const char *colourmapselect, int *n_colours, int ***rgb);
void set_image_RGB(image_info *image, double image_min, double image_max, int flag_RGB_log);
void set_image_RGBY(image_info *image_RGBY_in,
                    image_info *image_RGB_in,
                    image_info *image_Y_in,
                    double      RGB_min,
                    double      RGB_max,
                    double      Y_min,
                    double      Y_max,
                    int         flag_RGB_log,
                    int         flag_Y_log);
void set_image_RGBY_MARKED(image_info *image_RGBY_MARKED_in,
                           image_info *image_RY_in,
                           image_info *image_GY_in,
                           image_info *image_BY_in,
                           image_info *image_Y_in,
                           double      Y_min,
                           double      Y_max,
                           int         flag_Y_log);

void write_image(image_info *image, const char *filename_dir, const char *filename_root, int mode);
void read_image(image_info *image, const char *filename_dir, const char *filename_root);

void read_gadget_binary_render(char *filename_root_in, int snapshot_number, plist_info *plist, int mode);

void set_sph_kernel(double **kernel_radius, double **kernel_table_3d, double **kernel_table_2d, double *kernel_table_2d_average, int mode);

void add_mark_argument(render_info *render, const char *species, int value, const char *type, ...);
void create_mark_argument(render_info *render, mark_arg_info **new_arg);
void free_mark_arguments(mark_arg_info **argument);
void perform_marking(render_info *render);
void pick_best_snap(double a_search, double *snap_a_list, int n_snap_a_list, int *snap_best, double *snap_diff_best);
void process_SSimPL_halos(render_info *render,
                          int          i_snap,
                          int          i_pass,
                          int          mode,
                          int          select_function(int                i_group,
                                              int                j_subgroup,
                                              int                i_subgroup,
                                              int                flag_long_ids,
                                              process_halo_info *group_i,
                                              process_halo_info *subgroup_i,
                                              void *             params),
                          void         action_function(int                i_group,
                                               int                j_subgroup,
                                               int                i_subgroup,
                                               int                flag_long_ids,
                                               process_halo_info *group_i,
                                               process_halo_info *subgroup_i,
                                               void *             params),
                          void *       params);
#ifdef __cplusplus
}
#endif

#endif
