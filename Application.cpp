//
// Created by felix on 12.04.20.
//

#include <iostream>
#include "Application.h"
#include "GlobaleScope.h"

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
    std::vector<VkLayerProperties> layers(amountOfLayers);// = new VkLayerProperties[amountOfLayers];
    vkEnumerateInstanceLayerProperties(&amountOfLayers, layers.data());

    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    uint32_t amountOfExtensions = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, nullptr);
    std::vector<VkExtensionProperties> extensions(amountOfExtensions);
    vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, extensions.data());

    std::cout << "Amount of Extensions: " << amountOfExtensions << std::endl;
    for (const auto& ext : extensions) {
        std::cout << ext.extensionName << std::endl;
    }

    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &info;
    instanceCreateInfo.enabledLayerCount = validationLayers.size();
    instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    instanceCreateInfo.enabledExtensionCount = amountOfSdlExtentions;
    instanceCreateInfo.ppEnabledExtensionNames = sdlExtentions.data();

    result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
    VK_ASSERT(result);

    VkSurfaceKHR surface;
    SDL_Vulkan_CreateSurface(window, instance, &surface);

    deviceManager = std::make_shared<DeviceManager>(instance);
    deviceManager->setup();

    graphics = std::make_shared<Graphics>(instance, surface, deviceManager);
    graphics->setup();
}

void geo::Application::shutdown() {
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

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_EventType::SDL_QUIT) {
            return false;
        }
    }
    return true;
}

