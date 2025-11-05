#pragma once
#include <JuceHeader.h>

// This class will define our new "stylesheet"
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        // --- Define our color palette ---
        juce::Colour background = juce::Colour(0xFF2B2B2B);
        juce::Colour component = juce::Colour(0xFF3C3C3C);
        juce::Colour accent = juce::Colour(0xFF007ACC);
        juce::Colour highlight = juce::Colour(0xFF3399FF);
        juce::Colour text = juce::Colour(0xFFEFEFEF);

        // --- Apply to Sliders ---
        setColour(juce::Slider::backgroundColourId, component); // The "empty" part of the track
        setColour(juce::Slider::trackColourId, accent);     // The "filled" part of the track
        setColour(juce::Slider::thumbColourId, highlight);  // The circle/thumb you drag

        // --- Apply to Buttons ---
        setColour(juce::TextButton::buttonColourId, component);
        setColour(juce::TextButton::textColourOffId, text);
        setColour(juce::TextButton::buttonOnColourId, accent); // Color when button is toggled (like your loop button)

        // --- Apply to Labels ---
        setColour(juce::Label::textColourId, text);

        // --- Apply to ListBox (Playlist) ---
        setColour(juce::ListBox::backgroundColourId, component);
        setColour(juce::ListBox::outlineColourId, juce::Colours::transparentBlack);
        setColour(juce::ListBox::textColourId, text);
    }
};