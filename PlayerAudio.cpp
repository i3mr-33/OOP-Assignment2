#include "PlayerAudio.h"
#include <JuceHeader.h>
PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
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
            // 🔑 Disconnect old source first
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            // Create new reader source
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

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