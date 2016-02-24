/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameText.h

	This class allows for easy rendering of text
	to the game screen.
*/

#pragma once
#include "sssf_VS\stdafx.h"

class Game;
class TextGenerator;

const int MAX_TEXT_OBJECTS = 10;

class TextToDraw
{
private:
	wstring *text;

public:
	TextToDraw()	{}
	~TextToDraw()	{}
	int maxChars;
	int numChars;
	int x;
	int y;
	int width;
	int height;

	void setText(wstring *textToSet)
	{
		text = textToSet;
	}
	wstring* getText() { return text; }
};

class GameText
{
private:
	int textCounter;
	vector<TextToDraw*> *textToDraw;
	vector<TextToDraw*> *gameTextToDraw;
	vector<TextToDraw*> *settingsTextToDraw;
	vector<TextToDraw*> *itemListTextToDraw;
	vector<TextToDraw*> *statsTextToDraw;
	TextGenerator *textGenerator;

	// USED TO PRINT DEBUGGING OUTPUT
	TextFileWriter writer;

public:
	// INLINED ACCESSOR METHODS
	TextGenerator*	getTextGenerator()									{ return textGenerator; }

	int				getNumTextObjectsToDraw()							{ return textToDraw->size();		}
	TextToDraw*		getTextToDrawAtIndex(int index)						{ return textToDraw->at(index); }

	int				getNumGameTextObjectsToDraw()						{ return gameTextToDraw->size(); }
	TextToDraw*		getGameTextToDrawAtIndex(int index)					{ return gameTextToDraw->at(index); }

	int				getNumSettingsTextObjectsToDraw()					{ return settingsTextToDraw->size(); }
	TextToDraw*		getSettingsTextToDrawAtIndex(int index)				{ return settingsTextToDraw->at(index); }

	int				getNumItemListTextToDrawTextObjectsToDraw()			{ return itemListTextToDraw->size(); }
	TextToDraw*		getItemListTextToDrawTextToDrawAtIndex(int index)	{ return itemListTextToDraw->at(index); }
	
	int				getNumStatsTextToDrawTextObjectsToDraw()			{ return statsTextToDraw->size(); }
	TextToDraw*		getStatsTextToDrawTextToDrawAtIndex(int index)	{ return statsTextToDraw->at(index); }
	

	// INLINED MUTATOR METHOD
	void setTextGenerator(TextGenerator *initTextGenerator)
	{
		textGenerator = initTextGenerator;
	}

	// METHODS DEFINED IN GameText.cpp
	GameText();
	~GameText();
	void addText(wstring *textToAdd, int initX, int initY, int initWidth, int initHeight);
	void addGameText(wstring *textToAdd, int initX, int initY, int initWidth, int initHeight);
	void addSettingsText(wstring *textToAdd, int initX, int initY, int initWidth, int initHeight);
	void addInGameItemListText(wstring *textToAdd, int initX, int initY, int initWidth, int initHeight);
	void addStatsText(wstring *textToAdd, int initX, int initY, int initWidth, int initHeight);

	void changeTextOnly(wstring *textToAdd, int index);
	void changeGameTextOnly(wstring *textToAdd, int index);
	void changeSettingsTextOnly(wstring *textToSet, int index);
	void changeInGameItemListTextOnly(wstring *textToSet, int index);
	void changeStatsTextOnly(wstring *textToSet, int index);

	void initDebugFile(string debugFileName);
	void moveText(int index, int xMove, int yMove);
	void writeDebugOutput(string output);
};