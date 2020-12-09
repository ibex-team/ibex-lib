install (FILES "${MODULE_DIR}/ibex-config-utils.cmake" ${EXPORTFILES}
          DESTINATION ${CMAKE_INSTALL_CONFIGCMAKE} COMPONENT devtools)
# install some CMake files that can be useful for plugins
install (FILES "${MODULE_DIR}/FindCppUnit.cmake"
          DESTINATION ${CMAKE_INSTALL_CONFIGCMAKE}/cmake.utils
          COMPONENT devtools)
