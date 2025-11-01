#pragma once							// PlayerAudio.h
#include <JuceHeader.h>

class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    bool loadFile(const juce::File& file);
    void start();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;
	float getGain() const;
    void toggleLooping();
	bool isLooping() const; 
    bool isRunning()const;
	void toggleRunning();
    void setPrevGain(float x);
	void toggleMute();
    bool isMuted();
	float getPrevGain() const;
    void setPlaybackSpeed(double ratio);
    double getCurrentSpeed() const;
private:
    
    bool looping = false;
    bool running = true;
    float previousGain = 0.5f;
    bool muted = false;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource , false };

    // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};