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
list(APPEND INCFILES "gbpSPH.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "display_gadget_header.c" )
list(APPEND SRCFILES "display_gadget_info.c" )
list(APPEND SRCFILES "process_gadget_file.c" )
list(APPEND SRCFILES "process_gadget_file_functions.c" )
list(APPEND SRCFILES "allocate_gadget_particles.c" )
list(APPEND SRCFILES "init_gadget_read.c" )
list(APPEND SRCFILES "init_smooth_read.c" )
list(APPEND SRCFILES "set_gadget_filename.c" )
list(APPEND SRCFILES "set_smooth_filename.c" )
list(APPEND SRCFILES "change_gadget_filename.c" )
list(APPEND SRCFILES "free_plist.c" )
list(APPEND SRCFILES "free_types.c" )
list(APPEND SRCFILES "init_plist.c" )
list(APPEND SRCFILES "mark_particles.c" )
list(APPEND SRCFILES "prep_types.c" )
list(APPEND SRCFILES "read_mark_file.c" )
list(APPEND SRCFILES "read_smooth.c" )
list(APPEND SRCFILES "read_gadget_binary.c" )
list(APPEND SRCFILES "read_gadget_binary_header.c" )
list(APPEND SRCFILES "mark_gadget.c" )
list(APPEND SRCFILES "mark_particles.c" )
list(APPEND SRCFILES "read_mark_file.c" )
list(APPEND SRCFILES "write_mark_file.c" )
list(APPEND SRCFILES "write_gadget_ascii.c" )
list(APPEND SRCFILES "write_gadget_csv.c" )
list(APPEND SRCFILES "write_gadget_bin.c" )
list(APPEND SRCFILES "write_gadget_binary_new.c" )
list(APPEND SRCFILES "write_gadget_binary.c" )

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "file" )
list(APPEND EXEFILES "make_snaplist.c" )
list(APPEND EXEFILES "make_expanded_snaplist.c" )
list(APPEND EXEFILES "gadget2ascii.c" )
list(APPEND EXEFILES "gadget2bin.c" )
list(APPEND EXEFILES "gadget_excise.c" )
list(APPEND EXEFILES "gadget_split.c" )
list(APPEND EXEFILES "mark_gadget.c" )
list(APPEND EXEFILES "gadget2csv.c" )
list(APPEND EXEFILES "gadget_info.c" )
list(APPEND EXEFILES "gadget_header.c" )
list(APPEND EXEFILES "gadget_replicate.c" )
list(APPEND EXEFILES "gadget_splitmerge.c" )
list(APPEND EXEFILES "gadget_subsample.c" )
list(APPEND EXEFILES "gadget_mark.c" )
list(APPEND EXEFILES "smooth2ascii.c" )

# Add data files
# eg. list(APPEND DATAFILES "file" )
# -- NONE FOR THIS DIR --

# Set data subdirectory
# eg. set(DATASUBDIR "dir" )

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# eg. list(APPEND PASSDIRS "dir" )
# -- NONE FOR THIS DIR --

