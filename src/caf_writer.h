#ifndef AMBIX_CAF_WRITER_CAF_WRITER_H_
#define AMBIX_CAF_WRITER_CAF_WRITER_H_

#include <fstream>

class CAFWriter {
protected:

    struct CAFFileHeader {
        uint32_t file_type;    // 'caff'
        uint16_t file_version; // 1
        uint16_t file_flags;   // 0
    };

    struct CAFAudioFormat {

        double sample_rate; // Using double instead of _Float64 for simpler code in big_endian.h;
        uint32_t format_id;
        uint32_t format_flags;
        uint32_t bytes_per_packet;
        uint32_t frames_per_packet;
        uint32_t channels_per_frame;
        uint32_t bits_per_channel;

    };

    struct AudioDescriptionChunk {
        uint32_t chunk_type;
        uint64_t chunk_size;
        CAFAudioFormat format;
    };

    // Default constructor of base class CAFWriter, initializes header and description chunk variables
    CAFWriter();
    
    CAFFileHeader header;
    
    AudioDescriptionChunk desc_chunk;

    // Writes the required CAF file header
    void WriteFileHeader(std::ofstream&);
    
    // Writes the required audio description chunk
    void WriteAudioDescriptionChunk(std::ofstream&);
};

#endif // AMBIX_CAF_WRITER_CAFFILE_H_
