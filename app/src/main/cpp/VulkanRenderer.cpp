#include <cstdint>
#include "VulkanRenderer.h"

using namespace std;

VKRenderer::VKRenderer() {
    uint32_t layerCount;
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));
// 두번째가 null 이라서 사용 할 수 있는 layer 개수만 반환해줌

    vector<VkLayerProperties> layerProps(layerCount);
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&layerCount, layerProps.data()));
// layer 정보가 layerProps 파라미터에 쓰여지고,  layerCount에도 몇개인지 써짐

    for (const auto& layerProp : layerProps)
    {
        aout <<"hjk "<< layerProp.layerName << endl;
    }
}

VKRenderer::~VKRenderer() {

}

