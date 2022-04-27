#pragma once
#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "helpers.h"

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

namespace md
{


  class Settings : public boost::noncopyable
  {
  public:
    static bool LoadSettings(const fs::path& filepath = settingsFilePath);
    static Rgb GetRgbFromPath(const std::string& path);
    static FixedBox GetFixedBoxFromPath(const std::string& path);
    static FixedBar GetFixedBarFromPath(const std::string& path);
    static std::string GetStringFromPath(const std::string& path);
    static std::string GetResourcePathByFilename(const std::string& filename);
    static Line GetLineByPath(const std::string& path);
    template<typename T>
    static T GetValue(const std::string& valuePath, const T& defaultValue)
    {
      try
      {
        return settings.get_child(valuePath).get_value<T>(defaultValue);
      }
      catch (...)
      {
        // log << message
        return defaultValue;
      }
    }
  private:
    static pt::ptree settings;
    static bool isSettingsLoaded;
    // expected in cwd
    static constexpr const char* const settingsFilePath = "data/settings.json";
  };



} // namespace md

#endif // __SETTINGS_H__