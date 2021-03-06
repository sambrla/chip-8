#include <iostream>
#include <string>
#include <cxxopts.hpp>
#include "chip8.hpp"

#define CXX_UINT(def) cxxopts::value<unsigned>()->default_value(#def)

int main(int argc, char** argv)
{
    cxxopts::Options options(argv[0],
        "A simple Chip-8 interpreter\ngithub.com/sambrla/chip-8\n");
    options.positional_help("<ROM>");
    options.show_positional_help();

    options.add_options()
        ("i,ipc",       "Instructions to execute per cycle", CXX_UINT(9), "IPC")
        ("r,high-dpi",  "Scale window for high DPI displays")
        ("c,compat",    "Enable alternative shift and load behaviour. "
                        "May be required for some ROMs to work correctly")
        ("h,help",      "Print help");

    options.add_options("hidden")
        ("rom", "Path to ROM file", cxxopts::value<std::string>());

    try
    {
        options.parse_positional({"rom"});
        const auto result = options.parse(argc, argv);

        // Print help if option specified or no ROM path provided
        if (result.count("help") || !result.count("rom"))
        {
            std::cout << options.help() << std::endl;
            return 0;
        }

        // Init the interpreter
        Chip8 interpreter(result["ipc"].as<unsigned>(), result.count("high-dpi"));

        // Run the ROM (with compatibility if specified)
        interpreter.run(result["rom"].as<std::string>(), result.count("compat"));
    }
    catch (const cxxopts::OptionException& e)
    {
        std::cerr << "Parse error: " << e.what()
                  << ". Use -h or --help to see valid options" << std::endl;
        return 1;
    }
}
