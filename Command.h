//
// Created by felix on 09.06.20.
//

#ifndef GEOMETRYENGINE_COMMAND_H
#define GEOMETRYENGINE_COMMAND_H


#include "Manageable.h"
#include "DeviceManager.h"

namespace geo {
    class Command : public Manageable {
    public:
        explicit Command(sp<DeviceManager> deviceManager);
        virtual ~Command() = default;

        void setup() override;
        void shutdown() override;

    private:
        VkCommandPoolCreateInfo poolCreateInfo;
        VkCommandPool commandPool;

        sp<DeviceManager> deviceManager;
    };
}


#endif //GEOMETRYENGINE_COMMAND_H
