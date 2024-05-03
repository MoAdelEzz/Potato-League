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
#include "states/play-state.hpp"
#include "states/shader-test-state.hpp"
#include "states/mesh-test-state.hpp"
#include "states/transform-test-state.hpp"
#include "states/pipeline-test-state.hpp"
#include "states/texture-test-state.hpp"
#include "states/sampler-test-state.hpp"
#include "states/material-test-state.hpp"
#include "states/entity-test-state.hpp"
#include "states/renderer-test-state.hpp"
#include "states/level2.hpp"

int main(int argc, char **argv)
{
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
    app.registerState<LoadingScreenstate>("loading-screen");
    app.registerState<Menustate>("menu");
    app.registerState<LevelSelectState>("level-select");
    app.registerState<Playstate>("play");
    app.registerState<ShaderTestState>("shader-test");
    app.registerState<MeshTestState>("mesh-test");
    app.registerState<TransformTestState>("transform-test");
    app.registerState<PipelineTestState>("pipeline-test");
    app.registerState<TextureTestState>("texture-test");
    app.registerState<SamplerTestState>("sampler-test");
    app.registerState<MaterialTestState>("material-test");
    app.registerState<EntityTestState>("entity-test");
    app.registerState<RendererTestState>("renderer-test");
    app.registerState<Level2state>("level2");

    // Then choose the state to run based on the option "start-scene" in the config
    if (configs[0].contains(std::string{"start-scene"}))
    {
        app.changeState(configs[0]["start-scene"].get<std::string>());
    }

    // Finally run the application
    // Here, the application loop will run till the terminatio condition is statisfied
    return app.run(run_for_frames);
}