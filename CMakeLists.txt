project( swun )
cmake_minimum_required( VERSION 2.6 )

set( VERSION        0.0.2 )
set( TARGET         swun )
set( SRCS           src/main.cpp src/MainWindow.cpp src/SlackPKG.cpp )
set( MOC_HEADERS    src/MainWindow.h )
set( RESOURCES      res/swun.qrc )

#qt stuff
find_package(Qt4 4.7.0 COMPONENTS QtCore QtGui REQUIRED )
QT4_ADD_RESOURCES( RES_SRCS ${RESOURCES})
QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADERS} )

include( ${QT_USE_FILE} )
add_definitions( -DSWUN_VERSION="${VERSION}" )


#build
add_executable( ${TARGET} ${SRCS} ${MOC_SRCS} ${RES_SRCS})
target_link_libraries( ${TARGET} ${QT_LIBRARIES} )


#install
set( BIN_PERMS OWNER_EXECUTE OWNER_WRITE OWNER_READ 
               GROUP_EXECUTE GROUP_READ 
               WORLD_EXECUTE WORLD_READ )
install( TARGETS ${TARGET} DESTINATION bin 
                           PERMISSIONS ${BIN_PERMS}
                           CONFIGURATIONS Release )
install( PROGRAMS swun.sh DESTINATION bin 
                       CONFIGURATIONS Release )
install( FILES LICENSE.txt README DESTINATION doc/${TARGET}-${VERSION} )
install( FILES swun.desktop DESTINATION share/applications )
install( FILES man/swun.1 DESTINATION man/man1 )


#help stuff
add_custom_target(changelog COMMAND git --no-pager log --format='%ai %an <%ae> %n%n%x09* %s%d%n' > ChangeLog WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
