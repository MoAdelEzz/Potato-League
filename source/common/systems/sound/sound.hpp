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
    AudioPlayer();

    // Destructor: Cleans up resources
    ~AudioPlayer();

    // Play a sound from a file and store it with a unique key
    void playSound(const std::string &key, const std::string &filename);

    // Stop a sound based on the key
    void stopSound(const std::string &key);

    // Stop all sounds
    void stopAllSounds();

private:
    ma_engine engine;                                   // The audio engine
    std::unordered_map<std::string, ma_sound *> sounds; // Stores the sounds
};