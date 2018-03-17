# Set default compilers (must come before project line)
# By checking ENV, we can alter compiler versions in CI
# environments (for example)
macro(init_compilers)
    message(STATUS "")
    message(STATUS "--- Configuring compiler ---")
    message(STATUS "")    
    if(DEFINED ENV{CXX} AND DEFINED ENV{CC})
        set(CMAKE_C_COMPILER $ENV{CC})
        set(CMAKE_CXX_COMPILER $ENV{CXX})
        message(STATUS "C   compiler set from environment: ${CMAKE_C_COMPILER}")
        message(STATUS "C++ compiler set from environment: ${CMAKE_CXX_COMPILER}")
    else()
        set(CMAKE_C_COMPILER   "clang")
        set(CMAKE_CXX_COMPILER "clang++")
        message(STATUS "C   compiler set from default: ${CMAKE_C_COMPILER}")
        message(STATUS "C++ compiler set from default: ${CMAKE_CXX_COMPILER}")
    endif()
endmacro(init_compilers)

# Set compiler flags
macro(set_compiler_flags)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu99 -Wextra")
    set(CMAKE_CXX_STANDARD 11)
endmacro(set_compiler_flags)

# Check for minimum versions of compilers to avoid ABI conflicts
macro(validate_compilers)
    # -- C --
    message(STATUS "Checking c compiler version:")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(VERSION_MIN "5.0.0")
        if(CMAKE_C_COMPILER_VERSION VERSION_LESS ${VERSION_MIN})
            message(FATAL_ERROR "Insufficient gcc version (${CMAKE_C_COMPILER_VERSION}<${VERSION_MIN})")
        endif()
    elseif((CMAKE_C_COMPILER_ID STREQUAL "Clang") OR (CMAKE_C_COMPILER_ID STREQUAL "AppleClang"))
        set(VERSION_MIN "3.9.0")
        if(CMAKE_C_COMPILER_VERSION VERSION_LESS ${VERSION_MIN})
            message(FATAL_ERROR "Insufficient Clang version (${CMAKE_C_COMPILER_VERSION}<${VERSION_MIN})")
        endif()
    else()
        message(FATAL_ERROR "Unsupported compiler or platform (${CMAKE_CXX_COMPILER_ID})")
    endif()
    message(STATUS "   ID     : ${CMAKE_C_COMPILER_ID}")
    message(STATUS "   VERSION: ${CMAKE_C_COMPILER_VERSION}")
    # -- C++ --
    message(STATUS "Checking c++ compiler version:")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(VERSION_MIN "5.0.0")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${VERSION_MIN})
            message(FATAL_ERROR "Insufficient gcc version (${CMAKE_CXX_COMPILER_VERSION}<${VERSION_MIN})")
        endif()
    elseif((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
        set(VERSION_MIN "3.9.0")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${VERSION_MIN})
            message(FATAL_ERROR "Insufficient Clang version (${CMAKE_CXX_COMPILER_VERSION}<${VERSION_MIN})")
        endif()
    else()
        message(FATAL_ERROR "Unsupported compiler or platform (${CMAKE_CXX_COMPILER_ID})")
    endif()
    message(STATUS "   ID     : ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "   VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
    message(STATUS "")
    message(STATUS "--- Finished configuring compiler ---")
    message(STATUS "")
endmacro(validate_compilers)
