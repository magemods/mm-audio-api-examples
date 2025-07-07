#include "global.h"
#include "modding.h"
#include "recomputils.h"
#include "audio_api.h"

INCBIN(drum, "src/audio/drum.raw");

RECOMP_CALLBACK("magemods_audio_api", AudioApi_Init) void my_mod_on_init_drum_test() {

    // Build Drum Object
    AdpcmLoop drum_sample_LOOP = {
        { 0, (drum_end - drum) / 2, 0, 0 }, {}
    };
    Sample drum_sample = {
        0, CODEC_S16, MEDIUM_CART, false, false,
        drum_end - drum,
        drum,
        &drum_sample_LOOP,
        NULL
    };
    EnvelopePoint drum_env[] = {
        ENVELOPE_POINT(    2, 32700),
        ENVELOPE_POINT(  298,     0),
        ENVELOPE_POINT(    1,     0),
        ENVELOPE_HANG(),
    };
    Drum drum = {
        251, 74, false,
        { &drum_sample, 1.0f },
        drum_env,
    };

    // In soundfont 0x03, the timpani is drumId 33-63.
    // Replace each entry with tuning ranging from 0.45f - 2.0f

    for (s32 i = 33; i < 64; i++) {
        drum.tunedSample.tuning = 0.05f + 0.05f * (i - 33);
        AudioApi_ReplaceDrum(0x03, i, &drum);
    }
}
