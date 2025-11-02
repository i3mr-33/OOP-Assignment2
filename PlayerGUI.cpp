#include "PlayerGUI.h"
#include <JuceHeader.h>
PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &loopButton, &playButton, &goToStartButton, &goToEndButton, &muteButton ,&nextButton, &prevButton , &forwardButton , &backwardButton , &setAButton, &setBButton, &abLoopButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }
    playlistBox.setModel(this);
    addAndMakeVisible(playlistBox);

    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
    positionSlider.setRange(0, 48, 1);
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

    metaDataLabel.setJustificationType(juce::Justification::topLeft);
    metaDataLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    metaDataLabel.setFont(juce::Font(16.0f));
    metaDataLabel.setText("No file loaded.", juce::dontSendNotification);
    addAndMakeVisible(metaDataLabel);
    playerAudio.onFileChanged = [this](const juce::File& file) { updateMetaDataLabel(file); };

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
int PlayerGUI::getNumRows()
{
    return playlistItems.size();
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g,
    int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);

    if (rowNumber < playlistItems.size())
    {
        g.setColour(juce::Colours::black);
        g.drawText(playlistItems[rowNumber], 2, 0, width - 4, height,
            juce::Justification::centredLeft);
        
    }
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
    muteButton.setBounds(getWidth() - 160, 15, 60, 40);

   
    backwardButton.setBounds(x - 115, y, 70, 40);    
    goToStartButton.setBounds(x - 195, y, 70, 40);   
    playButton.setBounds(x - 35, y, 70, 40);        
    goToEndButton.setBounds(x + 125, y, 70, 40);     
    forwardButton.setBounds(x + 45, y, 70, 40);    

    positionSlider.setBounds(65, getHeight() / 2 + 100, getWidth() - 130, 30);
    volumeSlider.setBounds(20, getHeight() / 2 + 200, getWidth() - 40, 30);
    currentTimeLabel.setBounds(10, getHeight() / 2 + 100, 60, 30);
    totalTimeLabel.setBounds(getWidth() - 70, getHeight() / 2 + 100, 60, 30);
    metaDataLabel.setBounds(15, 80, 250, 80);
    prevButton.setBounds(getWidth() - 160, 60, 60, 40);
    nextButton.setBounds(getWidth() - 80, 60, 60, 40);
    playlistBox.setBounds(getWidth() - 160, 120, 140, 100);

    setAButton.setBounds(15, y, 80, 40);
    setBButton.setBounds(115, y, 80, 40);
    abLoopButton.setBounds(215, y, 80, 40);
    speedSlider.setBounds(20, 80, getWidth() - 40, 30);
}




void PlayerGUI::updateMetaDataLabel(const juce::File& file)
{
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    juce::String infoText;

    if (reader != nullptr)
    {
        auto metadata = reader->metadataValues;

        if (metadata.size() == 0)
        {
            infoText = file.getFileName();
        }
        else
        {
            double durationSeconds = reader->lengthInSamples / reader->sampleRate;
            int minutes = int(durationSeconds) / 60;
            int seconds = int(durationSeconds) % 60;

            juce::String minutesStr = (minutes < 10 ? "0" : "") + juce::String(minutes);
            juce::String secondsStr = (seconds < 10 ? "0" : "") + juce::String(seconds);

            juce::String title = metadata.getValue("title", "Unknown");
            juce::String author = metadata.getValue("artist", "Unknown");
            juce::String album = metadata.getValue("album", "Unknown");

            infoText = "Title: " + title + "\n"
                "Author: " + author + "\n"
                "Album: " + album + "\n"
                "Duration: " + juce::String(minutesStr) + " : " + juce::String(secondsStr);
        }
    }
    else
    {
        infoText = "Unable to read file metadata.";
    }

    metaDataLabel.setText(infoText, juce::dontSendNotification);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>("Select an audio file", juce::File{}, "*.mp3;*.wav");

        fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& chooser)
            {
                auto file = chooser.getResult();
                if (file.existsAsFile())
                {
                    // Add to playlist & load
                    playlistItems.add(file.getFileName());
                    playlistBox.updateContent();
                    playerAudio.addToPlaylist({ file });
                    playerAudio.loadFile(file);

                    // Update metadata label
                    updateMetaDataLabel(file);

                    // Set position slider range
                    positionSlider.setRange(0, playerAudio.getLength(), 1);
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
    else if (button == &playButton)
    {
        playerAudio.toggleRunning();
        if (playerAudio.isRunning())
            playButton.setButtonText(juce::CharPointer_UTF8(u8"▶️"));
        else
            playButton.setButtonText(juce::CharPointer_UTF8(u8"⏸️"));
    }
    else if (button == &goToStartButton)
    {
        playerAudio.setPosition(0.0);
    }
    else if (button == &goToEndButton)
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
    else if (button == &nextButton)
    {
        playerAudio.playNext();
    }
    else if (button == &prevButton)
    {
        playerAudio.playPrevious();
    }
    else if (button == &forwardButton)
    {
        double newPosition = playerAudio.getPosition() + 10.0; 
        if (newPosition > playerAudio.getLength())
            newPosition = playerAudio.getLength(); 
        playerAudio.setPosition(newPosition); 
    }
    else if (button == &backwardButton)
    {
        double newPosition = playerAudio.getPosition() - 10.0; 
        if (newPosition < 0.0)
            newPosition = 0.0; 
        playerAudio.setPosition(newPosition);
    }
    else if (button == &setAButton)
    {
        playerAudio.setLoopA();
    }
    else if (button == &setBButton)
    {
        playerAudio.setLoopB();
    }
    else if (button == &abLoopButton)
    {
        playerAudio.toggleABLooping();
        if (playerAudio.isABLooping())
            abLoopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
        else
            abLoopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkcyan);
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
