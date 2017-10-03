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
list(APPEND INCFILES "gbpCosmo_core.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "a_of_z.c" )
list(APPEND SRCFILES "z_of_a.c" )
list(APPEND SRCFILES "init_deltat_a.c" )
list(APPEND SRCFILES "deltat_a.c" )
list(APPEND SRCFILES "R_Delta_z.c" )
list(APPEND SRCFILES "Omega_z.c" )
list(APPEND SRCFILES "Delta_vir.c" )
list(APPEND SRCFILES "Ha_Ho.c" )
list(APPEND SRCFILES "t_Hubble_z.c" )
list(APPEND SRCFILES "t_dyn_z.c" )
list(APPEND SRCFILES "n_dyn_ztoz.c" )
list(APPEND SRCFILES "t_age_z.c" )
list(APPEND SRCFILES "t_age_a.c" )
list(APPEND SRCFILES "E_z.c" )
list(APPEND SRCFILES "H_z.c" )
list(APPEND SRCFILES "H_convert.c" )
list(APPEND SRCFILES "dlna_dtau.c" )
list(APPEND SRCFILES "rho_crit_z.c" )
list(APPEND SRCFILES "rho_crit_z_strip.c" )
list(APPEND SRCFILES "D_Hubble.c" )
list(APPEND SRCFILES "D_comove.c" )
list(APPEND SRCFILES "D_comove_transverse.c" )
list(APPEND SRCFILES "D_angular.c" )
list(APPEND SRCFILES "D_angular_1to2.c" )
list(APPEND SRCFILES "D_luminosity.c" )
list(APPEND SRCFILES "read_gbpCosmo_file.c" )
list(APPEND SRCFILES "init_cosmo_default.c" )
list(APPEND SRCFILES "init_cosmo.c" )
list(APPEND SRCFILES "free_cosmo.c" )

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

