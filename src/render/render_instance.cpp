#include "render_instance.h"
#include <SDL_vulkan.h>
#include <spdlog/spdlog.h>
#include <vector>
#include <stdexcept>

namespace
{

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    auto message = std::string("Vulkan message ") + pCallbackData->pMessage;
    switch (messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            spdlog::error(message);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            spdlog::warn(message);
            break;
        default:
            spdlog::info(message);
    }
    return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

}

RenderInstance::RenderInstance(SDL_Window *window, bool enableValidation)
    : _sdlWindow(window)
{
    init(enableValidation);
}

RenderInstance::~RenderInstance()
{
    destroy();
}

VkInstance RenderInstance::getVkInstance() const
{
    return _vkInstance;
}

VkSurfaceKHR RenderInstance::getVkSurface() const
{
    return _vkSurface;
}

VkAllocationCallbacks* RenderInstance::allocator()
{
    return nullptr;
}

SDL_Window* RenderInstance::getSdlWindow()
{
    return _sdlWindow;
}

void RenderInstance::init(bool enableValidation)
{
    initVkInstance(enableValidation);
    if (enableValidation)
    {
        setupDebugMessenger();
    }
    createSurface();
}

void RenderInstance::initVkInstance(bool enableValidation)
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan demo";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 2, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 2, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;
    
    std::vector<const char*> extensions;
    std::vector<const char*> layers;
    if (enableValidation)
    {
        layers.push_back("VK_LAYER_KHRONOS_validation");
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    
    unsigned int sdlExtensionsCount = 0;
    if (!SDL_Vulkan_GetInstanceExtensions(_sdlWindow, &sdlExtensionsCount, nullptr))
    {
        throw std::runtime_error("Could not get required extensions from sdl");
    }
    
    size_t additionalExtensionsCount = extensions.size();
    extensions.resize(additionalExtensionsCount + sdlExtensionsCount);
    if (!SDL_Vulkan_GetInstanceExtensions(_sdlWindow, &sdlExtensionsCount, extensions.data() + additionalExtensionsCount))
    {
        throw std::runtime_error("Could not get required extensions from sdl");
    }

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    createInfo.enabledLayerCount = layers.size();
    createInfo.ppEnabledLayerNames = layers.data();
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidation)
    {
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
    }
    
    if (vkCreateInstance(&createInfo, allocator(), &_vkInstance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance!");
    }
}

void RenderInstance::setupDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);
    if (createDebugUtilsMessengerEXT(_vkInstance, &createInfo, allocator(), &_vkDebugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("Failed to set up debug messenger!");
    }
}

void RenderInstance::createSurface()
{
    if (!SDL_Vulkan_CreateSurface(_sdlWindow, _vkInstance, &_vkSurface))
    {
        throw std::runtime_error("Cannot create vulkan surface from sdl");
    }
}

void RenderInstance::destroy()
{
    if (_vkSurface != VK_NULL_HANDLE)
    {
        // because allocator not provided to sdl2, destroying dont use allocators too
        vkDestroySurfaceKHR(_vkInstance, _vkSurface, nullptr);
        _vkSurface = VK_NULL_HANDLE;
    }

    if (_vkDebugMessenger != VK_NULL_HANDLE)
    {
        destroyDebugUtilsMessengerEXT(_vkInstance, _vkDebugMessenger, allocator());
        _vkDebugMessenger = VK_NULL_HANDLE;
    }
    
    if (_vkInstance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(_vkInstance, allocator());
        _vkInstance = VK_NULL_HANDLE;
    }
}
