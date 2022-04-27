#pragma once
#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <JuceHeader.h>
#include "settings.h"
#include "helpers.h"
#include "components.h"
#include "notification-window.h"

class MainWindow : public juce::DocumentWindow, public juce::Button::Listener
{
public:
  MainWindow(const juce::String& name);
  void closeButtonPressed() override;
  void paint(juce::Graphics& g) override;
  void buttonClicked(juce::Button* b) override;
  ~MainWindow();

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow);
  std::vector<md::FixedBarComponent> backgroundLayout;
  md::LineComponent hSplitLine;
  md::LineComponent vSplitLine;
  juce::TextButton button;
  juce::TextButton closeButton;
  md::Image orangeCircleImage;
  md::Image logoImage;
  // todo: make queue of NotificationWindows
  DocumentWindow::SafePointer<NotificationWindow> notificationWindow;
  unsigned animationTickIntervalMs = 20u; // 50 fps
};

#endif // !__MAIN_WINDOW_H__
