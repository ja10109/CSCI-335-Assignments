#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

#include <vector>

class AudioProcessor
{
public:
	std::vector<short> Compress(const std::vector<short>& audio, short threshold, float rate);
	std::vector<short> CutOutSilence(const std::vector<short>& audio, short level, int silenceLength);
	std::vector<short> StretchTwice(const std::vector<short>& audio);
	std::vector<short> Normalize(const std::vector<short>& audio, short normalizeTarget);

private:
	short round_closest(short dividend, short divisor);
};

#endif
