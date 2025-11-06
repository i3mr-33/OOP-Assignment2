#include "PlayerGUI.h"
#include <JuceHeader.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>
PlayerGUI::PlayerGUI()
{
	setLookAndFeel(&customLookAndFeel);
    // Add buttons
    for (auto* btn : { &loadButton, &loopButton, &playButton, &goToStartButton, &goToEndButton, &muteButton 
        ,&nextButton, &prevButton , &forwardButton , &backwardButton , &setAButton, 
        &setBButton, &abLoopButton,&markerButton , & gotoMarkerButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    playlistBox.setModel(this);
    playlistBox.setColour(juce::ListBox::backgroundColourId, juce::Colours::darkgrey);

    playlistTitleLabel.setJustificationType(juce::Justification::centredLeft);
    playlistTitleLabel.setText("Playlist:", juce::dontSendNotification);
    playlistTitleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    playlistTitleLabel.setFont(juce::Font(18.0f, juce::Font::plain));

    addAndMakeVisible(playlistBox); 
    addAndMakeVisible(playlistTitleLabel);

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
    speedSlider.setRange(0.5, 1.5, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.setTextValueSuffix("x Speed");
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);

    metaDataLabel.setJustificationType(juce::Justification::topLeft);
    metaDataLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    metaDataLabel.setFont(juce::Font(16.0f));
    metaDataLabel.setText("No file loaded.", juce::dontSendNotification);
    addAndMakeVisible(metaDataLabel);
    playerAudio.onFileChanged = [this](const juce::File& file) { updateMetaDataLabelWithTagLib(file); };

}
PlayerGUI::~PlayerGUI()
{
	setLookAndFeel(nullptr);
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
    if (playlistItems.isEmpty())
        return 1; 
    else 
    return playlistItems.size();
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g,
    int width, int height, bool rowIsSelected)
{
  
    if (playlistItems.isEmpty())
    {
        g.setColour(juce::Colours::lightgrey);
        g.setFont(juce::Font(16.0f, juce::Font::italic));
        g.drawText("No songs loaded", 0, 0, width, height,
            juce::Justification::centred);
        return;
    }

   
    if (rowNumber == currentlyPlayingRow)
        g.setColour(juce::Colours::aqua);   
    else
        g.setColour(juce::Colours::white);  

   
    if (rowNumber < playlistItems.size())
    {
        juce::String text = juce::String(rowNumber + 1) + ") " + playlistItems[rowNumber];
        g.setFont(juce::Font(16.0f, juce::Font::plain));
        g.drawText(text, 8, 0, width - 10, height,
            juce::Justification::centredLeft);
    }
}
void PlayerGUI::resized()
{
    int y = getHeight() / 2;
    int x = 200;

    loadButton.setBounds(15, 15, 90, 50);
    loopButton.setBounds(getWidth() - 80, 15, 60, 40);
    muteButton.setBounds(getWidth() - 160, 15, 60, 40);

   
    backwardButton.setBounds(x - 90, y + 270, 70, 40);    
    goToStartButton.setBounds(x - 180, y + 270, 70, 40);
    playButton.setBounds(x , y + 270, 70, 40);
    goToEndButton.setBounds(x + 180, y + 270, 70, 40);
    forwardButton.setBounds(x + 90, y + 270, 70, 40);

    prevButton.setBounds(getWidth() - 160, 60, 60, 40);
    nextButton.setBounds(getWidth() - 80, 60, 60, 40);
    playlistBox.setBounds(getWidth() - 160, 130, 140, 100);
    playlistTitleLabel.setBounds(getWidth() - 160, 100, 200, 40);

    setAButton.setBounds(20, y + 200, 80, 40);
    setBButton.setBounds(120, y + 200, 80, 40);
    abLoopButton.setBounds(220, y + 200, 80, 40);

    currentTimeLabel.setBounds(10, getHeight() - 40, 60, 30);
    totalTimeLabel.setBounds(getWidth() - 70, getHeight() - 40, 60, 30);
    metaDataLabel.setBounds(15, 80, getWidth() - 40, 60);
    
    positionSlider.setBounds(60, getHeight() - 40, getWidth() - 130, 30);
    volumeSlider.setBounds(getWidth() - 350, y + 180, 350, 30);
    speedSlider.setBounds(getWidth() - 350, y + 220, 350, 30);
    
	offcolour = abLoopButton.findColour(juce::TextButton::buttonOnColourId);
    markerButton.setBounds(130, 15, 90, 50);
    gotoMarkerButton.setBounds(240, 15 , 90, 50);


}


void PlayerGUI::updateMetaDataLabelWithTagLib(const juce::File& file)
{
    TagLib::FileRef f(file.getFullPathName().toStdString().c_str());
    juce::String infoText;

    if (!f.isNull() && f.tag())
    {
        auto tag = f.tag();

        juce::String title = juce::String(tag->title().toCString(true));
        juce::String artist = juce::String(tag->artist().toCString(true));
        juce::String album = juce::String(tag->album().toCString(true));
        
        if (title.isEmpty())
            title = file.getFileNameWithoutExtension();
 
        infoText = "Title: " + title;

        if (!artist.isEmpty()) infoText += "\nArtist: " + artist;
        if (!album.isEmpty())  infoText += "\nAlbum: " + album;
    }
    else
    {
        infoText = "Title: " + file.getFileNameWithoutExtension();
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
                    playlistBox.repaint();
                    playerAudio.addToPlaylist({ file });
                    playerAudio.loadFile(file);
                    currentlyPlayingRow = playlistItems.indexOf(file.getFileName());

                    playlistBox.updateContent();
                    playlistBox.repaint();

                    // Update metadata label
                    updateMetaDataLabelWithTagLib(file);

                    // Set position slider range
                    positionSlider.setRange(0, playerAudio.getLength(), 1);
                    // Update Total Time
                    double totalLength = playerAudio.getLength();
                    int totalMinutes = (int)totalLength / 60;
                    int totalSecondsInt = (int)totalLength % 60;
                    totalTimeLabel.setText(
                        juce::String::formatted("%02d:%02d", totalMinutes, totalSecondsInt),
                        juce::dontSendNotification);
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

        
        auto currentFile = playerAudio.getCurrentFile();
        currentlyPlayingRow = playlistItems.indexOf(currentFile.getFileName());

       
        playlistBox.updateContent();
        playlistBox.repaint();
    }
    else if (button == &prevButton)
    {
        playerAudio.playPrevious();

       
        auto currentFile = playerAudio.getCurrentFile();
        currentlyPlayingRow = playlistItems.indexOf(currentFile.getFileName());

        
        playlistBox.updateContent();
        playlistBox.repaint();
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
        playerAudio.toggleA();
        if (playerAudio.getLoopB() != -1.0 && playerAudio.getLoopB() <= playerAudio.getLoopA())
        {
            playerAudio.setLoopB(-1.0);
            setBButton.setButtonText("Set B");
        }
    }
    else if (button == &setBButton)
        {

            double current = playerAudio.getPosition();

            if (playerAudio.getLoopA() == -1.0)
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                    "Invalid Operation", "Please set point A before setting point B.");
                return;
            }

            if (current <= playerAudio.getLoopA())
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                    "Invalid Range", "Point B must be after point A.");
                return;
            }
                playerAudio.toggleB();
                playerAudio.setLoopB();
        }
    else if (button == &abLoopButton)
     {
            playerAudio.toggleABLooping();
			abLoopButton.setToggleState(playerAudio.isABLooping(), juce::dontSendNotification);
     }
    else if (button == &markerButton)
    {
        double currentPos = playerAudio.getPosition();

        if (playerAudio.getMarker() >= 0)
        {
            playerAudio.clearMarker();
            markerButton.setButtonText("Marker");
        }
        else
        {
            playerAudio.setMarker(currentPos);

            
            int totalSeconds = int(currentPos);
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;

            juce::String timeString = std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);

            markerButton.setButtonText("Marker: " + timeString);
        }
        }

    else if (button == &gotoMarkerButton)
    {
        if (playerAudio.getMarker() >= 0)
            playerAudio.setPosition(playerAudio.getMarker());
     }



    
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF2B2B2B));
    juce::Rectangle<int> waveformBounds(40, getHeight() / 2 - 100, 700, 160);
    g.setColour(juce::Colours::black);
    g.fillRect(waveformBounds);
    g.setColour(juce::Colours::white);
    g.drawRect(waveformBounds, 1.0f);
    if (playerAudio.getThumbnail().isFullyLoaded())
    {
        g.setColour(juce::Colour(0xFF007ACC));
        playerAudio.getThumbnail().drawChannel(
            g,
            waveformBounds.reduced(2),
            0.0,
            playerAudio.getLength(),
            0,
            1.0f
        );
    }
    double totalLength = playerAudio.getLength();
    if (totalLength > 0.0)
    {
        float progress = (float)(playerAudio.getPosition() / totalLength);
        int xPosition = waveformBounds.getX() + (int)(waveformBounds.getWidth() * progress);
        g.setColour(juce::Colours::red);
        g.drawLine(xPosition, waveformBounds.getY(), xPosition, waveformBounds.getBottom(), 2.0f); 
    }
    double marker = playerAudio.getMarker();
    if (marker >= 0.0)
    {
        juce::Rectangle<int> waveformBounds(20, getHeight() / 2 - 100, 800, 120); 
        float xPos = waveformBounds.getX() +
            (float)(marker / playerAudio.getLength() * waveformBounds.getWidth());
        g.setColour(juce::Colours::yellow);
        g.drawLine(xPos, waveformBounds.getY(), xPos, waveformBounds.getBottom(), 2.0f);
    }

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
    double totalLength = playerAudio.getLength();
    if (totalLength > 0.0)
    {
        double currentPosition = playerAudio.getPosition();
        positionSlider.setValue(currentPosition, juce::dontSendNotification);
        int currentMinutes = (int)currentPosition / 60;
        int currentSeconds = (int)currentPosition % 60;
        currentTimeLabel.setText(
            juce::String::formatted("%02d:%02d", currentMinutes, currentSeconds),
            juce::dontSendNotification
        );
    }
    else
    {
        currentTimeLabel.setText("00:00", juce::dontSendNotification);
        positionSlider.setValue(0.0, juce::dontSendNotification);
    }
    repaint();
        positionSlider.setValue(playerAudio.getPosition(), juce::dontSendNotification);
        double value = positionSlider.getValue();
        int totalSeconds = int(value);
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;
        juce::String timeString = std::to_string(minutes) + " : " + std::to_string(seconds);
        if (playerAudio.isAOn())
        {
            if(playerAudio.getLoopB() <= playerAudio.getLoopA() && playerAudio.getLoopB() != -1.0)
            {
			}
            else
            {
                playerAudio.toggleA();
                setAButton.setButtonText("A \n" + timeString);
            }
        }
        if (playerAudio.isBOn())
        {
            if (playerAudio.getLoopB() <= playerAudio.getLoopA() && playerAudio.getLoopB() != -1.0)
            {
            }
            else
            { 
            playerAudio.toggleB();
            setBButton.setButtonText("B \n" + timeString);
            }
            }
        currentTimeLabel.setText(timeString, juce::dontSendNotification);
        value = playerAudio.getLength();
        totalSeconds = int(value);
        minutes = totalSeconds / 60;
        seconds = totalSeconds % 60;
        timeString = std::to_string(minutes) + " : " + std::to_string(seconds);

        totalTimeLabel.setText(timeString, juce::dontSendNotification);
    
    if(playerAudio.isABLooping())
        {
        double currentPosition = playerAudio.getPosition();
        if (currentPosition >= playerAudio.getLength() || currentPosition >= playerAudio.getLoopB())
        {
            playerAudio.setPosition(playerAudio.getLoopA());
        }
	}
}
void PlayerGUI::positionSliderSetVBounds()
{
    positionSlider.setRange(0, playerAudio.getLength(), 1);
}  
