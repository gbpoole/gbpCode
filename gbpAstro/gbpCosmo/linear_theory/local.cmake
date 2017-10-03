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
list(APPEND INCFILES "gbpCosmo_linear_theory.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "R_of_k.c" )
list(APPEND SRCFILES "k_of_R.c" )
list(APPEND SRCFILES "M_of_R.c" )
list(APPEND SRCFILES "M_of_k.c" )
list(APPEND SRCFILES "R_of_M.c" )
list(APPEND SRCFILES "k_of_M.c" )
list(APPEND SRCFILES "R_NL_z.c" )
list(APPEND SRCFILES "init_gbpCosmo2gbpCosmo.c" )
list(APPEND SRCFILES "z_gbpCosmo2gbpCosmo.c" )
list(APPEND SRCFILES "L_gbpCosmo2gbpCosmo.c" )
list(APPEND SRCFILES "M_gbpCosmo2gbpCosmo.c" )
list(APPEND SRCFILES "merger_rate_B_n.c" )
list(APPEND SRCFILES "pspec_names.c" )
list(APPEND SRCFILES "power_spectrum.c" )
list(APPEND SRCFILES "power_spectrum_variance.c" )
list(APPEND SRCFILES "dln_Inv_sigma_dlogM.c" )
list(APPEND SRCFILES "sigma2_integrand.c" )
list(APPEND SRCFILES "sigma_M.c" )
list(APPEND SRCFILES "sigma_R.c" )
list(APPEND SRCFILES "ln_sigma_M.c" )
list(APPEND SRCFILES "ln_Inv_sigma_M.c" )
list(APPEND SRCFILES "dln_sigma_dlnM.c" )
list(APPEND SRCFILES "dDplus_da.c" )
list(APPEND SRCFILES "dDplus_dz.c" )
list(APPEND SRCFILES "Dplus.c" )
list(APPEND SRCFILES "Dplus_unnormed.c" )
list(APPEND SRCFILES "linear_growth_factor.c" )
list(APPEND SRCFILES "W_k_tophat.c" )
list(APPEND SRCFILES "M_sc.c" )
list(APPEND SRCFILES "lk_sc.c" )

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

