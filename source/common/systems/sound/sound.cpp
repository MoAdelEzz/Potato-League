#define MINIAUDIO_IMPLEMENTATION
#include "sound.hpp" // Include the header file

#include <iostream> // For error messages and debugging

// Constructor: Initializes the audio engine
AudioPlayer::AudioPlayer()
{
    ma_result result = ma_engine_init(nullptr, &engine); // Initialize the engine
    if (result != MA_SUCCESS)
    {
        std::cerr << "Failed to initialize audio engine: " << ma_result_description(result) << std::endl;
    }
}

// Destructor: Cleans up resources
AudioPlayer::~AudioPlayer()
{
    // Stop all sounds and uninitialize them
    stopAllSounds();
    ma_engine_uninit(&engine); // Clean up the engine
}

// Play a sound and store it with a unique key
void AudioPlayer::playSound(const std::string &key, const std::string &filename)
{
    // // Check if the key already exists, if it does, stop and replace the sound
    // if (sounds.find(key) != sounds.end())
    // {
    //     stopSound(key); // Stop the existing sound
    // }

    ma_sound *sound = new ma_sound();
    ma_result result = ma_sound_init_from_file(&engine, filename.c_str(), 0, nullptr, nullptr, sound);

    if (result == MA_SUCCESS)
    {
        ma_sound_start(sound); // Start playing the sound
        sounds[key] = sound;   // Store the sound in the map
        std::cout << "Playing sound: " << filename << std::endl;
    }
    else
    {
        std::cerr << "Error loading audio file: " << filename << std::endl;
        delete sound; // Clean up if initialization failed
    }
}

// Stop a specific sound based on the key
void AudioPlayer::stopSound(const std::string &key)
{
    if (sounds.find(key) != sounds.end())
    {
        ma_sound_stop(sounds[key]); // Stop the sound
        delete sounds[key];         // Clean up
        sounds.erase(key);          // Remove from the map
        std::cout << "Stopped sound: " << key << std::endl;
    }
    else
    {
        std::cerr << "Sound with key " << key << " not found." << std::endl;
    }
}

// Stop all sounds
void AudioPlayer::stopAllSounds()
{
    for (auto &pair : sounds)
    {
        ma_sound_stop(pair.second); // Stop each sound
        delete pair.second;         // Clean up
    }
    sounds.clear(); // Clear the map
    std::cout << "All sounds stopped." << std::endl;
}
