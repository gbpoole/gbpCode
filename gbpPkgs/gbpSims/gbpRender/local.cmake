# Set empty defaults
set(LIBDIRS    "" )
set(SRCDIRS    "" )
set(INCFILES   "" )
set(SRCFILES   "" )
set(EXEFILES   "" )
set(DATAFILES  "" )
set(PASSDIRS   "" )
set(DATASUBDIR "" )

# Add subdirectories that are roots to libraries
# eg. list(APPEND LIBDIRS "dir" )
# -- NONE FOR THIS DIR --

# Add directories that contribute source files 
# eg. list(APPEND SRCDIRS "dir" )
# -- NONE FOR THIS DIR --

# Add header files
# eg. list(APPEND INCFILES "file" )
list(APPEND INCFILES "gbpRender.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "RGB_lookup.c" )
list(APPEND SRCFILES "fetch_render_colour_index.c" )
list(APPEND SRCFILES "close_movie.c" )
list(APPEND SRCFILES "add_mark_argument.c" )
list(APPEND SRCFILES "create_mark_argument.c" )
list(APPEND SRCFILES "free_mark_arguments.c" )
list(APPEND SRCFILES "pick_best_snapshot.c" )
list(APPEND SRCFILES "process_SSimPL_halos.c" )
list(APPEND SRCFILES "perform_marking.c" )
list(APPEND SRCFILES "init_perspective.c" )
list(APPEND SRCFILES "free_perspective.c" )
list(APPEND SRCFILES "copy_perspective.c" )
list(APPEND SRCFILES "init_scene.c" )
list(APPEND SRCFILES "free_scenes.c" )
list(APPEND SRCFILES "init_camera.c" )
list(APPEND SRCFILES "seal_scenes.c" )
list(APPEND SRCFILES "seal_camera.c" )
list(APPEND SRCFILES "seal_render.c" )
list(APPEND SRCFILES "free_camera.c" )
list(APPEND SRCFILES "init_render.c" )
list(APPEND SRCFILES "free_render.c" )
list(APPEND SRCFILES "add_render_scene.c" )
list(APPEND SRCFILES "check_camera_sealed.c" )
list(APPEND SRCFILES "parse_render_file.c" )
list(APPEND SRCFILES "parse_set_scene_quantity.c" )
list(APPEND SRCFILES "read_frame.c" )
list(APPEND SRCFILES "write_frame_metadata.c" )
list(APPEND SRCFILES "set_frame.c" )
list(APPEND SRCFILES "set_render_scale.c" )
list(APPEND SRCFILES "set_camera_depths.c" )
list(APPEND SRCFILES "free_camera_depths.c" )
list(APPEND SRCFILES "set_sph_kernel.c" )
list(APPEND SRCFILES "set_transfer_function.c" )
list(APPEND SRCFILES "read_gadget_binary_render.c" )
list(APPEND SRCFILES "create_colour_table.c" )
list(APPEND SRCFILES "free_image.c" )
list(APPEND SRCFILES "init_image.c" )
list(APPEND SRCFILES "open_movie.c" )
list(APPEND SRCFILES "read_image.c" )
list(APPEND SRCFILES "fetch_image_array.c" )
list(APPEND SRCFILES "rotate_particle.c" )
list(APPEND SRCFILES "set_pixel_space.c" )
list(APPEND SRCFILES "transform_particle.c" )
list(APPEND SRCFILES "free_particle_map_quantities.c" )
list(APPEND SRCFILES "init_particle_map_quantities.c" )
list(APPEND SRCFILES "set_particle_map_quantities.c" )
list(APPEND SRCFILES "compute_f_stretch.c" )
list(APPEND SRCFILES "compute_perspective_transformation.c" )
list(APPEND SRCFILES "check_if_particle_marked.c" )
list(APPEND SRCFILES "init_make_map_noabs.c" )
list(APPEND SRCFILES "init_make_map_abs.c" )
list(APPEND SRCFILES "set_image_RGB.c" )
list(APPEND SRCFILES "set_image_RGBY.c" )
list(APPEND SRCFILES "set_image_RGBY_MARKED.c" )
list(APPEND SRCFILES "write_frame.c" )
list(APPEND SRCFILES "write_image.c" )
list(APPEND SRCFILES "write_image_to_movie.c" )
list(APPEND SRCFILES "render_frame.c" )
list(APPEND SRCFILES "set_render_state.c" )

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "file" )
list(APPEND EXEFILES "make_colour_bar" )
list(APPEND EXEFILES "render" )

# Add data files
# eg. list(APPEND DATAFILES "file" )
list(APPEND DATAFILES "rgb.txt" )
list(APPEND DATAFILES "gbpCode_cmap_red.txt" )
list(APPEND DATAFILES "gbpCode_cmap_green.txt" )
list(APPEND DATAFILES "gbpCode_cmap_blue.txt" )
list(APPEND DATAFILES "gbpCode_cmap_greyscale.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_found.txt" )
list(APPEND DATAFILES "gbpCode_cmap_GiggleZ.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H000_R090_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H000_R180_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H000_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H030_R090_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H030_R180_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H030_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H060_R090_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H060_R180_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H060_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H090_R090_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H090_R180_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H090_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H120_R090_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H120_R180_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H120_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H150_R090_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H150_R180_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H150_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H180_R090_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H180_R180_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H180_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H210_R090_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H210_R180_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H210_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H240_R090_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H240_R180_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H240_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H270_R090_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H270_R180_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H270_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H300_R090_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H300_R180_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H300_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H330_R090_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H330_R180_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_iso_H330_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H000_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H000_R360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H000_R540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H000_Rn270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H000_Rn360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H000_Rn540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H030_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H030_R360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H030_R540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H030_Rn270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H030_Rn360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H030_Rn540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H060_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H060_R360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H060_R540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H060_Rn270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H060_Rn360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H060_Rn540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H090_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H090_R360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H090_R540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H090_Rn270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H090_Rn360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H090_Rn540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H120_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H120_R360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H120_R540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H120_Rn270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H120_Rn360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H120_Rn540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H150_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H150_R360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H150_R540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H150_Rn270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H150_Rn360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H150_Rn540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H180_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H180_R360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H180_R540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H180_Rn270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H180_Rn360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H180_Rn540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H210_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H210_R360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H210_R540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H210_Rn270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H210_Rn360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H210_Rn540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H240_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H240_R360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H240_R540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H240_Rn270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H240_Rn360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H240_Rn540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H270_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H270_R360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H270_R540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H270_Rn270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H270_Rn360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H270_Rn540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H300_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H300_R360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H300_R540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H300_Rn270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H300_Rn360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H300_Rn540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H330_R270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H330_R360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H330_R540_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H330_Rn270_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H330_Rn360_C050.txt" )
list(APPEND DATAFILES "gbpCode_cmap_linear_H330_Rn540_C050.txt" )

# Set data subdirectory
# eg. set(DATASUBDIR "dir" )
set(DATASUBDIR "gbpRender" )

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# eg. list(APPEND PASSDIRS "dir" )
# -- NONE FOR THIS DIR --

