cmake_minimum_required(VERSION 3.5)

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

# Set default compilers (must come before project line)
set(CMAKE_C_COMPILER   "gcc")
set(CMAKE_CXX_COMPILER "g++")

# Uncomment for a verbose run
#set(CMAKE_VERBOSE_MAKEFILE ON)

# Declare the project name
project (gbpCode)

# Get the list of root directories for libraries
#   Do this by finding all directories that have
#   a '.cmake.libdir' file.  Note that if any new
#   directories of this type - or any new files - 
#   get added, cmake will need to be re-run.
file(GLOB_RECURSE gbpCode_LIBDIR_files ".cmake.libdir")
message(STATUS gbpCode_LIBDIR_files)
set (gbpCode_LIBDIRS  "")
set (gbpCode_LIBNAMES "")
foreach (_libdir_file ${gbpCode_LIBDIR_files})
    get_filename_component(_libdir  ${_libdir_file} DIRECTORY)
    get_filename_component(_libname ${_libdir}      NAME)
    list (APPEND gbpCode_LIBDIRS  ${_libdir})
    list (APPEND gbpCode_LIBNAMES ${_libname})
endforeach(_libdir_file)
if(NOT gbpCode_LIBDIRS)
    message(FATAL_ERROR "Could not find any libraries to build.")
else()
    message(STATUS "The following libraries will be built: ${gbpCode_LIBNAMES}")
endif(NOT gbpCode_LIBDIRS)

# Build the libraries
list(LENGTH gbpCode_LIBDIRS len_lib)
math(EXPR n_lib "${len_lib}-1")
foreach(i_lib RANGE ${n_lib})
    list(GET gbpCode_LIBDIRS  ${i_lib} _libdir)
    list(GET gbpCode_LIBNAMES ${i_lib} _libname)
    message(STATUS "Initializing ${_libname} from ${_libdir}...")

    # Assemble the list of sources
    file(GLOB_RECURSE _SOURCES ${_libdir}/*.[ch,cu,cpp,hh])
    list(LENGTH _SOURCES N_FILES)
    message(STATUS "   ${N_FILES} files added to library.")

    # Create library
    add_library(${_libname} SHARED ${_SOURCES})

    message(STATUS "Finished initializing ${_libname}.")
endforeach (i_lib RANGE ${n_lib})

#set (gbpCode_INCLUDE_DIRS "")
#set (gbpCode_INCLUDE_DIRS "")
#foreach (_headerFile ${gbpCode_HEADERS})
#    get_filename_component(_dir ${_headerFile} PATH)
#    list (APPEND gbpCode_INCLUDE_DIRS ${_dir})
#    target_include_directories(gbpCode_INCLUDE_DIRS 
#endforeach()
#list(REMOVE_DUPLICATES gbpCode_INCLUDE_DIRS)
#message(STATUS gbpCode_LIB_DIRS)
##add_executable (gbpCodeExe ${gbpCode_SOURCES})
#include_directories(${gbpCode_INCLUDE_DIRS})
#
