cmake_minimum_required(VERSION 3.0.2)
  project(my_project LANGUAGES CXX)

  find_package(IBEX REQUIRED)
  ibex_init_common() # IBEX should have installed this function
  message(STATUS "Found IBEX version ${IBEX_VERSION}")

  add_executable(${PROJECT_NAME} main.cpp)
  target_link_libraries(${PROJECT_NAME} Ibex::ibex)
