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
list(APPEND INCFILES "gbpParse_core.h")

# Add source files
# eg. list(APPEND SRCFILES "filename.ext")
list(APPEND SRCFILES "swap_endian.c" )
list(APPEND SRCFILES "rewrite_swap_endian.c" )
list(APPEND SRCFILES "count_lines.c" )
list(APPEND SRCFILES "count_lines_data.c" )
list(APPEND SRCFILES "count_lines_paramters.c" )
list(APPEND SRCFILES "count_words.c" )
list(APPEND SRCFILES "check_comment.c" )
list(APPEND SRCFILES "check_space.c" )
list(APPEND SRCFILES "check_parameter.c" )
list(APPEND SRCFILES "scan_to_nth_word.c" )
list(APPEND SRCFILES "grab_word.c" )
list(APPEND SRCFILES "grab_tail.c" )
list(APPEND SRCFILES "grab_double.c" )
list(APPEND SRCFILES "grab_float.c" )
list(APPEND SRCFILES "grab_real.c" )
list(APPEND SRCFILES "grab_int.c" )
list(APPEND SRCFILES "grab_long.c" )
list(APPEND SRCFILES "grab_next_line.c" )
list(APPEND SRCFILES "grab_next_line_data.c" )
list(APPEND SRCFILES "grab_next_line_parameter.c" )
list(APPEND SRCFILES "grab_size_t.c" )
list(APPEND SRCFILES "grab_word.c" )
list(APPEND SRCFILES "strip_path.c" )
list(APPEND SRCFILES "strip_file_root.c" )
list(APPEND SRCFILES "search_and_replace.c" )
list(APPEND SRCFILES "float_to_text.c" )

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
