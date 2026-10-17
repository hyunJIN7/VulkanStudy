#include <cstdint>
#include <iomanip>
#include <string_view>
#include "VulkanRenderer.h"

using namespace std;

VKRenderer::VKRenderer() {
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

    VkInstanceCreateInfo instanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = static_cast<uint32_t>(instanceLayerNames.size()),
        .ppEnabledLayerNames = instanceLayerNames.data()
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

    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(mPhysicalDevice, &physicalDeviceProperties);

    aout << "Selected Physical Device Info" << endl;
    aout << setw(16) << left << " - Device Name : " << string_view(physicalDeviceProperties.deviceName) << endl;

}

VKRenderer::~VKRenderer() {
    vkDestroyInstance(mInstance, nullptr);
}

