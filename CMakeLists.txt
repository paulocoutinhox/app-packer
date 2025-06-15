cmake_minimum_required(VERSION 3.15)
project(MyApp VERSION 1.0.0)

# Set Objective-C++ for macOS
if(APPLE)
    set(CMAKE_OBJCXX_STANDARD 17)
    set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)
endif()

# Find SDL2 package
find_package(SDL2 REQUIRED)

option(GENERATE_ICONS "Generate icons from SVG" OFF)

set(SVG_ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/common/icon.svg)
set(ICON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons)
file(MAKE_DIRECTORY ${ICON_DIR})

# Icon generation
if(GENERATE_ICONS)
    if(WIN32)
        foreach(SIZE 16 32 48 64 128 256)
            add_custom_command(
                OUTPUT ${ICON_DIR}/icon_${SIZE}x${SIZE}.png
                COMMAND cairosvg -f png -W ${SIZE} -H ${SIZE} -o ${ICON_DIR}/icon_${SIZE}x${SIZE}.png ${SVG_ICON}
                DEPENDS ${SVG_ICON}
            )
            list(APPEND ICON_PNGS ${ICON_DIR}/icon_${SIZE}x${SIZE}.png)
        endforeach()
        add_custom_command(
            OUTPUT ${ICON_DIR}/myapp.ico
            COMMAND convert ${ICON_PNGS} ${ICON_DIR}/myapp.ico
            DEPENDS ${ICON_PNGS}
        )
        add_custom_target(generate_icons ALL DEPENDS ${ICON_PNGS} ${ICON_DIR}/myapp.ico)
    elseif(APPLE)
        foreach(SIZE 16 32 64 128 256 512)
            add_custom_command(
                OUTPUT ${ICON_DIR}/icon_${SIZE}x${SIZE}.png
                COMMAND cairosvg -f png --output-width ${SIZE} --output-height ${SIZE} -o ${ICON_DIR}/icon_${SIZE}x${SIZE}.png ${SVG_ICON}
                DEPENDS ${SVG_ICON}
            )
            list(APPEND ICON_PNGS ${ICON_DIR}/icon_${SIZE}x${SIZE}.png)
        endforeach()
        add_custom_command(
            OUTPUT ${ICON_DIR}/myapp.icns
            COMMAND ${CMAKE_COMMAND} -E make_directory ${ICON_DIR}/myapp.iconset
            COMMAND ${CMAKE_COMMAND} -E copy ${ICON_DIR}/icon_16x16.png ${ICON_DIR}/myapp.iconset/icon_16x16.png
            COMMAND ${CMAKE_COMMAND} -E copy ${ICON_DIR}/icon_32x32.png ${ICON_DIR}/myapp.iconset/icon_16x16@2x.png
            COMMAND ${CMAKE_COMMAND} -E copy ${ICON_DIR}/icon_32x32.png ${ICON_DIR}/myapp.iconset/icon_32x32.png
            COMMAND ${CMAKE_COMMAND} -E copy ${ICON_DIR}/icon_64x64.png ${ICON_DIR}/myapp.iconset/icon_32x32@2x.png
            COMMAND ${CMAKE_COMMAND} -E copy ${ICON_DIR}/icon_128x128.png ${ICON_DIR}/myapp.iconset/icon_128x128.png
            COMMAND ${CMAKE_COMMAND} -E copy ${ICON_DIR}/icon_256x256.png ${ICON_DIR}/myapp.iconset/icon_128x128@2x.png
            COMMAND ${CMAKE_COMMAND} -E copy ${ICON_DIR}/icon_256x256.png ${ICON_DIR}/myapp.iconset/icon_256x256.png
            COMMAND ${CMAKE_COMMAND} -E copy ${ICON_DIR}/icon_512x512.png ${ICON_DIR}/myapp.iconset/icon_256x256@2x.png
            COMMAND ${CMAKE_COMMAND} -E copy ${ICON_DIR}/icon_512x512.png ${ICON_DIR}/myapp.iconset/icon_512x512.png
            COMMAND ${CMAKE_COMMAND} -E copy ${ICON_DIR}/icon_512x512.png ${ICON_DIR}/myapp.iconset/icon_512x512@2x.png
            COMMAND iconutil -c icns ${ICON_DIR}/myapp.iconset -o ${ICON_DIR}/myapp.icns
            DEPENDS ${ICON_PNGS}
        )
        add_custom_target(generate_icons ALL DEPENDS ${ICON_PNGS} ${ICON_DIR}/myapp.icns)
    elseif(UNIX)
        foreach(SIZE 48 64 128 256 512)
            add_custom_command(
                OUTPUT ${ICON_DIR}/icon_${SIZE}x${SIZE}.png
                COMMAND cairosvg -f png -W ${SIZE} -H ${SIZE} -o ${ICON_DIR}/icon_${SIZE}x${SIZE}.png ${SVG_ICON}
                DEPENDS ${SVG_ICON}
            )
            list(APPEND ICON_PNGS ${ICON_DIR}/icon_${SIZE}x${SIZE}.png)
        endforeach()
        add_custom_target(generate_icons ALL DEPENDS ${ICON_PNGS})
    endif()
endif()

# Build targets per OS
set(SOURCES src/main.cpp)

# Common configuration for all platforms
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

if(WIN32)
    # Resource file
    set(APP_ICON_RESOURCE "${ICON_DIR}/windows/myapp.rc")
    add_executable(myapp WIN32 ${SOURCES} ${APP_ICON_RESOURCE})
    target_link_libraries(myapp PRIVATE SDL2::SDL2)
elseif(APPLE)
    # Bundle with icon and plist
    set(APP_ICON_MACOS "${ICON_DIR}/myapp.icns")
    set_source_files_properties(${APP_ICON_MACOS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    file(GLOB_RECURSE ASSET_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")
    set_source_files_properties(${ASSET_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/assets")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/macos/Info.plist ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
    add_executable(myapp MACOSX_BUNDLE ${SOURCES} ${APP_ICON_MACOS} ${ASSET_FILES})
    set_target_properties(myapp PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
    )
    target_link_libraries(myapp PRIVATE "-framework Cocoa")
else()
    # Linux and others
    add_executable(myapp ${SOURCES})
    target_link_libraries(myapp PRIVATE SDL2::SDL2)
    install(TARGETS myapp DESTINATION bin)
    install(DIRECTORY assets/ DESTINATION share/myapp/assets)
    install(FILES ${ICON_DIR}/icon_128x128.png DESTINATION share/icons/hicolor/128x128/apps RENAME myapp.png)
    install(FILES myapp.desktop DESTINATION share/applications)
endif()
