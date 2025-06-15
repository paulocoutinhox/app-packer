cmake_minimum_required(VERSION 3.16)
project(MyApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/deps.cmake)

add_executable(MyApp src/main.cpp)
target_link_libraries(MyApp PRIVATE imgui glfw ${OPENGL_gl_LIBRARY})

if(APPLE)
  set_target_properties(MyApp PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.myapp"
    MACOSX_BUNDLE_BUNDLE_NAME "MyApp"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
  )

  # Install only the bundle
  install(TARGETS MyApp BUNDLE DESTINATION . COMPONENT Runtime)
else()
  install(TARGETS MyApp DESTINATION bin)
endif()

include(cmake/cpack.cmake OPTIONAL)
