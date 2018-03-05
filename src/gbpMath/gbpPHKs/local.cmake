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
list(APPEND INCFILES "hilbert_Moore.h" )
list(APPEND INCFILES "SK_hilbert_util.h" )
list(APPEND INCFILES "SK_sfc_boundary.h" )
list(APPEND INCFILES "SK_sfc_curve.h" )
list(APPEND INCFILES "gbpPHKs.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "hilbert_Moore.c" )
list(APPEND SRCFILES "SK_hilbert_util.c" )
list(APPEND SRCFILES "SK_sfc_boundary.c" )
list(APPEND SRCFILES "SK_sfc_curve.c" )
list(APPEND SRCFILES "compute_PHK_from_Cartesian.c" )
list(APPEND SRCFILES "compute_PHK_to_Cartesian.c" )
list(APPEND SRCFILES "compute_PHK_volume_keys.c" )
list(APPEND SRCFILES "compute_PHK_boundary_keys.c" )

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

