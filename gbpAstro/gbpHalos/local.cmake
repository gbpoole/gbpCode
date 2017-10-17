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
list(APPEND INCFILES "gbpHalos.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
list(APPEND SRCFILES "fopen_catalog.c" )
list(APPEND SRCFILES "fread_catalog.c" )
list(APPEND SRCFILES "fclose_catalog.c" )
list(APPEND SRCFILES "check_if_substructure_hierarchy_defined.c" )
list(APPEND SRCFILES "init_halo_trend.c" )
list(APPEND SRCFILES "init_halo_trend_coordinate.c" )
list(APPEND SRCFILES "init_halo_trend_functions.c" )
list(APPEND SRCFILES "read_groups.c" )
list(APPEND SRCFILES "read_groups_AHF.c" )
list(APPEND SRCFILES "match_halos.c" )
list(APPEND SRCFILES "match_score_f_goodness.c" )
list(APPEND SRCFILES "maximum_match_score.c" )
list(APPEND SRCFILES "compute_group_analysis.c" )
list(APPEND SRCFILES "write_group_analysis.c" )

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "file" )
list(APPEND EXEFILES "query_catalog_counts.c" )
list(APPEND EXEFILES "make_catalog_mass_function.c" ) 
list(APPEND EXEFILES "make_halo_particle_list.c" ) 
list(APPEND EXEFILES "make_catalog_SSFctn.c" ) 
list(APPEND EXEFILES "catalog_splitmerge.c" ) 
list(APPEND EXEFILES "make_catalog_subvolume_stats.c" ) 
list(APPEND EXEFILES "make_catalog_summary.c" ) 
list(APPEND EXEFILES "haloIDs2stdout.c" ) 
list(APPEND EXEFILES "update_properties.c" ) 
list(APPEND EXEFILES "convert_PHK2ascii.c" ) 
list(APPEND EXEFILES "reorder_halo_ids.c" ) 
list(APPEND EXEFILES "query_catalog_indices.c" ) 
list(APPEND EXEFILES "remove_duplicates.c" ) 
list(APPEND EXEFILES "update_halo_files_format.c" ) 
list(APPEND EXEFILES "make_group_PHKs.c" ) 
list(APPEND EXEFILES "query_group.c" ) 
list(APPEND EXEFILES "query_catalog.c" ) 
list(APPEND EXEFILES "make_catalog_ascii.c" ) 
list(APPEND EXEFILES "make_catalog_group_ascii.c" ) 
list(APPEND EXEFILES "make_group_analysis.c" ) 
list(APPEND EXEFILES "cat_group_analysis.c" ) 
list(APPEND EXEFILES "convert_AHF.c" )

# Add data files
# eg. list(APPEND DATAFILES "file" )
# -- NONE FOR THIS DIR --

# Set data subdirectory
# eg. set(DATASUBDIR "dir" )

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# eg. list(APPEND PASSDIRS "dir" )
# -- NONE FOR THIS DIR --

