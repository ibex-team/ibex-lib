# Options
set (FILIB_DIR "" CACHE PATH "Path to the Filib lib and include directories")

# Looking for filib.h
set (FILIB_HDR "interval/interval.hpp")
set (MSG "Looking for ${FILIB_HDR}")
message (STATUS "${MSG}")
find_path (filib_INCLUDE_DIR ${FILIB_HDR} HINTS ${FILIB_DIR}
           PATH_SUFFIXES include
           DOC "Set to exact include directory to bypass internal test")
if (filib_INCLUDE_DIR)
  message (STATUS "${MSG} -- found at ${filib_INCLUDE_DIR}")
else()
  message (FATAL_ERROR "${MSG} -- not found")
endif()

# Looking for libprim
set (FILIB_LIB "prim")
set (MSG "Looking for ${FILIB_LIB}")
message (STATUS "${MSG}")
find_library (filib_LIBRARY NAMES ${FILIB_LIB} HINTS ${FILIB_DIR}
              PATH_SUFFIXES lib
              DOC "Set to exact lib directory to bypass internal test")
if (filib_LIBRARY)
  message (STATUS "${MSG} -- found at ${filib_LIBRARY}")
else()
  message (FATAL_ERROR "${MSG} -- not found")
endif()

# 
mark_as_advanced (filib_INCLUDE_DIR filib_LIBRARY)

# Create an interface imported target
add_library (filib INTERFACE IMPORTED GLOBAL)
set_target_properties (filib PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${filib_INCLUDE_DIR}
    INTERFACE_LINK_LIBRARIES ${filib_LIBRARY}
  )

# With Filib, we need to add flags "-frounding-math" and "-ffloat-store"
# It is necessary to use filib, to avoid problem with x80 processor.
# This option prevents undesirable excess precision on machines such as the
# 68000 where the floating registers (of the 68881) keep more precision than a
# "double" is supposed to have. Similarly for the x86 architecture. For most
# programs, the excess precision does only good, but a few programs rely on the
# precise definition of IEEE floating point. Use -ffloat-store for such
# programs, after modifying them to store all pertinent intermediate
# computations into variables.
foreach (flag "-frounding-math" "-ffloat-store")
  CHECK_CXX_COMPILER_FLAG (${flag} COMPILER_SUPPORTS_${flag})
  if (COMPILER_SUPPORTS_${flag})
    get_target_property (OPT filib INTERFACE_COMPILE_OPTIONS)
    if (OPT)
      list (APPEND OPT ${flag})
    else ()
      set (OPT ${flag})
    endif ()
    set_target_properties (filib PROPERTIES INTERFACE_COMPILE_OPTIONS "${OPT}")
  else ()
    message (WARNING "The compiler does not the flag ${flag} needed by Filib. "
                     "You may get incorrect/inconsistent results.")
  endif()
endforeach ()
