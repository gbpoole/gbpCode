# This file contains all the macros that are used
#   by cmake to build, debug, etc. this project
message(STATUS "Loading macros...")

# Set a varibale that we can check to make
#   sure that we have access to these macros
set(MACROS_LOADED ON)

# Macro to get all the information we
#    need about a given directory
macro(set_dir_state dir)
    # Read a file which specifies the content
    #   and subdirectory structure of the
    #   given directory.
    include( ${dir}/local.cmake )

    # Create a list of all the subdirectories
    set(ALLDIRS "" )
    list(APPEND ALLDIRS ${LIBDIRS} )
    list(APPEND ALLDIRS ${SRCDIRS} )
    list(APPEND ALLDIRS ${PASSDIRS} )
endmacro()

# Macro which adds a directory to the project's
#    list of include direcories, if it is one
macro(add_header_directory cur_dir )
    if( INCFILES )
        list(APPEND INC_DIRS_PROJECT ${cur_dir} )
        message(STATUS "   -> " ${cur_dir} )
    endif()
endmacro()

# Macro to assemble a list of all directories
#    with a header file
macro(find_header_directories cur_dir )
    # Perform some initialization on the first call
    if( ${cur_dir} STREQUAL ${CMAKE_SOURCE_DIR} )
        message(STATUS "Assembling a list of project header directories..." )
        set(INC_DIRS_PROJECT "" )
    endif()

    # Process this directory
    set_dir_state(${cur_dir})
    add_header_directory( ${cur_dir} ${INCFILES} )

    # Recurse through all directories
    foreach(_dir_name ${ALLDIRS} )
        find_header_directories( ${cur_dir}/${_dir_name} ) 
    endforeach()

    # Add the directories we have assembled to the project
    include_directories( ${INC_DIRS_PROJECT} )

    if( ${cur_dir} STREQUAL ${CMAKE_SOURCE_DIR} )
        message(STATUS "Done." )
        message(STATUS "" )
    endif()
endmacro()

# Macro for adding sources to a library build
macro(collect_library_sources lib_name cur_dir )
    set_dir_state( ${cur_dir} )

    # Report if there are any files being added from this directory
    if( SRCFILES OR INCFILES )
        message(STATUS "   Adding source files in " ${cur_dir} " to " ${lib_name} )
    endif()

    # Check that there are no LIBDIRS sitting
    #    in the subdirectory structure of
    #    any other LIBDIRS
    if(LIBDIRS)
        message(FATAL_ERROR "Library directory ${cur_dir} sits in the subdirectory tree of library ${lib_name}")
    endif()

    # Add any source files in this directory
    #   to the library's file list
    foreach(_SRC ${SRCFILES} )
        list(APPEND LIBRARY_SOURCES ${cur_dir}/${_SRC} )
    endforeach()

    # Recurse over the source directories 
    foreach( _dir_name ${SRCDIRS} )
        collect_library_sources( ${lib_name} ${cur_dir}/${_dir_name}  )
    endforeach()
endmacro()

# Macro for initializing a specific library
macro(build_library lib_name cur_dir )
    message(STATUS "Initializing " ${lib_name} )
    set(LIBRARY_SOURCES "" )

    # Collect the sources for the library and add it to targets
    collect_library_sources( ${lib_name} ${cur_dir} )
    add_library(${lib_name} STATIC ${LIBRARY_SOURCES})

    # Take care of dependancies.  We are assuming that
    #   libraries were listed in order of dependancy.
    if(DEPLIST)
        message(STATUS "   Adding dependencies: " "${DEPLIST}" )
        add_dependencies(${lib_name} ${DEPLIST})
    endif()
    list(APPEND DEPLIST ${lib_name} )

    # Status message
    message(STATUS "Done." )
    message(STATUS "" )
endmacro()

# Main macro which initializes all project targets
macro(process_targets cur_dir )

    # First, add libraries (because they
    # may be dependencies for other targets)
    set_dir_state(${cur_dir})
    foreach(_lib_name ${LIBDIRS} )
        build_library( ${_lib_name} ${cur_dir}/${_lib_name} ) 
    endforeach()

    # Set-up the dependencies for the executables
    #   The list needs to be reversed so that 
    #   dependencies between dependencies are 
    #   properly resolved.
    # n.b.: We are assuming that libraries are
    #       listed in order of dependancy.
    set(_DEPLIST_REV "" )
    list(APPEND  _DEPLIST_REV ${DEPLIST} )
    list(REVERSE _DEPLIST_REV)

    # Then, add any executables that are in this directory
    set_dir_state(${cur_dir})
    foreach( _exe_file_nopath ${EXEFILES} )
        # Add the path to the _exe_file_nopath
        set( _exe_file ${cur_dir}/${_exe_file_nopath} )

        # Add executable to the target list
        get_filename_component( _exe_name ${_exe_file} NAME_WE )
        message(STATUS "Adding executable " ${_exe_name} " from " ${_exe_file} " (dependencies: " "${_DEPLIST_REV}" ")")
        add_executable(${_exe_name} ${_exe_file})

        # Add dependencies. 
        target_link_libraries(${_exe_name} ${_DEPLIST_REV})
        if(DEPLIST)
            add_dependencies(${_exe_name} ${DEPLIST})
        endif()
    endforeach()

    # Assemble data directory

    # Recurse over all directories
    set_dir_state(${cur_dir})
    foreach( _dir_i ${ALLDIRS} )
        message(STATUS "test:" ${cur_dir}/${_dir_i})
        process_targets( ${cur_dir}/${_dir_i} )
    endforeach()

endmacro()

# Macro for printing variables
macro(print_variable variableName)
    message(STATUS "${variableName}=${${variableName}}")
endmacro()
macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        print_variable(${_variableName}) 
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

message(STATUS "Macros loaded.")
