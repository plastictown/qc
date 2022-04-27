#include "main-window.h"

MainWindow::MainWindow(const juce::String& name)
  : juce::DocumentWindow(name, juce::Colours::lightgrey, 0) // random color, no buttons
{
  static constexpr const int mainWindowDefaultWidth = 500;
  static constexpr const int mainWindowDefaultHeight = 830;

  const auto backgroundColor = md::Settings::GetRgbFromPath("MainWindow.BackgroundColour");
  backgroundLayout = {
    md::FixedBarComponent("MainWindow.FixedLayout.Header"),
    md::FixedBarComponent("MainWindow.FixedLayout.LeftBar"),
    md::FixedBarComponent("MainWindow.FixedLayout.CenterBar"),
    md::FixedBarComponent("MainWindow.FixedLayout.RightBar"),
    md::FixedBarComponent("MainWindow.FixedLayout.Footer")
  };

  // -- main window -- //
  centreWithSize(md::Settings::GetValue("MainWindow.width", mainWindowDefaultWidth),
    md::Settings::GetValue("MainWindow.height", mainWindowDefaultHeight));
  setBackgroundColour(juce::Colour::fromRGB(backgroundColor.r,
    backgroundColor.g, backgroundColor.b));
  setUsingNativeTitleBar(false);
  setTitleBarHeight(0);
  setResizable(false, false);

  // -- button -- //
  const juce::Colour buttonColor = (JColourFromRgb(md::Settings::GetRgbFromPath(
    "MainWindow.FixedLayout.NotificationButton.BackgroundColour")));
  button.setBounds(JRectFromBox(md::Settings::GetFixedBoxFromPath(
    "MainWindow.FixedLayout.NotificationButton.Box")));
  button.setButtonText(md::Settings::GetStringFromPath("MainWindow.FixedLayout.NotificationButton.Text"));
  button.setColour(juce::TextButton::buttonColourId, buttonColor);
  button.setColour(juce::TextButton::buttonOnColourId, buttonColor);
  button.addListener(dynamic_cast<juce::Button::Listener*>(this));
  addAndMakeVisible(button);

  // -- close button -- //
  closeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
  closeButton.setButtonText("X");
  closeButton.addListener(dynamic_cast<juce::Button::Listener*>(this));
  closeButton.setBounds(486, 0, 14, 14); // todo: from config if needed
  addAndMakeVisible(closeButton);

  // -- images -- //
  if (orangeCircleImage.LoadImage("MainWindow.FixedLayout.OrangeCircleIcon"))
  {
    addAndMakeVisible(orangeCircleImage.GetComponent());
  }
  if (logoImage.LoadImage("MainWindow.FixedLayout.IqOptionLogo"))
  {
    addAndMakeVisible(logoImage.GetComponent());
  }

  // -- lines -- //
  md::Rgb linesColorRaw = md::Settings::GetRgbFromPath("MainWindow.FixedLayout.SplitLinesColour");
  hSplitLine.SetColour(linesColorRaw);
  vSplitLine.SetColour(linesColorRaw);
  hSplitLine.CoordsFromPath("MainWindow.FixedLayout.HorizontalSplitLine");
  vSplitLine.CoordsFromPath("MainWindow.FixedLayout.VerticalSplitLine");

  setVisible(true);
}

void MainWindow::closeButtonPressed()
{
  juce::JUCEApplication::getInstance()->systemRequestedQuit();
}

void MainWindow::paint(juce::Graphics& g)
{
  g.fillAll(this->getBackgroundColour());
  for (const auto& bar : backgroundLayout)
  {
    bar.Draw(g);
  }
  hSplitLine.Draw(g);
  vSplitLine.Draw(g);
}

void MainWindow::buttonClicked(juce::Button* b)
{
  if (b == &button)
  {
    if (notificationWindow) {
      delete notificationWindow;
    }
    notificationWindow = new NotificationWindow{ "Notification" }; // todo: from config
    addAndMakeVisible(*notificationWindow);
    notificationWindow->setBounds(50, 526, 300, 200); // todo: from config
    notificationWindow->setVisible(true);
    notificationWindow->toFront(true);
  }
  if (b == &closeButton)
  {
    juce::JUCEApplicationBase::quit();
  }
}

MainWindow::~MainWindow()
{
  button.removeListener(dynamic_cast<juce::Button::Listener*>(this));
  closeButton.removeListener(dynamic_cast<juce::Button::Listener*>(this));
  notificationWindow = nullptr;
}