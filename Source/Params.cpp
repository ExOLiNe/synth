#include "Params.h"

void logJumps(const float* ptr, int channel, int numSamples, int msg)
{
    for (int i = 0; i < numSamples; ++i)
    {
        if (ptr[i] > 1)
        {
            std::stringstream ss;
            ss << "JUMP channel: " << channel << " value: " << ptr[i] << " flat: " << msg;
            juce::Logger::getCurrentLogger()->writeToLog(ss.str());
        }
    }
}