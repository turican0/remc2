#include "EFXPresets.h"

bool InitializeOpenAL(char *szALDevice, unsigned long ulNumOutputChannels, unsigned long ulNumEffects, unsigned long ulNumSamples, unsigned long ulFrequency);
bool ShutdownOpenAL();
unsigned long GetNumEffects();
unsigned long GetNumOutputChannels();
unsigned long GetNumSamplesPerBuffer();
bool Start();
bool Stop();
unsigned long GetBuffersProcessed();
void CommitData(void *pOutput, void **pEffectOutputs);
bool SetEFXEAXReverbProperties(EFXEAXREVERBPROPERTIES *pEFXEAXReverb, unsigned long ulEffectNumber);
