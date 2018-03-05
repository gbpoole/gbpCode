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
# -- NONE FOR THIS DIR --

# Add header files
# eg. list(APPEND INCFILES "filename.ext" )
list(APPEND INCFILES "gbpParse_parameter_files.h" )

# Add source files
# eg. list(APPEND SRCFILES "filename.ext")
list(APPEND SRCFILES "add_parameter_to_list.c" )
list(APPEND SRCFILES "fetch_parameter_list_item.c" )
list(APPEND SRCFILES "fetch_parameter_data.c" )
list(APPEND SRCFILES "free_parameter_item.c" )
list(APPEND SRCFILES "free_parameter_list.c" )
list(APPEND SRCFILES "init_parameter_item.c" )
list(APPEND SRCFILES "init_parameter_list.c" )
list(APPEND SRCFILES "read_gbpParam_file.c" )
list(APPEND SRCFILES "remove_parameter_character.c" )

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
