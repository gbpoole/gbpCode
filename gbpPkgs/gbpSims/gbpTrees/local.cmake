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
list(APPEND SRCDIRS "core" )
list(APPEND SRCDIRS "analysis" )

# Add header files
# eg. list(APPEND INCFILES "file" )
list(APPEND INCFILES "gbpTrees.h" )

# Add source files
# eg. list(APPEND SRCFILES "file" )
# -- NONE FOR THIS DIR --

# Add executable source files (those with a main())
# eg. list(APPEND EXEFILES "file" )
list(APPEND EXEFILES "make_trees_stats.c" )
list(APPEND EXEFILES "make_match_score_table.c" )
list(APPEND EXEFILES "query_2way_matches.c" )
list(APPEND EXEFILES "make_atable_match_score.c" )
list(APPEND EXEFILES "query_trees_SAGE.c" )
list(APPEND EXEFILES "query_trees_volume.c" )
list(APPEND EXEFILES "make_catalog_markers_ascii.c" )
list(APPEND EXEFILES "make_cross_catalog_matches.c" )
list(APPEND EXEFILES "query_progenitors.c" )
list(APPEND EXEFILES "query_trees.c" )
list(APPEND EXEFILES "query_forest_trees_IDs.c" )
list(APPEND EXEFILES "extract_tree_horizontal.c" )
list(APPEND EXEFILES "most_massive_subgroup_analysis.c" )
list(APPEND EXEFILES "tree_stats_horizontal.c" )
list(APPEND EXEFILES "validate_ghost_trees.c" )
list(APPEND EXEFILES "parse_tree_case.c" )
list(APPEND EXEFILES "crossmatches2ascii.c" )
list(APPEND EXEFILES "check_match_files.c" )
list(APPEND EXEFILES "matchfile2ascii.c" )
list(APPEND EXEFILES "make_trees.c" )
list(APPEND EXEFILES "make_trees_markers.c" )
list(APPEND EXEFILES "make_trees_analysis.c" )
list(APPEND EXEFILES "make_trees_matches.c" )
list(APPEND EXEFILES "make_trees_vertical.c" )
list(APPEND EXEFILES "make_trees_horizontal.c" )
list(APPEND EXEFILES "make_trees_analysis_relaxation.c" )
list(APPEND EXEFILES "make_single_matches.c" )
list(APPEND EXEFILES "make_forests.c" )
list(APPEND EXEFILES "query_matches.c" )
list(APPEND EXEFILES "make_matched_catalog.c" )
list(APPEND EXEFILES "make_match_catalog.c" )
list(APPEND EXEFILES "convert_Bolshoi.c" )
list(APPEND EXEFILES "tree_groups.c" )
list(APPEND EXEFILES "view_tree.c" )
list(APPEND EXEFILES "view_tree_mil.c" )

# Add data files
# eg. list(APPEND DATAFILES "file" )
# -- NONE FOR THIS DIR --

# Set data subdirectory
# eg. set(DATASUBDIR "dir" )

# Add subdirectories that we want to decend into
#   but which we won't scan for sources, etc
# eg. list(APPEND PASSDIRS "dir" )
# -- NONE FOR THIS DIR --

