/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Mu45EQAudioProcessor::Mu45EQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       fft(fftOrder)
#endif
{
    // Instantiate your AudioParameters
    // Filter CutOff parameter
    addParameter(mFilterFcParam = new juce::AudioParameterFloat("LP Filter Cutoff (Hz)", // parameterID,
    "LPFiltFc", // parameterName,
    40.0f, // minValue,
    10000.0f, // maxValue,
    10000.0f)); // defaultValue
    // Filter Q parameter
    addParameter(mFilterQParam = new juce::AudioParameterFloat("LP Q", // parameterID,
    "LPFiltQ", // parameterName,
    1.0f, // minValue,
    10.0f, // maxValue,
    3.0f)); // defaultValue
    addParameter(mHSgParam = new juce::AudioParameterFloat("HS gain", // parameterID,
    "HSGain", // parameterName,
    -100.0f, // minValue,
    12.0f, // maxValue,
    0.0f)); // defaultValue
    
    addParameter(mHPFcParam = new juce::AudioParameterFloat("HP Cutoff (Hz)", // parameterID,
    "HPFiltFc", // parameterName,
    40.0f, // minValue,
    10000.0f, // maxValue,
    40.0f)); // defaultValue
    // Filter Q parameter
    addParameter(mHPQParam = new juce::AudioParameterFloat("HP Q", // parameterID,
    "HPFiltQ", // parameterName,
    1.0f, // minValue,
    10.0f, // maxValue,
    3.0f)); // defaultValue
    addParameter(mLSgParam = new juce::AudioParameterFloat("LS gain", // parameterID,
    "LSGain", // parameterName,
    -100.0f, // minValue,
    12.0f, // maxValue,
    0.0f)); // defaultValue

    addParameter(mPeakfParam = new juce::AudioParameterFloat("Peak Cutoff (Hz)", // parameterID,
    "PeakFiltFc", // parameterName,
    40.0f, // minValue,
    10000.0f, // maxValue,
    300.0f)); // defaultValue
    // Filter Q parameter
    addParameter(mPeakqParam = new juce::AudioParameterFloat("Peak Q", // parameterID,
    "PeakFiltQ", // parameterName,
    1.0f, // minValue,
    20.0f, // maxValue,
    3.0f)); // defaultValue
    addParameter(mPeakgParam = new juce::AudioParameterFloat("Peak gain",
    "PeakFiltG",
    -60.0f,
    24.0f,
    0.0f));
    
    addParameter(mPeakfParam2 = new juce::AudioParameterFloat("Peak2 Cutoff (Hz)", // parameterID,
    "PeakFiltFc2", // parameterName,
    40.0f, // minValue,
    10000.0f, // maxValue,
    1000.0f)); // defaultValue
    // Filter Q parameter
    addParameter(mPeakqParam2 = new juce::AudioParameterFloat("Peak2 Q", // parameterID,
    "PeakFiltQ2", // parameterName,
    1.0f, // minValue,
    20.0f, // maxValue,
    3.0f)); // defaultValue
    addParameter(mPeakgParam2 = new juce::AudioParameterFloat("Peak2 gain",
    "PeakFiltG2",
    -60.0f,
    24.0f,
    0.0f));
    
    addParameter(mPeakfParam3 = new juce::AudioParameterFloat("Peak3 Cutoff (Hz)", // parameterID,
    "PeakFiltFc3", // parameterName,
    40.0f, // minValue,
    10000.0f, // maxValue,
    3000.0f)); // defaultValue
    // Filter Q parameter
    addParameter(mPeakqParam3 = new juce::AudioParameterFloat("Peak3 Q", // parameterID,
    "PeakFiltQ3", // parameterName,
    1.0f, // minValue,
    20.0f, // maxValue,
    3.0f)); // defaultValue
    addParameter(mPeakgParam3 = new juce::AudioParameterFloat("Peak3 gain",
    "PeakFiltG3",
    -60.0f,
    24.0f,
    0.0f));
}

Mu45EQAudioProcessor::~Mu45EQAudioProcessor()
{
}

//==============================================================================
const juce::String Mu45EQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Mu45EQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Mu45EQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Mu45EQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Mu45EQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Mu45EQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Mu45EQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Mu45EQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Mu45EQAudioProcessor::getProgramName (int index)
{
    return {};
}

void Mu45EQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Mu45EQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mFs = sampleRate;
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Mu45EQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Mu45EQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif
void Mu45EQAudioProcessor::setFilterType_1(Mu45EQAudioProcessor::FilterType_1 type){
    currenttype_1 = type;
}
Mu45EQAudioProcessor::FilterType_1 Mu45EQAudioProcessor::getFilterType_1(){
    return currenttype_1;
}
void Mu45EQAudioProcessor::setFilterType_2(Mu45EQAudioProcessor::FilterType_2 type){
    currenttype_2 = type;
}
Mu45EQAudioProcessor::FilterType_2 Mu45EQAudioProcessor::getFilterType_2(){
    return currenttype_2;
}
void Mu45EQAudioProcessor::calcAlgorithmParams()
{
    // Get the user parameter values
    float fc = mFilterFcParam->get();
    float Q = mFilterQParam->get();
    float HSgain = mHSgParam->get();
    
    float hpfc = mHPFcParam->get();
    float hpq = mHPQParam->get();
    float LSgain = mLSgParam -> get();
    
    float peakf = mPeakfParam->get();
    float peakq = mPeakqParam->get();
    float peakg = mPeakgParam->get();
    
    float peakf2 = mPeakfParam2->get();
    float peakq2 = mPeakqParam2->get();
    float peakg2 = mPeakgParam2->get();
    
    float peakf3 = mPeakfParam3->get();
    float peakq3 = mPeakqParam3->get();
    float peakg3 = mPeakgParam3->get();
    
    // Calculate filter coefficients
    float coeffs[5]; // an array of 5 floats for filter coeffs: [b0, b1, b2, a1, a2]
    Mu45FilterCalc::calcCoeffsLPF(coeffs, fc, Q, mFs);
    
    float hpcoeffs[5];
    Mu45FilterCalc::calcCoeffsHPF(hpcoeffs, hpfc, hpq, mFs);
    
    float peakcoeffs[5];
    Mu45FilterCalc::calcCoeffsPeak(peakcoeffs, peakf, peakg, peakq, mFs);
    
    float peakcoeffs2[5];
    Mu45FilterCalc::calcCoeffsPeak(peakcoeffs2, peakf2, peakg2, peakq2, mFs);
    
    float peakcoeffs3[5];
    Mu45FilterCalc::calcCoeffsPeak(peakcoeffs3, peakf3, peakg3, peakq3, mFs);
    
    float HScoeffs[5];
    Mu45FilterCalc::calcCoeffsHighShelf(HScoeffs, fc, HSgain, mFs);
    
    float LScoeffs[5];
    Mu45FilterCalc::calcCoeffsLowShelf(LScoeffs, hpfc, LSgain, mFs);
    
    // Set the coefficients for each filter
    if(getFilterType_2() == LP){
        mFilterL.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
        mFilterR.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
    }else if(getFilterType_2() == HS){
        mFilterL.setCoefficients(HScoeffs[0], HScoeffs[1], HScoeffs[2], HScoeffs[3], HScoeffs[4]);
        mFilterR.setCoefficients(HScoeffs[0], HScoeffs[1], HScoeffs[2], HScoeffs[3], HScoeffs[4]);
    }
    if(getFilterType_1() == HP){
        mHPL.setCoefficients(hpcoeffs[0], hpcoeffs[1], hpcoeffs[2], hpcoeffs[3], hpcoeffs[4]);
        mHPR.setCoefficients(hpcoeffs[0], hpcoeffs[1], hpcoeffs[2], hpcoeffs[3], hpcoeffs[4]);
    }else if (getFilterType_1() == LS){
        mHPL.setCoefficients(LScoeffs[0], LScoeffs[1], LScoeffs[2], LScoeffs[3], LScoeffs[4]);
        mHPR.setCoefficients(LScoeffs[0], LScoeffs[1], LScoeffs[2], LScoeffs[3], LScoeffs[4]);
    }
    
    mPL.setCoefficients(peakcoeffs[0], peakcoeffs[1], peakcoeffs[2], peakcoeffs[3], peakcoeffs[4]);
    mPR.setCoefficients(peakcoeffs[0], peakcoeffs[1], peakcoeffs[2], peakcoeffs[3], peakcoeffs[4]);
    
    mPL2.setCoefficients(peakcoeffs2[0], peakcoeffs2[1], peakcoeffs2[2], peakcoeffs2[3], peakcoeffs2[4]);
    mPR2.setCoefficients(peakcoeffs2[0], peakcoeffs2[1], peakcoeffs2[2], peakcoeffs2[3], peakcoeffs2[4]);
    
    mPL3.setCoefficients(peakcoeffs3[0], peakcoeffs3[1], peakcoeffs3[2], peakcoeffs3[3], peakcoeffs3[4]);
    mPR3.setCoefficients(peakcoeffs3[0], peakcoeffs3[1], peakcoeffs3[2], peakcoeffs3[3], peakcoeffs3[4]);
    // DBG("Fc = "<<fc); // For debugging
}


void Mu45EQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // JUCE book-keeping to make sure there's no bad data in the output buffers
    juce:: ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear (i, 0, buffer.getNumSamples());
    // Update the algorithm params
    calcAlgorithmParams();
    // Get the left and right audio buffers
    auto* channelDataLeft = buffer.getWritePointer(0);
    auto* channelDataRight = buffer.getWritePointer(1);
    // The inner loop! Step through each sample in the audio buffer
    for (int samp = 0; samp < buffer.getNumSamples(); samp++)
    {
    // Process each audio sample
        
        channelDataLeft[samp] = mHPL.tick(channelDataLeft[samp]);
        channelDataRight[samp] = mHPR.tick(channelDataRight[samp]);
        
        channelDataLeft[samp] = mPL.tick(channelDataLeft[samp]);
        channelDataRight[samp] = mPR.tick(channelDataRight[samp]);
        
        channelDataLeft[samp] = mPL2.tick(channelDataLeft[samp]);
        channelDataRight[samp] = mPR2.tick(channelDataRight[samp]);
        
        channelDataLeft[samp] = mPL3.tick(channelDataLeft[samp]);
        channelDataRight[samp] = mPR3.tick(channelDataRight[samp]);
        
        channelDataLeft[samp] = mFilterL.tick(channelDataLeft[samp]);
        channelDataRight[samp] = mFilterR.tick(channelDataRight[samp]);
    }
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);
            // Fill fftData and process FFT
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                fftData[fftDataPosition] = channelData[i];
                fftDataPosition++;
                // If fftData is full, perform FFT and reset fftDataPosition
                if (fftDataPosition == fftSize)
                {
                    std::fill(fftOutput.begin(), fftOutput.end(), 0);
                    juce::FloatVectorOperations::copy(fftOutput.data(), fftData.data(), fftSize);
                    fft.performFrequencyOnlyForwardTransform(fftOutput.data());
                    fftDataPosition = 0;
                }
            }
        }

}

//==============================================================================
bool Mu45EQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Mu45EQAudioProcessor::createEditor()
{
    return new Mu45EQAudioProcessorEditor (*this);
}

//==============================================================================
void Mu45EQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Mu45EQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Mu45EQAudioProcessor();
}
