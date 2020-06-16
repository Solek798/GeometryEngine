//
// Created by felix on 12.04.20.
//

#include <iostream>
#include "Application.h"
#include "GlobaleScope.h"
#include <chrono>
#include <vector>

geo::Application::Application() {
    window = nullptr;
}

void geo::Application::setup() {
    VkResult result;

    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Geometry Engine", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN);

    uint32_t amountOfSdlExtentions = 0;

    SDL_Vulkan_GetInstanceExtensions(window, &amountOfSdlExtentions, nullptr);
    std::vector<const char*> sdlExtentions(amountOfSdlExtentions);
    SDL_Vulkan_GetInstanceExtensions(window, &amountOfSdlExtentions, sdlExtentions.data());

    info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    info.pNext = nullptr;
    info.pApplicationName = "Geometry App";
    info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    info.pEngineName = "Geometry Engine";
    info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    info.apiVersion = VK_API_VERSION_1_2;

    uint32_t amountOfLayers = 0;
    vkEnumerateInstanceLayerProperties(&amountOfLayers, nullptr);
    std::vector<VkLayerProperties> layers(amountOfLayers);
    vkEnumerateInstanceLayerProperties(&amountOfLayers, layers.data());



    uint32_t amountOfExtensions = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, nullptr);
    std::vector<VkExtensionProperties> extensions(amountOfExtensions);
    vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, extensions.data());

#ifdef GEO_DEBUG_STATS
    std::cout << "Amount of Extensions: " << amountOfExtensions << std::endl;
    for (const auto& ext : extensions) {
        std::cout << ext.extensionName << std::endl;
    }
#endif

    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &info;
    instanceCreateInfo.enabledExtensionCount = amountOfSdlExtentions;
    instanceCreateInfo.ppEnabledExtensionNames = sdlExtentions.data();
#ifdef GEO_DEBUG
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    instanceCreateInfo.enabledLayerCount = validationLayers.size();
    instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#else
    instanceCreateInfo.enabledLayerCount = 0;
    instanceCreateInfo.ppEnabledLayerNames = nullptr;
#endif

    result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
    VK_ASSERT(result);

    VkSurfaceKHR surface;
    SDL_Vulkan_CreateSurface(window, instance, &surface);

#ifdef GEO_STATUS_NOTIFICATIONS
    std::cout << "#> Application ready!" << std::endl;
#endif

    deviceManager = std::make_shared<DeviceManager>(instance);
    deviceManager->setup();

    graphics = std::make_shared<Graphics>(instance, surface, deviceManager);
    graphics->setup();
}

void geo::Application::shutdown() {
    deviceManager->waitForAllDevices();

    graphics->shutdown();
    graphics.reset();

    deviceManager->shutdown();
    deviceManager.reset();

    vkDestroyInstance(instance, nullptr);

    SDL_DestroyWindow(window);
    SDL_Quit();
}

geo::DeviceManager& geo::Application::getDeviceManager() const {
    return *deviceManager;
}

bool geo::Application::run() {

    SDL_Event event;

    SDL_Delay(200);

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_EventType::SDL_QUIT) {
            return false;
        }
    }
    auto start = std::chrono::high_resolution_clock::now();
    graphics->draw();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "FPS: " << 1000000.0 / duration.count() << std::endl;

    return true;
}

