#include <cstdint>
#include <iomanip>
#include <string_view>
#include <cassert>
#include "VulkanRenderer.h"

//#define VK_USE_PLATFORM_ANDROID_KHR
//#include <vulkan/vulkan.h>

using namespace std;

VKRenderer::VKRenderer(ANativeWindow *window) {
    // ====================================================
    // 1. VkInstance 생성
    // ====================================================
    //APPLICATION 정의
    VkApplicationInfo applicationInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Practice Vulkan",
        .applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
        .apiVersion = VK_MAKE_API_VERSION(0, 1, 3, 0)
    };

    // 사용할 수 있는 Layer 얻어와서
    uint32_t instanceLayerCount;
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr));

    vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data()));

    // 사용할 활성화할 instance layer 설정
    vector<const char*> instanceLayerNames;
    for (const auto &layerProperty : instanceLayerProperties) {
        aout <<"hjk "<< layerProperty.layerName << endl;
        instanceLayerNames.push_back(layerProperty.layerName);
    }

    // 사용 가능한 Vulkan Instance Extension 얻기
    uint32_t instanceExtensionCount;
    VK_CHECK_ERROR(vkEnumerateInstanceExtensionProperties(
            nullptr, &instanceExtensionCount, nullptr));
    vector<VkExtensionProperties> instanceExtensionProperties(instanceExtensionCount);
    VK_CHECK_ERROR(vkEnumerateInstanceExtensionProperties(
            nullptr, &instanceExtensionCount, instanceExtensionProperties.data()));

    vector<const char *> instanceExtensionNames;
    for (const auto &properties: instanceExtensionProperties) {
        if (properties.extensionName == string("VK_KHR_surface") ||
            properties.extensionName == string("VK_KHR_android_surface")) {
            instanceExtensionNames.push_back(properties.extensionName);
        }
    }
    assert(instanceExtensionNames.size() == 2);

    VkInstanceCreateInfo instanceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = static_cast<uint32_t>(instanceLayerNames.size()),
            .ppEnabledLayerNames = instanceLayerNames.data(),
            .enabledExtensionCount = static_cast<uint32_t>(instanceExtensionNames.size()),
            .ppEnabledExtensionNames = instanceExtensionNames.data()
    };
    VK_CHECK_ERROR(vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance));

    // ====================================================
    // 2. vkPhysicalDevice 선택
    // ====================================================
    uint32_t physicalDeviceCount;
    VK_CHECK_ERROR(vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, nullptr));

    vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    VK_CHECK_ERROR(vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, physicalDevices.data()));

    // 간단한 예제를 위해 첫번째 사용
    mPhysicalDevice = physicalDevices[0];

    // gpu 모든 정보 있다고 함
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(mPhysicalDevice, &physicalDeviceProperties);

    aout << "Selected Physical Device Info" << endl;
    aout << setw(16) << left << " - Device Name : " << string_view(physicalDeviceProperties.deviceName) << endl;

    // ====================================================
    // 3. VkDevice 생성
    // ====================================================
    uint32_t queueFamilyPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice,
                                             &queueFamilyPropertiesCount, nullptr);
    vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount); // 해당 queue family 속성 얻어오기
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice,
                                             &queueFamilyPropertiesCount,
                                             queueFamilyProperties.data());

    // 특정 Queue Family가 Graphics bit 지원하는지 확인
    for (mQueueFamilyIndex = 0; mQueueFamilyIndex != queueFamilyPropertiesCount; ++mQueueFamilyIndex) {
        if (queueFamilyProperties[mQueueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            break;
        }
    }

    const vector<float> queuePriorities{1.0};
    VkDeviceQueueCreateInfo deviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = mQueueFamilyIndex,
            .queueCount = 1,
            .pQueuePriorities = queuePriorities.data()
    };

    // 사용 가능한 Vulkan Device Extension 얻기
    uint32_t deviceExtensionCount;
    VK_CHECK_ERROR(vkEnumerateDeviceExtensionProperties(mPhysicalDevice,
                                                        nullptr, &deviceExtensionCount, nullptr));
    vector<VkExtensionProperties> deviceExtensionProperties(deviceExtensionCount);
    VK_CHECK_ERROR(vkEnumerateDeviceExtensionProperties(mPhysicalDevice,
                                                        nullptr, &deviceExtensionCount, deviceExtensionProperties.data()));
    // 화면 출력을 위한 vulkan device extension 선택
    vector<const char *> deviceExtensionNames;
    for (const auto &properties: deviceExtensionProperties) {
        if (properties.extensionName == string("VK_KHR_swapchain")) {
            deviceExtensionNames.push_back(properties.extensionName);
        }
    }
    assert(deviceExtensionNames.size() == 1);

    // vulkan device 정의 후 생성
    VkDeviceCreateInfo deviceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &deviceQueueCreateInfo,
            .enabledExtensionCount = static_cast<uint32_t>(deviceExtensionNames.size()),
            .ppEnabledExtensionNames = deviceExtensionNames.data()
    };

    VK_CHECK_ERROR(vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mDevice));

    // vulkan queue 얻기
    vkGetDeviceQueue(mDevice, mQueueFamilyIndex, 0, &mQueue);

    // ====================================================
    // 4. VkSurface 생성
    // ====================================================
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
            .window = window
    };
    VK_CHECK_ERROR(vkCreateAndroidSurfaceKHR(mInstance,
                                             &surfaceCreateInfo,
                                             nullptr,
                                             &mSurface));

    // Vulkan Physical Device가 Vulkan Surface를 지원하는지 확인
    VkBool32 supported;
    VK_CHECK_ERROR(vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice,
                                                        mQueueFamilyIndex,
                                                        mSurface,
                                                        &supported));
    assert(supported);
}

VKRenderer::~VKRenderer() {
    vkDestroyInstance(mInstance, nullptr);
    vkDestroyDevice(mDevice, nullptr);
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
}

