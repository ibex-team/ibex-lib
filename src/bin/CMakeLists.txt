add_executable (ibexsolve ibexsolve.cpp ibexsolve.h)
target_link_libraries (ibexsolve ibex)
install (TARGETS ibexsolve DESTINATION ${CMAKE_INSTALL_BINDIR}
                           COMPONENT binaries)
