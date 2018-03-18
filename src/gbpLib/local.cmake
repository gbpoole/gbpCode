# Set empty defaults
set(LIBDIRS    "" )
set(SRCDIRS    "" )
set(INCFILES   "" )
set(SRCFILES   "" )
set(EXEFILES   "" )
set(TARGETDEPS "" )
set(DATAFILES  "" )
set(DATASUBDIR "" )
set(PASSDIRS   "" )

# Add subdirectories that are roots to libraries
# eg. list(APPEND LIBDIRS "dirname")
# -- NONE FOR THIS DIR --

# Add directories that contribute source files 
# eg. list(APPEND SRCDIRS "dirname")
list(APPEND SRCDIRS "extern/gbpSID" )
list(APPEND SRCDIRS "gbpSort" )
list(APPEND SRCDIRS "gbpCalc" )
list(APPEND SRCDIRS "gbpParse" )
list(APPEND SRCDIRS "gbpADaPS" )
list(APPEND SRCDIRS "gbpMultifile")
if(USE_CFITSIO OR SCAN_ALL_DIRS)
    list(APPEND SRCDIRS "gbpFITS")
endif()
if(USE_HDF5 OR SCAN_ALL_DIRS)
    list(APPEND SRCDIRS "gbpHDF5")
endif()

# Add header files
# eg. list(APPEND INCFILES "filename.ext")
list(APPEND INCFILES "gbpLib.h")
list(APPEND INCFILES "gbpConstants.h")

# Add source files
# eg. list(APPEND SRCFILES "filename.ext")
# -- NONE FOR THIS DIR --

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "filename.ext")
# -- NONE FOR THIS DIR --

# Add target dependencies 
# eg. list(APPEND TARGETDEPS "dep_name")
# -- NONE FOR THIS DIR --

# Add data files
# eg. list(APPEND DATAFILES "filename")
# -- NONE FOR THIS DIR --

# Set data subdirectory
# eg. set(DATASUBDIR "dir" )
# -- NONE FOR THIS DIR --

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
