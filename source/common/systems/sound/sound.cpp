#define MINIAUDIO_IMPLEMENTATION
#include "sound.hpp" // Include the header file

#include <iostream> // For error messages and debugging

// Constructor: Initializes the audio engine
AudioPlayer::AudioPlayer()
{
    cnt = 0;
    currentSong = 0;
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
    // stopAllSounds();
    // ma_engine_uninit(&engine); // Clean up the engine
}

AudioPlayer *AudioPlayer::getInstance()
{
    // If there is no instance of class
    // then we can create an instance.
    if (AudioPlayer::instancePtr == NULL)
    {

        AudioPlayer::instancePtr = new AudioPlayer();

        return AudioPlayer::instancePtr;
    }
    else
    {
        return AudioPlayer::instancePtr;
    }
}

bool isSoundPlaying(ma_sound *sound)
{
    return ma_sound_is_playing(sound) == MA_TRUE;
}

// Play a sound and store it with a unique key
void AudioPlayer::playSound(const std::string &key)
{
    // Check if the key already exists, if it does, stop and replace the sound

    for (auto &s : sounds)
    {

        if (isSoundPlaying(s.second))
            stopSound(s.first); // Stop the existing sound
    }

    ma_sound_start(sounds[key]); // Start playing the sound
}

void AudioPlayer ::addSound(const std::string &key, const std::string &filename)
{
    ma_sound *sound = new ma_sound();
    ma_result result = ma_sound_init_from_file(&engine, filename.c_str(), 0, nullptr, nullptr, sound);

    if (result == MA_SUCCESS)
    {
        sounds[key] = sound; // Store the sound in the map
    }
    else
    {
        std::cerr << "Error loading audio file: " << filename << std::endl;
        delete sound; // Clean up if initialization failed
    }
}

void AudioPlayer ::addOriginalsound(const std::string &key)
{
    originalSounds[cnt] = key;
    cnt++;
}

void AudioPlayer ::playNextSound()
{
    for (auto &s : sounds)
    {

        if (isSoundPlaying(s.second))
            stopSound(s.first); // Stop the existing sound
    }

    currentSong++;
    currentSong %= cnt;
    ma_sound_start(sounds[originalSounds[currentSong]]); // Start playing the sound
}

void AudioPlayer ::playCurrentSound()
{
    bool check = true;
    for (auto &s : sounds)
    {

        if (isSoundPlaying(s.second))
            check = false;
    }
    if (check)
        ma_sound_start(sounds[originalSounds[currentSong]]); // Start playing the sound
}

void AudioPlayer ::playPreviousSound()
{
    for (auto &s : sounds)
    {

        if (isSoundPlaying(s.second))
            stopSound(s.first); // Stop the existing sound
    }

    currentSong--;
    if (currentSong < 0)
        currentSong = cnt - 1;
    ma_sound_start(sounds[originalSounds[currentSong]]); // Start playing the sound
}

// Stop a specific sound based on the key
void AudioPlayer::stopSound(const std::string &key)
{
    if (sounds.find(key) != sounds.end())
    {
        if (isSoundPlaying(sounds[key]))
        {
            ma_sound_stop(sounds[key]); // Stop the sound
            std::cout << "Stopped sound: " << key << std::endl;
        }
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
        if (isSoundPlaying(pair.second))
            ma_sound_stop(pair.second); // Stop each sound
    }
    std::cout << "All sounds stopped." << std::endl;
}
