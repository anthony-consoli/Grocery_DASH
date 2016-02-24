#pragma once

#include <xaudio2.h>
#include <windows.h>
#include <xact3.h>
#include "GroceryDashAudio.h"
#include <unordered_map>
#include <string>



IXAudio2 *engine;
IXAudio2MasteringVoice * pMasteringVoice;
IXAudio2SourceVoice * pSourceVoice;
CWaveFile wav;
//IXAudio2SourceVoice* sourceVoices[4];
//CWaveFile wav1;
//unsigned char *wavData;
//unsigned long wavSize;


GroceryDashAudio::GroceryDashAudio(){

	CoInitializeEx(0, COINIT_MULTITHREADED);
	// Create an IXAudio2 object
	HRESULT hr = XAudio2Create(&engine);


	// Create a mastering voice
	hr = engine->CreateMasteringVoice(&pMasteringVoice);
	
	LPSTR audioNames[NUM_MUSIC_FILES];
	audioNames[0] = "data/audio/DashTheme.wav";
	audioNames[1] = "data/audio/BassSelect.wav";
	audioNames[2] = "data/audio/BellSelect.wav";
	audioNames[3] = "data/audio/CartToWall.wav";
	audioNames[4] = "data/audio/BellOk.wav";
	audioNames[5] = "data/audio/BellBack.wav";
	audioNames[6] = "data/audio/Level1.wav";
	audioNames[7] = "data/audio/Level2.wav";
	audioNames[8] = "data/audio/Level3.wav";
	audioNames[9] = "data/audio/checkoutSound.wav";


	//wavFileArray = new CWaveFile[4];

	for (int i = 0; i < NUM_MUSIC_FILES; i++)
	{

		wav.Open(audioNames[i], 0, WAVEFILE_READ);
		WAVEFORMATEX *waveformat = wav.GetFormat();
		unsigned long wavSize = wav.GetSize();
		unsigned char *wavData = new unsigned char[wavSize];
		//wav.Read(wavData, wavSize, &wavSize);

		//Store wav
		wavFileArray[i] = wav;

		engine->CreateSourceVoice(&pSourceVoice, waveformat,0,XAUDIO2_DEFAULT_FREQ_RATIO,0,0,0);
		sourceVoices.push_back(pSourceVoice);
		if (i == (NUM_MUSIC_FILES-1))
		{
			std::copy(wavFileArray, &wavFileArray[sizeof(wavFileArray) / sizeof(CWaveFile)], std::back_inserter(wavFiles));
		}
	}

	
	for (int i = 0; i < NUM_MUSIC_FILES; i++)
	{
		// Start the source voice
		sourceVoices[i]->Start(0, XAUDIO2_COMMIT_NOW);
	}
	
	
	

	
}
	 void GroceryDashAudio::playAudio(int i)
	 {
		 sourceVoices[i]->Start(0, XAUDIO2_COMMIT_NOW);
		 unsigned long wavSize = wavFiles[i].GetSize();
		 unsigned char *wavData = new unsigned char[wavFiles[i].GetSize()];
		 wavFiles[i].ResetFile();
		 wavFiles[i].Read(wavData, wavSize, &wavSize);
		 // Create a button to reference the byte array
		 XAUDIO2_BUFFER buffer = { 0 };
		 buffer.AudioBytes = wavSize;
		 buffer.pAudioData = wavData;
		 buffer.Flags = XAUDIO2_END_OF_STREAM;
		 buffer.PlayBegin = 0;
		 if (i == 0 || i == 6 || i == 7 || i == 8)
			buffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;
		// buffer.PlayLength = 5 * 44100;
		 // Submit the buffer
		 HRESULT hr = sourceVoices[i]->SubmitSourceBuffer(&buffer);




	 }

	 unsigned long GroceryDashAudio::getWav(){
		 return 0;
	 }

	 IXAudio2SourceVoice* GroceryDashAudio::getVoice(int i)
	 {
		 return sourceVoices[i];
	 }

