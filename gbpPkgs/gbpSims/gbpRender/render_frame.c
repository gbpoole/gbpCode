#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

void render_frame(render_info  *render){
  size_t     i_particle;
  size_t     j_particle;
  size_t     k_particle;
  int        i_pixel;              
  size_t     i_kernel;
  size_t     n_pixels;
  size_t     n_unmasked;
  double     xmin,ymin;
  double     pixel_size;
  double     pixel_size_x;
  double     pixel_size_y;
  double     pixel_area;
  float     *weight =NULL;
  float     *value  =NULL;
  char      *colour =NULL;
  size_t    *z_index=NULL;
  int        kx_min;
  int        kx_max;
  int        ky_min;
  int        ky_max;
  int        kz_min;
  int        kz_max;
  char      *mask       =NULL;
  double    *temp_image =NULL;
  double    *RGB_image  =NULL;
  double    *Y_image    =NULL;
  double    *z_image    =NULL;
  double    *RY_image   =NULL;
  double    *GY_image   =NULL;
  double    *BY_image   =NULL;
  double     part_pos_x;
  double     part_pos_y;
  double     z_i;
  double     part_h_z;
  double     part_h_xy;
  double     pixel_pos_x;
  double     pixel_pos_y;
  double     prob;
  int        kx;
  int        ky;
  int        pos;
  size_t     n_particles;
  float      *x;
  float      *y;
  float      *z;
  float      *h_smooth;
  float      *f_stretch;
  size_t     n_x;
  size_t     n_y;
  size_t     n_z;
  double     kernel;
  int        flag_weigh;
  int        flag_line_integral;
  int        n_images;

  double     deldr2;
  double     deldr2i;
  double     radius2;
  double     radius4;
  double     radius;
  double     ikernel;
  double     d2;
  double     d;
  double     radius_kernel;
  double     radius2_norm;
  double     f_table;
  int        i_table;
  double    *kernel_radius;
  double    *kernel_table;
  double     kernel_table_avg;
  double     radius_kernel_max;
  double     radius_kernel_max2;

  double     d_o,d_x_o,d_y_o,d_z_o;

  double       x_o;
  double       y_o;
  double       z_o;
  double       x_c;
  double       y_c;
  double       z_c;
  double       FOV;
  double       FOV_x_object_plane;
  double       FOV_y_object_plane;
  double       FOV_x_image_plane;
  double       FOV_y_image_plane;
  double       box_size;
  int          nx;
  int          ny;
  int          v_mode;
  int          w_mode;
  char        *parameter;
  int          flag_comoving;
  int          flag_fade;
  double       alpha_fade;
  int          flag_force_periodic;
  int          flag_add_absorption;
  double       expansion_factor;
  double       focus_shift_x;
  double       focus_shift_y;
  ADaPS       *transfer;
  double       h_Hubble;
  double       f_image_plane;
  double       d_image_plane;
  double       d_near_field;
  double       d_taper_field;
  double       stereo_offset;
  int          i_x,i_y;
  int         *mask_buffer;

  int          i_image;
  int          camera_mode;

  double       f_absorption;
  f_absorption=render->f_absorption;
  if(f_absorption<0.)
     f_absorption=0.;

  x_o          =render->camera->perspective->p_o[0];
  y_o          =render->camera->perspective->p_o[1];
  z_o          =render->camera->perspective->p_o[2];
  x_c          =render->camera->perspective->p_c[0];
  y_c          =render->camera->perspective->p_c[1];
  z_c          =render->camera->perspective->p_c[2];
  d_o          =sqrt(pow(render->camera->perspective->p_o[0]-render->camera->perspective->p_c[0],2.)+
                     pow(render->camera->perspective->p_o[1]-render->camera->perspective->p_c[1],2.)+
                     pow(render->camera->perspective->p_o[2]-render->camera->perspective->p_c[2],2.));
  FOV          =render->camera->perspective->FOV;
  focus_shift_x=render->camera->perspective->focus_shift_x;
  focus_shift_y=render->camera->perspective->focus_shift_y;
  h_Hubble     =render->h_Hubble;

  double unit_factor;
  char   unit_text[32];
  int    kernel_flag;
  int    flag_velocity_space=render->camera->flag_velocity_space;
  if(flag_velocity_space){
     SID_log("Rendering in velocity-space.",SID_LOG_COMMENT);
     kernel_flag=SPH_KERNEL_GAUSSIAN;
     unit_factor=1e3;
     sprintf(unit_text,"km/s");
  }
  else{
     SID_log("Rendering in real-space.",SID_LOG_COMMENT);
     kernel_flag=SPH_KERNEL_GADGET;
     unit_factor=M_PER_MPC/h_Hubble;
     sprintf(unit_text,"Mpc/h");
  }

  // Put all the render distances into the Gadget units of the
  //    relevant space (r or v-space)
  x_o          *=unit_factor;
  y_o          *=unit_factor;
  z_o          *=unit_factor;
  x_c          *=unit_factor;
  y_c          *=unit_factor;
  z_c          *=unit_factor;
  d_o          *=unit_factor;
  FOV          *=unit_factor;
  focus_shift_x*=unit_factor;
  focus_shift_y*=unit_factor;

  nx                 =render->camera->width;
  ny                 =render->camera->height;
  v_mode             =render->v_mode;
  w_mode             =render->w_mode;
  camera_mode        =render->camera->camera_mode;
  flag_comoving      =render->flag_comoving;
  flag_fade          =render->flag_fade;
  alpha_fade         =render->alpha_fade;
  flag_force_periodic=render->flag_force_periodic;
  flag_add_absorption=render->flag_add_absorption;
  expansion_factor   =render->camera->perspective->time;
  f_image_plane      =render->camera->f_image_plane;
  d_near_field       =render->camera->f_near_field*d_o;
  d_taper_field      =render->camera->f_taper_field*d_o;
  d_image_plane      =d_o*f_image_plane;
  box_size           =((double *)ADaPS_fetch(render->plist_list[0]->data,"box_size"))[0];

  // Sanity check the taper distances
  double taper_width=d_taper_field-d_near_field;
  if(taper_width<0.)
     SID_trap_error("The near-field distance (%le [%s]) must be less than the taper distance (%le [%s]) if non-zero.",ERROR_LOGIC,
                    d_near_field*unit_factor, unit_text,
                    d_taper_field*unit_factor,unit_text);

  // Set FOV
  if(d_near_field>0.)
     SID_log("Near  field  = %le [%s]",SID_LOG_COMMENT,d_near_field /unit_factor,unit_text);
  if(d_taper_field>0.)
     SID_log("Taper field  = %le [%s]",SID_LOG_COMMENT,d_taper_field/unit_factor,unit_text);
  if(d_near_field>0. || d_taper_field>0.)
     SID_log("Image plane  = %le [%s]",SID_LOG_COMMENT,d_image_plane/unit_factor,unit_text);
  SID_log("f_absorption = %le",SID_LOG_COMMENT,f_absorption);
  if(nx>=ny){
    FOV_y_object_plane=FOV;
    FOV_x_object_plane=FOV_y_object_plane*(double)nx/(double)ny;    
  }
  else{
    FOV_x_object_plane=FOV;
    FOV_y_object_plane=FOV_x_object_plane*(double)nx/(double)ny;        
  }
  FOV_x_image_plane=FOV_x_object_plane*f_image_plane;
  FOV_y_image_plane=FOV_y_object_plane*f_image_plane;

  // Loop over the left/right stereo pair (if necessary)
  if(check_mode_for_flag(camera_mode,CAMERA_STEREO))
    i_image =0;
  else
    i_image =1;
  for(;i_image<2;i_image++){

    switch(i_image){
      // Left image
      case 0:
        fetch_image_array(render->camera->image_RGB_left, &RGB_image);
        fetch_image_array(render->camera->image_RGBY_left,&temp_image); // use as workspace
        fetch_image_array(render->camera->image_Y_left,   &Y_image);
        fetch_image_array(render->camera->image_Z_left,   &z_image);
        fetch_image_array(render->camera->image_RY_left,  &RY_image);
        fetch_image_array(render->camera->image_GY_left,  &GY_image);
        fetch_image_array(render->camera->image_BY_left,  &BY_image);
        stereo_offset=-d_image_plane/render->camera->stereo_ratio;
        break;
      case 1:
        // Right image
        if(check_mode_for_flag(camera_mode,CAMERA_STEREO)){
           fetch_image_array(render->camera->image_RGB_right, &RGB_image);
           fetch_image_array(render->camera->image_RGBY_right,&temp_image); // use as workspace
           fetch_image_array(render->camera->image_Y_right,   &Y_image);
           fetch_image_array(render->camera->image_Z_right,   &z_image);
           fetch_image_array(render->camera->image_RY_right,  &RY_image);
           fetch_image_array(render->camera->image_GY_right,  &GY_image);
           fetch_image_array(render->camera->image_BY_right,  &BY_image);
           stereo_offset=d_image_plane/render->camera->stereo_ratio;
        }
        // Mono image (stereo turned off)
        else{
           fetch_image_array(render->camera->image_RGB, &RGB_image);
           fetch_image_array(render->camera->image_RGBY,&temp_image); // use as workspace
           fetch_image_array(render->camera->image_Y,   &Y_image);
           fetch_image_array(render->camera->image_Z,   &z_image);
           fetch_image_array(render->camera->image_RY,  &RY_image);
           fetch_image_array(render->camera->image_GY,  &GY_image);
           fetch_image_array(render->camera->image_BY,  &BY_image);
           stereo_offset=0;           
        }
        break;
    }

    SID_log("Projecting to a %dx%d pixel array...",SID_LOG_OPEN|SID_LOG_TIMER,nx,ny);

    // Set physical image-plane domain
    xmin  = -FOV_x_image_plane/2.; // Things will be centred on (x_o,y_o,z_o) later
    ymin  = -FOV_y_image_plane/2.; // Things will be centred on (x_o,y_o,z_o) later

    xmin-=stereo_offset;

    // Compute image scales
    n_pixels    =nx*ny;
    pixel_size_x=FOV_x_image_plane/(double)nx;
    pixel_size_y=FOV_y_image_plane/(double)ny;
    pixel_size  =0.5*(pixel_size_x+pixel_size_y);
    pixel_area  =pixel_size_x*pixel_size_y;
    if(fabs((pixel_size_x-pixel_size_y)/pixel_size_x)>1e-4)
      SID_log_warning("pixels are not square by %7.3f%%",0,fabs((pixel_size_x-pixel_size_y)/pixel_size_x)*1e2);

    // Generate the smoothing kernal
    set_sph_kernel(&(render->kernel_radius),
                   &(render->kernel_table_3d),
                   &(render->kernel_table),
                   &(render->kernel_table_avg),
                   kernel_flag|SPH_KERNEL_2D);
    kernel_radius     =render->kernel_radius;
    kernel_table      =render->kernel_table;
    kernel_table_avg  =render->kernel_table_avg;
    radius_kernel_max =kernel_radius[N_KERNEL_TABLE];
    radius_kernel_max2=radius_kernel_max*radius_kernel_max;

    double x_o_in=x_o;
    double y_o_in=y_o;
    double z_o_in=z_o;
    double x_c_in=x_c;
    double y_c_in=y_c;
    double z_c_in=z_c;
    double FOV_x =FOV_x_object_plane;
    double FOV_y =FOV_y_object_plane;
    double x_o_out;
    double y_o_out;
    double z_o_out;
    double x_c_out;
    double y_c_out;
    double z_c_out;
    double x_hat;
    double y_hat;
    double z_hat;
    double theta;
    double theta_roll;
    double d_o;
    compute_perspective_transformation(x_o_in,
                                       y_o_in,
                                       z_o_in,
                                       x_c_in,
                                       y_c_in,
                                       z_c_in,
                                       unit_factor,
                                       unit_text,
                                       f_image_plane,
                                       stereo_offset,
                                       &FOV_x,
                                       &FOV_y,
                                       &d_o, // Includes f_image_plane correction
                                       &x_o_out,
                                       &y_o_out,
                                       &z_o_out,
                                       &x_c_out,
                                       &y_c_out,
                                       &z_c_out,
                                       &x_hat,
                                       &y_hat,
                                       &z_hat,
                                       &theta,
                                       &theta_roll);

    // Initialize make_map
    int i_x_min_local;
    int i_x_max_local;
    if(flag_add_absorption)
       init_make_map_abs(render,
                         x_o,y_o,z_o,
                         x_c,y_c,z_c,
                         unit_factor,unit_text,
                         f_image_plane,
                         box_size,FOV_x_object_plane,FOV_y_object_plane,
                         xmin,ymin,
                         pixel_size_x,pixel_size_y,
                         radius_kernel_max,
                         nx,ny,
                         expansion_factor,
                         focus_shift_x,
                         focus_shift_y,
                         d_near_field,
                         stereo_offset,
                         flag_comoving,
                         flag_force_periodic,
                         camera_mode,
                         &flag_weigh,
                         &flag_line_integral,
                         &x,&y,&z,
                         &h_smooth,
                         &f_stretch,
                         &value,
                         &weight,
                         &colour,
                         &z_index,
                         &i_x_min_local,
                         &i_x_max_local,
                         &n_particles);
    else
       init_make_map_noabs(render,
                           x_o,y_o,z_o,
                           x_c,y_c,z_c,
                           unit_factor,unit_text,
                           f_image_plane,
                           box_size,FOV_x_object_plane,FOV_y_object_plane,
                           xmin,ymin,
                           pixel_size_x,pixel_size_y,
                           radius_kernel_max,
                           nx,ny,
                           expansion_factor,
                           focus_shift_x,
                           focus_shift_y,
                           d_near_field,
                           stereo_offset,
                           flag_comoving,
                           flag_force_periodic,
                           camera_mode,
                           &flag_weigh,
                           &flag_line_integral,
                           &x,&y,&z,
                           &h_smooth,
                           &f_stretch,
                           &value,
                           &weight,
                           &colour,
                           &z_index,
                           &i_x_min_local,
                           &i_x_max_local,
                           &n_particles);

    // Initialize image arrays
    mask=(char *)SID_malloc(sizeof(char)*n_pixels);
    for(i_pixel=0;i_pixel<n_pixels;i_pixel++){
      if(RGB_image!=NULL)
         RGB_image[i_pixel]=0.;
      if(temp_image!=NULL)
         temp_image[i_pixel]=0.;
      if(Y_image!=NULL)
         Y_image[i_pixel]=0.;
      if(z_image!=NULL)
        z_image[i_pixel]=0.;
      if(RY_image!=NULL)
        RY_image[i_pixel]=0.;
      if(GY_image!=NULL)
        GY_image[i_pixel]=0.;
      if(BY_image!=NULL)
        BY_image[i_pixel]=0.;
      mask[i_pixel]=FALSE;
    }

    // Perform projection
    size_t        n_particles_used_local=0;
    size_t        n_particles_used      =0;
    pcounter_info pcounter;
    SID_init_pcounter(&pcounter,n_particles,10);
    SID_log("Performing projection...",SID_LOG_OPEN|SID_LOG_TIMER);
    n_particles_used_local=0;
    for(size_t ii_particle=0;ii_particle<n_particles;ii_particle++){
      i_particle=z_index[n_particles-1-ii_particle];
      z_i       =(double)z[i_particle];

      if(z_i>d_near_field){
        n_particles_used_local++;

        // Set pixel space ranges and positions
        part_h_z     =(double)h_smooth[i_particle];
        part_h_xy    =part_h_z*f_stretch[i_particle];
        radius_kernel=part_h_xy;
        if(fpclassify(radius_kernel)==FP_NORMAL){
           radius2_norm =1./(part_h_xy*part_h_xy);
           part_pos_x   =(double)(x[i_particle]*f_stretch[i_particle]);
           part_pos_y   =(double)(y[i_particle]*f_stretch[i_particle]);
           kx_min=(int)((part_pos_x-radius_kernel-xmin)/pixel_size_x);
           kx_max=(int)((part_pos_x+radius_kernel-xmin)/pixel_size_x+ONE_HALF);
           ky_min=(int)((part_pos_y-radius_kernel-ymin)/pixel_size_y);
           ky_max=(int)((part_pos_y+radius_kernel-ymin)/pixel_size_y+ONE_HALF);

           // Compute any potential fading
           // alpha_fade=2 for normal inverse-square fading past the image plane
           double f_fade;
           if(flag_fade && z_i>d_o)
              f_fade=pow(d_image_plane/z_i,-alpha_fade);
           else
              f_fade=1;

           // Compute any potential tapering
           double f_taper;
           if(taper_width>0. && z_i<d_taper_field)
              f_taper=(z_i-d_near_field)/taper_width;
           else
              f_taper=1;

           // Combine dimming factors into one
           double f_dim;
           f_dim=f_taper*f_fade;

           // Set the particle values and weights
           double v_i =(double)value[i_particle];
           double w_i =(double)weight[i_particle];
           double R_i =1.;
           double G_i =1.;
           double B_i =1.;
           if(colour!=NULL){
              R_i =RGB_lookup(render,colour[i_particle],0);
              G_i =RGB_lookup(render,colour[i_particle],1);
              B_i =RGB_lookup(render,colour[i_particle],2);
           }
           double vw_i=v_i*w_i;
           double zw_i=z_i*w_i;

           // Loop over the kernal
           for(kx=kx_min,pixel_pos_x=xmin+(kx_min+0.5)*pixel_size_x;kx<=kx_max;kx++,pixel_pos_x+=pixel_size_x){
             if(kx>=0 && kx<nx){
               for(ky=ky_min,pixel_pos_y=ymin+(ky_min+0.5)*pixel_size_y;ky<=ky_max;ky++,pixel_pos_y+=pixel_size_y){
                 if(ky>=0 && ky<ny){
                   radius2=
                     (pixel_pos_x-part_pos_x)*(pixel_pos_x-part_pos_x)+
                     (pixel_pos_y-part_pos_y)*(pixel_pos_y-part_pos_y);
                   radius2*=radius2_norm;
                   // Construct images here
                   if(radius2<radius_kernel_max2){
                     pos    =ky+kx*ny;
                     f_table=sqrt(radius2)/radius_kernel_max;
                     i_table=(int)(f_table*(double)N_KERNEL_TABLE);
                     kernel =kernel_table[i_table]+
                       (kernel_table[i_table+1]-kernel_table[i_table])*
                       (f_table-kernel_radius[i_table])*(double)N_KERNEL_TABLE;
                     double w_k=w_i*kernel;
                     // Perform addition
                     if(Y_image!=NULL)    Y_image[pos]   +=(f_dim    -f_absorption*   Y_image[pos])*w_k;
                     if(temp_image!=NULL) temp_image[pos]+=(f_dim*v_i-f_absorption*temp_image[pos])*w_k;
                     if(z_image!=NULL)    z_image[pos]   +=(f_dim*z_i-f_absorption*   z_image[pos])*w_k;
                     if(RY_image!=NULL)   RY_image[pos]  +=(f_dim*R_i-f_absorption*  RY_image[pos])*w_k;
                     if(GY_image!=NULL)   GY_image[pos]  +=(f_dim*G_i-f_absorption*  GY_image[pos])*w_k;
                     if(BY_image!=NULL)   BY_image[pos]  +=(f_dim*B_i-f_absorption*  BY_image[pos])*w_k;
                     mask[pos]=TRUE;
                   }
                 }
               } // loop over kernel
             } 
           } // loop over kernel
         } // check on fp_classify
      } // near-field selection
      SID_check_pcounter(&pcounter,ii_particle);
    } // loop over particles
    SID_Barrier(SID.COMM_WORLD);
    SID_Allreduce(&n_particles_used_local,&n_particles_used,1,SID_SIZE_T,SID_SUM,SID.COMM_WORLD);
    SID_log("n_particles_used=%zd",SID_LOG_COMMENT,n_particles_used);
    SID_log("Done.",SID_LOG_CLOSE);

    SID_log("Image normalization, etc...",SID_LOG_OPEN|SID_LOG_TIMER);

    // Add results from all ranks if this is being run in parallel
    //   First, clear the parts of the image not in this rank's domain ...
    for(kx=0;kx<nx;kx++){
       if(!(kx>=i_x_min_local && kx<=i_x_max_local)){
          for(ky=0;ky<ny;ky++){
             pos=ky+kx*ny;
             if(temp_image!=NULL)
                temp_image[pos]=0.;
             if(Y_image!=NULL)
                Y_image[pos]=0.;
             if(z_image!=NULL)
                z_image[pos]=0.;
             if(RY_image!=NULL){
                RY_image[pos]=0.;
                GY_image[pos]=0.;
                BY_image[pos]=0.;
             }
          }
       }
    }

    // Join mask results.  The MPI 1.1 standard does not specify MPI_SUM 
    //   on MPI_CHAR types so we need to do this awkward thing for the mask array ...
#if USE_MPI
    mask_buffer=(int *)SID_malloc(sizeof(int)*nx);
    for(i_y=0;i_y<ny;i_y++){
      for(i_x=0,i_pixel=i_y*nx;i_x<nx;i_x++,i_pixel++)
        mask_buffer[i_x]=(int)mask[i_pixel];
      SID_Allreduce(SID_IN_PLACE,mask_buffer,nx,SID_INT,SID_MAX,SID.COMM_WORLD);
      for(i_x=0,i_pixel=i_y*nx;i_x<nx;i_x++,i_pixel++){
         if(mask_buffer[i_x])
           mask[i_pixel]=TRUE;
      }
    }
    SID_free(SID_FARG mask_buffer);
#endif
    SID_Allreduce(SID_IN_PLACE,temp_image,n_pixels,SID_DOUBLE,SID_SUM,SID.COMM_WORLD);
    SID_Allreduce(SID_IN_PLACE,Y_image,   n_pixels,SID_DOUBLE,SID_SUM,SID.COMM_WORLD);
    if(z_image!=NULL)
      SID_Allreduce(SID_IN_PLACE,z_image,n_pixels,SID_DOUBLE,SID_SUM,SID.COMM_WORLD);
    if(RY_image!=NULL){
      SID_Allreduce(SID_IN_PLACE,RY_image,n_pixels,SID_DOUBLE,SID_SUM,SID.COMM_WORLD);
      SID_Allreduce(SID_IN_PLACE,GY_image,n_pixels,SID_DOUBLE,SID_SUM,SID.COMM_WORLD);
      SID_Allreduce(SID_IN_PLACE,BY_image,n_pixels,SID_DOUBLE,SID_SUM,SID.COMM_WORLD);
    }

    // Create final normalized images and clear the temp_image which has been used as a buffer
    if(RGB_image!=NULL){
       for(i_pixel=0;i_pixel<n_pixels;i_pixel++){
          if(mask[i_pixel])
             RGB_image[i_pixel]=temp_image[i_pixel]/Y_image[i_pixel];
          temp_image[i_pixel]=0.;
       }
    }
    if(RY_image!=NULL){
       for(i_pixel=0;i_pixel<n_pixels;i_pixel++){
          if(mask[i_pixel])
             RY_image[i_pixel]=RY_image[i_pixel]/Y_image[i_pixel];
       }
    }
    if(GY_image!=NULL){
       for(i_pixel=0;i_pixel<n_pixels;i_pixel++){
          if(mask[i_pixel])
             GY_image[i_pixel]=GY_image[i_pixel]/Y_image[i_pixel];
       }
    }
    if(BY_image!=NULL){
       for(i_pixel=0;i_pixel<n_pixels;i_pixel++){
          if(mask[i_pixel])
             BY_image[i_pixel]=BY_image[i_pixel]/Y_image[i_pixel];
       }
    }

    // Normalize z-frame (if needed)
    if(z_image!=NULL){
       for(i_pixel=0;i_pixel<n_pixels;i_pixel++){
         if(mask[i_pixel])
           z_image[i_pixel]=z_image[i_pixel]/Y_image[i_pixel];
         else
           z_image[i_pixel]=0.;
       }
    }

    // log functionality has been passed to to output phase
    //   to preserve precision and to make post-rendering changes easier
    // Take log_10 (if needed)
    //if(check_mode_for_flag(v_mode,MAKE_MAP_LOG)){
    //  SID_log("Taking log of RGB image...",SID_LOG_OPEN);
    //  for(i_pixel=0;i_pixel<n_pixels;i_pixel++){
    //    if(mask[i_pixel])
    //      RGB_image[i_pixel]=take_log10(RGB_image[i_pixel]);
    //    else
    //      RGB_image[i_pixel]=LOG_ZERO;
    //  }
    //  SID_log("Done.",SID_LOG_CLOSE);
    //}
    //if(check_mode_for_flag(w_mode,MAKE_MAP_LOG)){
    //  SID_log("Taking log of Y image...",SID_LOG_OPEN);
    //  for(i_pixel=0;i_pixel<n_pixels;i_pixel++){
    //    if(mask[i_pixel])
    //      Y_image[i_pixel]=take_log10(Y_image[i_pixel]);
    //    else
    //      Y_image[i_pixel]=LOG_ZERO;
    //  }
    //  SID_log("Done.",SID_LOG_CLOSE);
    //}

    // Compute some image statistics
    double min_RGB_image;
    double max_RGB_image;
    double min_Y_image;
    double max_Y_image;
    double min_z_image;
    double max_z_image;
    double min_RY_image;
    double max_RY_image;
    double min_GY_image;
    double max_GY_image;
    double min_BY_image;
    double max_BY_image;
    for(i_pixel=0,n_unmasked=0;i_pixel<n_pixels;i_pixel++) 
      if(mask[i_pixel]) 
        n_unmasked++;
    if(RGB_image!=NULL){
      calc_min(RGB_image,&min_RGB_image,n_pixels,SID_DOUBLE,CALC_MODE_DEFAULT);
      calc_max(RGB_image,&max_RGB_image,n_pixels,SID_DOUBLE,CALC_MODE_DEFAULT);
    }
    if(Y_image!=NULL){
      calc_min(Y_image,&min_Y_image,n_pixels,SID_DOUBLE,CALC_MODE_DEFAULT);
      calc_max(Y_image,&max_Y_image,n_pixels,SID_DOUBLE,CALC_MODE_DEFAULT);
    }
    if(z_image!=NULL){
      calc_min(z_image,&min_z_image,n_pixels,SID_DOUBLE,CALC_MODE_DEFAULT);
      calc_max(z_image,&max_z_image,n_pixels,SID_DOUBLE,CALC_MODE_DEFAULT);
    }
    if(RY_image!=NULL){
      calc_min(RY_image,&min_RY_image,n_pixels,SID_DOUBLE,CALC_MODE_DEFAULT);
      calc_max(RY_image,&max_RY_image,n_pixels,SID_DOUBLE,CALC_MODE_DEFAULT);
    }
    if(GY_image!=NULL){
      calc_min(GY_image,&min_GY_image,n_pixels,SID_DOUBLE,CALC_MODE_DEFAULT);
      calc_max(GY_image,&max_GY_image,n_pixels,SID_DOUBLE,CALC_MODE_DEFAULT);
    }
    if(BY_image!=NULL){
      calc_min(BY_image,&min_BY_image,n_pixels,SID_DOUBLE,CALC_MODE_DEFAULT);
      calc_max(BY_image,&max_BY_image,n_pixels,SID_DOUBLE,CALC_MODE_DEFAULT);
    }
    SID_log("Done.",SID_LOG_CLOSE);
  
    // Report image statistics
    if(n_unmasked>0){
       SID_log("Image statistics:",SID_LOG_OPEN);
       SID_log("coverage=%3d%%",SID_LOG_COMMENT,(int)(100.*(double)n_unmasked/(double)n_pixels));
       if(RGB_image!=NULL){
          SID_log("RGB min =%le",SID_LOG_COMMENT,min_RGB_image);
          SID_log("RGB max =%le",SID_LOG_COMMENT,max_RGB_image);
       }
       if(Y_image!=NULL){
          SID_log("Y   min =%le",SID_LOG_COMMENT,min_Y_image);
          SID_log("Y   max =%le",SID_LOG_COMMENT,max_Y_image);
       }
       if(RY_image!=NULL){
          SID_log("RY  min =%le",SID_LOG_COMMENT,min_RY_image);
          SID_log("RY  max =%le",SID_LOG_COMMENT,max_RY_image);
       }
       if(GY_image!=NULL){
          SID_log("GY  min =%le",SID_LOG_COMMENT,min_GY_image);
          SID_log("GY  max =%le",SID_LOG_COMMENT,max_GY_image);
       }
       if(BY_image!=NULL){
          SID_log("BY  min =%le",SID_LOG_COMMENT,min_BY_image);
          SID_log("BY  max =%le",SID_LOG_COMMENT,max_BY_image);
       }
       if(z_image!=NULL){
          SID_log("Z   min =%le [%s]",SID_LOG_COMMENT,min_z_image/unit_factor,unit_text);
          SID_log("Z   max =%le [%s]",SID_LOG_COMMENT,max_z_image/unit_factor,unit_text);
       }
       SID_log("",SID_LOG_SILENT_CLOSE);
    }
    else
      SID_log("IMAGE IS EMPTY.",SID_LOG_COMMENT);

    // Clean-up
    SID_free(SID_FARG x);
    SID_free(SID_FARG y);
    SID_free(SID_FARG z);
    SID_free(SID_FARG h_smooth);
    SID_free(SID_FARG f_stretch);
    SID_free(SID_FARG value);
    SID_free(SID_FARG weight);
    SID_free(SID_FARG colour);
    SID_free(SID_FARG z_index);
    SID_free(SID_FARG mask);
  
    SID_log("Done.",SID_LOG_CLOSE);
  }
}

