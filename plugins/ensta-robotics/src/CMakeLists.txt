################################################################################
# libibex-robotics
################################################################################

list (APPEND SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcPixelMap.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcPixelMap.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcSegment.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtcSegment.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcInPolygon.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcInPolygon.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PixelMap.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PixelMap.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepPolygon.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepPolygon.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Tube.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Tube.h
  )

# Create the target for libibex-robotics
add_library (ibex-robotics ${SRC})
target_compile_options (ibex-robotics PUBLIC ${IBEX_CFLAGS})
target_include_directories (ibex-robotics PUBLIC ${IBEX_INCLUDE_DIRS})
target_link_libraries (ibex-robotics PUBLIC ${IBEX_LDFLAGS})

################################################################################
# installation
################################################################################
# Set HDR from SRC
foreach (srcfile ${SRC})
  if (srcfile MATCHES "\\.h$" OR srcfile MATCHES "\\.hpp$")
    list (APPEND HDR ${srcfile})
  endif ()
endforeach()

install (TARGETS ibex-robotics DESTINATION ${CMAKE_INSTALL_LIBDIR})
install (FILES ${HDR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ibex-robotics)
