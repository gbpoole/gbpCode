# Set empty defaults
set(LIBDIRS    "" )
set(SRCDIRS    "" )
set(INCFILES   "" )
set(SRCFILES   "" )
set(EXEFILES   "" )
set(TARGETDEPS "" )
set(DATAFILES  "" )
set(PASSDIRS   "" )
set(DATASUBDIR "" )

# Add subdirectories that are roots to libraries
# eg. list(APPEND LIBDIRS "dirname")
# -- NONE FOR THIS DIR --

# Add directories that contribute source files 
# eg. list(APPEND SRCDIRS "dirname")
list(APPEND SRCDIRS "core")
list(APPEND SRCDIRS "parameter_files")

# Add header files
# eg. list(APPEND INCFILES "filename.ext")
list(APPEND INCFILES "gbpParse.h")

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
# eg. list(APPEND TARGETDEPS "dep_name")
# -- NONE FOR THIS DIR --

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# -- NONE FOR THIS DIR --
