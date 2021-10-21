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
    
    SDL_Window* getSdlWindow();
    
private:
    void init(bool enableValidation);
    
    void initVkInstance(bool enableValidation);
    
    void setupDebugMessenger();

    void createSurface();
    
    void destroy();
    
    VkInstance _vkInstance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT _vkDebugMessenger = VK_NULL_HANDLE;
    VkSurfaceKHR _vkSurface = VK_NULL_HANDLE;
    SDL_Window* _sdlWindow = nullptr;
};
