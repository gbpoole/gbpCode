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
list(APPEND SRCDIRS "gbpDomain" )
list(APPEND SRCDIRS "gbpSort" )
list(APPEND SRCDIRS "gbpStats" )
list(APPEND SRCDIRS "gbpInterpolate" )
list(APPEND SRCDIRS "gbpMisc" )
list(APPEND SRCDIRS "gbpHist" )
list(APPEND SRCDIRS "gbpRNG" )
list(APPEND SRCDIRS "gbpPHKs" )
list(APPEND SRCDIRS "gbpMCMC" )
if(USE_FFTW)
    list(APPEND SRCDIRS "gbpFFT" )
else()
    message(STATUS "fftw not found.  Skipping config of gbpFFT." )
endif()

# Add header files
# eg. list(APPEND INCFILES "file" )
list(APPEND INCFILES "gbpMath.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
# -- NONE FOR THIS DIR --

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

