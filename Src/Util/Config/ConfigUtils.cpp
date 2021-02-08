#include "Src/Util/Config/ConfigUtils.hpp"

namespace ConfigUtils
{
void ConfigLoader::LoadConfig()
{
  tinyxml2::XMLDocument doc;
  doc.LoadFile("Config.xml");

  tinyxml2::XMLElement *pRoot = doc.FirstChildElement();

  if (pRoot != nullptr)
  {
    for (tinyxml2::XMLElement *e = pRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
      GetVariantValueAndInsertToMap(e, aConfigMap);
    }
  }
}

void ConfigLoader::GetVariantValueAndInsertToMap(tinyxml2::XMLElement *xmlelemnt, std::unordered_map<std::string, VariantType> &aConfigMap)
{
  VariantType value;
  std::string elementType = xmlelemnt->FirstChildElement("Type")->GetText();
  std::string elementName = xmlelemnt->FirstChildElement("Name")->GetText();

  if (elementType == "I")
  {
    value = xmlelemnt->FirstChildElement("Value")->IntText();
  }
  else if (elementType == "U")
  {
    value = xmlelemnt->FirstChildElement("Value")->UnsignedText();
  }
  else if (elementType == "F")
  {
    value = xmlelemnt->FirstChildElement("Value")->FloatText();
  }
  else if (elementType == "D")
  {
    value = xmlelemnt->FirstChildElement("Value")->DoubleText();
  }
  else if (elementType == "S")
  {
    value = xmlelemnt->FirstChildElement("Value")->GetText();
  }

  if (aConfigMap.find(elementName) == aConfigMap.end())
  {
    aConfigMap[elementName] = value;
  }
  else
  {
    printf("Element already in map change Name !! \n");
  }
}

}