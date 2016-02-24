#pragma once

#include <xaudio2.h>
#include <xact3.h>
#include "SDKwavefile.h"
#include <vector>
#include <iterator>
#include <algorithm>


class GroceryDashAudio{
public:

	static const int NUM_MUSIC_FILES = 10;
	
	//Vector for wavFiles
	std::vector<CWaveFile> wavFiles;
	
	//Vector containing source voices
	std::vector<IXAudio2SourceVoice*> sourceVoices;
	
	//Array for Wav Files
	CWaveFile wavFileArray[NUM_MUSIC_FILES];
	
	
	//Default Constructor
	GroceryDashAudio();


	//Methods to load and play sounds
	void GroceryDashAudio::loadSourceVoice();
	void GroceryDashAudio::playAudio(int i);
	CWaveFile* GroceryDashAudio::loadWavFiles();
	void GroceryDashAudio::createSourceVoice();

	//Getter Methods
	IXAudio2SourceVoice* getVoice(int i);
	unsigned long GroceryDashAudio::getWav();
	
	
};