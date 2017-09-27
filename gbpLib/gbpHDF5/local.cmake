# Set empty defaults
set(LIBDIRS    "" )
set(SRCDIRS    "" )
set(INCFILES   "" )
set(SRCFILES   "" )
set(EXEFILES   "" )
set(TARGETDEPS "" )
set(DATAFILES  "" )
set(PASSDIRS   "" )

# Add subdirectories that are roots to libraries
# eg. list(APPEND LIBDIRS "dirname")
# -- NONE FOR THIS DIR --

# Add directories that contribute source files 
# eg. list(APPEND SRCDIRS "dirname")
# -- NONE FOR THIS DIR --

# Add header files
# eg. list(APPEND INCFILES "filename.ext")
list(APPEND INCFILES "gbpHDF5.h")

# Add source files
# eg. list(APPEND SRCFILES "filename.ext")
list(APPEND SRCFILES "read_HDF5_1D.c")
list(APPEND SRCFILES "read_HDF5_3D.c")

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "filename.ext")
# -- NONE FOR THIS DIR --

# Add target dependencies 
# eg. list(APPEND TARGETDEPS "dep_name")
# -- NONE FOR THIS DIR --

# Add data files
# eg. list(APPEND TARGETDEPS "dep_name")
# -- NONE FOR THIS DIR --

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# -- NONE FOR THIS DIR --
