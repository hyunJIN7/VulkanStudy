#ifndef VULKANSTUDY_VULKANRENDERER_H
#define VULKANSTUDY_VULKANRENDERER_H

#pragma once
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
    VKRenderer();
    ~VKRenderer();
    VkInstance mInstance = VK_NULL_HANDLE;
    VkPhysicalDevice mPhysicalDevice;
    uint32_t mQueueFamilyIndex = 0;
    VkDevice mDevice;
    VkQueue mQueue;
};

#endif //VULKANSTUDY_VULKANRENDERER_H
