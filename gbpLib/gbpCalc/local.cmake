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
# eg. list(APPEND INCFILES "filename.ext")
list(APPEND INCFILES "gbpCalc.h" )

# Add source files
# eg. list(APPEND SRCFILES "filename.ext")
list(APPEND SRCFILES "calc_min.c" )
list(APPEND SRCFILES "calc_min_global.c" )
list(APPEND SRCFILES "calc_max.c" )
list(APPEND SRCFILES "calc_max_global.c" )
list(APPEND SRCFILES "calc_mean.c" )
list(APPEND SRCFILES "calc_mean_global.c" )
list(APPEND SRCFILES "calc_sum.c" )
list(APPEND SRCFILES "calc_sum_global.c" )
list(APPEND SRCFILES "calc_stddev.c" )
list(APPEND SRCFILES "calc_stddev_global.c" )
list(APPEND SRCFILES "calc_mean.c" )
list(APPEND SRCFILES "calc_mean_global.c" )
list(APPEND SRCFILES "calc_median.c" )
list(APPEND SRCFILES "calc_median_global.c" )

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
