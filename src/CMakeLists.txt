find_package(PkgConfig)
pkg_check_modules(STATEFS contextkit-statefs REQUIRED)

set(NAME contextkit)
set(QMLPKG org/freedesktop/contextkit)
set(SRC plugin.cpp ck_property.cpp)

set(HDRS ck_property.hpp)

qt4_wrap_cpp(MOC_SRC ${HDRS})

add_definitions(-DQT_SHARED)

add_library(${NAME} MODULE ${SRC} ${MOC_SRC})
target_link_libraries(${NAME}
  ${QT_QTCORE_LIBRARY}
  ${QT_DECLARATIVE_LIBRARY}
  ${STATEFS_LIBRARIES}
  )

install(TARGETS ${NAME} DESTINATION lib/qt4/imports/${QMLPKG})
install(FILES qmldir DESTINATION lib/qt4/imports/${QMLPKG})
