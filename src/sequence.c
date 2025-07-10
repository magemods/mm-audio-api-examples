#include "global.h"
#include "modding.h"
#include "recomputils.h"
#include "audio_api.h"

INCBIN(sequence, "src/audio/DetectiveSky612-DanceOfDeath.zseq");

s32 newSeqId;

RECOMP_CALLBACK("magemods_audio_api", AudioApi_Init) void my_mod_on_init_sequence_test() {

    // Build the sequence table entry
    AudioTableEntry mySeq = {
        (uintptr_t) sequence,    // romAddr
        sequence_end - sequence, // size
        MEDIUM_CART,             // medium
        CACHE_EITHER,            // cachePolicy
        0, 0, 0,                 // shortData
    };

    // Replace the file select sequence with our own custom one
    AudioApi_ReplaceSequence(NA_BGM_FILE_SELECT, &mySeq);

    // Change the file select sequence's first soundfont to font 0x03
    AudioApi_ReplaceSequenceFont(NA_BGM_FILE_SELECT, 0, 0x03);
}
