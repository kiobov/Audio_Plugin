/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


auto getPhaserRateName() { return juce::String("Phaser RateHz"); }
auto getPhaserCenterFreqName() { return juce::String("Phaser Center FreqHZ"); }
auto getPhaserDepthName() { return juce::String("Phaser Depth %"); }
auto getPhaserFeedbackName() { return juce::String("Phaser Feedback %"); }
auto getPhaserMixName() { return juce::String("Phaser Mix %"); }

auto getChorusRateName() { return juce::String("Chorus RateHz"); }
auto getChorusDepthName() { return juce::String("Chorus Depth Ms"); }
auto getChorusCenterDelayName() { return juce::String("Chorus Center Delay Ms"); }
auto getChorusFeedbackName() { return juce::String("Chorus Feedback %"); }
auto getChorusMixName() { return juce::String("Chorus Mix %"); }




//==============================================================================
Project13AudioProcessor::Project13AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    auto floatParams = std::array
    {
        &phaserRateHz,
        &phaserCenterFreqHz,
        &phaserDepthPercent,
        &phaserFeedbackPercent,
        &phaserMixPercent,

        & chorusRateHz ,
        & chorusDepthPercent ,
        & chorusCenterDelayMs ,
        & chorusFeedbackPercent ,
        & chorusMixPercent ,

    };
    auto floatNameFuncs = std::array
    {
        &getPhaserRateName,
        &getPhaserCenterFreqName,
        &getPhaserDepthName,
        &getPhaserFeedbackName,
        &getPhaserMixName,

        & getChorusRateName,
        & getChorusDepthName,
        & getChorusCenterDelayName,
        & getChorusFeedbackName,
        & getChorusMixName,

    };

    for (size_t i = 0; i < floatParams.size(); ++i) {
        auto ptrToParamPtr = floatParams[i];
        *ptrToParamPtr = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(
            floatNameFuncs[i]()));
        jassert(*ptrToParamPtr != nullptr);
    }

}

Project13AudioProcessor::~Project13AudioProcessor()
{
}

//==============================================================================
const juce::String Project13AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Project13AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Project13AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Project13AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Project13AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Project13AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Project13AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Project13AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Project13AudioProcessor::getProgramName (int index)
{
    return {};
}

void Project13AudioProcessor::changeProgramName (int index, const juce::String& newName)
{

}

//==============================================================================
void Project13AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Project13AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Project13AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
juce::AudioProcessorValueTreeState::ParameterLayout 
Project13AudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    const int versionHint = 1;
    /*
    Phaser:
    Rate: hz
    Depth: 0 to 1
    Center: frq hz
    Feedback: -1 to +1
    Mix:  0-1*/
    auto name = getPhaserRateName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{
        name, versionHint },name, 
        juce::NormalisableRange<float>(0.01f, 2.f, 0.01f,1.f), 
        0.2f, "Hz"));
     name = getPhaserCenterFreqName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{
        name, versionHint }, name,
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
        1000.f, "Hz"));
    //what depth comes before center
     name = getPhaserDepthName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{
        name, versionHint }, name,
        juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f),
        0.05f, "%"));
     name = getPhaserFeedbackName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{
        name, versionHint }, name,
        juce::NormalisableRange<float>(-.1f, 1.f, 0.01f, 1.f),
        0.0f, "%"));
     name = getPhaserMixName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{
        name, versionHint }, name,
        juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f),
        0.05f, "%"));

    /*
    Chorus:
    Rate: hz
    Depth: 0 to 1
    Center delay: frq hz
    Feedback: -1 to +1
    Mix:  0-1*/
    name = getChorusRateName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{name,
    versionHint}, 
        name, 
        juce::NormalisableRange<float>(0.01f, 100.f, 0.01f,1.f), 0.2f,"Hz")
        );
    name = getChorusDepthName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name,
    versionHint },
        name,
        juce::NormalisableRange<float>(0.01f, 100.f, 0.01f, 1.f), 0.05f, "Hz")
    );
    name = getChorusCenterDelayName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name,
    versionHint },
        name,
        juce::NormalisableRange<float>(0.01f, 100.f, 0.01f, 1.f), 7.f, "%")
    );
    name = getChorusFeedbackName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name,
    versionHint },
        name,
        juce::NormalisableRange<float>(0.01f, 100.f, 0.01f, 1.f), 0.0f, "%")
    );

    name = getChorusMixName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name,
    versionHint },
        name,
        juce::NormalisableRange<float>(0.01f, 100.f, 0.01f, 1.f), 0.05f, "%")
    );

    return layout;
}



void Project13AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    //Done: add APVTS 
    //TODO:  create audio parameters for all dsp choices 
    //TODO: update dsp  here from audio paramenter 
    //TODO: save / load settinggss 
    //TODO: save and load dsp order 
    //TODO: drag -to-reorder gui 
    //TODO: gui designing for ech dsp instance 
    //TODO: metering 
    //TODO: prepare all dsp 
    //TODO: wet/dry knob 
    //TODO:  mono &stereo versions 
    //TODO:  modulators 
    //TODO:  thread safe filter updating 
    //TODO:  pre/post filtering 
    //TODO:  delay module 


    auto newDSPOrder = DSP_Order();

    while (dspOrderFifo.pull(newDSPOrder)) 
    {

    }

    //if pulled replace dsporeder
    if (newDSPOrder != DSP_Order())
        dspOrder = newDSPOrder;

    //convert ds[p order to array of pointers 
    DSP_Pointers dspPointers;

    for (size_t i = 0; i < dspPointers.size(); ++i) 
    {
        switch (dspOrder[i]) 
        {
        case DSP_Option::Phase:
                dspPointers[i] = &phaser;
                break;
        case DSP_Option::Chorus:
            dspPointers[i] = &chorus;
            break;
        case DSP_Option::OverDrive:
            dspPointers[i] = &overdrive;
            break;
        case DSP_Option::LadderFilter:
            dspPointers[i] = &ladderFilter;
            break;
        case DSP_Option::END_OF_LIST:
            jassertfalse;
            break;
        }
    }

    //NOW PROCESS 
    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    for (size_t i = 0; i < dspPointers.size(); ++i) {
        if (dspPointers[i] != nullptr) {
            dspPointers[i] -> process(context);

        }
    }
    
}

//==============================================================================
bool Project13AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Project13AudioProcessor::createEditor()
{
    return new Project13AudioProcessorEditor (*this);
}

//==============================================================================
void Project13AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Project13AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Project13AudioProcessor();
}
