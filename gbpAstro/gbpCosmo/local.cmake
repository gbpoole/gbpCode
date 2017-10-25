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
list(APPEND SRCDIRS "core" )
list(APPEND SRCDIRS "linear_theory" )
list(APPEND SRCDIRS "NFW_etc" )
list(APPEND SRCDIRS "mass_functions" )
list(APPEND SRCDIRS "bias" )

# Add header files
# eg. list(APPEND INCFILES "file" )
list(APPEND INCFILES "gbpCosmo.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
# -- NONE FOR THIS DIR --

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "file" )
list(APPEND EXEFILES "plot_mergers.c" )
list(APPEND EXEFILES "scale_gbpCosmo2gbpCosmo.c" )
list(APPEND EXEFILES "plot_Pk.c" )
list(APPEND EXEFILES "make_mass_function.c" )
list(APPEND EXEFILES "make_snap_list.c" )
list(APPEND EXEFILES "D_angular_1to2.c" )
list(APPEND EXEFILES "plot_cosmo.c" )
list(APPEND EXEFILES "plot_clustering.c" )

# Add data files
# eg. list(APPEND DATAFILES "file" )
list(APPEND DATAFILES "cosmology_WMAP-1.txt" ) 
list(APPEND DATAFILES "transfer_function_WMAP-1.txt" ) 
list(APPEND DATAFILES "cosmology_WMAP-5.txt" ) 
list(APPEND DATAFILES "transfer_function_WMAP-5.txt" ) 
list(APPEND DATAFILES "cosmology_WMAP-7.txt" ) 
list(APPEND DATAFILES "transfer_function_WMAP-7.txt" ) 
list(APPEND DATAFILES "cosmology_Planck-2013.txt" ) 
list(APPEND DATAFILES "transfer_function_Planck-2013.txt" ) 
list(APPEND DATAFILES "cosmology_Planck-2015.txt" ) 
list(APPEND DATAFILES "transfer_function_Planck-2015.txt" )

# Set data subdirectory
# eg. set(DATASUBDIR "dir" )
set(DATASUBDIR "gbpCosmo" )

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# eg. list(APPEND PASSDIRS "dir" )
# -- NONE FOR THIS DIR --

