#pragma once
#include <string>
#include <windows.h>

// CGX Audio Extension
// Uses Windows Multimedia API (MCI & PlaySound) 
// Supports: BGM (MP3/WAV) via MCI, SFX (WAV) via PlaySound

class CGX_Audio {
public:
	// Plays background music in a loop (Supports .mp3, .wav)
	// Uses MCI (Media Control Interface) which allows concurrent playback with SFX
	static void playBGM(std::string filename);

	// Stops the currently playing background music
	static void stopBGM();

	// Plays a sound effect once (Best for .wav)
	// Uses PlaySound API (Async)
	static void playSFX(std::string filename);

	// Adjust Global Volume (MCI only - BGM)
	// Volume: 0 to 1000
	static void setBGMVolume(int volume);
};
