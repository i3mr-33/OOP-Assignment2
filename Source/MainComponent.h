#pragma once						// MainComponent.h
#include <JuceHeader.h>
#include "PlayerGUI.h"
#include "CustomLookAndFeel.h"

class MainComponent : public juce::AudioAppComponent,
                      public juce::Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
	void paint(juce::Graphics& g) override;
private:
    PlayerGUI player1;
	PlayerGUI player2;
	CustomLookAndFeel customLookAndFeel;

	float masterVolume = 1.0f;
	float lastmasterVolume = 1.0f;
    juce::Slider masterVolumeSlider;
    juce::Label masterVolumeLabel;
    juce::File getSessionSaveFile();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
