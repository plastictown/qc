#include "notification-window.h"

NotificationWindow::NotificationWindow(const juce::String& name, unsigned tickIntervalMs)
  : juce::DocumentWindow(name, juce::Colours::lightgrey, false),
  tickInterval{ tickIntervalMs }
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
    }
    setAlpha(newAlpha);
  }
}

void NotificationWindow::closeButtonPressed()
{
  disappear(500); // todo: from config
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
  alphaAnimationDelta = -(getAlpha() / static_cast<float>(nTicks));
}