#include "settings.h"
#include "main-window.h"

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

    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (NiceUiApp)
