#ifndef VULKANSTUDY_VULKANRENDERER_H
#define VULKANSTUDY_VULKANRENDERER_H

#pragma once
#include <android/native_window.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <cstdlib>
#include "AndroidOut.h"

#define VK_CHECK_ERROR(x)                                                   \
    do {                                                                    \
        VkResult result_ = (x);                                             \
        if (result_ != VK_SUCCESS) {                                        \
            aout << "Vulkan error " << result_ << " at "                    \
                 << __FILE__ << ":" << __LINE__ << std::endl;               \
            abort();                                                        \
        }                                                                   \
    } while (0)

class VKRenderer
{
public:
    explicit VKRenderer(ANativeWindow* window);
    ~VKRenderer();

private:
    VkInstance mInstance = VK_NULL_HANDLE;
    VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
    VkDevice mDevice = VK_NULL_HANDLE;
    VkQueue mQueue = VK_NULL_HANDLE;
    uint32_t mQueueFamilyIndex = 0;
    VkSurfaceKHR mSurface = VK_NULL_HANDLE;
};

#endif //VULKANSTUDY_VULKANRENDERER_H
