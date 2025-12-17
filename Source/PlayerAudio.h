#pragma once							// PlayerAudio.h
#include <JuceHeader.h>

class PlayerAudio : public juce::ChangeListener
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

    void addToPlaylist(const juce::Array<juce::File>& files);  
    void playNext();                                         
    void playPrevious();                                        
    int getCurrentIndex() const;                              
    juce::File getCurrentFile() const;
    std::function<void(const juce::File&)> onFileChanged;

    void setLoopA(double var = 0);
	void setLoopB(double var = 0);
    double getLoopA();
	double getLoopB();
	void toggleABLooping();
	bool isABLooping() const;
    void setPlaybackSpeed(double ratio);
    double getCurrentSpeed() const; 
    juce::AudioThumbnail& getThumbnail() { return thumbnail; }
    void toggleA();
	void toggleB();
	bool isAOn() const;
	bool isBOn() const;

    void addMarker(double pos);      
    void removeMarker(int index);    
    void clearMarkers();              
    double getMarker(int index) const; 
    int getNumMarkers() const;        

    juce::String getLastLoadedFilePath() const; 
    void setPlaybackPositionInSeconds(double pos);
    double getPlaybackPositionInSeconds() const;
    void PlayerAudio::setMarker(double pos);
    

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    
private:
	double loopStart = -1.0;
	bool AisOn = false;
	bool BisOn = false;
	double loopEnd = -1.0;
	bool isABLoopingvar = false;
    bool looping = false;
    bool running = true;
    float previousGain = 0.5f;
    bool muted = false;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    double markerPosition = -1.0; 
    juce::Array<juce::File> playlist;  
    int currentIndex = -1; 
    juce::Array<double> markers;

  


    juce::ResamplingAudioSource resampleSource{ &transportSource , false };

    juce::AudioThumbnailCache thumbnailCache{ 5 };
    juce::AudioThumbnail thumbnail{ 512, formatManager, thumbnailCache };
    juce::File lastLoadedFile;
    // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};