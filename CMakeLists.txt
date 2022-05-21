
cmake_minimum_required(VERSION 2.8)

project(fm-radio-sn)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC OFF)

find_package(Qt5 COMPONENTS Core Widgets SerialPort REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5SerialPort_INCLUDE_DIRS})

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5SerialPort_DEFINITIONS})

QT5_WRAP_CPP(HEADERS_MOC
    src/ui/panels/defaulthorizontalpanel.h
    src/fmradio/appwindow.h
    src/fmradio/appmanager.h
    src/fmradio/controlpanel.h
    src/fmradio/ui/playlist/playlistmodeleditordelegate.h
    src/fmradio/ui/playlist/playlistproxymodeldelegate.h
    src/fmradio/ui/playlist/playlistproxymodel.h
    src/fmradio/ui/settings/settingsdialog.h
    src/fmradio/ui/widgets/tunerknob.h
    src/fmradio/ui/widgets/volumeknob.h
    src/fmradio/ui/widgets/radioknob.h
    src/fmradio/ui/appcontainer.h
    src/fmradio/ui/settings/settingsdialog.h
    src/fmradio/ui/playlist/stareditor.h
)

QT5_WRAP_UI(UI_HEADERS
    src/ui/panels/defaulthorizontalpanel.ui
    src/fmradio/ui/settings/settingsdialog.ui
)

set(SOURCES
    src/main.cpp
    src/myuseragent.cpp
    src/ui/panels/defaulthorizontalpanel.cpp
    src/fmradio/appwindow.cpp
    src/fmradio/useragent.cpp
    src/fmradio/appmanager.cpp
    src/fmradio/controlpanel.cpp
    src/fmradio/core/io/protocol.cpp
    src/fmradio/core/io/output.cpp
    src/fmradio/core/io/event.cpp
    src/fmradio/core/events.cpp
    src/fmradio/core/commands.cpp
    src/fmradio/core/controller.cpp
    src/fmradio/core/eventhandler.cpp
    src/fmradio/core/frequencyrange.cpp
    src/fmradio/core/inputlogger.cpp
    src/fmradio/ui/settings/settingsdialog.cpp
    src/fmradio/ui/settings/logmodel.cpp
    src/fmradio/ui/settings/logmodeldelegate.cpp
    src/fmradio/ui/widgets/tunerknob.cpp
    src/fmradio/ui/widgets/volumeknob.cpp
    src/fmradio/ui/widgets/radioknob.cpp
    src/fmradio/ui/appcontainer.cpp
    src/fmradio/ui/playlist/playlistmodel.cpp
    src/fmradio/ui/playlist/playlistmodeleditordelegate.cpp
    src/fmradio/ui/playlist/stareditor.cpp
    src/fmradio/ui/playlist/playlistproxymodel.cpp
    src/fmradio/ui/widgets/slidingtext.cpp
    src/fmradio/ui/widgets/statustext.cpp
    src/fmradio/ui/playlist/playlistproxymodeldelegate.cpp
)

FILE(GLOB_RECURSE QRC_FILES ${CMAKE_SOURCE_DIR}/src/*.qrc)
qt5_add_resources(RESOURCES ${QRC_FILES})


include_directories(${CMAKE_SOURCE_DIR}/src/)
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} -std=c++11")

add_executable(
    ${CMAKE_PROJECT_NAME}
    ${SOURCES}
    ${RESOURCES}
    ${HEADERS_MOC}
    ${UI_HEADERS}
)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    stdc++
    pthread
)

qt5_use_modules(${CMAKE_PROJECT_NAME} Core Widgets SerialPort)


INSTALL(PROGRAMS ./bin/fm-radio-sn DESTINATION /usr/local/bin)
INSTALL(FILES ./CONTROL/fm-radio-sn.desktop DESTINATION /usr/share/applications)
INSTALL(FILES ./CONTROL/fm-radio-sn.png DESTINATION /usr/share/pixmaps)


# ***********************************************************************
# generate DEB package

if("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")

    string(TIMESTAMP DATETIME_NOW %Y%m%d%H%M UTC)

    if(NOT DEFINED ARCH)

        execute_process(
            COMMAND "/usr/bin/dpkg" "--print-architecture"
            RESULT_VARIABLE RESULT
            OUTPUT_VARIABLE ARCH
        )

        string(STRIP "${ARCH}" ARCH)

    endif()

    if("${ARCH}" STREQUAL "")
        set(ARCH "amd64")
    endif()

    set(VERSION_MAJOR "1")
    set(VERSION_MINOR "0")
    set(VERSION_PATCH "${DATETIME_NOW}")

    set(CPACK_GENERATOR "DEB")
    SET(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
    SET(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
    SET(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${ARCH})
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5core5a,libqt5widgets5")
	SET(CPACK_PACKAGE_VENDOR "Alexander Arefyev")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "FM Radio SN radio module control utility")
    set(CPACK_PACKAGE_DESCRIPTION "FM Radio SN radio module control utility")
	set(CPACK_PACKAGE_CONTACT "Alexander Arefyev")

    set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}-${VERSION_PATCH}")

    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

    include(CPack)

endif()
