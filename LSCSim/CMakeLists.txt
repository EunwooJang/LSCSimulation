set(PROJECT_NAME "LSCSim")

project(${PROJECT_NAME})
message(STATUS "cmaking ${PROJECT_NAME} ...")

# ----------------------------------------------------------------------------
# Locate sources and headers for this project
# ----------------------------------------------------------------------------
file(GLOB ${PROJECT_NAME}_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB ${PROJECT_NAME}_HEADERS ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.hh)

file(MAKE_DIRECTORY ${PROJECT_INCLUDE_DIR}/${PROJECT_NAME})

foreach(HEADER_i ${${PROJECT_NAME}_HEADERS})
  configure_file(${HEADER_i} ${PROJECT_INCLUDE_DIR}/${PROJECT_NAME} COPYONLY)
endforeach(HEADER_i)

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${PROJECT_NAME} GLG4Sim MCObjs ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

install(PROGRAMS
  ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.so
  DESTINATION
  ${CMAKE_INSTALL_LIBDIR}
)

# ----------------------------------------------------------------------------
# build executables
# ----------------------------------------------------------------------------
set(LINK_PACKAGES MCObjs GLG4Sim LSCSim)
set(LINK_EXTERNALS ${ROOT_LIBRARIES} ${Geant4_LIBRARIES})

file(GLOB ${PROJECT_NAME}_EXECUTABLES ${PROJECT_SOURCE_DIR}/test/*.cc)

# 1. lscsim.cc → main 실행파일
add_executable(main ${PROJECT_SOURCE_DIR}/test/lscsim.cc)
target_link_libraries(main LSCSim ${LINK_PACKAGES} ${LINK_EXTERNALS})
install(TARGETS main DESTINATION ${CMAKE_INSTALL_BINDIR})

# 2. different.cc → main2 실행파일
add_executable(main2 ${PROJECT_SOURCE_DIR}/test/different.cc)
target_link_libraries(main2 LSCSim ${LINK_PACKAGES} ${LINK_EXTERNALS})
install(TARGETS main2 DESTINATION ${CMAKE_INSTALL_BINDIR})


install(DIRECTORY ${PROJECT_SOURCE_DIR}/mac DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/data DESTINATION ${CMAKE_INSTALL_PREFIX})
