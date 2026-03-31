target_sources(monoToAmbix
    PRIVATE
    main.cc  big_endian.h
    caf_writer.h caf_writer.cc
    ambix_basic_writer.h ambix_basic_writer.cc
    ambix_encoder.h ambix_encoder.cc
    )
