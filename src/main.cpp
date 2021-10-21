#include <SDL.h>
#include <spdlog/spdlog.h>

#include "render/render_instance.h"
#include "render/render_device.h"

int main(int argc, char *args[])
{
    spdlog::info("Start application");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Window *window = SDL_CreateWindow("vulkan demo", -1, -1, 640, 480, SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

    RenderInstance renderInstance(window, true);
    RenderDevice renderDevice(renderInstance);
    
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT: quit = true; break;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    
    spdlog::info("End application");
    return 0;
}
