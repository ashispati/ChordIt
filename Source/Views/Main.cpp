#include "../JuceLibraryCode/JuceHeader.h"
#include "MainWindow.h"

class ChordItApplication  : public JUCEApplication
{
public:
    ChordItApplication() {
        Desktop::getInstance().setOrientationsEnabled(Desktop::rotatedClockwise);
    }

    ~ChordItApplication () {}
    

    void initialise (const String& ) override {
        mainWindow = new MainAppWindow();
    }

    void shutdown() override {
        mainWindow = 0;
    }

    void systemRequestedQuit() override {
        quit();
    }

    
    const String getApplicationName() override {
        return "ChordIt";
    }
    
    const String getApplicationVersion() override {
        return ProjectInfo::versionString;
    }
    
    
    bool moreThanOneInstanceAllowed() override {
        return true;
    }
    
    
    void anotherInstanceStarted (const String& commandLine) override {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    
private:
    ScopedPointer<MainAppWindow> mainWindow;
    
 
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (ChordItApplication)
