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
list(APPEND INCFILES "gbpSwapEndian.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "swap_endian_snapshot.c" )
list(APPEND SRCFILES "swap_endian_smooth.c" )
list(APPEND SRCFILES "swap_endian_grids.c" )
list(APPEND SRCFILES "swap_endian_halos.c" )
list(APPEND SRCFILES "swap_endian_catalogs.c" )
list(APPEND SRCFILES "check_integrity_grids.c" )
list(APPEND SRCFILES "check_integrity_halos.c" )
list(APPEND SRCFILES "check_integrity_catalogs.c" )

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "file" )
list(APPEND EXEFILES "check_integrity_SSimPL.c" )
list(APPEND EXEFILES "swap_endian_SSimPL.c" )

# Add data files
# eg. list(APPEND DATAFILES "file" )
# -- NONE FOR THIS DIR --

# Set data subdirectory
# eg. set(DATASUBDIR "dir" )

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# eg. list(APPEND PASSDIRS "dir" )
# -- NONE FOR THIS DIR --

