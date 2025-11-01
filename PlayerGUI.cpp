#include"PlayerGUI.h"
#include <JuceHeader.h>
PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &loopButton, &playButton, &goToStartButton, &goToEndButton, &muteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
	positionSlider.setRange(0,48, 1);
	positionSlider.setValue(0);
	positionSlider.addListener(this);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	addAndMakeVisible(positionSlider);

    setSize(500, 250);
	startTimerHz(30);
	currentTimeLabel.setText("00:00", juce::dontSendNotification);
	currentTimeLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(currentTimeLabel);
	totalTimeLabel.setText("00:00", juce::dontSendNotification);
	totalTimeLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(totalTimeLabel);
    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.setTextValueSuffix("x Speed");
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);
}
PlayerGUI::~PlayerGUI()
{
}
void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);

}
void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
    positionSlider.setRange(0, playerAudio.getLength(), 1);
}
void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}
//void PlayerGUI::resized()
//{
//    int y = getHeight()/2;
//	int x = getWidth() / 2;
//    loadButton.setBounds(15, 15, 110, 60);
//	loopButton.setBounds(getWidth() - 80, 15, 60, 40);
//    playButton.setBounds(x - 40, y, 80, 40);
//    goToStartButton.setBounds(x - 160, y, 80, 40);
//    goToEndButton.setBounds(x + 80, y, 80, 40);
//	muteButton.setBounds(getWidth() - 160, 15, 60, 40);
//    /*prevButton.setBounds(340, y, 80, 40);
//    nextButton.setBounds(440, y, 80, 40);*/
//
//    volumeSlider.setBounds(20, getHeight()/2 + 200, getWidth() - 40, 30);
//	positionSlider.setBounds(20, getHeight()/2 + 100, getWidth() - 40, 30);
//    
//}
void PlayerGUI::resized()
{
    int y = getHeight() / 2;
    int x = getWidth() / 2;
    loadButton.setBounds(15, 15, 110, 60);
    loopButton.setBounds(getWidth() - 80, 15, 60, 40);
    playButton.setBounds(x - 40, y, 80, 40);
    goToStartButton.setBounds(x - 160, y, 80, 40);
    goToEndButton.setBounds(x + 80, y, 80, 40);
    muteButton.setBounds(getWidth() - 160, 15, 60, 40);
    positionSlider.setBounds(65, getHeight() / 2 + 100, getWidth() - 130, 30);
    volumeSlider.setBounds(20, getHeight() / 2 + 200, getWidth() - 40, 30);
	currentTimeLabel.setBounds(10, getHeight() / 2 + 100, 60, 30);
	totalTimeLabel.setBounds(getWidth() - 70, getHeight() / 2 + 100, 60, 30);
    speedSlider.setBounds(20, 80, getWidth() - 40, 30);
}
void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>("Select an audio file...",
            juce::File{},
            "*.wav;*.mp3;*.aiff");

        fileChooser->launchAsync(juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& chooser)
            {
                auto file = chooser.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                }
            });
    }
    else if (button == &loopButton)
    {
        playerAudio.toggleLooping();

        loopButton.setToggleState(playerAudio.isLooping(), juce::dontSendNotification);

        if (playerAudio.isLooping())
            loopButton.setButtonText(juce::CharPointer_UTF8(u8"🔁"));
        else
            loopButton.setButtonText(juce::CharPointer_UTF8(u8"➡️"));
    }
    
    else if (button == &playButton) // Play function 
    {
        playerAudio.toggleRunning();
        if (playerAudio.isRunning())
            playButton.setButtonText(juce::CharPointer_UTF8(u8"▶️"));
        else
            playButton.setButtonText(juce::CharPointer_UTF8(u8"⏸️"));
    }

    else if (button == &goToStartButton) // go to start function 
    {
        playerAudio.setPosition(0.0);
    }

    else if (button == &goToEndButton) // go to end function 
    {
        playerAudio.setPosition(playerAudio.getLength());
    }
    else if (button == &muteButton)
    {
        if (playerAudio.isMuted())
        {
            playerAudio.setGain(playerAudio.getPrevGain());
            volumeSlider.setValue(playerAudio.getPrevGain());
            muteButton.setButtonText(juce::CharPointer_UTF8(u8"🔊"));
            playerAudio.toggleMute();
        }
        else
        {
            muteButton.setButtonText(juce::CharPointer_UTF8(u8"🔇"));
            playerAudio.toggleMute();
            playerAudio.setPrevGain((float)playerAudio.getGain());
            playerAudio.setGain(0.0f);
            volumeSlider.setValue(0.0f);
        }
    }
}
void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
    else if (slider == &positionSlider)
    {
        playerAudio.setPosition((float)slider->getValue());
        double value = positionSlider.getValue();
        int totalSeconds = int(value);
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;
        juce::String timeString = std::to_string(minutes) + " : " + std::to_string(seconds);

        currentTimeLabel.setText(timeString, juce::dontSendNotification);
	}
    else if (slider == &speedSlider)
    {
        double newSpeed = speedSlider.getValue();
        playerAudio.setPlaybackSpeed(newSpeed);
        stopTimer();
        startTimerHz((int)(30.0 * newSpeed));
    }
}
void PlayerGUI::timerCallback()
{
    if (playerAudio.isRunning())
    {
        positionSlider.setValue(playerAudio.getPosition(), juce::dontSendNotification);
        double value = positionSlider.getValue();
        int totalSeconds = int(value);
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;
        juce::String timeString = std::to_string(minutes) + " : " + std::to_string(seconds);
        currentTimeLabel.setText(timeString, juce::dontSendNotification);
        value = playerAudio.getLength();
        totalSeconds = int(value);
        minutes = totalSeconds / 60;
        seconds = totalSeconds % 60;
        timeString = std::to_string(minutes) + " : " + std::to_string(seconds);

        totalTimeLabel.setText(timeString, juce::dontSendNotification);
    }
}
void PlayerGUI::positionSliderSetVBounds()
{
    positionSlider.setRange(0, playerAudio.getLength(), 1);
}

