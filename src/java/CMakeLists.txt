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
                          OUTPUT_NAME ${JAVA_PACKAGE})

  # Generate header file
  #set (Java_JAVAH_EXECUTABLE ${Java_JAVAC_EXECUTABLE})
  if (Java_VERSION VERSION_LESS "10")
    create_javah (TARGET ${JAVA_SIGNATURE}_Ibex.h
      CLASSES ${JAVA_PACKAGE}.Ibex
      DEPENDS ibex-java-jar)
  else ()
    add_custom_command (OUTPUT ${JAVA_SIGNATURE}_Ibex.h
                        COMMAND "${Java_JAVAC_EXECUTABLE}" Ibex.java -h .
                        DEPENDS ibex-java-jar
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "Building C header files from classes...")
    add_custom_target (ibex-java-header ALL
                  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${JAVA_SIGNATURE}_Ibex.h)
  endif ()

  # Build the ibex-java lib
  add_library (ibex-java ${CMAKE_CURRENT_BINARY_DIR}/ibex_Java.cpp
                         ${CMAKE_CURRENT_BINARY_DIR}/${JAVA_SIGNATURE}_Ibex.h)
  target_include_directories (ibex-java PUBLIC "${JNI_INCLUDE_DIRS}")
  target_link_libraries (ibex-java ibex)

  # Installation
  install_jar (ibex-java-jar DESTINATION ${CMAKE_INSTALL_JARDIR})
  install (TARGETS ibex-java DESTINATION ${CMAKE_INSTALL_LIBDIR})
else ()
  message (STATUS "Building the Java Interface: no")
endif ()
