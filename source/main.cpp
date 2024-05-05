#include <iostream>
#include <fstream>
#include <flags/flags.h>
#include <json/json.hpp>
#include <filesystem>
#include <vector>

#include <application.hpp>
#include "states/level-select-state.hpp"
#include "states/menu-state.hpp"
#include "states/loading-screen-state.hpp"
#include "states/shader-test-state.hpp"
#include "states/mesh-test-state.hpp"
#include "states/transform-test-state.hpp"
#include "states/pipeline-test-state.hpp"
#include "states/texture-test-state.hpp"
#include "states/sampler-test-state.hpp"
#include "states/material-test-state.hpp"
#include "states/entity-test-state.hpp"
#include "states/renderer-test-state.hpp"
#include "states/level1.hpp"
#include "states/level2.hpp"
#include "states/level3.hpp"
#include "states/level4.hpp"

#include "common/systems/sound/sound.hpp"

#include "states/win-state.hpp"
#include "states/lose-state.hpp"

AudioPlayer *AudioPlayer ::instancePtr = NULL;

AudioPlayer *audioPlayer = AudioPlayer ::getInstance();

int main(int argc, char **argv)
{

    audioPlayer->addSound("countdown", "./assets/audio/countdown.mp3");
    audioPlayer->addSound("bomb", "./assets/audio/bomb.mp3");
    audioPlayer->addSound("messi_win", "./assets/audio/messi_win.mp3");
    audioPlayer->addSound("messi_lose", "./assets/audio/messi_lose.mp3");
    audioPlayer->addSound("sui", "./assets/audio/sui.mp3");
    audioPlayer->addSound("whistle", "./assets/audio/whistle.mp3");
    audioPlayer->addSound("WhataSave", "./assets/audio/WhataSave.mp3");
    audioPlayer->addSound("win", "./assets/audio/win.mp3");
    audioPlayer->addSound("gameover", "./assets/audio/gameover.mp3");
    audioPlayer->addSound("hurryup", "./assets/audio/hurryup.mp3");
    audioPlayer->addSound("chipi", "./assets/audio/chipi.mp3");
    audioPlayer->addSound("legends", "./assets/audio/legends.mp3");
    audioPlayer->addSound("warriors", "./assets/audio/warriors.mp3");

    audioPlayer->addOriginalsound("chipi");
    audioPlayer->addOriginalsound("legends");
    audioPlayer->addOriginalsound("warriors");
    

    std::vector<nlohmann::json> configs;
    flags::args args(argc, argv); // Parse the command line arguments
    // config_path is the path to the json file containing the application configuration
    // Default: "config/app.json"
    // std::string config_path = args.get<std::string>("c", "config/app.jsonc");
    // run_for_frames is how many frames to run the application before automatically closing
    // This is useful for testing multiple configurations in a batch
    // Default: 0 where the application runs indefinitely until manually closed
    int run_for_frames = args.get<int>("f", 0);

    std::string directory_path = "config/";

    // Iterate over files in the directory
    for (const auto &entry : std::filesystem::directory_iterator(directory_path))
    {
        // Check if the entry is a regular file
        if (entry.is_regular_file())
        {
            // Open the file
            std::ifstream file_in(entry.path());
            if (!file_in)
            {
                std::cerr << "Couldn't open file: " << entry.path() << std::endl;
                continue; // Continue to the next file
            }
            // Read the file into a JSON object
            nlohmann::json app_config = nlohmann::json::parse(file_in, nullptr, true, true);
            // Close the file
            file_in.close();
            configs.push_back(app_config);
        }
    }
    if (!configs.size())
        return -1;
    // Open the config file and exit if failed
    // std::ifstream file_in(config_path);
    // if (!file_in)
    // {
    //     std::cerr << "Couldn't open file: " << config_path << std::endl;
    //     return -1;
    // }
    // // Read the file into a json object then close the file
    // nlohmann::json app_config = nlohmann::json::parse(file_in, nullptr, true, true);
    // file_in.close();

    // Create the application
    our::Application app(configs);

    // Register all the states of the project in the application
    app.registerState<LoadingScreenstate>(LoadingScreenstate::getStateName_s());
    app.registerState<Menustate>(Menustate::getStateName_s());
    app.registerState<LevelSelectState>(LevelSelectState::getStateName_s());
    app.registerState<Level1state>(Level1state::getStateName_s());
    app.registerState<Level2state>(Level2state::getStateName_s());
    app.registerState<Level3state>(Level3state::getStateName_s());
    app.registerState<Level4state>(Level4state::getStateName_s());
    app.registerState<Winstate>(Winstate::getStateName_s());
    app.registerState<Losestate>(Losestate::getStateName_s());
    app.registerState<ShaderTestState>(ShaderTestState::getStateName_s());
    app.registerState<MeshTestState>(MeshTestState::getStateName_s());
    app.registerState<TransformTestState>(TransformTestState::getStateName_s());
    app.registerState<PipelineTestState>(PipelineTestState::getStateName_s());
    app.registerState<TextureTestState>(TextureTestState::getStateName_s());
    app.registerState<SamplerTestState>(SamplerTestState::getStateName_s());
    app.registerState<MaterialTestState>(MaterialTestState::getStateName_s());
    app.registerState<EntityTestState>(EntityTestState::getStateName_s());
    app.registerState<RendererTestState>(RendererTestState::getStateName_s());

    // Then choose the state to run based on the option "start-scene" in the config
    if (configs[0].contains(std::string{"start-scene"}))
    {
        app.changeState(configs[0]["start-scene"].get<std::string>());
    }

    // Finally run the application
    // Here, the application loop will run till the terminatio condition is statisfied
    return app.run(run_for_frames);
}