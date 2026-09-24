add_executable (ibexsolve ibexsolve.cpp parse_args.h)
add_executable (ibexopt ibexopt.cpp parse_args.h)
add_executable (ibexopt-ml ibexopt-ml.cpp parse_args.h)
target_link_libraries (ibexsolve ibex)
target_link_libraries (ibexopt ibex)
target_link_libraries (ibexopt-ml ibex)
set_target_properties (ibexsolve PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                      "${CMAKE_BINARY_DIR}/bin")
set_target_properties (ibexopt PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                      "${CMAKE_BINARY_DIR}/bin")
set_target_properties (ibexopt-ml PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                      "${CMAKE_BINARY_DIR}/bin")
install (TARGETS ibexsolve DESTINATION ${CMAKE_INSTALL_BINDIR}
                           COMPONENT binaries)
install (TARGETS ibexopt DESTINATION ${CMAKE_INSTALL_BINDIR}
                           COMPONENT binaries)
install (TARGETS ibexopt-ml DESTINATION ${CMAKE_INSTALL_BINDIR}
                           COMPONENT binaries)

# The reference Ipopt strategy (see the header of the file). It only exists when
# the Ipopt loup finder is compiled in.
if (IBEX_WITH_IPOPT)
  add_executable (ibexopt-ipopt ibexopt-ipopt.cpp)
  target_link_libraries (ibexopt-ipopt ibex)
  set_target_properties (ibexopt-ipopt PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                        "${CMAKE_BINARY_DIR}/bin")
  install (TARGETS ibexopt-ipopt DESTINATION ${CMAKE_INSTALL_BINDIR}
                             COMPONENT binaries)
endif ()
