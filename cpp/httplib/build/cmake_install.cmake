# Install script for directory: /home/jgd/workplace/cpp-http-survey/cpp/httplib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_server" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_server")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_server"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_server")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable" TYPE EXECUTABLE FILES "/home/jgd/workplace/cpp-http-survey/executable/regular_httplib_server")
  if(EXISTS "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_server" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_server")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_server")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_client" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_client")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_client"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_client")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable" TYPE EXECUTABLE FILES "/home/jgd/workplace/cpp-http-survey/executable/regular_httplib_client")
  if(EXISTS "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_client" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_client")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/regular_httplib_client")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_server" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_server")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_server"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_server")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable" TYPE EXECUTABLE FILES "/home/jgd/workplace/cpp-http-survey/executable/concurrent_httplib_server")
  if(EXISTS "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_server" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_server")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_server")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_client" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_client")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_client"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_client")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable" TYPE EXECUTABLE FILES "/home/jgd/workplace/cpp-http-survey/executable/concurrent_httplib_client")
  if(EXISTS "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_client" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_client")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/jgd/workplace/cpp-http-survey/cpp/httplib/../../executable/concurrent_httplib_client")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/jgd/workplace/cpp-http-survey/cpp/httplib/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
