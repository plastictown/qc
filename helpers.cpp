#include "helpers.h"
#include "settings.h"

namespace md
{
  juce::Rectangle<int> JRectFromBox(const md::FixedBox& box)
  {
    return juce::Rectangle<int>(box.left, box.top, box.width, box.height);
  }

  juce::Colour JColourFromRgb(const md::Rgb& rgb)
  {
    return juce::Colour::fromRGB(rgb.r, rgb.g, rgb.b);
  }

  bool Image::LoadImage(const std::string& path)
  {
    const std::string imagePath = md::Settings::GetResourcePathByFilename(
      md::Settings::GetStringFromPath(path + ".filename"));

    if (imagePath.empty())
    {
      return false;
    }
    image = juce::ImageFileFormat::loadFrom(juce::File::File(imagePath));
    if (!image.isValid())
    {
      return false;
    }
    component.setImage(image);
    component.setBounds(JRectFromBox(md::Settings::GetFixedBoxFromPath(path + ".Box")));
    return true;
  }

  juce::ImageComponent& Image::GetComponent()
  {
    return component;
  }

  const juce::ImageComponent& Image::GetComponent() const
  {
    return component;
  }
}