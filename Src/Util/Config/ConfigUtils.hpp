#pragma once
#include <variant>
#include <tinyxml2.h>
#include <string>
#include <unordered_map>

using VariantType = std::variant<int, unsigned, float, double, std::string>;

namespace ConfigUtils
{
  class ConfigLoader
  {
  public:
    ConfigLoader()
    {
      aConfigMap = std::unordered_map<std::string, VariantType>();
      LoadConfig();
    }

    
    VariantType GetValueFromMap(const std::string &name)
    {
        VariantType value = aConfigMap.at(name);
        return value;
    }

    void LoadConfig();
    void GetVariantValueAndInsertToMap(tinyxml2::XMLElement *xmlelemnt, std::unordered_map<std::string, VariantType> &aConfigMap);

    std::unordered_map<std::string, VariantType> aConfigMap;
  };

} // namespace ConfigUtils