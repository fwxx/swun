project( swun )
cmake_minimum_required( VERSION 3.8 )

set( VERSION        0.0.4 )
set( TARGET         swun )
set( SRCS           src/main.cpp
                    src/MainWindow.h
                    src/MainWindow.cpp
                    src/PackageBackend.h
                    src/SlackPKG.h
                    src/SlackPKG.cpp
                    )
set( RESOURCES      res/swun.qrc )

#qt stuff
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
find_package(Qt5 5.15 COMPONENTS Core Widgets REQUIRED )

add_definitions( -DSWUN_VERSION="${VERSION}" )


#build
add_executable( ${TARGET} ${SRCS} ${RESOURCES})
target_link_libraries( ${TARGET} Qt5::Core Qt5::Widgets )


#install
set( BIN_PERMS OWNER_EXECUTE OWNER_WRITE OWNER_READ 
               GROUP_EXECUTE GROUP_READ 
               WORLD_EXECUTE WORLD_READ )
install( TARGETS ${TARGET} DESTINATION bin 
                           PERMISSIONS ${BIN_PERMS}
                           CONFIGURATIONS Release )
install( PROGRAMS swun.sh DESTINATION bin 
                       CONFIGURATIONS Release )
install( FILES LICENSE.txt README.md DESTINATION doc/${TARGET}-${VERSION} )
install( FILES swun.desktop DESTINATION share/applications )
install( FILES man/swun.1 DESTINATION man/man1 )


#help stuff
add_custom_target(changelog COMMAND git --no-pager log --format='%ai %an <%ae> %n%n%x09* %s%d%n' > ChangeLog WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
