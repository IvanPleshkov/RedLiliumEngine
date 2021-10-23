#pragma once

#include <string>
#include <string_view>

class ResourcesManager
{
public:
    ResourcesManager();
    
    ~ResourcesManager();
    
    std::string readResourceData(std::string_view resource);
    
private:
    std::string _path;
};
