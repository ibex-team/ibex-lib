# Ipopt loup finder, ported from github.com/bneveu/ibex-lib (branch qibex, 045ea97).
# Optional: without Ipopt the library builds exactly as before, and the modes that
# need it are simply absent from ibexopt-ml.

if (NOT IBEX_WITH_IPOPT)
  return ()
endif ()

# Only the .cpp files go into target_sources. The generated umbrella header
# ibex.h is built from the headers listed there, and these two pull in
# <coin-or/IpTNLP.hpp>, which brings a "using namespace Ipopt" and an
# Ipopt::Vector that collides with ibex::Vector in every file that does
# "using namespace ibex". An optional component with a heavy external
# dependency has no business in the umbrella header: include it explicitly.
target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderIpopt.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderDefaultIpopt.cpp
)

install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderIpopt.h
               ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderDefaultIpopt.h
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ibex COMPONENT devtools)

target_include_directories (ibex PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
  ${IPOPT_INCLUDE_DIRS})

target_compile_definitions (ibex PUBLIC __IBEX_WITH_IPOPT__)

# The ibex target is defined in the parent directory, and linking a target from
# elsewhere needs CMP0079 (CMake >= 3.13). Setting it here rather than raising
# the project-wide minimum keeps the rest of the build on the policies it was
# written for.
if (POLICY CMP0079)
  cmake_policy (SET CMP0079 NEW)
  target_link_libraries (ibex PUBLIC ${IPOPT_LIBRARIES})
  target_link_directories (ibex PUBLIC ${IPOPT_LIBRARY_DIRS})
else ()
  message (FATAL_ERROR "IBEX_WITH_IPOPT needs CMake >= 3.13 (policy CMP0079)")
endif ()
