#pragma once
#ifndef __NOTIFICATION_WINDOW_H__
#define __NOTIFICATION_WINDOW_H__

#include <thread>
#include <chrono>
#include <atomic>

#include <JuceHeader.h>

// todo: size animation in disappear()
class NotificationWindow : public juce::DocumentWindow, public juce::Button::Listener
{
public:
  NotificationWindow(const juce::String& name, unsigned tickIntervalMs = 20u /*50 fps*/);
  ~NotificationWindow();
  void tick(); // for animation
  void closeButtonPressed() override;
  void buttonClicked(juce::Button*) override;
  void disappear(const unsigned milliseconds);

private:
  std::unique_ptr<std::thread> tickThread = nullptr;
  std::atomic<bool> m_stopTickThread = false;
  unsigned disappearTimeMs = 500u;
  // must: guard for these values or make it atomic too
  unsigned tickInterval = 20u;
  float alphaAnimationDelta = 0.0f;
  float xAnimationDelta;
  float yAnimationDelta;
  float decreaseSizeAspect = 0.5f;
  juce::Component::SafePointer<juce::Label> label;
};


#endif // __NOTIFICATION_WINDOW_H__