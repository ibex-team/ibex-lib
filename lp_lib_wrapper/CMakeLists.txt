include (ibex-install-3rd)

subdir_list (LP_LIB_LIST RELATIVE)

set(LP_LIB "none" CACHE STRING "Library used for linear programming")
set_property(CACHE LP_LIB PROPERTY STRINGS ${LP_LIB_LIST})

list(FIND LP_LIB_LIST ${LP_LIB} is_valid)
if (is_valid LESS 0)
  message (FATAL_ERROR "Unsupported library for linear programming: ${LP_LIB}")
else ()
  message (STATUS "Library for linear programming: ${LP_LIB}")
endif ()

# LP_LIB in upper case
string (TOUPPER "${LP_LIB}" LP_LIB_UPPER)

# Absolute path of wrapper directory
set (LP_LIB_ABSPATH ${CMAKE_CURRENT_SOURCE_DIR}/${LP_LIB})

# Go into wrapper directory
add_subdirectory (${LP_LIB})

# The proper name of the linear programming lib
set (LP_LIB_NAME ${${LP_LIB_UPPER}_NAME} PARENT_SCOPE)

# The name of the target that ibex should depend on
set (LP_LIB_TARGET ${${LP_LIB_UPPER}_TARGET} PARENT_SCOPE)

# The version (if available) of the linear programming lib
set (LP_LIB_VERSION ${${LP_LIB_UPPER}_VERSION} PARENT_SCOPE)

# Check that mandatory file exists and add it in IBEX_SRC
set (LP_LIB_REQUIRED_FILES ibex_LPLibWrapper.cpp ibex_LPLibWrapper.h)

set (PRE "Error with wrapper of linear programming ${LP_LIB}")
foreach (f ${LP_LIB_REQUIRED_FILES})
  if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LP_LIB}/${f})
    message (FATAL_ERROR "${PRE}, it should have a file named ${f}")
  else ()
    list (APPEND IBEX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${LP_LIB}/${f})
  endif ()
endforeach ()

# Add directory of wrapper in list of included directory
list (APPEND IBEX_INCDIRS ${LP_LIB_ABSPATH})

# Propagate new values of list to parent scope
set (IBEX_INCDIRS ${IBEX_INCDIRS} PARENT_SCOPE)
set (IBEX_SRC ${IBEX_SRC} PARENT_SCOPE)
