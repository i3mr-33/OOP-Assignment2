#pragma once
#include <JuceHeader.h>


class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {

        juce::Colour background = juce::Colour(0xFF2B2B2B);
        juce::Colour component = juce::Colour(0xFF3C3C3C);
        juce::Colour accent = juce::Colour(0xFF007ACC);
        juce::Colour highlight = juce::Colour(0xFF3399FF);
        juce::Colour text = juce::Colour(0xFFEFEFEF);

        setColour(juce::Slider::backgroundColourId, component); 
        setColour(juce::Slider::trackColourId, accent);     
        setColour(juce::Slider::thumbColourId, highlight);  

        setColour(juce::TextButton::buttonColourId, component);
        setColour(juce::TextButton::textColourOffId, text);
        setColour(juce::TextButton::buttonOnColourId, accent);

        setColour(juce::Label::textColourId, text);

        setColour(juce::ListBox::backgroundColourId, component);
        setColour(juce::ListBox::outlineColourId, juce::Colours::transparentBlack);
        setColour(juce::ListBox::textColourId, text);
    }
};