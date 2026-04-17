#include "ambix_basic_writer.h"
#include "big_endian.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>

AmbixBasicWriter::AmbixBasicWriter(const std::string& input_filename) {

    if (!audio_file.load(input_filename)) {
        throw std::runtime_error("Could not load WAV file: " + input_filename);

    }

    if (audio_file.getNumChannels() != 1) {
        throw std::runtime_error(std::format("AmbixBasicWriter currently only supports mono input files (input has {} channels.)", audio_file.getNumChannels()));
    }

    std::cout << "Details of the original WAV File:" << std::endl;
    audio_file.printSummary();
}

void AmbixBasicWriter::WriteAudioDataChunk(std::ofstream& output_file, double azimuth, double elevation, double distance) {
    
    uint32_t name = big_endian::MakeFourCC('d', 'a', 't', 'a'); // 'data'
    big_endian::Write(output_file, name);
    uint32_t numChannels = desc_chunk.format.channels_per_frame;
    uint64_t numSamples = static_cast<uint64_t>(audio_file.getNumSamplesPerChannel());

    // TODO: Fix this for  variable packet size case, etc.
    uint64_t dataSize = desc_chunk.format.bytes_per_packet * numSamples;
    big_endian::Write(output_file, dataSize);
    
    uint32_t editCount = 0; // Initial edit count
    big_endian::Write(output_file, editCount);

    if (distance < 0.0) {
        throw std::invalid_argument("Distance must be non-negative.");
    }
    const float gain = 1.0f / std::max(static_cast<float>(distance), kMinDistance);

    float sample_value;
    std::vector<float> sh_coeffs = ambix_encoder.CalculateSphericalHarmonicsCoefficients(azimuth, elevation);

    for (uint64_t i = 0; i < numSamples; i++) {
        sample_value = audio_file.samples[0][i];

        for (float coeff : sh_coeffs) { // Already in the ACN order
            big_endian::Write(output_file, gain * coeff * sample_value);
        }
    }
}    

bool AmbixBasicWriter::WriteToCAF(const std::string& output_filename, double azimuth, double elevation, double distance, int max_degree) {
        
    ambix_encoder = ambix::AmbixEncoder(max_degree);

    std::ofstream output_file;
    output_file.open(output_filename, std::ios::binary | std::ios::out);

    if (!output_file.is_open()) {
        throw std::runtime_error("Could not open output file: " + output_filename);
    }

    WriteFileHeader(output_file);
    
    
    desc_chunk.format.sample_rate = static_cast<double>(audio_file.getSampleRate());
    desc_chunk.format.format_id = big_endian::MakeFourCC('l', 'p', 'c', 'm'); // 'lpcm'
    desc_chunk.format.format_flags = 0x1; // For float

    /*
     Since we are initializing the AudioFile with a float template, 
     the bit depth is going to be 32 regardless of the input.
    */
    uint32_t bit_depth = 32;
    
    uint32_t num_channels = ambix_encoder.NumChannels();

    desc_chunk.format.bytes_per_packet = num_channels * (bit_depth/8);
    desc_chunk.format.frames_per_packet = 1;
    desc_chunk.format.channels_per_frame = num_channels;
    desc_chunk.format.bits_per_channel = bit_depth;

    WriteAudioDescriptionChunk(output_file);

    WriteAudioDataChunk(output_file, azimuth, elevation, distance);

    bool success = output_file.good();
    output_file.close();
    return success;
}
