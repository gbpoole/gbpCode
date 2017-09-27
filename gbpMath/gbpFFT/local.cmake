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
# -- NONE FOR THIS DIR --

# Add header files
# eg. list(APPEND INCFILES "file" )
list(APPEND INCFILES "gbpFFT.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "add_buffer_FFT_R.c" )
list(APPEND SRCFILES "compute_FFT.c" )
list(APPEND SRCFILES "compute_iFFT.c" )
list(APPEND SRCFILES "index2indices_FFT_k.c" )
list(APPEND SRCFILES "index2indices_FFT_R.c" )
list(APPEND SRCFILES "index_FFT_k.c" )
list(APPEND SRCFILES "index_FFT_R.c" )
list(APPEND SRCFILES "index_local_FFT_k.c" )
list(APPEND SRCFILES "index_local_FFT_R.c" )
list(APPEND SRCFILES "k_field_FFT.c" )
list(APPEND SRCFILES "k_mag_field_FFT.c" )
list(APPEND SRCFILES "set_FFT_padding_state.c" )
list(APPEND SRCFILES "pad_index_FFT_R.c" )
list(APPEND SRCFILES "remove_buffer_FFT_R.c" )
list(APPEND SRCFILES "R_field_FFT.c" )

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

