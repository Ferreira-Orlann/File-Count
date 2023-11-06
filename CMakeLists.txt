# CMakeList.txt : projet CMake pour File Count, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

# Activez Rechargement à chaud pour les compilateurs MSVC si cela est pris en charge.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("FileCount")

set(GLFW_VERSION 3.3.8)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
find_package(glfw ${GLFW_VERSION} QUIET) # QUIET or REQUIRED
if (NOT glfw_FOUND) # If there's none, fetch and build glfw3
  include(FetchContent)
  FetchContent_Declare(
    glfw
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/glfw/glfw/releases/download/${GLFW_VERSION}/glfw-${GLFW_VERSION}.zip
  )
  FetchContent_GetProperties(glfw3)
  if (NOT glfw3_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(glfw)
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
    message("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
  endif()
endif()

# Ajoutez une source à l'exécutable de ce projet.
add_executable (${PROJECT_NAME} 
    "source/main.cpp" 
    "source/file.cpp" 
    "source/directory.cpp" 
    "source/test.cpp"
    "source/app.cpp")

target_link_libraries(${PROJECT_NAME} glfw)

include_directories("include")

set(IMGUI_ENABLE_FREETYPE OFF)
set(IMGUI_EXAMPLES OFF)
set(IMGUI_ENABLE_STDLIB_SUPPORT ON)
add_subdirectory("libs/imgui-docking")
target_link_libraries(${PROJECT_NAME} imgui imgui-glfw imgui-opengl3)
include_directories("libs/imgui-docking/backends")
target_include_directories(${PROJECT_NAME} PRIVATE "libs/imgui-docking/backends")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()
 