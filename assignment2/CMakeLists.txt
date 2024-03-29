get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SHADERS "*.glsl")

file(GLOB RENDERING "Rendering/*.cpp")
file(GLOB RENDERING "Rendering/*.h")
file(GLOB MESHGENERATORS "MeshGenerators/*.cpp")
file(GLOB MESHGENERATORS "MeshGenerators/*.h")
file(GLOB IMGUI "../external/OpenGP/src/OpenGP/external/imgui/*.h" )
file(GLOB IMGUI "../external/OpenGP/src/OpenGP/external/imgui/*.cpp" )

add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS} ${MESHGENERATORS} ${RENDERING} ${SHADERS} ${IMGUI})
if(WIN32)
        target_link_libraries(${EXERCISENAME} "legacy_stdio_definitions.lib")
endif()
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})

#--- data need to be copied to run folder
file(COPY ${PROJECT_SOURCE_DIR}/data/1.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/data/earth.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/data/arrow.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/data/soup.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
