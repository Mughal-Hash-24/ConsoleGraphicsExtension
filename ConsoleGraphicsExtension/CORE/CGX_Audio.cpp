#include "../INCLUDE/CGX_Audio.h"
#include <mmsystem.h>
#include <iostream>

// Auto-link winmm.lib so the user doesn't have to change project settings
#pragma comment(lib, "winmm.lib")

using namespace std;

void CGX_Audio::playBGM(std::string filename) {
    // 1. Close any previous BGM
    stopBGM();

    // 2. Open the file using MCI
    // We use "mpegvideo" type to support both MP3 and WAV roughly
    // We alias it as "bgm" to control it later
    string cmdOpen = "open \"" + filename + "\" type mpegvideo alias bgm";
    MCIERROR err = mciSendStringA(cmdOpen.c_str(), NULL, 0, NULL);
    if (err != 0) {
        char errBuf[256];
        mciGetErrorStringA(err, errBuf, 256);
        std::cout << "[CGX_Audio Error] Open Failed: " << errBuf << " | Path: " << filename << std::endl;
        throw std::runtime_error("Failed to open BGM");
    }

    // 3. Play continuously
    string cmdPlay = "play bgm repeat";
    err = mciSendStringA(cmdPlay.c_str(), NULL, 0, NULL);
    if (err != 0) {
        char errBuf[256];
        mciGetErrorStringA(err, errBuf, 256);
        std::cout << "[CGX_Audio Error] Play Failed: " << errBuf << std::endl;
        throw std::runtime_error("Failed to play BGM");
    }
}

void CGX_Audio::stopBGM() {
    // Stop and Close the 'bgm' alias
    mciSendStringA("stop bgm", NULL, 0, NULL);
    mciSendStringA("close bgm", NULL, 0, NULL);
}

void CGX_Audio::playSFX(std::string filename) {
    // PlaySound only supports WAV. For MP3 SFX, we must use MCI with a separate alias to avoid stopping BGM.
    std::string alias = "sfx"; 
    
    // 1. Close previous SFX (if playing)
    // Note: This means rapid clicks might cut off the previous click sound, but that's standard for a single channel.
    mciSendStringA(("close " + alias).c_str(), NULL, 0, NULL);

    // 2. Open the file
    std::string cmdOpen = "open \"" + filename + "\" type mpegvideo alias " + alias;
    MCIERROR err = mciSendStringA(cmdOpen.c_str(), NULL, 0, NULL);
    
    if (err != 0) {
        // If fail, try fallback (maybe it's a short WAV?) or just log
        char errBuf[256];
        mciGetErrorStringA(err, errBuf, 256);
        // std::cout << "[CGX_Audio SFX Error] Open: " << errBuf << std::endl; 
        return;
    }

    // 3. Play
    std::string cmdPlay = "play " + alias;
    mciSendStringA(cmdPlay.c_str(), NULL, 0, NULL);
}

void CGX_Audio::setBGMVolume(int volume) {
    // Volume range 0-1000
    string cmd = "setaudio bgm volume to " + to_string(volume);
    mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}
