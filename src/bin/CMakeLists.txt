add_executable (ibexsolve ibexsolve.cpp ibexsolve.h)
add_executable (ibexopt ibexopt.cpp ibexopt.h)
target_link_libraries (ibexsolve ibex)
target_link_libraries (ibexopt ibex)
set_target_properties (ibexsolve PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                      "${CMAKE_BINARY_DIR}/bin")
set_target_properties (ibexopt PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                      "${CMAKE_BINARY_DIR}/bin")
install (TARGETS ibexsolve DESTINATION ${CMAKE_INSTALL_BINDIR}
                           COMPONENT binaries)
install (TARGETS ibexopt DESTINATION ${CMAKE_INSTALL_BINDIR}
                           COMPONENT binaries)
