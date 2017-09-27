# Set empty defaults
set(LIBDIRS    "" )
set(SRCDIRS    "" )
set(INCFILES   "" )
set(SRCFILES   "" )
set(EXEFILES   "" )
set(DATAFILES  "" )
set(PASSDIRS   "" )

# Add subdirectories that are roots to libraries
# eg. list(APPEND LIBDIRS "dir" )
# -- NONE FOR THIS DIR --

# Add directories that contribute source files 
# eg. list(APPEND SRCDIRS "dir" )
# -- NONE FOR THIS DIR --

# Add header files
# eg. list(APPEND INCFILES "file" )
list(APPEND INCFILES "gbpMisc.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "init_array_linear.c" )
list(APPEND SRCFILES "init_array_log.c" )
list(APPEND SRCFILES "apply_rotation.c" )
list(APPEND SRCFILES "compute_centroid3D.c" )
list(APPEND SRCFILES "take_log10.c" )
list(APPEND SRCFILES "take_alog10.c" )
list(APPEND SRCFILES "take_ln.c" )
list(APPEND SRCFILES "take_aln.c" )
list(APPEND SRCFILES "invert_matrix.c" )
list(APPEND SRCFILES "add_quad.c" )
list(APPEND SRCFILES "force_periodic.c" )
list(APPEND SRCFILES "force_periodic_double.c" )
list(APPEND SRCFILES "is_a_member.c" )
list(APPEND SRCFILES "find_index.c" )
list(APPEND SRCFILES "find_index_int.c" )
list(APPEND SRCFILES "d_periodic.c" )
list(APPEND SRCFILES "compute_triaxiality.c" )
list(APPEND SRCFILES "bisect_array.c" )
list(APPEND SRCFILES "compute_Daubechies_scaling_fctns.c" )

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "file" )
# -- NONE FOR THIS DIR --

# Add data files
# eg. list(APPEND DATAFILES "file" )
# -- NONE FOR THIS DIR --

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# eg. list(APPEND PASSDIRS "dir" )
# -- NONE FOR THIS DIR --

