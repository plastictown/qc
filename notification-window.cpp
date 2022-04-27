#include "notification-window.h"
#include "settings.h"

NotificationWindow::NotificationWindow(const juce::String& name, unsigned tickIntervalMs)
  : juce::DocumentWindow(name, juce::Colours::lightgrey, false),
  tickInterval{tickIntervalMs} // use default
{
  setBackgroundColour(juce::Colour::fromRGB(25, 32, 45)); // todo: from config
  setUsingNativeTitleBar(false);
  setTitleBarHeight(34); // todo: from config
  setResizable(false, false);
  setTitleBarTextCentred(false);

  setTitleBarButtonsRequired(juce::DocumentWindow::closeButton, false); // todo: style

  //--- remove this --- //
  label = new juce::Label("dummyName", "NEW VIDEO TUTORIAL!");
  label->setFont(juce::Font(14.0f, juce::Font::bold));
  label->setColour(juce::Label::textColourId, juce::Colours::white);
  label->setJustificationType(juce::Justification::centred);
  label->setBounds(65, 100, 170, 16);
  addAndMakeVisible(label);

  // -- animation loop -- //
  tickThread = std::make_unique<std::thread>(
    [this]()
    {
      while (!m_stopTickThread && getAlpha() != 0.0f)
      {
        tick();
        std::this_thread::sleep_for(std::chrono::milliseconds{ this->tickInterval });
      }
    }
  );

  // from config or default
  disappearTimeMs = md::Settings::GetValue<unsigned>("Animation.disappearTimeMs", disappearTimeMs);

}

NotificationWindow::~NotificationWindow()
{
  if (tickThread)
  {
    m_stopTickThread = true;
    tickThread->join();
  }
}

void NotificationWindow::tick() // for animation
{
  if (alphaAnimationDelta != 0.0f)
  {
    float newAlpha = getAlpha() + alphaAnimationDelta;
    if (newAlpha <= 0.0f || newAlpha >= 1.0f)
    {
      alphaAnimationDelta = 0.0f; // stop animation
      xAnimationDelta = 0.0f;
      yAnimationDelta = 0.0f;
    }
    setAlpha(newAlpha);
    const auto bounds = getBounds().toFloat();
    setBounds(bounds.withLeft(bounds.getX() + xAnimationDelta).withTop(bounds.getY() + yAnimationDelta).toNearestInt());
  }
}

void NotificationWindow::closeButtonPressed()
{
  disappear(disappearTimeMs); // todo: from config
}

void NotificationWindow::buttonClicked(juce::Button*)
{
  // stub for juce::Button::Listener abstract class
}

void NotificationWindow::disappear(const unsigned milliseconds)
{
  const unsigned nTicks = milliseconds / tickInterval;
  if (nTicks == 0)
  {
    setAlpha(0.0f);
    return;
  }
  const auto currentRect = getBounds();
  const float widthDecrease = currentRect.getWidth() -(currentRect.getWidth() * decreaseSizeAspect);
  const float heightDecrease = currentRect.getHeight() - (currentRect.getHeight() * decreaseSizeAspect);
  xAnimationDelta = widthDecrease / nTicks;
  yAnimationDelta = heightDecrease / nTicks;
  alphaAnimationDelta = -(getAlpha() / static_cast<float>(nTicks));
}