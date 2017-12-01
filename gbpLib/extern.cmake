macro (local_extern cur_dir )
    # Make sure we have updated the Clara submodule
    if(NOT EXISTS "${cur_dir}/gbpSID/local.cmake")
        message(STATUS "  -> Checking out gbpSID submodule")
        execute_process(COMMAND git submodule update --init WORKING_DIRECTORY ${cur_dir})
    else()
        message(STATUS "  -> gbpSID submodule has already been checked out")
    endif()
endmacro()
