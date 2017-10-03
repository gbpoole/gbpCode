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
list(APPEND INCFILES "gbpCosmo_NFW_etc.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "set_NFW_params.c" )
list(APPEND SRCFILES "R_vir_NFW.c" )
list(APPEND SRCFILES "c_vir_NFW.c" )
list(APPEND SRCFILES "rho_NFW.c" )
list(APPEND SRCFILES "rho_NFW_fft.c" )
list(APPEND SRCFILES "M_r_NFW.c" )
list(APPEND SRCFILES "V_circ_NFW.c" )
list(APPEND SRCFILES "V_circ_vir_NFW.c" )
list(APPEND SRCFILES "V_max_NFW.c" )
list(APPEND SRCFILES "Vmax_to_Mvir_NFW.c" )
list(APPEND SRCFILES "R_half_V_max_NFW.c" )
list(APPEND SRCFILES "R_V_max_NFW.c" )
list(APPEND SRCFILES "Delta_half_V_max_NFW.c" )
list(APPEND SRCFILES "V2_circ.c" )

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "file" )
# -- NONE FOR THIS DIR --

# Add data files
# eg. list(APPEND DATAFILES "file" )
# -- NONE FOR THIS DIR --

# Set data subdirectory
# eg. set(DATASUBDIR "dir" )

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# eg. list(APPEND PASSDIRS "dir" )
# -- NONE FOR THIS DIR --

