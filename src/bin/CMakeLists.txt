add_executable (ibexsolve ibexsolve.cpp ibexsolve.h)
target_link_libraries (ibexsolve ibex)
set_target_properties (ibexsolve PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                      "${CMAKE_BINARY_DIR}/bin")
install (TARGETS ibexsolve DESTINATION ${CMAKE_INSTALL_BINDIR}
                           COMPONENT binaries)
