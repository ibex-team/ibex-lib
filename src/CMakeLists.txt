################################################################################
# libibex
################################################################################

# Generate ibex_Setting.h and add its path to the list of include directories
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Setting.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/ibex_Setting.h
)
list (APPEND IBEX_INCDIRS ${CMAKE_CURRENT_BINARY_DIR})
list (APPEND IBEX_SRC ${CMAKE_CURRENT_BINARY_DIR}/ibex_Setting.h)

# Recurse on all subdirectories (with 'operators' first)
set (SUBDIR_LIST operators arithmetic bisector combinatorial contractor data
                 function numeric parser predicate set solver strategy symbolic
                 system tools)

foreach (subdir ${SUBDIR_LIST})
  add_subdirectory (${subdir})
endforeach()

# Configure flex and bison for the parser
find_package(BISON)
find_package(FLEX)
FLEX_TARGET(Lexer
  ${CMAKE_CURRENT_SOURCE_DIR}/parser/lexer.l
  ${CMAKE_CURRENT_BINARY_DIR}/parser/lexer.lex.cc
  COMPILE_FLAGS "-Pibex")
list (APPEND IBEX_SRC ${FLEX_Lexer_OUTPUTS})
BISON_TARGET(Parser
  ${CMAKE_CURRENT_SOURCE_DIR}/parser/parser.yc
  ${CMAKE_CURRENT_BINARY_DIR}/parser/parser.tab.cc
  COMPILE_FLAGS "--name-prefix=ibex --report=all --file-prefix=parser")
list (APPEND IBEX_SRC ${BISON_Parser_OUTPUTS})

ADD_FLEX_BISON_DEPENDENCY (Lexer Parser)
list (APPEND IBEX_INCDIRS ${CMAKE_CURRENT_BINARY_DIR}/parser)

# Create the target for libibex
add_library (ibex ${IBEX_SRC})
target_include_directories(ibex PUBLIC "$<BUILD_INTERFACE:${IBEX_INCDIRS}>")
target_include_directories(ibex PUBLIC "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
target_include_directories(ibex PUBLIC "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}/ibex>")
target_link_libraries (ibex PUBLIC ${INTERVAL_LIB_TARGET} ${LP_LIB_TARGET})

################################################################################
# ibex.h
################################################################################
# Get list of header and inl files
list_filter_header (IBEX_HDR ${IBEX_SRC})
list_filter_inl (IBEX_INL ${IBEX_SRC})

# remove parser.h from IBEX_HDR (internal header, should not be distributed)
list (REMOVE_ITEM IBEX_HDR ${CMAKE_CURRENT_SOURCE_DIR}/parser/parser.h)

# Generate ibex.h
set (IBEX_MAIN_HEADER ${CMAKE_CURRENT_BINARY_DIR}/ibex.h)
generate_main_header (${IBEX_MAIN_HEADER} "ibex_Setting.h" ${IBEX_HDR}
                                                        WITH_GUARD "__IBEX_H__")

################################################################################
# installation
################################################################################
install (TARGETS ibex EXPORT ibexExport DESTINATION ${CMAKE_INSTALL_LIBDIR}
                                                            COMPONENT devtools)
install (FILES ${IBEX_MAIN_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
                                                            COMPONENT devtools)
install (FILES ${IBEX_HDR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ibex
                                                            COMPONENT devtools)
install (FILES ${IBEX_OPS_HDR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ibex
                                                            COMPONENT devtools)
install (FILES ${IBEX_INL} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ibex
                                                            COMPONENT devtools)

# export ibex target in ibex-config.cmake
install (EXPORT ibexExport FILE ibex-config.cmake
                           NAMESPACE Ibex::
                           DESTINATION ${CMAKE_INSTALL_CONFIGCMAKE}
                           COMPONENT devtools)

# export companion file for version
include (CMakePackageConfigHelpers)
set (VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/ibex-config-version.cmake")
write_basic_package_version_file (${VERSION_FILE} COMPATIBILITY AnyNewerVersion)
install (FILES ${VERSION_FILE} DESTINATION ${CMAKE_INSTALL_CONFIGCMAKE}
                                                            COMPONENT devtools)

################################################################################
# binaries
################################################################################
add_subdirectory (bin)
