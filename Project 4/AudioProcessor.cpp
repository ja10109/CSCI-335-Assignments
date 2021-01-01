#include "AudioProcessor.h"
#include <stdexcept>
#include <algorithm>

//function COMPRESS
//returns a vector of shorts with values from audio vector but values higher than greater than the threshold is modified
//in the algorithm (value - threshold)/rate + threshold
std::vector<short> AudioProcessor::Compress(const std::vector<short>& audio, short threshold, float rate)
{
	//INVALID ARGUMENT CHECKS
	if (threshold < 0)
		throw std::invalid_argument("Threshold is negative");
	if (rate < 1.0)
		throw std::invalid_argument("Rate is less than 1.0");

	//set vector compressed with audio vector size
	std::vector<short> compressed(audio.size());
	//set negative threshold value 
	short negative_threshold = threshold * -1;
	float n{ 0.0 };

	for (size_t i{ 0 }; i < audio.size(); i++)
	{
		//if audio value > 0 && > threshold, adjust
		if (audio[i] >= 0 && audio[i] > threshold)
		{
			//float n is the modified value
			n = (audio[i] - threshold) / rate + threshold;
			//first check tenth decimal of n to see if it's rounded up
			if (int(n * 10) % 10 >= 5)
				n++;
			//new compressed value is rounded n
			compressed[i] = short(n);
		}
		//if audio < 0 && < -(threshold), adjust
		else if (audio[i] < 0 && audio[i] < negative_threshold)
		{
			//float n is modified audio value
			n = ((std::abs(audio[i]) - threshold) / rate + threshold);
			//check decimal value to round up or not
			if (int(n * 10) % 10 >= 5)
				n++;
			//change back to negative
			n = n * -1;
			//new compressed value is rounded n
			compressed[i] = short(n);
		}
		//audio values within the threshold remains the same
		else
			compressed[i] = audio[i];
	}
	return compressed;
}

//function CUTOUTSILENCE takes a vector input of shorts and reduces the vector based on audio level and silence
//ex: {1,-3,10,25,56,-1,-2,-16,-20,-8,-1,1,7,20} level = 10, length = 3 -> {25,56,-1,-2,-16,-20,12}
std::vector<short> AudioProcessor::CutOutSilence(const std::vector<short>& audio, short level, int silenceLength)
{
	if (level < 0)
		throw std::invalid_argument("Level is negative");
	if (silenceLength < 1)
		throw std::invalid_argument("SilenceLength is less than 1");
	std::vector<short> rt;

	bool is_silence = false;
	int counter{ 0 };
	int iterator{ 0 };
	for (size_t i{ 0 }; i < audio.size(); i++)
	{
		//if audio is a silence set bool to true
		if (std::abs(audio[i]) <= level && audio.size() - i >= silenceLength)
		{
			is_silence = true;
			iterator = i;
		}
		//while silence is true
		while (is_silence == true)
		{
			//if # of consecutive silences are enough, and audio isn't a silence, set silence to false and modify i
			if (counter >= silenceLength && std::abs(audio[iterator]) > level)
			{
				i = iterator;
				counter = 0;
				is_silence = false;
			}
			//continue if silence 
			else if (std::abs(audio[iterator]) <= level)
			{
				iterator++;
				counter++;
			}
			//if not silence and not enough consecutive silences do nothing
			else
			{
				counter = 0;
				is_silence = false;
			}
		}
		if(i < audio.size())
			rt.push_back(audio[i]);
	}

	
	return rt;
}

//function STRETCHTWICE
//returns a vector with modified values between each original audio values
//ex: {1,4,23,-2,28,10} -> {1,3,4,14,23,11,-2,-15,-28,-9,10}
std::vector<short> AudioProcessor::StretchTwice(const std::vector<short>& audio)
{
	std::vector<short> adjust(0);
	if (audio.size() == 0)
		return adjust;
	//new vector size is original size * 2 - 1
	int adjusted_size = audio.size() * 2 - 1;
	//set new return vector with adjusted size
	adjust.resize(adjusted_size);
	int audio_index{ 0 };
	for (size_t i{ 0 }; i < adjusted_size; i++)
	{
		//at each odd index of the vector, add the STRECTHED value
		if (i % 2 == 1)
		{
			//find STRECTHED value using round_closest helper function
			short modified = round_closest(std::abs((audio[audio_index] + audio[audio_index+1])),2);
			//if original values were negative set to negative
			if ((audio[audio_index] + audio[audio_index + 1]) < 0)
				modified = modified * -1;
			//set value
			adjust[i] = modified;
			//increment in original audio vector
			audio_index++;
		}
		//else value stays the same
		else
			adjust[i] = audio[audio_index];
	}
	return adjust;
}

//function NORMALIZE that adjust audio values by multiplying values by the abs(greatest audio value) / normalizeTarget
//ex: {1,3,-2,5,-4,0} normalizeTarget = 10, so modifier is 20/10 = 2 -> {2,6,-4,10,-8,0}
std::vector<short> AudioProcessor::Normalize(const std::vector<short>& audio, short normalizeTarget)
{
	//INVALID ARGUMENT CHECK
	if (normalizeTarget < 1)
		throw std::invalid_argument("noramlizeTarget is less than 1");

	//return vector normalized set to audio vector size
	std::vector<short> normalized(audio.size());
	float modifier{ 0.0 };
	float n{ 0.0 };

	//first find the absolute value of the largest audio value
	for (size_t i{ 0 }; i < audio.size(); i++)
		modifier = std::max(std::abs(float(audio[i])), modifier);
	for (size_t i{ 0 }; i < audio.size(); i++)
	{
		//float n is the modified value
		n = std::abs(audio[i]) * (normalizeTarget / modifier);
		//check if n needs to be rounded up
		if (int(n * 10) % 10 >= 5)
			n++;
		//vector value is n rounded
		normalized[i] = short(n);
		//if original audio value was negative, set new audio value to negative as well
		if (audio[i] < 0)
			normalized[i] = normalized[i] * -1;
	}
	return normalized;
}

//helper function for stretchTwice function to round short values
short AudioProcessor::round_closest(short dividend, short divisor)
{
	return (dividend + (divisor / 2)) / divisor;
}