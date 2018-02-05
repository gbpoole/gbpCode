# Configure any external projects, submodules, etc.
macro(local_extern cur_dir )
    # gbpBuild provides the boilerplate for this project's development environment
    add_external_submodule( ${cur_dir} "gbpBuild" ${cur_dir}"/gbpBuild/README.md" )
endmacro()
