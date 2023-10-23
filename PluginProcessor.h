/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "StkLite-4.6.2/BiQuad.h"
#include <juce_dsp/juce_dsp.h>

//==============================================================================
/**
*/
class Mu45EQAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    Mu45EQAudioProcessor();
    ~Mu45EQAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    enum FilterType_1
        {
            HP,
            LS,
        };
    enum FilterType_2
        {
            LP,
            HS,
        };
    void setFilterType_1(FilterType_1 type);
    FilterType_1 getFilterType_1();
    void setFilterType_2(FilterType_2 type);
    FilterType_2 getFilterType_2();
    
    juce::dsp::FFT fft;
    static const int fftOrder = 11; // 2048
    static const int fftSize = 1 << fftOrder;
    std::array<float, fftSize> fftData;
    std::array<float, fftSize * 2> fftOutput;
    std::array<float, fftSize * 2>& getFFTOutput()  {
        return fftOutput;
    }
    int fftDataPosition = 0;

    
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mu45EQAudioProcessor)
    void calcAlgorithmParams();

    // User Parameters
    juce::AudioParameterFloat* mFilterFcParam;
    juce::AudioParameterFloat* mFilterQParam;
    juce::AudioParameterFloat* mHSgParam;
    
    juce::AudioParameterFloat* mHPFcParam;
    juce::AudioParameterFloat* mHPQParam;
    juce::AudioParameterFloat* mLSgParam;
    
    juce::AudioParameterFloat* mPeakfParam;
    juce::AudioParameterFloat* mPeakqParam;
    juce::AudioParameterFloat* mPeakgParam;
    
    juce::AudioParameterFloat* mPeakfParam2;
    juce::AudioParameterFloat* mPeakqParam2;
    juce::AudioParameterFloat* mPeakgParam2;
    
    juce::AudioParameterFloat* mPeakfParam3;
    juce::AudioParameterFloat* mPeakqParam3;
    juce::AudioParameterFloat* mPeakgParam3;
    // Mu45: Objects we need
    stk::BiQuad mFilterL, mFilterR; // The filters
    stk::BiQuad mHPL,mHPR; //high pass/ LS
    stk::BiQuad mLPL,mLPR; // low pass/ HS
    stk::BiQuad mPL,mPR; // peak
    stk::BiQuad mPL2,mPR2; // peak2
    stk::BiQuad mPL3,mPR3; // peak3

    
    float mFs; // Sampling rate
    FilterType_1 currenttype_1;
    FilterType_2 currenttype_2;
};
