get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SHADERS "*.glsl")

file(GLOB RENDERING "Rendering/*.cpp")
file(GLOB RENDERING "Rendering/*.h")

add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS} ${SHADERS} ${RENDERING})
if(WIN32)
        target_link_libraries(${EXERCISENAME} "legacy_stdio_definitions.lib")
endif()
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})
