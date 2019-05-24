# Options
set (SOPLEX_DIR "" CACHE PATH "Path to the Soplex lib and include directories")

# Looking for soplex.h
set (SOPLEX_HDR "soplex.h")
set (MSG "Looking for ${SOPLEX_HDR}")
message (STATUS "${MSG}")
find_path (soplex_INCLUDE_DIR ${SOPLEX_HDR} HINTS ${SOPLEX_DIR}
           PATH_SUFFIXES include
           DOC "Set to exact include directory to bypass internal test")
if (soplex_INCLUDE_DIR)
  message (STATUS "${MSG} -- found at ${soplex_INCLUDE_DIR}")
else()
  message (FATAL_ERROR "${MSG} -- not found")
endif()

# Looking for libsoplex
set (SOPLEX_LIB "soplex")
set (MSG "Looking for ${SOPLEX_LIB}")
message (STATUS "${MSG}")
find_library (soplex_LIBRARY NAMES ${SOPLEX_LIB} HINTS ${SOPLEX_DIR}
              PATH_SUFFIXES lib
              DOC "Set to exact lib directory to bypass internal test")
if (soplex_LIBRARY)
  message (STATUS "${MSG} -- found at ${soplex_LIBRARY}")
else()
  message (FATAL_ERROR "${MSG} -- not found")
endif()

#
mark_as_advanced (soplex_INCLUDE_DIR soplex_LIBRARY)

# Create an interface imported target
add_library (soplex INTERFACE IMPORTED GLOBAL)
set_target_properties (soplex PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${soplex_INCLUDE_DIR}
    INTERFACE_LINK_LIBRARIES ${soplex_LIBRARY}
  )
