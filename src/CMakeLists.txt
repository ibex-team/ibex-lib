################################################################################
# libibex
################################################################################
add_library (ibex "")

target_include_directories (ibex PUBLIC "$<BUILD_INTERFACE:${IBEX_INCDIRS}>")

target_link_libraries (ibex PUBLIC ${INTERVAL_LIB_TARGET} ${LP_LIB_TARGET})
if (WIN32)
  # We (may) need this for strdup under Windows (see issue #287)
  target_compile_options (ibex PUBLIC "-U__STRICT_ANSI__")
endif ()

################################################################################
# ibex_Setting.h
################################################################################
configure_file (ibex_Setting.h.in ibex_Setting.h)
target_sources (ibex PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/ibex_Setting.h)
target_include_directories (ibex PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

################################################################################
# Recurse on all subdirectories to add source files
################################################################################
# source files from interval_lib and lp_lib wrappers
target_sources (ibex PRIVATE ${IBEX_SRC})

# Note: operators subdir is handled by CMakeLists.txt from symbolic
foreach (subdir arithmetic bisector cell combinatorial contractor data function
                loup numeric optim parser predicate set solver strategy symbolic system
                tools)
  add_subdirectory (${subdir})
endforeach()

################################################################################
# Parser with Flex and Bison
################################################################################
# Note: we handle the parser here as target_sources does not handle well
# generated files in subdirectory

# FLEX
set (FLEX_FLAGS "-Pibex")
if (WIN32)
  find_package (FLEX 2.5.6) # We need version >= 2.5.6 to have --nounistd
  string (APPEND FLEX_FLAGS " --nounistd")
else ()
  find_package (FLEX)
endif ()
if (NOT FLEX_FOUND)
  message (FATAL_ERROR "Flex is needed to build the parser")
endif ()
flex_target (Flex parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/parser/lexer.lex.cc
                                                  COMPILE_FLAGS "${FLEX_FLAGS}")

# BISON
find_package (BISON)
if (NOT BISON_FOUND)
  message (FATAL_ERROR "Flex is needed to build the parser")
endif ()
bison_target (Parser parser/parser.yc
                  ${CMAKE_CURRENT_BINARY_DIR}/parser/parser.tab.cc
                  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser/parser.tab.hh
                  COMPILE_FLAGS "--name-prefix=ibex --report=all")
add_flex_bison_dependency (Flex Parser)

target_sources (ibex PRIVATE ${FLEX_Flex_OUTPUTS} ${BISON_Parser_OUTPUTS})
target_include_directories (ibex PUBLIC
                        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/parser>")

################################################################################
# ibex.h and ibex_Setting.h
################################################################################
# Get list of headers
get_target_property (IBEX_SRC ibex SOURCES)
list_filter (IBEX_HDR "\\.(h|hpp)$" ${IBEX_SRC})
list_filter (IBEX_OPS_HDR "operators/.*\\.h" ${IBEX_SRC})
if (IBEX_OPS_HDR)
  list (REMOVE_ITEM IBEX_HDR ${IBEX_OPS_HDR}) # remove operators headers
endif ()

# Generate ibex.h
set (IBEX_MAIN_HEADER ${CMAKE_CURRENT_BINARY_DIR}/ibex.h)
generate_main_header (${IBEX_MAIN_HEADER} ${IBEX_HDR} WITH_GUARD "__IBEX_H__")

################################################################################
# installation
################################################################################
target_include_directories (ibex PUBLIC
                        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
                        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ibex>")

install (TARGETS ibex EXPORT ibexExport DESTINATION ${CMAKE_INSTALL_LIBDIR}
                                                            COMPONENT devtools)
install (FILES ${IBEX_MAIN_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
                                                            COMPONENT devtools)
list_filter (IBEX_SRC_INSTALLABLE "\\.(h|hpp|inl)$" ${IBEX_SRC})
install (FILES ${IBEX_SRC_INSTALLABLE}
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ibex COMPONENT devtools)

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
