// managerFactory.hpp
#pragma once
#include <functional>
#include <unordered_map>
#include <string>
#include "iManager.hpp"

class managerFactory
{
public:
    using ManagerCreator = std::function<iManager*()>;

    static void registerManager(const std::string& type, ManagerCreator creator);
    static iManager* create(const std::string& type);

private:
    static std::unordered_map<std::string, ManagerCreator>& registry();
};