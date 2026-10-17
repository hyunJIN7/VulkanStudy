#include <jni.h>

#include <memory>

#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <game-activity/GameActivity.h>

#include <vulkan/vulkan.h>

#include "AndroidOut.h"
#include "VulkanRenderer.h"

struct AppState {
    std::unique_ptr<VKRenderer> renderer;
};

extern "C" {

void handle_cmd(android_app* pApp, int32_t cmd) {
    auto* state = static_cast<AppState*>(pApp->userData);

    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            aout << "INIT_WINDOW" << std::endl;

            if (pApp->window != nullptr && state != nullptr) {
                state->renderer =
                        std::make_unique<VKRenderer>(pApp->window);
            }
            break;

        case APP_CMD_TERM_WINDOW:
            aout << "TERM_WINDOW" << std::endl;

            if (state != nullptr) {
                state->renderer.reset();
            }
            break;

        default:
            break;
    }
}
bool motion_event_filter_func(const GameActivityMotionEvent *motionEvent) {
    auto sourceClass = motionEvent->source & AINPUT_SOURCE_CLASS_MASK;
    return (sourceClass == AINPUT_SOURCE_CLASS_POINTER ||
            sourceClass == AINPUT_SOURCE_CLASS_JOYSTICK);
}

void android_main(struct android_app* pApp) {
    aout << "Welcome to android_main" << std::endl;

    AppState state;
    pApp->userData = &state;
    pApp->onAppCmd = handle_cmd;

    android_app_set_motion_event_filter(
            pApp, motion_event_filter_func
    );

    while (!pApp->destroyRequested) {
        int events;
        android_poll_source* pSource;

        int result = ALooper_pollOnce(
                -1,
                nullptr,
                &events,
                reinterpret_cast<void**>(&pSource)
        );

        if (result >= 0 && pSource != nullptr) {
            pSource->process(pApp, pSource);
        }

        // TODO: 매 프레임 Vulkan 렌더링
    }
}
}