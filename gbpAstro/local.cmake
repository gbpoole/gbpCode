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
list(APPEND LIBDIRS "gbpCosmo" )
list(APPEND LIBDIRS "gbpSPH" ) 
list(APPEND LIBDIRS "gbpHalos" ) 
list(APPEND LIBDIRS "gbpTrees" )
if(USE_GD)
    list(APPEND LIBDIRS "gbpRender" )
else()
    message(STATUS "libgd not found.  Skipping config of gbpRender." )
endif()
if(USE_FFTW)
    list(APPEND LIBDIRS "gbpClustering" )
    list(APPEND LIBDIRS "gbpSwapEndian" )
else()
    message(STATUS "fftw not found.  Skipping config of gbpClustering & gbpSwapEndian." )
endif()

# Add directories that contribute source files 
# eg. list(APPEND SRCDIRS "dir" )
# -- NONE FOR THIS DIR --

# Add header files
# eg. list(APPEND INCFILES "file" )
# -- NONE FOR THIS DIR --

# Add source files
# eg. list(APPEND SRCFILES "file" )
# -- NONE FOR THIS DIR --

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "file" )
# -- NONE FOR THIS DIR --

# Add data files
# eg. list(APPEND DATAFILES "file" )
# -- NONE FOR THIS DIR --

# Set data subdirectory
# eg. set(DATASUBDIR "dir" )
# -- NONE FOR THIS DIR --

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# eg. list(APPEND PASSDIRS "dir" )
# -- NONE FOR THIS DIR --

