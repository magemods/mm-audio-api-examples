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

    // Also add as a new sequence and store the new sequence ID
    newSeqId = AudioApi_AddSequence(&mySeq);
    AudioApi_AddSequenceFont(newSeqId, 0x03);
}

RECOMP_HOOK("Player_Update") void onPlayer_Update(Player* this, PlayState* play) {
    // When the player presses the L button, start our new sequence
    if (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_L)) {
        AudioApi_StartSequence(SEQ_PLAYER_BGM_MAIN, newSeqId, 0, 0);
    }
}
