set(QMLPKG org/freedesktop/contextkit)

#-Wno-psabi is to remove next g++ warning/note:
#the mangling of 'va_list' has changed in GCC 4.4
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -fPIC -W -Wall -Wextra -Werror -Wno-psabi -g -O2 -std=c++0x")


if(${USEQT} STREQUAL "4")

set(NAME contextkit)

find_package(Qt4 4.7.0 REQUIRED)
include(${QT_USE_FILE})
add_definitions(-DQT_SHARED)
include_directories(${QT_INCLUDE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(STATEFS contextkit-statefs-qt4 REQUIRED)
pkg_check_modules(CONTEXTSUBSCRIBER contextsubscriber-1.0)

include_directories(
  ${QT_INCLUDE_DIR}
  ${QT_QTDECLARATIVE_INCLUDE_DIR}
  ${CONTEXTSUBSCRIBER_INCLUDE_DIRS}
)

link_directories(
  ${QT_LIBRARY_DIR}
  ${CONTEXTSUBSCRIBER_LIBRARY_DIRS}
)

qt4_wrap_cpp(MOC_SRC plugin.cpp ck_property.hpp)
set(SRC plugin.cpp ck_property.cpp)
add_library(${NAME} MODULE ${SRC} ${MOC_SRC})

target_link_libraries(${NAME}
  ${QT_QTCORE_LIBRARY}
  ${QT_DECLARATIVE_LIBRARY}
  ${STATEFS_LIBRARIES}
  )

install(TARGETS ${NAME} DESTINATION lib/qt4/imports/${QMLPKG})
install(FILES qmldir DESTINATION lib/qt4/imports/${QMLPKG})

elseif(${USEQT} STREQUAL "5")

set(NAME contextkit)

if(Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(STATEFS contextkit-statefs REQUIRED)

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)

include_directories(
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Qml_INCLUDE_DIRS}
)
add_definitions(
  ${Qt5Core_DEFINITIONS}
  ${Qt5Qml_DEFINITIONS}
)
link_directories(
  ${Qt5Core_LIBRARY_DIRS}
  ${Qt5Qml_LIBRARY_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STATEFS_CFLAGS}")

set(SRC plugin-qt5.cpp ck_property.cpp)
qt5_wrap_cpp(MOC_SRC plugin-qt5.hpp ck_property.hpp)
add_library(${NAME} MODULE ${SRC} ${MOC_SRC})

target_link_libraries(${NAME}
  ${Qt5Core_LIBRARIES}
  ${Qt5Qml_LIBRARIES}
  ${STATEFS_LIBRARIES}
  )

install(PROGRAMS libcontextkit.so DESTINATION lib/qt5/qml/${QMLPKG})
install(FILES qmldir DESTINATION lib/qt5/qml/${QMLPKG})

endif(${USEQT} STREQUAL "4")
