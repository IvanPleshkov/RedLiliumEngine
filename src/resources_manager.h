#pragma once

#include <string>
#include <string_view>
#include <memory>

class Mesh;

class ResourcesManager
{
public:
    ResourcesManager();
    
    ~ResourcesManager();
    
    std::string readResourceData(std::string_view resource);
    
    std::unique_ptr<Mesh> loadObj(std::string_view filename);
    
private:
    std::string _path;
};
