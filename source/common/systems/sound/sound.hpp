#pragma once

// #include <irrKlang.h>
// #include <iostream>

// using namespace irrklang;

// #pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

#include <miniaudio.h>
#include <string>        // To work with strings like filename
#include <unordered_map> // To manage multiple sounds
#include <memory>        // For smart pointers

// Class to manage multiple audio sounds
class AudioPlayer
{
public:
    // Constructor: Initializes the audio engine

    static AudioPlayer *instancePtr;

    AudioPlayer();

    // Destructor: Cleans up resources
    ~AudioPlayer();

    // Play a sound from a file and store it with a unique key

    void addSound(const std::string &key, const std::string &filename);
    void addOriginalsound(const std::string &key);
    void playNextSound();
    void playCurrentSound();
    void playPreviousSound();

    void playSound(const std::string &key);

    // Stop a sound based on the key
    void stopSound(const std::string &key);
    static AudioPlayer *getInstance();

    // Stop all sounds
    void stopAllSounds();

private:
    ma_engine engine; // The audio engine
    int currentSong;
    int cnt;
    std::unordered_map<int, std::string> originalSounds; // Stores the sounds
    std::unordered_map<std::string, ma_sound *> sounds;  // Stores the sounds
};