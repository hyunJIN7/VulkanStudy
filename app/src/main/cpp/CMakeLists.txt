# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("vulkanstudy")

find_package(game-activity REQUIRED CONFIG)
find_package(Vulkan REQUIRED)

add_library(vulkanstudy SHARED
        main.cpp
        AndroidOut.cpp
        VulkanRenderer.cpp
)

set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")

target_compile_definitions(vulkanstudy PRIVATE
        VK_USE_PLATFORM_ANDROID_KHR)

# Configure libraries CMake uses to link your target library.
target_link_libraries(vulkanstudy
        # The game activity
        game-activity::game-activity_static

        # EGL and other dependent libraries required for drawing
        # and interacting with Android system
        jnigraphics
        android
        log
        Vulkan::Vulkan)