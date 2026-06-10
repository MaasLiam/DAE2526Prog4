cmake_minimum_required(VERSION 3.26)
project(Minigin)

if(EMSCRIPTEN)
  message("Using Emscripten!")
endif()

if(EMSCRIPTEN)
  set(TARGET_NAME Galaga_web)
  set(TARGET_PARAM )
else()
  set(TARGET_NAME Galaga)
  set(TARGET_PARAM WIN32)
endif()

set(ENGINE_TARGET MiniginEngine)

# engine static library
add_library(${ENGINE_TARGET} STATIC
  Minigin/Font.cpp
  Minigin/GameObject.cpp
  Minigin/InputManager.cpp
  Minigin/Minigin.cpp
  Minigin/Renderer.cpp
  Minigin/ResourceManager.cpp
  Minigin/Scene.cpp
  Minigin/SceneManager.cpp
  Minigin/TextComponent.cpp
  Minigin/Texture2D.cpp
  Minigin/Component.cpp 
  Minigin/TransformComponent.cpp 
  Minigin/RenderComponent.cpp
  Minigin/FPSComponent.cpp 
  Minigin/RotationComponent.cpp
  Minigin/KeyboardInput.cpp
  Minigin/ControllerInput.cpp 
  Minigin/MoveObjectCommand.cpp
  Minigin/Subject.cpp
  Minigin/NullSoundSystem.cpp
  Minigin/ServiceLocator.cpp
  Minigin/LoggingSoundSystem.cpp
  Minigin/SDLSoundSystem.cpp
)

# game executable
add_executable(${TARGET_NAME} ${TARGET_PARAM}
  Galaga/Main.cpp
  Galaga/Galaga.cpp
  Galaga/ThrashTheCache.cpp 

  Galaga/Components/HealthComponent.cpp
  Galaga/Components/DisplayLivesComponent.cpp
  Galaga/Components/ScoreComponent.cpp
  Galaga/Components/DisplayScoreComponent.cpp
  Galaga/Components/BulletComponent.cpp
  Galaga/Components/CollisionComponent.cpp

  Galaga/Commands/DamagePlayerCommand.cpp
  Galaga/Commands/AddScoreCommand.cpp
  Galaga/Commands/ShootCommand.cpp

  Galaga/Achievements/SteamAchievements.cpp
  Galaga/Achievements/SteamAchievementObserver.cpp

  Galaga/Components/EnemyComponent.cpp
  Galaga/Components/EnemyPlayerCollisionComponent.cpp
  Galaga/Components/BulletEnemyCollisionComponent.cpp
  Galaga/Components/EnemyFormationControllerComponent.cpp
  Galaga/Components/MissileLimitComponent.cpp
  Galaga/Components/GalagaGameControllerComponent.cpp
  Galaga/Level/LevelLoader.cpp 
  Galaga/Commands/SkipStageCommand.cpp
  Galaga/Commands/ToggleMuteCommand.cpp
  Galaga/HighScores/HighScoreManager.cpp
 "Galaga/Commands/ChangeInitialCommand.cpp" "Galaga/Commands/MoveInitialCursorCommand.cpp" "Galaga/Commands/ConfirmHighScoreNameCommand.cpp" "Galaga/Commands/SelectGameModeCommand.cpp")

# enable c++20 features
target_compile_features(${ENGINE_TARGET} PUBLIC cxx_std_20)
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

include(FetchContent)
# add glm
find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
  FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
  )
  FetchContent_MakeAvailable(glm)
endif()

# Add SDL3
find_package(SDL3 3.4 CONFIG QUIET)
if(NOT SDL3_FOUND)
  if (WIN32)
    FetchContent_Declare(
      SDL3
      URL https://www.libsdl.org/release/SDL3-devel-3.4.0-VC.zip
      DOWNLOAD_NO_PROGRESS ON
      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL3)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_SOURCE_DIR}")
    find_package(SDL3 CONFIG REQUIRED)
  else()
    FetchContent_Declare(
      SDL3
      GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
      GIT_TAG release-3.4.0
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL3)
  endif()
endif()

# Add SDL3_ttf
find_package(SDL3_ttf 3.2.2 CONFIG QUIET)
if(NOT SDL3_ttf_FOUND)
  if (WIN32)
    FetchContent_Declare(
      SDL3_ttf
      URL https://www.libsdl.org/projects/SDL_ttf/release/SDL3_ttf-devel-3.2.2-VC.zip
      DOWNLOAD_NO_PROGRESS ON
      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL3_ttf)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_SOURCE_DIR}")
    find_package(SDL3_ttf CONFIG REQUIRED)
  else()
    FetchContent_Declare(
      SDL3_ttf
      GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
      GIT_TAG release-3.2.2
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )
    set(SDL3TTF_INSTALL OFF)
    if(EMSCRIPTEN)
      list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/emscripten")
      set(SDL3TTF_USE_SYSTEM_FREETYPE ON)
      set(SDL3TTF_VENDORED OFF)
      set(SDLTTF_PLUTOSVG OFF)
      add_compile_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
      add_link_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
    endif()
    FetchContent_MakeAvailable(SDL3_ttf)
  endif()
endif()

# Add SDL3_mixer
find_package(SDL3_mixer CONFIG QUIET)

if(NOT SDL3_mixer_FOUND)
  FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
  )

  set(SDL3MIXER_VENDORED ON)
  set(SDL3MIXER_SAMPLES OFF)
  set(SDL3MIXER_INSTALL OFF)

  FetchContent_MakeAvailable(SDL3_mixer)
endif()

# Add Dear ImGui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG v1.92.5-docking
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
target_include_directories(imgui PUBLIC
  ${imgui_SOURCE_DIR}
  ${sdl3_SOURCE_DIR}/include
)
target_link_libraries(imgui PUBLIC SDL3::SDL3)
target_compile_features(imgui PUBLIC cxx_std_20)

# Add imgui_plot
FetchContent_Declare(
  imgui_plot
  GIT_REPOSITORY https://github.com/soulthreads/imgui-plot.git
)

FetchContent_GetProperties(imgui_plot)
if(NOT imgui_plot_POPULATED)
  FetchContent_Populate(imgui_plot)
endif()

add_library(imgui_plot STATIC
  ${imgui_plot_SOURCE_DIR}/src/imgui_plot.cpp
)

target_include_directories(imgui_plot PUBLIC
  ${imgui_plot_SOURCE_DIR}/include
  ${imgui_SOURCE_DIR}
)

target_link_libraries(imgui_plot PUBLIC imgui)
target_compile_features(imgui_plot PUBLIC cxx_std_20)

if (WIN32)
  # Add VLD (Visual Leak Detector) for memory leak detection on Windows
  find_package(VLD CONFIG)
endif()

# Add steam if available
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/steam")
find_package(Steamworks QUIET)
if(Steamworks_FOUND)
  message(STATUS "Steamworks found: ${Steamworks_VERSION}")
  message(STATUS "Steamworks include: ${Steamworks_INCLUDE_DIRS}")
  message(STATUS "Steamworks library: ${Steamworks_LIBRARIES}")
else()
  message(STATUS "Steamworks not found")
endif()

target_include_directories(${ENGINE_TARGET} PUBLIC 
  ${CMAKE_SOURCE_DIR}/Minigin/
  ${glm_SOURCE_DIR}
  ${sdl3_SOURCE_DIR}/include
  ${sdl3_image_SOURCE_DIR}/include
  ${sdl3_ttf_SOURCE_DIR}/include
  ${imgui_SOURCE_DIR}
  ${VLD_INCLUDE_DIR}
)

target_include_directories(${TARGET_NAME} PUBLIC 
  ${CMAKE_SOURCE_DIR}/Galaga/
  ${CMAKE_SOURCE_DIR}/Galaga/Components/
  ${CMAKE_SOURCE_DIR}/Galaga/Commands/
  ${CMAKE_SOURCE_DIR}/Galaga/Achievements/
  ${CMAKE_SOURCE_DIR}/Galaga/Level/
  ${CMAKE_SOURCE_DIR}/Galaga/HighScores/
  ${imgui_plot_SOURCE_DIR}/include
)

target_link_libraries(${ENGINE_TARGET} PUBLIC 
  SDL3::SDL3 
  SDL3_ttf::SDL3_ttf
  SDL3_mixer::SDL3_mixer
  imgui
  ${VLD_LIBRARIES}
)

target_link_libraries(${TARGET_NAME} PUBLIC
  ${ENGINE_TARGET}
  imgui_plot
)

if(MSVC)
  # compile at warning level 4 + treat warnings as errors
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
  target_compile_options(${ENGINE_TARGET} PRIVATE /W4 /WX)
  target_compile_options(imgui_plot PRIVATE /W3)
else()
  target_compile_options(${ENGINE_TARGET} PRIVATE -Wall -Wextra -Werror)
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Werror)

  target_compile_options(imgui_plot PRIVATE -Wall -Wextra)

  if(EMSCRIPTEN)
    target_compile_options(imgui_plot PRIVATE -Wno-sign-compare)
  endif()
endif()

if(EMSCRIPTEN)
  set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -g")
  set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Data@/\"")

  # Use the Emscripten toolchain
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmscriptenToolchain.cmake)

  # Additional settings for Emscripten build
  set_target_properties(${TARGET_NAME} PROPERTIES
      COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
      LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
  )

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/web" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
  )

  # Have emscripten generate a html page too.
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

elseif(WIN32)
  # Set minigin as the default startup project
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
  # Set working directory to output folder
  set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

  # Copy SDL dlls to output folder
  set(SDL_RUNTIME_DLLS
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
  )

  foreach(dllTarget IN LISTS SDL_RUNTIME_DLLS)
    add_custom_command(
      TARGET ${TARGET_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "$<TARGET_FILE:${dllTarget}>"
          "$<TARGET_FILE_DIR:${TARGET_NAME}>"
      VERBATIM
    )
  endforeach()

  if(Steamworks_FOUND)
    target_link_libraries("${TARGET_NAME}" PUBLIC Steamworks::Steamworks)
    target_compile_definitions(${TARGET_NAME} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy "${Steamworks_RUNTIME_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>")
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${TARGET_NAME}>/steam_appid.txt"
      COMMENT "Generating steam_appid.txt"
    )
  endif()

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
  )
elseif(APPLE)
  set_target_properties(${TARGET_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    INSTALL_RPATH "@executable_path/../Frameworks"
    BUILD_WITH_INSTALL_RPATH TRUE
  )
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3-shared>" "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks/"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3_ttf::SDL3_ttf-shared>" "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks/"
    COMMAND install_name_tool -change "@rpath/libSDL3_ttf.0.dylib" "@executable_path/../Frameworks/libSDL3_ttf.0.2.2.dylib" "$<TARGET_FILE:${TARGET_NAME}>"
    COMMAND install_name_tool -change "@rpath/libSDL3.0.dylib" "@executable_path/../Frameworks/libSDL3.0.dylib" "$<TARGET_FILE:${TARGET_NAME}>"
  )

  if(Steamworks_FOUND)
    target_link_libraries("${TARGET_NAME}" PUBLIC Steamworks::Steamworks)
    target_compile_definitions(${TARGET_NAME} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUIL D
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Steamworks_LIBRARIES}" "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks/"
    )
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${TARGET_NAME}>/steam_appid.txt"
      COMMENT "Generating steam_appid.txt"
    )
  endif()

  # Copy Data folder to output directory after build
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
  )
else()
 if(Steamworks_FOUND)
    target_link_libraries("${TARGET_NAME}" PUBLIC Steamworks::Steamworks)
    target_compile_definitions(${TARGET_NAME} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Steamworks_LIBRARIES}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    )
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${TARGET_NAME}>/steam_appid.txt"
      COMMENT "Generating steam_appid.txt"
    )
  endif()

  # Copy Data folder to output directory after build
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
  )
endif()
