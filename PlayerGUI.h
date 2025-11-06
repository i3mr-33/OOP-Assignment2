#pragma once						// PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "CustomLookAndFeel.h"


class MarkerListModel;

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ListBoxModel 
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback()override;
    void positionSliderSetVBounds();
    void loadFile(const juce::File& file);

    
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g,
        int width, int height, bool rowIsSelected) override;

    
    int getNumMarkerRows();
    void paintMarkerListBoxItem(int rowNumber, juce::Graphics& g,
        int width, int height, bool rowIsSelected);

    void updateMetaDataLabelWithTagLib(const juce::File& file);
    std::function<void(const juce::File&)> onFileChanged;
    PlayerAudio& getPlayerAudio() { return playerAudio; }

    void updateMarkerList();
    // shortcuts
    bool keyPressed(const juce::KeyPress& key) override;
    void setupKeyboardShortcuts();
    
private:
    PlayerAudio playerAudio;
    CustomLookAndFeel customLookAndFeel;

    // GUI elements
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton loopButton{ juce::CharPointer_UTF8(u8"➡️") };
    juce::TextButton pauseButton{ juce::CharPointer_UTF8(u8"⏸️") };
    juce::TextButton playButton{ juce::CharPointer_UTF8(u8"▶️") };
    juce::TextButton goToStartButton{ juce::CharPointer_UTF8(u8"⏮️") };
    juce::TextButton goToEndButton{ juce::CharPointer_UTF8(u8"⏭️") };
    juce::TextButton muteButton{ juce::CharPointer_UTF8(u8"🔊") };
    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Label currentTimeLabel;
    juce::Label totalTimeLabel;
    juce::Label test;

    juce::Label metaDataLabel;
    juce::TextButton nextButton{ juce::CharPointer_UTF8(u8"▶️") };
    juce::TextButton prevButton{ juce::CharPointer_UTF8(u8"◀️") };
    juce::ListBox playlistBox;
    juce::Label playlistTitleLabel;
    juce::StringArray playlistItems;
    juce::TextButton forwardButton{ juce::CharPointer_UTF8(u8"⏩") };
    juce::TextButton backwardButton{ juce::CharPointer_UTF8(u8"⏪") };



    juce::TextButton setAButton{ "Set A" };
    juce::TextButton setBButton{ "Set B" };
    juce::TextButton abLoopButton{ "A-B Loop" };
    juce::TextButton markerButton{ "Marker" };
    juce::TextButton removeMarkerButton{ "Remove Marker" };
    juce::ListBox markerListBox;
    juce::Label markerTitleLabel;

    
    std::unique_ptr<MarkerListModel> markerModel;


    juce::Slider speedSlider;
    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::StringArray markerNames;


    int currentlyPlayingRow = -1;

    juce::Colour offcolour;
    // shortcut
    juce::Array<juce::KeyPress> keyPresses;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};