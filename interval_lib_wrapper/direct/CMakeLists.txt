# TODO	Logs.warn ("Using interval library direct may not give exact results")

# We need to add flags "-frounding-math" and "-ffloat-store"
# It is necessary to avoid problem with x80 processor.
foreach (flag "-frounding-math" "-ffloat-store")
  string (TOUPPER "${flag}" upperflag)
  string (REPLACE "-" "_" upperflag "${upperflag}")
  check_cxx_compiler_flag (${flag} COMPILER_SUPPORTS${upperflag})
  if (COMPILER_SUPPORTS${upperflag})
    list (APPEND DIRECT_FLAGS ${flag})
  endif()
endforeach ()

if (DIRECT_FLAGS)
  set (direct_compile_option COMPILE_OPTIONS ${DIRECT_FLAGS})
endif ()

create_target_import_and_export (direct "IGNORE" DIRECT_EXPORTFILE
                                        ${direct_compile_option}
                                        NAMESPACE Ibex::)

list (APPEND EXPORTFILES "${DIRECT_EXPORTFILE}")
set (EXPORTFILES "${EXPORTFILES}" PARENT_SCOPE)
