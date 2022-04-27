#include <thread>
#include <chrono>
#include <atomic>

#include <JuceHeader.h>

#include "settings.h"
#include "helpers.h"
#include "components.h"

// todo: move to separate files
// todo: size animation in disappear()
class NotificationWindow : public juce::DocumentWindow, public juce::Button::Listener
{
public:
  NotificationWindow(const juce::String& name, unsigned tickIntervalMs = 20u)
    : juce::DocumentWindow(name, juce::Colours::lightgrey, false),
      tickInterval{tickIntervalMs}
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
          std::this_thread::sleep_for(std::chrono::milliseconds{this->tickInterval});
        }
      }
    );
  }

  ~NotificationWindow()
  {
    if (tickThread)
    {
      m_stopTickThread = true;
      tickThread->join();
    }
  }

  void tick() // for animation
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

  void closeButtonPressed() override
  {
    disappear(500); // todo: from config
  }

  void buttonClicked(juce::Button*) override
  {
  }

  void disappear(const unsigned milliseconds)
  {
    const unsigned nTicks = milliseconds / tickInterval;
    if (nTicks == 0)
    {
      setAlpha(0.0f);
      return;
    }
    alphaAnimationDelta = -(getAlpha() / static_cast<float>(nTicks));
  }

private:
  std::unique_ptr<std::thread> tickThread = nullptr;
  std::atomic<bool> m_stopTickThread = false;
  // must: guard for these values or make it atomic too
  unsigned tickInterval = 20u;
  float alphaAnimationDelta = 0.0f;
  juce::Component::SafePointer<juce::Label> label;
};

// -- main class -- //

class NiceUiApp  : public juce::JUCEApplication
{
public:
  NiceUiApp()
    {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    void initialise (const juce::String&) override
    {
      if (!md::Settings::LoadSettings())
      {
        // log << message
        // maybe quit or show alert window
      }
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override
    {
      mainWindow = nullptr;
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    // -- main window class -- //

    class MainWindow : public juce::DocumentWindow, public juce::Button::Listener
    {
    public:
      MainWindow(const juce::String& name)
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
          backgroundColor.g,backgroundColor.b));
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

      void closeButtonPressed() override
      {
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
      }

      void paint(juce::Graphics& g) override
      {
        g.fillAll(this->getBackgroundColour());
        for (const auto& bar : backgroundLayout)
        {
          bar.Draw(g);
        }
        hSplitLine.Draw(g);
        vSplitLine.Draw(g);
      }

      void buttonClicked(juce::Button* b) override
      {
        if (b == &button)
        {
          if (notificationWindow) {
            delete notificationWindow;
          }
          notificationWindow = new NotificationWindow{"Notification"}; // todo: from config
          addAndMakeVisible(*notificationWindow);
          notificationWindow->setBounds(50, 526, 300, 200); // todo: from config
          notificationWindow->setVisible(true);
          notificationWindow->toFront(true);
        }
        if (b == &closeButton)
        {
          quit();
        }
      }

      ~MainWindow()
      {
        button.removeListener(dynamic_cast<juce::Button::Listener*>(this));
        closeButton.removeListener(dynamic_cast<juce::Button::Listener*>(this));
        notificationWindow = nullptr;
      }

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
    };


    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (NiceUiApp)
