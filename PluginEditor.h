/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Mu45EQAudioProcessorEditor  : public juce::AudioProcessorEditor,public juce::Slider::Listener,public juce::ComboBox::Listener, public juce::Timer
{
public:
    Mu45EQAudioProcessorEditor (Mu45EQAudioProcessor&);
    ~Mu45EQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* comboBox) override;
    void timerCallback() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Mu45EQAudioProcessor& audioProcessor;
    //lowpass
    juce::Slider mFilterFcSlider;
    juce::Slider mFilterQSlider;
    juce::Slider mHSgSlider;
    //high pass
    juce::Slider mHPfSlider;
    juce::Slider mHPqSlider;
    juce::Slider mLSgSlider;
    //peak
    juce::Slider mPfSlider;
    juce::Slider mPqSlider;
    juce::Slider mPgSlider;
    //peak2
    juce::Slider mPfSlider2;
    juce::Slider mPqSlider2;
    juce::Slider mPgSlider2;
    //peak3
    juce::Slider mPfSlider3;
    juce::Slider mPqSlider3;
    juce::Slider mPgSlider3;
    
    juce::ComboBox mHPLSSelector;
    juce::ComboBox mLPHSSelector;
    
    juce::Label fLabel;
    juce::Label qLabel;
    juce::Label gLabel;
    
    juce::Label peakLabel1;
    juce::Label peakLabel2;
    juce::Label peakLabel3;
    
    juce::Rectangle<int> spectrumArea; 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mu45EQAudioProcessorEditor)
};

