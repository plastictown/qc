#include <string>
#include "components.h"

namespace md
{
  FixedBarComponent::FixedBarComponent()
    :rawData{}
  {};

  FixedBarComponent::~FixedBarComponent()
  {};

  FixedBarComponent::FixedBarComponent(const std::string& path)
    :rawData{ md::Settings::GetFixedBarFromPath(path) }
  {}

  void FixedBarComponent::FromSettings(const std::string& path)
  {
    rawData = md::Settings::GetFixedBarFromPath(path);
  }

  const md::FixedBar& FixedBarComponent::GetData() const
  {
    return rawData;
  }

  void FixedBarComponent::Draw(juce::Graphics& g) const
  {
    juce::Colour c = juce::Colour::fromRGB(
      rawData.colour.r,
      rawData.colour.g,
      rawData.colour.b
    );
    g.setColour(c);
    g.fillRect(juce::Rectangle<int>(
      rawData.box.left,
      rawData.box.top,
      rawData.box.width,
      rawData.box.height
      ));
  }

  // ----------------------------------------------------------------------- //

  void LineComponent::SetColour(const juce::Colour& c)
  {
    colour = c;
  }

  void LineComponent::SetColour(const md::Rgb& rgb)
  {
    colour = juce::Colour::fromRGB(rgb.r, rgb.g, rgb.b);
  }

  void LineComponent::CoordsFromPath(const std::string& path)
  {
    coords = md::Settings::GetLineByPath(path);
  }

  void LineComponent::Draw(juce::Graphics& g) const
  {
    g.setColour(colour);
    g.drawLine(coords.ax, coords.ay, coords.bx, coords.by);
  }

} // namespace md