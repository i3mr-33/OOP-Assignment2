#pragma once						// PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
	public juce::Timer ,
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
    void updateMetaDataLabel(const juce::File& file);

private:
    PlayerAudio playerAudio;

    // GUI elements
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton loopButton{juce::CharPointer_UTF8(u8"➡️")};
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
    juce::TextButton nextButton{ juce:: CharPointer_UTF8(u8"▶️") };
    juce::TextButton prevButton{juce:: CharPointer_UTF8(u8"◀️" )};
    juce::ListBox playlistBox;  
    juce::StringArray playlistItems; 
    juce::TextButton forwardButton{ juce::CharPointer_UTF8(u8"⏩") }; 
    juce::TextButton backwardButton{ juce::CharPointer_UTF8(u8"⏪") };

  

	juce::TextButton setAButton{ "Set A" };
	juce::TextButton setBButton{ "Set B" };
	juce::TextButton abLoopButton{ "A-B Loop" };


    juce::Slider speedSlider;
    std::unique_ptr<juce::FileChooser> fileChooser;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
// test text