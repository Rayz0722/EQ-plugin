/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Mu45EQAudioProcessorEditor::Mu45EQAudioProcessorEditor (Mu45EQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    startTimerHz(30);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
    // Setup your sliders and other gui components - - - -
    auto& params = processor.getParameters();
    // Cuttoff Freq Slider
    juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(0);
    mFilterFcSlider.setBounds(670, 300, 100, 80);
    mFilterFcSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mFilterFcSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mFilterFcSlider.setRange(audioParam->range.start, audioParam->range.end);
    mFilterFcSlider.setValue(*audioParam);
    mFilterFcSlider.addListener(this);
    mFilterFcSlider.setDoubleClickReturnValue(true, 10000.0f);
    addAndMakeVisible(mFilterFcSlider);
    // Q slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(1);
    mFilterQSlider.setBounds(670, 400, 100, 80);
    mFilterQSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //mFilterQSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mFilterQSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mFilterQSlider.setRange(audioParam->range.start, audioParam->range.end);
    mFilterQSlider.setValue(*audioParam);
    mFilterQSlider.addListener(this);
    mFilterQSlider.setDoubleClickReturnValue(true, 3.0f);
    addAndMakeVisible(mFilterQSlider);
    // Gain slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(2);
    mHSgSlider.setBounds(670, 500, 100, 80);
    mHSgSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //mFilterQSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mHSgSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mHSgSlider.setRange(audioParam->range.start, audioParam->range.end);
    mHSgSlider.setValue(*audioParam);
    mHSgSlider.addListener(this);
    mHSgSlider.setDoubleClickReturnValue(true, 0.0f);
    addAndMakeVisible(mHSgSlider);
    
    
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(3);
    mHPfSlider.setBounds(150, 300, 100, 80);
    mHPfSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mHPfSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mHPfSlider.setRange(audioParam->range.start, audioParam->range.end);
    mHPfSlider.setValue(*audioParam);
    mHPfSlider.addListener(this);
    mHPfSlider.setDoubleClickReturnValue(true, 40.0f);
    addAndMakeVisible(mHPfSlider);
    
    fLabel.setText("Frequency(HZ):", juce::NotificationType::dontSendNotification);
    fLabel.attachToComponent(&mHPfSlider, true);
    addAndMakeVisible(fLabel);
    
    // Q slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(4);
    mHPqSlider.setBounds(150, 400, 100, 80);
    mHPqSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //mFilterQSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mHPqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mHPqSlider.setRange(audioParam->range.start, audioParam->range.end);
    mHPqSlider.setValue(*audioParam);
    mHPqSlider.addListener(this);
    mHPqSlider.setDoubleClickReturnValue(true, 3.0f);
    addAndMakeVisible(mHPqSlider);
    
    qLabel.setText("Q:", juce::NotificationType::dontSendNotification);
    qLabel.attachToComponent(&mHPqSlider, true);
    addAndMakeVisible(qLabel);
    // Gain slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(5);
    mLSgSlider.setBounds(150, 500, 100, 80);
    mLSgSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //mFilterQSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mLSgSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mLSgSlider.setRange(audioParam->range.start, audioParam->range.end);
    mLSgSlider.setValue(*audioParam);
    mLSgSlider.addListener(this);
    mLSgSlider.setDoubleClickReturnValue(true, 0.0f);
    addAndMakeVisible(mLSgSlider);
    gLabel.setText("Gain(Db):", juce::NotificationType::dontSendNotification);
    gLabel.attachToComponent(&mLSgSlider, true);
    addAndMakeVisible(gLabel);
    
    peakLabel1.setText("Peak 1", juce::NotificationType::dontSendNotification);
    peakLabel1.setJustificationType(juce::Justification::centred);
    peakLabel1.setBounds(280, 250, 100, 25);
    addAndMakeVisible(peakLabel1);
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(6);
    mPfSlider.setBounds(280, 300, 100, 80);
    mPfSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mPfSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mPfSlider.setRange(audioParam->range.start, audioParam->range.end);
    mPfSlider.setValue(*audioParam);
    mPfSlider.addListener(this);
    mPfSlider.setDoubleClickReturnValue(true, 300.0f);
    addAndMakeVisible(mPfSlider);
    // Q slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(7);
    mPqSlider.setBounds(280, 400, 100, 80);
    mPqSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //mFilterQSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mPqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mPqSlider.setRange(audioParam->range.start, audioParam->range.end);
    mPqSlider.setValue(*audioParam);
    mPqSlider.addListener(this);
    mPqSlider.setDoubleClickReturnValue(true, 3.0f);
    addAndMakeVisible(mPqSlider);
    // Gain slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(8);
    mPgSlider.setBounds(280, 500, 100, 80);
    mPgSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //mFilterQSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mPgSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mPgSlider.setRange(audioParam->range.start, audioParam->range.end);
    mPgSlider.setValue(*audioParam);
    mPgSlider.addListener(this);
    mPgSlider.setDoubleClickReturnValue(true, 0.0f);
    addAndMakeVisible(mPgSlider);
    
    peakLabel2.setText("Peak 2", juce::NotificationType::dontSendNotification);
    peakLabel2.setJustificationType(juce::Justification::centred);
    peakLabel2.setBounds(410, 250, 100, 25);
    addAndMakeVisible(peakLabel2);
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(9);
    mPfSlider2.setBounds(410, 300, 100, 80);
    mPfSlider2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mPfSlider2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mPfSlider2.setRange(audioParam->range.start, audioParam->range.end);
    mPfSlider2.setValue(*audioParam);
    mPfSlider2.addListener(this);
    mPfSlider2.setDoubleClickReturnValue(true, 1000.0f);
    addAndMakeVisible(mPfSlider2);
    // Q slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(10);
    mPqSlider2.setBounds(410, 400, 100, 80);
    mPqSlider2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //mFilterQSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mPqSlider2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mPqSlider2.setRange(audioParam->range.start, audioParam->range.end);
    mPqSlider2.setValue(*audioParam);
    mPqSlider2.addListener(this);
    mPqSlider2.setDoubleClickReturnValue(true, 3.0f);
    addAndMakeVisible(mPqSlider2);
    // Gain slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(11);
    mPgSlider2.setBounds(410, 500, 100, 80);
    mPgSlider2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //mFilterQSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mPgSlider2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mPgSlider2.setRange(audioParam->range.start, audioParam->range.end);
    mPgSlider2.setValue(*audioParam);
    mPgSlider2.addListener(this);
    mPgSlider2.setDoubleClickReturnValue(true, 0.0f);
    addAndMakeVisible(mPgSlider2);
    
    peakLabel3.setText("Peak 3", juce::NotificationType::dontSendNotification);
    peakLabel3.setJustificationType(juce::Justification::centred);
    peakLabel3.setBounds(540, 250, 100, 25);
    addAndMakeVisible(peakLabel3);
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(12);
    mPfSlider3.setBounds(540, 300, 100, 80);
    mPfSlider3.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mPfSlider3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mPfSlider3.setRange(audioParam->range.start, audioParam->range.end);
    mPfSlider3.setValue(*audioParam);
    mPfSlider3.addListener(this);
    mPfSlider3.setDoubleClickReturnValue(true, 3000.0f);
    addAndMakeVisible(mPfSlider3);
    // Q slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(13);
    mPqSlider3.setBounds(540, 400, 100, 80);
    mPqSlider3.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //mFilterQSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mPqSlider3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mPqSlider3.setRange(audioParam->range.start, audioParam->range.end);
    mPqSlider3.setValue(*audioParam);
    mPqSlider3.addListener(this);
    mPqSlider3.setDoubleClickReturnValue(true, 3.0f);
    addAndMakeVisible(mPqSlider3);
    // Gain slider
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(14);
    mPgSlider3.setBounds(540, 500, 100, 80);
    mPgSlider3.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    //mFilterQSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mPgSlider3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mPgSlider3.setRange(audioParam->range.start, audioParam->range.end);
    mPgSlider3.setValue(*audioParam);
    mPgSlider3.addListener(this);
    mPgSlider3.setDoubleClickReturnValue(true, 0.0f);
    addAndMakeVisible(mPgSlider3);
    
    
    mHPLSSelector.setBounds(150, 250, 100, 25);
    mHPLSSelector.addItem("HP", 1);
    mHPLSSelector.addItem("LS", 2);
    mHPLSSelector.setSelectedId(1);
    mHPLSSelector.addListener(this);
    addAndMakeVisible(mHPLSSelector);
    
    mLPHSSelector.setBounds(670, 250, 100, 25);
    mLPHSSelector.addItem("LP", 1);
    mLPHSSelector.addItem("HS", 2);
    mLPHSSelector.setSelectedId(1);
    mLPHSSelector.addListener(this);
    addAndMakeVisible(mLPHSSelector);
    
    spectrumArea.setBounds(10, 20, 780, 200);
}

Mu45EQAudioProcessorEditor::~Mu45EQAudioProcessorEditor()
{
}

//==============================================================================
void Mu45EQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    
    g.setColour (juce::Colours::black);
    g.fillRect (spectrumArea);
    g.setColour (juce::Colours::grey);
    g.drawRect (spectrumArea);
    
    auto fftOutput = audioProcessor.getFFTOutput();
    int numDisplayBins = 1024;
    float fs = 44100.0f;  // Replace with your actual sampling rate
    //int startBin = static_cast<int>(40.0 / (fs / Mu45EQAudioProcessor::fftSize));
    //int endBin = static_cast<int>(10000.0 / (fs / Mu45EQAudioProcessor::fftSize));
    float binWidth = static_cast<float>(spectrumArea.getWidth()) / numDisplayBins;
    float freqStep = (10000.0f - 40.0f) / numDisplayBins;
    float maxBinHeight = spectrumArea.getHeight();

    for (int i = 0; i < numDisplayBins; i++)
    {
        float targetFreq = 40.0f + i * freqStep; // Target frequency for this bin
        int fftIndex = static_cast<int>(targetFreq / (fs / Mu45EQAudioProcessor::fftSize)); // Corresponding FFT bin
        
        // Ensure that we're not accessing out of bounds
        if (fftIndex < 0) fftIndex = 0;
        if (fftIndex >= fftOutput.size()) fftIndex = fftOutput.size() - 1;

        float magnitude = fftOutput[fftIndex]*0.01f;
        float height = juce::jmap(magnitude, 0.0f, 1.0f, 0.0f, 50.0f);
        height = juce::jmin(height, maxBinHeight);
        juce::Rectangle<float> binRect(10+i * binWidth, spectrumArea.getBottom() - height, binWidth, height);
        g.setColour(juce::Colours::white);
        g.fillRect(binRect);
    }
}

void Mu45EQAudioProcessorEditor::timerCallback()
{
    // Trigger a repaint of just the spectrum area
    repaint(spectrumArea);
}

void Mu45EQAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void Mu45EQAudioProcessorEditor::sliderValueChanged(juce::Slider * slider)
{
    auto& params = processor.getParameters();
    if (slider == &mFilterFcSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(0);
        *audioParam = mFilterFcSlider.getValue(); // set the AudioParameter
        DBG("Fc Slider Changed");
    }
    else if (slider == &mFilterQSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(1);
        *audioParam = mFilterQSlider.getValue(); // set the param
        DBG("Q Slider Changed");
    }
    else if (slider == &mHSgSlider )
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(2);
        *audioParam = mHSgSlider.getValue(); // set the param
        DBG("HS gain Slider Changed");
    }
    else if (slider == &mHPfSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(3);
        *audioParam = mHPfSlider.getValue(); // set the AudioParameter
        DBG("HP Fc Slider Changed");
    }
    else if (slider == &mHPqSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(4);
        *audioParam = mHPqSlider.getValue(); // set the param
        DBG("HP Q Slider Changed");
    }
    else if(slider == &mLSgSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(5);
        *audioParam = mLSgSlider.getValue(); // set the param
        DBG("LS gain Slider Changed");
    }
    else if (slider == &mPfSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(6);
        *audioParam = mPfSlider.getValue(); // set the AudioParameter
        DBG("P Fc Slider Changed");
    }
    else if (slider == &mPqSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(7);
        *audioParam = mPqSlider.getValue(); // set the param
        DBG("P Q Slider Changed");
    }else if (slider == &mPgSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(8);
        *audioParam = mPgSlider.getValue(); // set the param
        DBG("P gain Slider Changed");
    }
    else if (slider == &mPfSlider2)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(9);
        *audioParam = mPfSlider2.getValue(); // set the AudioParameter
        DBG("P2 Fc Slider Changed");
    }
    else if (slider == &mPqSlider2)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(10);
        *audioParam = mPqSlider2.getValue(); // set the param
        DBG("P2 Q Slider Changed");
    }else if (slider == &mPgSlider2)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(11);
        *audioParam = mPgSlider2.getValue(); // set the param
        DBG("P2 gain Slider Changed");
    }
    else if (slider == &mPfSlider3)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(12);
        *audioParam = mPfSlider3.getValue(); // set the AudioParameter
        DBG("P3 Fc Slider Changed");
    }
    else if (slider == &mPqSlider3)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(13);
        *audioParam = mPqSlider3.getValue(); // set the param
        DBG("P3 Q Slider Changed");
    }else if (slider == &mPgSlider3)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(14);
        *audioParam = mPgSlider3.getValue(); // set the param
        DBG("P3 gain Slider Changed");
    };
}

void Mu45EQAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox) {
    if (comboBox == &mHPLSSelector) {
        int selectedType = mHPLSSelector.getSelectedId();
        if (selectedType == 1) {
            DBG("High Pass selected");
            audioProcessor.setFilterType_1(Mu45EQAudioProcessor::FilterType_1::HP);
            mHPqSlider.setEnabled(true);
            mLSgSlider.setEnabled(false);
        } else if (selectedType == 2) {
            DBG("Low Shelf selected");
            audioProcessor.setFilterType_1(Mu45EQAudioProcessor::FilterType_1::LS);
            mHPqSlider.setEnabled(false);
            mLSgSlider.setEnabled(true);
        }
    }else if(comboBox == &mLPHSSelector) {
        int selectedType = mLPHSSelector.getSelectedId();
        if (selectedType == 1) {
            DBG("Low Pass selected");
            audioProcessor.setFilterType_2(Mu45EQAudioProcessor::FilterType_2::LP);
            mFilterQSlider.setEnabled(true);
            mHSgSlider.setEnabled(false);
        } else if (selectedType == 2) {
            DBG("High Shelf selected");
            audioProcessor.setFilterType_2(Mu45EQAudioProcessor::FilterType_2::HS);
            mFilterQSlider.setEnabled(false);
            mHSgSlider.setEnabled(true);
        }
    }
}
