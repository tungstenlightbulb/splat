cmake_minimum_required(VERSION 2.6)
project(splat)

if(WIN32)
  set(SHAREDIR ".")
  set(DOCDIR "doc")
elseif(APPLE)
  set(SHAREDIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}")
  set(DOCDIR "${PROJECT_NAME}")
  set(MANDIR "share/man")
elseif(UNIX) # Linux, BSD etc
  set(SHAREDIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}")
  set(DOCDIR "${CMAKE_INSTALL_PREFIX}/share/doc/${PROJECT_NAME}")
endif()


add_subdirectory(src)
add_subdirectory(doc)

install(DIRECTORY sample_data DESTINATION "${SHAREDIR}")
