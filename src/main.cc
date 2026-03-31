#include <iostream>
#include <string>
#include <cstring>
#include <format>
#include <CLI/CLI.hpp>
#include "ambix_basic_writer.h"

int main(int argc, char* argv[]) {

    CLI::App app{"Ambisonic Encoder"};

    std::string input_filename;
    double azimuth = 0.0;
    double elevation = 0.0;
    int ambisonic_max_degree = 1;
    bool verbose = false;
    std::string output_filename;

    app.add_option("-i,--input", input_filename, "Input mono audio file path")->required();
    auto* outOpt = app.add_option("-o,--output", output_filename, "Output file path");
    app.add_option("-a,--azimuth", azimuth, "Azimuth in degrees (defaults to zero)");
    app.add_option("-e,--elevation", elevation, "Elevation in degrees (defaults to zero, which is horizontal)");
    app.add_option("-d,--degree", ambisonic_max_degree, "Ambisonic max degree (defaults to 1)");

    CLI11_PARSE(app, argc, argv);

    if (outOpt->count() == 0) { // If output name is not provided, set to input name appended with ambisonics parameters
        const std::string input_without_ext = input_filename.substr(0, input_filename.find_last_of('.'));
        output_filename = std::format("{}_{:.1f}_{:.1f}_{}.caf", input_without_ext, azimuth, elevation, ambisonic_max_degree);
    }
    try {
        AmbixBasicWriter writer {input_filename};
        writer.WriteToCAF(output_filename, azimuth, elevation, ambisonic_max_degree);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
