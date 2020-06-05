//
// Created by felix on 12.04.20.
//

#ifndef GEOMETRYENGINE__APPLICATION_H_
#define GEOMETRYENGINE__APPLICATION_H_

#include "vulkan/vulkan.h"
#include "GlobaleScope.h"
#include "DeviceManager.h"
#include "Manageable.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_vulkan.h"

namespace geo {
    class Application : Manageable {
    public:
        Application();
        virtual ~Application() = default;

        void setup() override;
        void shutdown() override;

        bool run();

        [[nodiscard]] DeviceManager& getDeviceManager() const;
    private:

        VkApplicationInfo info;
        VkInstanceCreateInfo instanceCreateInfo;
        VkInstance instance;
        VkSurfaceKHR surface;

        SDL_Window* window;

        up<DeviceManager> deviceManager;
    };
}

#endif //GEOMETRYENGINE__APPLICATION_H_
