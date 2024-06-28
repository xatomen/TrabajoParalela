# Install script for directory: /home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX

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
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenXLSX/headers" TYPE FILE FILES "/home/jorge/Escritorio/Proyectos/TrabajoParalela/cmake-build-debug/OpenXLSX-0.3.2/OpenXLSX/OpenXLSX-Exports.hpp")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenXLSX/headers" TYPE FILE FILES
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLCell.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLCellIterator.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLCellRange.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLCellReference.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLCellValue.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLColor.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLColumn.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLCommandQuery.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLConstants.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLContentTypes.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLDateTime.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLDocument.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLException.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLFormula.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLIterator.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLProperties.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLRelationships.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLRow.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLRowData.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLSharedStrings.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLSheet.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLWorkbook.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLXmlData.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLXmlFile.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLXmlParser.hpp"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/headers/XLZipArchive.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenXLSX" TYPE FILE FILES "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/OpenXLSX.hpp")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "lib" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/jorge/Escritorio/Proyectos/TrabajoParalela/cmake-build-debug/output/libOpenXLSXd.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX" TYPE FILE FILES
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/OpenXLSX-0.3.2/OpenXLSX/OpenXLSXConfig.cmake"
    "/home/jorge/Escritorio/Proyectos/TrabajoParalela/cmake-build-debug/OpenXLSX-0.3.2/OpenXLSX/OpenXLSX/OpenXLSXConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX/OpenXLSXTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX/OpenXLSXTargets.cmake"
         "/home/jorge/Escritorio/Proyectos/TrabajoParalela/cmake-build-debug/OpenXLSX-0.3.2/OpenXLSX/CMakeFiles/Export/c72cc94553a1a0c9b05f75dae42fb1d7/OpenXLSXTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX/OpenXLSXTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX/OpenXLSXTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX" TYPE FILE FILES "/home/jorge/Escritorio/Proyectos/TrabajoParalela/cmake-build-debug/OpenXLSX-0.3.2/OpenXLSX/CMakeFiles/Export/c72cc94553a1a0c9b05f75dae42fb1d7/OpenXLSXTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX" TYPE FILE FILES "/home/jorge/Escritorio/Proyectos/TrabajoParalela/cmake-build-debug/OpenXLSX-0.3.2/OpenXLSX/CMakeFiles/Export/c72cc94553a1a0c9b05f75dae42fb1d7/OpenXLSXTargets-debug.cmake")
  endif()
endif()
