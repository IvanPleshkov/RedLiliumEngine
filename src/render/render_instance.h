#pragma once

#include <vulkan/vulkan.h>
#include <SDL.h>

class RenderInstance
{
public:
    RenderInstance(SDL_Window *window, bool enableValidation);
    
    ~RenderInstance();

    VkInstance getVkInstance() const;
    
    VkSurfaceKHR getVkSurface() const;
    
    VkAllocationCallbacks* allocator();
    
private:
    void init(SDL_Window *window, bool enableValidation);
    
    void initVkInstance(SDL_Window *window, bool enableValidation);
    
    void setupDebugMessenger();

    void createSurface(SDL_Window *window);
    
    void destroy();
    
    VkInstance _vkInstance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT _vkDebugMessenger = VK_NULL_HANDLE;
    VkSurfaceKHR _vkSurface = VK_NULL_HANDLE;
};
