PROJECT(CONTEXTKIT_QML)
cmake_minimum_required(VERSION 2.8)

set(NAME contextkit)

find_package(Qt4 4.7.0 REQUIRED)
find_package(PkgConfig)

pkg_check_modules(CONTEXTSUBSCRIBER contextsubscriber-1.0)

include(${QT_USE_FILE})

#-Wno-psabi is to remove next g++ warning/note:
#the mangling of 'va_list' has changed in GCC 4.4
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -fPIC -W -Wall -Wextra -Werror -Wno-psabi -g -O2")

include_directories(
  ${QT_INCLUDE_DIR}
  ${QT_QTDECLARATIVE_INCLUDE_DIR}
  ${CONTEXTSUBSCRIBER_INCLUDE_DIRS}
)

link_directories(
  ${QT_LIBRARY_DIR}
  ${CONTEXTSUBSCRIBER_LIBRARY_DIRS}
)
