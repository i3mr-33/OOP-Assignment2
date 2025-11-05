 #include "MainComponent.h"


MainComponent::MainComponent()
{
    setLookAndFeel(&customLookAndFeel);
	addAndMakeVisible(player1);
	addAndMakeVisible(player2);
    addAndMakeVisible(masterVolumeSlider);
    masterVolumeSlider.setRange(0.0, 1.0, 0.01);
    masterVolumeSlider.setValue(0.8);
    masterVolumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    masterVolumeSlider.addListener(this);
    addAndMakeVisible(masterVolumeLabel);
    masterVolumeLabel.setText("Master Volume", juce::dontSendNotification);
    masterVolumeLabel.setJustificationType(juce::Justification::centredRight);
    masterVolumeLabel.attachToComponent(&masterVolumeSlider, true);

    lastmasterVolume = (float)masterVolumeSlider.getValue();
    masterVolume = lastmasterVolume;
    
    setSize(1000, 2000);
    setAudioChannels(0, 2);
}
MainComponent::~MainComponent()
{
    shutdownAudio();
}
void MainComponent::paint(juce::Graphics& g)
{
    // Fill the background with the dark color from your LookAndFeel
    // This will cover the light green
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


