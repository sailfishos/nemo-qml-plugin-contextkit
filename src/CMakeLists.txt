set(QMLPKG org/freedesktop/contextkit)

if(${USEQT} STREQUAL "4")

set(NAME contextkit)

find_package(Qt4 4.7.0 REQUIRED)
include(${QT_USE_FILE})
add_definitions(-DQT_SHARED)
include_directories(${QT_INCLUDE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(STATEFS contextkit-statefs-qt4 REQUIRED)

include_directories(
  ${QT_INCLUDE_DIR}
  ${QT_QTDECLARATIVE_INCLUDE_DIR}
  ${CONTEXTSUBSCRIBER_INCLUDE_DIRS}
)

link_directories(
  ${QT_LIBRARY_DIR}
  ${CONTEXTSUBSCRIBER_LIBRARY_DIRS}
)

set(CMAKE_AUTOMOC TRUE)

#-Wno-psabi is to remove next g++ warning/note:
#the mangling of 'va_list' has changed in GCC 4.4
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -fPIC -W -Wall -Wextra -Werror -Wno-psabi -g -O2")

set(SRC plugin.cpp ck_property.cpp)
set(HDRS ck_property.hpp)
add_library(${NAME} MODULE ${SRC})

target_link_libraries(${NAME}
  ${QT_QTCORE_LIBRARY}
  ${QT_DECLARATIVE_LIBRARY}
  ${STATEFS_LIBRARIES}
  )

install(TARGETS ${NAME} DESTINATION lib/qt4/imports/${QMLPKG})
install(FILES qmldir DESTINATION lib/qt4/imports/${QMLPKG})

elseif(${USEQT} STREQUAL "5")

set(NAME contextkit-qt5)

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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS} ${Qt5Qml_EXECUTABLE_COMPILE_FLAGS}")

#-Wno-psabi is to remove next g++ warning/note:
#the mangling of 'va_list' has changed in GCC 4.4
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -fPIC -W -Wall -Wextra -Werror -Wno-psabi -g -O2")

set(SRC plugin-qt5.cpp ck_property.cpp)
set(HDRS ck_property.hpp)
add_library(${NAME} MODULE ${SRC})

target_link_libraries(${NAME}
  ${Qt5Core_LIBRARIES}
  ${Qt5Qml_LIBRARIES}
  ${STATEFS_LIBRARIES}
  )

install(PROGRAMS libcontextkit-qt5.so DESTINATION lib/qt5/imports/${QMLPKG} RENAME libcontextkit.so)
install(FILES qmldir DESTINATION lib/qt5/imports/${QMLPKG})

endif(${USEQT} STREQUAL "4")
