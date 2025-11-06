#include "PlayerAudio.h"
#include <JuceHeader.h>
PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
    markers.clear();     
    AisOn = false;       
    BisOn = false;
    isABLoopingvar = false;
    loopStart = -1.0;
    loopEnd = -1.0;
}
PlayerAudio::~PlayerAudio()
{
}
void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
    
    if (looping)
    {
        if (transportSource.hasStreamFinished())
        {
			transportSource.setPosition(0.0);
			transportSource.start();
        }
    }
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}
void PlayerAudio::setPlaybackSpeed(double ratio)
{
    resampleSource.setResamplingRatio(ratio);
}
bool PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
           
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();
            
            thumbnail.setSource(new juce::FileInputSource(file));
            
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
           
            // add update to last file
            lastLoadedFile = file;
            // Attach safely
            transportSource.setSource(readerSource.get(),
                0,
                nullptr,
                reader->sampleRate);
            transportSource.start();
        }
    }
    return true;
}
void PlayerAudio::start()
{
    transportSource.start();
}
void PlayerAudio::stop()
{
    transportSource.stop();
}
void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}
void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}
double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}
double PlayerAudio::getCurrentSpeed() const
{
    double normalPosition = transportSource.getCurrentPosition();
    double speedRatio = resampleSource.getResamplingRatio();
    return resampleSource.getResamplingRatio();
}
double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}
void PlayerAudio::toggleLooping()
{
    looping = !looping;
}
bool PlayerAudio::isLooping() const
{
    return looping;
}
bool PlayerAudio::isRunning() const
{
    return transportSource.isPlaying();
}
void PlayerAudio::toggleRunning()
{
    if (isRunning())
    {
        stop();
    }
    else
    {
        start();
    }
}
void PlayerAudio::setPrevGain(float x)
{
    previousGain = x;
}
void PlayerAudio::toggleMute()
{
	muted = !muted;
}
bool PlayerAudio::isMuted()
{
    return muted;
}
float PlayerAudio::getPrevGain() const
{
    return previousGain;
}
float PlayerAudio::getGain() const
{
    return transportSource.getGain();
}



void PlayerAudio::addToPlaylist(const juce::Array<juce::File>& files)
{
    for (auto& file : files)
        playlist.add(file);

    if (currentIndex == -1 && playlist.size() > 0)
    {
        currentIndex = 0 ; 
    }
}

void PlayerAudio::playNext()
{
    if (playlist.isEmpty())
        return;

    currentIndex++;
    if (currentIndex >= playlist.size())
        currentIndex = 0; 

    loadFile(playlist[currentIndex]);
    start();
    if (onFileChanged)
        onFileChanged(playlist[currentIndex]);
}

void PlayerAudio::playPrevious()
{
    if (playlist.isEmpty())
        return;

    currentIndex--;
    if (currentIndex < 0)
        currentIndex = playlist.size() - 1; 

    loadFile(playlist[currentIndex]);
    start();
    if (onFileChanged)
        onFileChanged(playlist[currentIndex]);
}

int PlayerAudio::getCurrentIndex() const
{
    return currentIndex;
}

juce::File PlayerAudio::getCurrentFile() const
{
    if (currentIndex >= 0 && currentIndex < playlist.size())
        return playlist[currentIndex];

    return juce::File(); 
}





void PlayerAudio::setLoopA(double var )
{
    if (var == 0)
        loopStart = getPosition();
    else
        loopStart = var;
}
void PlayerAudio::setLoopB(double var )
{
	if (var == 0)
        loopEnd = getPosition();
    else
		loopEnd = var;
    
}
void PlayerAudio::toggleABLooping()
{
	isABLoopingvar = !isABLoopingvar;
}
bool PlayerAudio::isABLooping() const
{
    return isABLoopingvar;
}
double PlayerAudio::getLoopA()
{
    return loopStart;
}
double PlayerAudio::getLoopB()
{
    return loopEnd;
}
void PlayerAudio::toggleA()
{
    AisOn = !AisOn;
}
bool PlayerAudio::isAOn() const
{
    return AisOn;
}
void PlayerAudio::toggleB()
{
    BisOn = !BisOn;
}
bool PlayerAudio::isBOn() const
{
	return BisOn;
}
void PlayerAudio::addMarker(double pos)
{
    markers.add(pos);
}

juce::String PlayerAudio::getLastLoadedFilePath() const
{
    return lastLoadedFile.getFullPathName();
}

void PlayerAudio::setPlaybackPositionInSeconds(double pos)
{
    transportSource.setPosition(pos);
}

double PlayerAudio::getPlaybackPositionInSeconds() const
{
    return transportSource.getCurrentPosition();
}
void PlayerAudio::setMarker(double pos) 
{ 
    markerPosition = pos;
}
void PlayerAudio::removeMarker(int index) 
{ 
    if (index >= 0 && index < markers.size()) 
        markers.remove(index);
}
void PlayerAudio::clearMarkers() 
{
    markers.clear(); 
}
double PlayerAudio::getMarker(int index) const 
{ 
    return (index >= 0 && index < markers.size()) ? markers[index] : -1.0; 
}
int PlayerAudio::getNumMarkers() const 
{
    return markers.size();
}


