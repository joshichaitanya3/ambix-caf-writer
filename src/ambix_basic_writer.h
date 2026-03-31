#ifndef AMBIX_CAF_WRITER_AMBIX_BASIC_WRITER_H_
#define AMBIX_CAF_WRITER_AMBIX_BASIC_WRITER_H_

#include <string>
#include "AudioFile/AudioFile.h"
#include "caf_writer.h"
#include "ambix_encoder.h"

class AmbixBasicWriter : CAFWriter {

    private:

    AudioFile<float> audio_file;
    
    ambix::AmbixEncoder ambix_encoder;

    /*!\brief Computes the ambisonic channels and writes it to the file
    \param output_file File stream of the output file
    \param azimuth Horizontal angle in degrees, with 0 being front
    \param elevation Vertical angle in degrees, with 0 being horizontal and 90 being above
    */
    void WriteAudioDataChunk(std::ofstream&, double, double);
    
    public:


    /*!\brief Initializes the AmbixBasicWriter with file path to input mono .wav file
    \param input_filename Full path to the input .wav file
    */
    AmbixBasicWriter(const std::string& input_filename);

    /*!\brief Converts the mono input file into an ambix basic format CAF file
    \param output_filename Full path to the output file, with extension .caf
    \param azimuth Horizontal angle in degrees, with 0 being front 
    \param elevation Vertical angle in degrees, with 0 being horizontal and 90 being above
    \param max_degree Maximum degree to encode in ambisonics
    */
    bool WriteToCAF(const std::string& output_filename, double azimuth, double elevation, int max_degree);
        
};

#endif // AMBIX_CAF_WRITER_AMBIX_BASIC_WRITER_H_

