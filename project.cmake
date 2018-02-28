# Define these outside any of any macros or functions
set(GBPBUILD_REL_PATH "extern/gbpBuild" )
set(GBP_PROJECT_NAME  "gbpSID")

# Set options and dependencies
macro(project_options_and_dependancies)
    #========= Set project-specific options ========
    # Use double precision liberally ?
    option(USE_DOUBLE "Use double liberally" OFF)
    if(USE_DOUBLE)
        add_definitions(-DUSE_DOUBLE)
    endif()
    
    option(USE_FFTW2 "Use v2 of FFTW" OFF)
    if(USE_FFTW2)
        add_definitions(-DUSE_FFTW2)
    endif()

    # Turn on debugging mode
    option(SID_DEBUG "Enable SID debugging information" OFF)
    
    #=========== Add 3rd-party libraries ===========
    # (look in gbpBuild/cmake/3rd_party.cmake for a list of supported libraries)
    message(STATUS "")
    message(STATUS "Initializing 3rd-party libraries...")

    # List of *required* 3rd-Party libraries
    set_3rd_party_required("GSL")
    
    # List of *requested* 3rd-Party libraries (and their defaults)
    set_3rd_party_requested("GBP_DOCS_BUILD" ON)
    
    # List of *optional* 3rd-Party libraries (and their defaults)
    set_3rd_party_optional("MPI"     ON)
    set_3rd_party_optional("MPI_IO"  OFF)
    set_3rd_party_optional("GD"      ON)
    if(USE_FFTW2)
       set_3rd_party_optional("FFTW2" ON)
    else()
       set_3rd_party_optional("FFTW3" ON)
    endif()      
    set_3rd_party_optional("CUDA"    OFF)
    set_3rd_party_optional("CFITSIO" ON)
    set_3rd_party_optional("HDF5"    ON)    
    
    # Print status message
    message(STATUS "Finished initializing 3rd-party libraries.")
endmacro()
