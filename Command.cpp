//
// Created by felix on 09.06.20.
//

#include "Command.h"

#include <utility>

geo::Command::Command(sp<DeviceManager> deviceManager) : deviceManager(std::move(deviceManager)) {

}

void geo::Command::setup() {
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();

    poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolCreateInfo.pNext = nullptr;
    poolCreateInfo.flags = 0;
    poolCreateInfo.queueFamilyIndex = 0; // TODO: BAD INIT
    vkCreateCommandPool(logicalHandle, &poolCreateInfo, nullptr, &commandPool);

}

void geo::Command::shutdown() {
    auto logicalHandle = deviceManager->getCurrentDevice()->getLogicalHandle();

    vkDestroyCommandPool(logicalHandle, commandPool, nullptr);
}
