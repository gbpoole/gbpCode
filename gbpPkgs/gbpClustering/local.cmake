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
list(APPEND INCFILES "gbpClustering.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "read_groupings.c" )
list(APPEND SRCFILES "read_atable.c" )
list(APPEND SRCFILES "generate_randoms.c" )
list(APPEND SRCFILES "init_cfunc.c" )
list(APPEND SRCFILES "free_cfunc.c" )
list(APPEND SRCFILES "write_cfunc.c" )
list(APPEND SRCFILES "read_cfunc.c" )
list(APPEND SRCFILES "compute_cfunc.c" )
list(APPEND SRCFILES "init_pspec.c" )
list(APPEND SRCFILES "free_pspec.c" )
list(APPEND SRCFILES "write_pspec.c" )
list(APPEND SRCFILES "write_grid.c" )
list(APPEND SRCFILES "compute_pspec.c" )
list(APPEND SRCFILES "map_to_grid.c" )

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "file" )
# -- NONE FOR THIS DIR --
list(APPEND EXEFILES "make_atable_mass_function.c" )
list(APPEND EXEFILES "make_gadget_grid.c" )
list(APPEND EXEFILES "make_diff_grid.c" )
list(APPEND EXEFILES "make_gadget_pspec.c" )
list(APPEND EXEFILES "make_groupings.c" )
list(APPEND EXEFILES "make_groupings_pspec.c" )
list(APPEND EXEFILES "make_groupings_cfunc.c" )
list(APPEND EXEFILES "make_atable_pspec.c" )
list(APPEND EXEFILES "make_atable_cfunc.c" )

# Add data files
# eg. list(APPEND DATAFILES "file" )
# -- NONE FOR THIS DIR --

# Set data subdirectory
# eg. set(DATASUBDIR "dir" )

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# eg. list(APPEND PASSDIRS "dir" )
# -- NONE FOR THIS DIR --

