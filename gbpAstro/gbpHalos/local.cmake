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
list(APPEND SRCFILES "query_catalog_counts.c" )
list(APPEND SRCFILES "make_catalog_mass_function.c" ) 
list(APPEND SRCFILES "make_halo_particle_list.c" ) 
list(APPEND SRCFILES "make_catalog_SSFctn.c" ) 
list(APPEND SRCFILES "catalog_splitmerge.c" ) 
list(APPEND SRCFILES "make_catalog_subvolume_stats.c" ) 
list(APPEND SRCFILES "make_catalog_summary.c" ) 
list(APPEND SRCFILES "haloIDs2stdout.c" ) 
list(APPEND SRCFILES "update_properties.c" ) 
list(APPEND SRCFILES "convert_PHK2ascii.c" ) 
list(APPEND SRCFILES "reorder_halo_ids.c" ) 
list(APPEND SRCFILES "query_catalog_indices.c" ) 
list(APPEND SRCFILES "remove_duplicates.c" ) 
list(APPEND SRCFILES "update_halo_files_format.c" ) 
list(APPEND SRCFILES "make_group_PHKs.c" ) 
list(APPEND SRCFILES "query_group.c" ) 
list(APPEND SRCFILES "query_catalog.c" ) 
list(APPEND SRCFILES "make_catalog_ascii.c" ) 
list(APPEND SRCFILES "make_catalog_group_ascii.c" ) 
list(APPEND SRCFILES "make_group_analysis.c" ) 
list(APPEND SRCFILES "cat_group_analysis.c" ) 
list(APPEND SRCFILES "convert_AHF.c" )

# Add data files
# eg. list(APPEND DATAFILES "file" )
# -- NONE FOR THIS DIR --

# Set data subdirectory
# eg. set(DATASUBDIR "dir" )

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# eg. list(APPEND PASSDIRS "dir" )
# -- NONE FOR THIS DIR --

