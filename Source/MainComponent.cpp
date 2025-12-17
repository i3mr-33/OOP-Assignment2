 #include "MainComponent.h"


MainComponent::MainComponent()
{
    setLookAndFeel(&customLookAndFeel);
	addAndMakeVisible(player1);
	addAndMakeVisible(player2);
    addAndMakeVisible(masterVolumeSlider);
    masterVolumeSlider.setRange(0.0, 1.0, 0.01);
    masterVolumeSlider.setValue(0.6);
    masterVolumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    masterVolumeSlider.addListener(this);
    addAndMakeVisible(masterVolumeLabel);
    masterVolumeLabel.setText("Master Volume", juce::dontSendNotification);
    masterVolumeLabel.setJustificationType(juce::Justification::centredRight);
    masterVolumeLabel.attachToComponent(&masterVolumeSlider, true);

    lastmasterVolume = (float)masterVolumeSlider.getValue();
    masterVolume = lastmasterVolume;
    auto saveFile = getSessionSaveFile();
    auto savedData = saveFile.loadFileAsString();
    auto lines = juce::StringArray::fromLines(savedData);
    if (lines.size() >= 4)
    {
        // Player 1 Loading
        juce::File file1(lines[0]);
        double pos1 = lines[1].getDoubleValue();
        if (file1.existsAsFile())
        {
            player1.getPlayerAudio().loadFile(file1);
            player1.getPlayerAudio().setPlaybackPositionInSeconds(pos1);
        }
        // Player 2 Loading
        juce::File file2(lines[2]);
        double pos2 = lines[3].getDoubleValue();
        if (file2.existsAsFile())
        {
            player2.getPlayerAudio().loadFile(file2);
            player2.getPlayerAudio().setPlaybackPositionInSeconds(pos2);
        }
    }

    setSize(1000, 2000);
    setAudioChannels(0, 2);
}
MainComponent::~MainComponent()
{
    juce::String stateToSave;
    
    stateToSave << player1.getPlayerAudio().getLastLoadedFilePath() << "\n";
    stateToSave << player1.getPlayerAudio().getPlaybackPositionInSeconds() << "\n";
    
    stateToSave << player2.getPlayerAudio().getLastLoadedFilePath() << "\n";
    stateToSave << player2.getPlayerAudio().getPlaybackPositionInSeconds() << "\n";
    
    auto saveFile = getSessionSaveFile();
    saveFile.replaceWithText(stateToSave);
    shutdownAudio();
}
void MainComponent::paint(juce::Graphics& g)
{
    
    g.fillAll(juce::Colour(0xFF2B2B2B));
 
}
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
	player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{

    juce::AudioBuffer<float> tempBuffer(bufferToFill.buffer->getNumChannels(),
        bufferToFill.numSamples);

    juce::AudioSourceChannelInfo tempBufferInfo(&tempBuffer,
        bufferToFill.startSample,
        bufferToFill.numSamples);


    player1.getNextAudioBlock(bufferToFill);


    player2.getNextAudioBlock(tempBufferInfo);

    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        
        bufferToFill.buffer->addFrom(
            channel,                  
            bufferToFill.startSample, 
            tempBuffer,               
            channel,                  
            0,                        
            bufferToFill.numSamples   
        );
    }

    float targetGain = masterVolume; 

    bufferToFill.buffer->applyGainRamp(
        bufferToFill.startSample,
        bufferToFill.numSamples,
        lastmasterVolume,
        targetGain
    );

    lastmasterVolume = targetGain;
}
void MainComponent::releaseResources()
{
    player1.releaseResources();
	player2.releaseResources();
}

void MainComponent::resized()
{
	auto bounds = getLocalBounds();
	auto masterSliderBounds = bounds.removeFromTop(30);
    masterSliderBounds.reduce(10, 0);
    masterVolumeSlider.setBounds(masterSliderBounds);

	auto player1Bounds = bounds.removeFromLeft(bounds.getWidth() / 2);
	player1.setBounds(player1Bounds);
	player2.setBounds(bounds);
}
void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &masterVolumeSlider)
    {
        masterVolume = (float)masterVolumeSlider.getValue();
    }
}
juce::File MainComponent::getSessionSaveFile()
{
    auto appDataDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory);
    auto appSpecificDir = appDataDir.getChildFile("MyJuceDJMixer");
    appSpecificDir.createDirectory();
    return appSpecificDir.getChildFile("last_session_state.txt");
}

