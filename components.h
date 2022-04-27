#pragma once
#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

#include <JuceHeader.h>
#include "settings.h"
#include "helpers.h"

namespace md
{

  struct IDrawable
  {
  public:
    virtual void Draw(juce::Graphics&) const;
  };

  class FixedBarComponent
  {
  public:
    FixedBarComponent();
    ~FixedBarComponent();
    FixedBarComponent(const std::string& path);
    void FromSettings(const std::string& path);
    const md::FixedBar& GetData() const;
    void Draw(juce::Graphics& g) const;
  private:
    md::FixedBar rawData;
  };

  class LineComponent
  {
  public:
    void SetColour(const juce::Colour& c);
    void SetColour(const md::Rgb& rgb);
    void CoordsFromPath(const std::string& path);
    void Draw(juce::Graphics& g) const;
  private:
    md::Line coords;
    juce::Colour colour;
  };

} // namespace md

#endif // !__COMPONENTS_H__
