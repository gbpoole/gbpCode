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
list(APPEND INCFILES "gbpHist.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "init_histogram.c" )
list(APPEND SRCFILES "free_histogram.c" )
list(APPEND SRCFILES "clear_histogram.c" )
list(APPEND SRCFILES "clear_trend.c" )
list(APPEND SRCFILES "calc_histogram_index.c" )
list(APPEND SRCFILES "is_histogram_index_in_range.c" )
list(APPEND SRCFILES "add_to_histogram.c" )
list(APPEND SRCFILES "add_to_histogram_index.c" )
list(APPEND SRCFILES "finalize_histogram.c" )
list(APPEND SRCFILES "finalize_trend_property.c" )
list(APPEND SRCFILES "finalize_trend.c" )
list(APPEND SRCFILES "histogram_bin_x_lo.c" )
list(APPEND SRCFILES "histogram_bin_x_hi.c" )
list(APPEND SRCFILES "histogram_bin_x_mid.c" )
list(APPEND SRCFILES "compute_histogram_range.c" )
list(APPEND SRCFILES "init_trend.c" )
list(APPEND SRCFILES "init_trend_ordinate.c" )
list(APPEND SRCFILES "init_trend_coordinate.c" )
list(APPEND SRCFILES "free_trend.c" )
list(APPEND SRCFILES "init_trend_property.c" )
list(APPEND SRCFILES "free_trend_property.c" )
list(APPEND SRCFILES "add_item_to_trend.c" )
list(APPEND SRCFILES "write_trend_ascii.c" )
list(APPEND SRCFILES "write_trend_property_binning_file.c" )

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

