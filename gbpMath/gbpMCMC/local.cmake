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
list(APPEND INCFILES "gbpMCMC.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "add_MCMC_DS.c" )
list(APPEND SRCFILES "start_loop_MCMC.c" )
list(APPEND SRCFILES "end_loop_MCMC.c" )
list(APPEND SRCFILES "restart_MCMC.c" )
list(APPEND SRCFILES "autotune_MCMC.c" )
list(APPEND SRCFILES "autotune_MCMC_randomize.c" )
list(APPEND SRCFILES "autotune_MCMC_temperature.c" )
list(APPEND SRCFILES "autotune_MCMC_covariance.c" )
list(APPEND SRCFILES "compute_MCMC.c" )
list(APPEND SRCFILES "analyze_MCMC.c" )
list(APPEND SRCFILES "read_MCMC_state.c" )
list(APPEND SRCFILES "compute_MCMC_chain_stats.c" )
list(APPEND SRCFILES "compute_MCMC_ln_likelihood_default.c" )
list(APPEND SRCFILES "free_MCMC_covariance.c" )
list(APPEND SRCFILES "free_MCMC_arrays.c" )
list(APPEND SRCFILES "free_MCMC_DS.c" )
list(APPEND SRCFILES "free_MCMC.c" )
list(APPEND SRCFILES "generate_MCMC_chain.c" )
list(APPEND SRCFILES "generate_MCMC_parameters.c" )
list(APPEND SRCFILES "generate_MCMC_proposition.c" )
list(APPEND SRCFILES "init_MCMC.c" )
list(APPEND SRCFILES "init_MCMC_DS.c" )
list(APPEND SRCFILES "init_MCMC_arrays.c" )
list(APPEND SRCFILES "set_MCMC_autotune.c" )
list(APPEND SRCFILES "set_MCMC_covariance.c" )
list(APPEND SRCFILES "set_MCMC_mode.c" )
list(APPEND SRCFILES "set_MCMC_directory.c" )
list(APPEND SRCFILES "rm_MCMC_directory.c" )
list(APPEND SRCFILES "set_MCMC_coverage_size.c" )
list(APPEND SRCFILES "set_MCMC_likelihood_function.c" )
list(APPEND SRCFILES "set_MCMC_iterations.c" )
list(APPEND SRCFILES "set_MCMC_temperature.c" )
list(APPEND SRCFILES "read_MCMC_configuration.c" )
list(APPEND SRCFILES "read_MCMC_covariance.c" )

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "file" )
list(APPEND EXEFILES "example_line_fit.c" )
list(APPEND EXEFILES "covariance2ascii.c" )
list(APPEND EXEFILES "reanalyze_MCMC.c" )

# Add data files
# eg. list(APPEND DATAFILES "file" )
# -- NONE FOR THIS DIR --

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# eg. list(APPEND PASSDIRS "dir" )
# -- NONE FOR THIS DIR --

