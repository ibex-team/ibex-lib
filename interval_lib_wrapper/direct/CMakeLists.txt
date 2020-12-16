set (DIRECT_NAME "direct" PARENT_SCOPE)
set (DIRECT_TARGET "direct" PARENT_SCOPE)

add_library (direct INTERFACE)

message(WARNING "Using INTERVAL_LIB=${INTERVAL_LIB} may not give exact results")

# We need to add flags "-frounding-math" and "-ffloat-store"
# It is necessary to avoid problem with x80 processor.
foreach (flag "-frounding-math" "-ffloat-store")
  string (TOUPPER "${flag}" upperflag)
  string (REPLACE "-" "_" upperflag "${upperflag}")
  check_cxx_compiler_flag (${flag} COMPILER_SUPPORTS${upperflag})
  if (COMPILER_SUPPORTS${upperflag})
    list (APPEND _flags ${flag})
  endif()
endforeach ()

set_target_properties (direct PROPERTIES INTERFACE_COMPILE_OPTIONS "${_flags}")
install (TARGETS direct EXPORT ibexExport COMPONENT devtools)
