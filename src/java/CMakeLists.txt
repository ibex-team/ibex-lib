option (BUILD_JAVA_INTERFACE "Set to ON to build Java Interface" OFF)
set (JAVA_PACKAGE ibex CACHE STRING "Name of Ibex Java package (if enabled)")

if (BUILD_JAVA_INTERFACE)
  message (STATUS "Building the Java Interface: yes")
  # We need shared lib for Java
  if (NOT BUILD_SHARED_LIBS)
    message (FATAL_ERROR "Building Java Interface requires building Ibex as a "
                          "shared library: '-DBUILD_SHARED_LIBS=ON' needs"
                          " to be added to CMake command-line arguments")
  endif ()
  # Looking for Java
  find_package (Java COMPONENTS Development REQUIRED)
  find_package (JNI REQUIRED)

  # Compute JAVA_SIGNATURE from JAVA_PACKAGE
  string (REPLACE "." "_" JAVA_SIGNATURE ${JAVA_PACKAGE})

  message (STATUS "Java package name: " ${JAVA_PACKAGE})
  
  # Perform replacement in *.in files
  configure_file (ibex_Java.cpp.in ibex_Java.cpp @ONLY)
  configure_file (Ibex.java.in Ibex.java @ONLY)

  # Load CMake's tools for Java
  include (UseJava)

  # Build the jar
  add_jar (ibex-java-jar SOURCES ${CMAKE_CURRENT_BINARY_DIR}/Ibex.java
                          OUTPUT_NAME ${JAVA_PACKAGE}
                          GENERATE_NATIVE_HEADERS ibex-java-native)

  message(STATUS "Java version: " ${Java_VERSION})

  # Build the ibex-java lib
  add_library (ibex-java ${CMAKE_CURRENT_BINARY_DIR}/ibex_Java.cpp)
  
  ## Next command fails on Appveyor with error message "INTERFACE_LIBRARY targets may only have whitelisted properties. The property "NATIVE_HEADERS_DIRECTORY" is not allowed"
  #   target_include_directories(ibex-java PUBLIC $<TARGET_PROPERTY:ibex-java-native,NATIVE_HEADERS_DIRECTORY>)
  ## so we use instead explicit folder:
  target_include_directories(ibex-java PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/ibex-java-jar.dir/native_headers")
  ## ------------
  
  target_include_directories (ibex-java PUBLIC "${JNI_INCLUDE_DIRS}")
  target_link_libraries (ibex-java ibex)

  # Installation
  install_jar (ibex-java-jar DESTINATION ${CMAKE_INSTALL_JARDIR})
  install (TARGETS ibex-java DESTINATION ${CMAKE_INSTALL_LIBDIR})
else ()
  message (STATUS "Building the Java Interface: no")
endif ()
