//
// Created by felix on 12.04.20.
//

#ifndef GEOMETRYENGINE_DEVICEMANAGER_H
#define GEOMETRYENGINE_DEVICEMANAGER_H


#include <vector>
#include "Device.h"
#include "Manageable.h"
#include "GlobaleScope.h"

namespace geo {
    class DeviceManager : Manageable {
    public:
        explicit DeviceManager(VkInstance& instance);
        virtual ~DeviceManager() = default;

        void setup() override;
        void shutdown() override;

        [[nodiscard]] int getDeviceCount() const;
        [[nodiscard]] sp<Device> getDevice(int index) const;
    private:
        VkInstance instance;
        std::vector<sp<Device>> devices;
    };
}


#endif //GEOMETRYENGINE_DEVICEMANAGER_H
