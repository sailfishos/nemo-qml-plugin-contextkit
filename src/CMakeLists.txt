set(QMLPKG org/freedesktop/contextkit)

set(NAME contextkit)

add_definitions(-DQT_SHARED)
set(CMAKE_AUTOMOC TRUE)

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

#-Wno-psabi is to remove next g++ warning/note:
#the mangling of 'va_list' has changed in GCC 4.4
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -fPIC -W -Wall -Wextra -Werror -Wno-psabi -g -O2")

set(SRC plugin-qt5.cpp ck_property.cpp)
add_library(${NAME} MODULE ${SRC} ${MOC_SRC})

target_link_libraries(${NAME}
  ${Qt5Core_LIBRARIES}
  ${Qt5Qml_LIBRARIES}
  ${STATEFS_LIBRARIES}
  )

install(PROGRAMS libcontextkit.so DESTINATION lib/qt5/qml/${QMLPKG})
install(FILES qmldir DESTINATION lib/qt5/qml/${QMLPKG})
