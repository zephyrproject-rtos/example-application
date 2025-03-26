// managerFactory.cpp
#include "managerFactory.hpp"

std::unordered_map<std::string, managerFactory::ManagerCreator>& managerFactory::registry()
{
    static std::unordered_map<std::string, ManagerCreator> instance;
    return instance;
}

void managerFactory::registerManager(const std::string& type, ManagerCreator creator)
{
    registry()[type] = creator;
}

iManager* managerFactory::create(const std::string& type)
{
    auto it = registry().find(type);
    if (it != registry().end()) {
        return it->second();
    }
    return nullptr;
}
