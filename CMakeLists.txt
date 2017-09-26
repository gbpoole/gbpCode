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

# Declare the project name
project (gbpCode)

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -std=gnu99")

# Uncomment for a verbose run
#set(CMAKE_VERBOSE_MAKEFILE ON)

# Generate compile commands .json file
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Add 3rd-party libraries
message(STATUS "")
message(STATUS "Initializeing 3rd-party libraries...")

# -- MPI --
if(USE_MPI)
    find_package(MPI REQUIRED)
    include_directories(${MPI_INCLUDE_PATH})
    link_libraries(${MPI_C_LIBRARIES})
endif(USE_MPI)

# GSL
find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})
link_libraries(${GSL_LIBRARIES})

# FITS
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR};${CMAKE_MODULE_PATH}")
find_package(CFITSIO REQUIRED)
include_directories(${fits_INCLUDE_DIRS})
#set(LIBS ${LIBS} ${LibXML++_LIBRARIES})

# HDF5
find_package(HDF5 REQUIRED COMPONENTS C HL)
#if (NOT HDF5_IS_PARALLEL)
#    message(FATAL_ERROR "gbpCode requires HDF5 with parallel support.")
#endif()
include_directories(${HDF5_INCLUDE_DIRS})
link_libraries(${HDF5_C_LIBRARIES} ${HDF5_C_HL_LIBRARIES})
add_definitions(${HDF5_DEFINITIONS})

# THIS IS A TEMPORARY HACK TO OVERCOME A BUG IN SOME VERSIONS OF CMAKE
if(NOT (${HDF5_C_HL_LIBRARIES} MATCHES "libhdf5_hl"))
    list(GET HDF5_C_LIBRARIES 0 HDF5_LIB)
    get_filename_component(HDF5_LIBDIR ${HDF5_LIB} DIRECTORY)
    link_libraries("${HDF5_LIBDIR}/libhdf5_hl.so")
endif()

# FFTW
add_definitions(-DUSE_FFTW)
find_path(FFTW_INCLUDE_DIRS sfftw.h PATHS "${FFTW_ROOT}/*" NO_DEFAULT_PATH)
find_path(FFTW_INCLUDE_DIRS sfftw.h)
find_library(FFTW_LIBRARIES libsfftw.a PATHS "${FFTW_ROOT}/*" NO_DEFAULT_PATH)
find_library(FFTW_LIBRARIES libsfftw.a)

# OpenMP
option(USE_OpenMP "Use OpenMP" OFF)
if(USE_OpenMP)
    find_package(OpenMP REQUIRED)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# CUDA & CUFFT 
#  Note that the CUDA version should be >= 7.5
#  because c++11 support is needed!
option(USE_CUDA "Use CUDA" OFF)
if(USE_CUDA)
    find_package(CUDA REQUIRED)
    add_definitions(-DUSE_CUDA)
    option(USE_CUFFT "Use cuFFT" OFF)
    if(USE_CUFFT)
        add_definitions(-DUSE_CUFFT)
    endif()

    # Collect CUDA source code
    file(GLOB SOURCES_CU core/*.cu *.hh)
    set(SOURCES ${SOURCES_C} ${SOURCES_CU})

    # Pass options to NVCC
    set(CUDA_HOST_COMPILER ${CMAKE_CXX_COMPILER})
    set(CUDA_PROPAGATE_HOST_FLAGS ON)
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -arch=sm_30")

    ################################################################################
    # This ugly hack is needed to work around a bug in the fftw3.h header file
    #    see the following for a discussion: https://github.com/FFTW/fftw3/issues/18
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -D__INTEL_COMPILER")
    ################################################################################

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CUDA_C_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CUDA_EXE_LINKER_FLAGS}")

    ## Add the Meraxes executable to the project
    #cuda_add_executable(meraxes ${SOURCES})

    ## CUFFT
    #if(USE_CUFFT)
    #    CUDA_ADD_CUFFT_TO_TARGET(meraxes)
    #endif()
endif()
message(STATUS "Finished initializeing 3rd-party libraries.")
message(STATUS "")

# Get the list of root directories for libraries
#   Do this by finding all directories that have
#   a '.cmake.libdir' file.  Note that if any new
#   directories of this type - or any new files - 
#   get added, cmake will need to be re-run.
file(GLOB_RECURSE gbpCode_LIBDIR_files ".cmake.libdir")
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
message(STATUS "")

# Get a list of all header files
file(GLOB_RECURSE INC_FILES_ALL *.[h,hh])
set (INC_DIRS_ALL "")
foreach(_INC_FILE ${INC_FILES_ALL})
    get_filename_component(_INC_DIR ${_INC_FILE} DIRECTORY)
    list(APPEND INC_DIRS_ALL ${_INC_DIR})
endforeach(_INC_FILE ${INC_FILES_ALL})
list(REMOVE_DUPLICATES INC_DIRS_ALL)

# Initialize the libraries
list(LENGTH gbpCode_LIBDIRS len_lib)
math(EXPR n_lib "${len_lib}-1")
set (SOURCES_ALL "")
foreach(i_lib RANGE ${n_lib})
    list(GET gbpCode_LIBDIRS  ${i_lib} _libdir)
    list(GET gbpCode_LIBNAMES ${i_lib} _libname)
    message(STATUS "Initializing ${_libname} from ${_libdir}...")

    # Assemble the list of sources
    file(GLOB_RECURSE _SOURCES ${_libdir}/*.[ch,cu,cpp,hh])
    list(LENGTH _SOURCES N_FILES)

    # Validate source list:
    #  1) Make sure files were found
    if(NOT _SOURCES)
        message(FATAL_ERROR "No source files were found.")
    endif(NOT _SOURCES)

    #  2) Make sure the same file has not beed added to
    #     multiple libraries
    foreach(_SOURCE ${_SOURCES})
        if(${_SOURCE} IN_LIST SOURCES_ALL)
            message(FATAL_ERROR "The following file has been added to multiple libraries: ${_SOURCE}")
        endif(${_SOURCE} IN_LIST SOURCES_ALL)
    endforeach(_SOURCE in ${_SOURCES})

    # Accumulate a list of all source files used so far
    list(APPEND SOURCES_ALL _SOURCES)

    # Sources assembled sucessfully
    message(STATUS "   ${N_FILES} files added to library.")

    # Create library
    add_library(${_libname} SHARED ${_SOURCES})

    # Add the directories of all header files to the include path
    set (_INC_DIRS "")
    foreach(_SOURCE ${_SOURCES})
        get_filename_component(_SOURCE_EXT ${_SOURCE} EXT)
        if((${_SOURCE_EXT} STREQUAL ".h") OR (${_SOURCE_EXT} STREQUAL ".hh"))
            get_filename_component(_SOURCE_DIR ${_SOURCE} DIRECTORY)
            list(APPEND _INC_DIRS ${_SOURCE_DIR})
        endif((${_SOURCE_EXT} STREQUAL ".h") OR (${_SOURCE_EXT} STREQUAL ".hh"))
    endforeach(_SOURCE in ${_SOURCES})
    list(REMOVE_DUPLICATES _INC_DIRS)
    target_include_directories(${_libname} PUBLIC ${INC_DIRS_ALL})
    list(LENGTH _INC_DIRS N_INC)
    message(STATUS "   ${N_INC} directories added to library's include list.")

    # Finished initializing library
    message(STATUS "Finished initializing ${_libname}.")
endforeach (i_lib RANGE ${n_lib})
message(STATUS "")

