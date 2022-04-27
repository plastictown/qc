#include "settings.h"

namespace md
{
  pt::ptree Settings::settings{};
  bool Settings::isSettingsLoaded{false};

  bool Settings::LoadSettings(const fs::path& filepath)
  {
    isSettingsLoaded = false;
    const fs::path jsonPath = fs::current_path() / filepath;
    if (!fs::exists(jsonPath))
    {
      // log << message
      return false;
    }

    try
    {
      pt::read_json(jsonPath.string(), settings);
    }
    catch (boost::system::error_code&)
    {
      return isSettingsLoaded;
    }
    isSettingsLoaded = true;
    return isSettingsLoaded;
  }

  Rgb Settings::GetRgbFromPath(const std::string& path)
  {
    return Rgb{
      GetValue(path + ".r", 0),
      GetValue(path + ".g", 0),
      GetValue(path + ".b", 0)
    };
  }

  FixedBox Settings::GetFixedBoxFromPath(const std::string& path)
  {
    return FixedBox{
      GetValue(path + ".left", 0),
      GetValue(path + ".top", 0),
      GetValue(path + ".width", 0),
      GetValue(path + ".height", 0)
    };
  }

  FixedBar Settings::GetFixedBarFromPath(const std::string& path)
  {
    return FixedBar{
      GetRgbFromPath(path + ".BackgroundColour"),
      GetFixedBoxFromPath(path + ".Box")
    };
  }

  std::string Settings::GetStringFromPath(const std::string& path)
  {
    return GetValue<std::string>(path, std::string{});
  }

  std::string Settings::GetResourcePathByFilename(const std::string& filename)
  {
    const fs::path resourcePath = fs::current_path() / "data" / filename;
    if (!fs::exists(resourcePath))
    {
      // log << message
      return std::string{};
    }
    return resourcePath.string();
  }

  Line Settings::GetLineByPath(const std::string& path)
  {
    return Line{
      GetValue(path + ".ax", 0),
      GetValue(path + ".ay", 0),
      GetValue(path + ".bx", 0),
      GetValue(path + ".by", 0)
    };
  }
} // namespace md