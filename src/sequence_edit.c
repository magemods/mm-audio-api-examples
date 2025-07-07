#include "global.h"
#include "modding.h"
#include "recomputils.h"
#include "audio_api.h"

RECOMP_CALLBACK("magemods_audio_api", AudioApi_SequenceLoaded) void my_mod_on_load_sequence_edit_test(s32 seqId, u8* ramAddr) {
    if (seqId == 0) {
        // Modify sequence 0 directly. This is experimental and won't be necessary eventually.

        // Change CHAN_EV_SMALL_DOG_BARK's layer to CHAN_EV_RUPY_FALL's layer, which is 12 bytes ahead
        // This changes the dog bark's sound effect to the rupy falling sound effect

        u8* chanDogBarkPtr = (u8*)(ramAddr + 0x3D0D);
        chanDogBarkPtr[2] += 0xC;
    }
}
