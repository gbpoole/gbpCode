# Configure any external projects, submodules, etc.
macro(local_extern cur_dir )
    # gbpBuild provides the development environment for this project
    add_external_submodule( ${cur_dir} "gbpBuild" ${cur_dir}"/gbpBuild/README.md" )
endmacro()
