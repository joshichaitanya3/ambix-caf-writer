#include "caf_writer.h"
#include "big_endian.h"
#include <iostream>

CAFWriter::CAFWriter() {
    header.file_type = big_endian::MakeFourCC('c', 'a', 'f', 'f');
    header.file_version = 1;
    header.file_flags = 0;

    desc_chunk.chunk_type = big_endian::MakeFourCC('d', 'e', 's', 'c');
    desc_chunk.chunk_size = sizeof(CAFAudioFormat);
    std::cout << "Starting the CAFWriter..." << std::endl;
}

void CAFWriter::WriteFileHeader(std::ofstream& out) {

    big_endian::Write(out, header.file_type);
    big_endian::Write(out, header.file_version);
    big_endian::Write(out, header.file_flags);
}    

void CAFWriter::WriteAudioDescriptionChunk(std::ofstream& out) {

    // First, we write the chunk header
    big_endian::Write(out, desc_chunk.chunk_type);
    big_endian::Write(out, desc_chunk.chunk_size);

    // Now, we write the chunk data
    big_endian::Write(out, desc_chunk.format.sample_rate);
    big_endian::Write(out, desc_chunk.format.format_id);
    big_endian::Write(out, desc_chunk.format.format_flags);
    big_endian::Write(out, desc_chunk.format.bytes_per_packet);
    big_endian::Write(out, desc_chunk.format.frames_per_packet);
    big_endian::Write(out, desc_chunk.format.channels_per_frame);
    big_endian::Write(out, desc_chunk.format.bits_per_channel);

}    
