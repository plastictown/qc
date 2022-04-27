#pragma once
#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <JuceHeader.h>

namespace md
{
  struct Rgb
  {
    unsigned char r, g, b;
  };

  struct FixedBox
  {
    int left;
    int top;
    int width;
    int height;
  };

  struct FixedBar {
    Rgb colour;
    FixedBox box;
  };

  struct Line {
    int ax, ay, bx, by;
  };

  juce::Rectangle<int> JRectFromBox(const md::FixedBox& box);
  juce::Colour JColourFromRgb(const md::Rgb& rgb);

  class Image
  {
  public:
    bool LoadImage(const std::string& path);
    juce::ImageComponent& GetComponent();
    const juce::ImageComponent& GetComponent() const;
  private:
    juce::Image image;
    juce::ImageComponent component;
  };

} // namespace md
#endif // __HELPERS_H__