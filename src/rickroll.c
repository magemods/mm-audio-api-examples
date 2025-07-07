#include "global.h"
#include "modding.h"
#include "recomputils.h"
#include "audio_api.h"

INCBIN(rickroll, "src/audio/rickroll32kHz.raw");

// Default envelope points for most samples, you can define it once and re-use
EnvelopePoint default_env[] = {
    ENVELOPE_POINT(    1, 32700),
    ENVELOPE_POINT(    1, 32700),
    ENVELOPE_POINT(32700, 32700),
    ENVELOPE_HANG(),
};

RECOMP_CALLBACK("magemods_audio_api", AudioApi_Init) void my_mod_on_init_rickroll() {

    AdpcmLoop rickroll_sample_LOOP = {
        { 0, (rickroll_end - rickroll) / 2, 0, 0 }, {}
    };

    Sample rickroll_sample = {
        0, CODEC_S16, MEDIUM_CART, false, false,
        rickroll_end - rickroll,
        rickroll,
        &rickroll_sample_LOOP,
        NULL
    };

    Instrument rickroll_instrument = {
        false,
        INSTR_SAMPLE_LO_NONE,
        INSTR_SAMPLE_HI_NONE,
        251,
        default_env,
        INSTR_SAMPLE_NONE,
        { &rickroll_sample, 1.0f },
        INSTR_SAMPLE_NONE,
    };

    // Replace the cucco instrument with one that plays the intro to Never Going To Give You Up
    // The cucco instrument is in soundfont 0x00 with instId 61
    AudioApi_ReplaceInstrument(0x00, 61, &rickroll_instrument);
}

RECOMP_CALLBACK("magemods_audio_api", AudioApi_SequenceLoaded) void my_mod_on_load_sequence_rickroll(s32 seqId, u8* ramAddr) {
    if (seqId == 0) {

        // For instruments, if we update the sample, we must also update the corresponding layer's
        // NOTEDV delay (length), otherwise it will cut off early. This will be simplified in
        // future API releases.

        u8* chanCuccoPtr = (u8*)(ramAddr + 0x2938);

        // Sequence commands starting at chanCuccoPtr:
        //
        // C1 - ASEQ_OP_CHAN_INSTR
        // 3D - instr 61
        // 88 - ASEQ_OP_CHAN_LDLAYER 0x88 + <layerNum:b3>
        // 29 - LAYER_293E
        // 3E
        // FF - ASEQ_OP_END
        // 67 - ASEQ_OP_LAYER_NOTEDV  0x40 + 39
        // 80 - <delay:var>
        // A6
        // 69 - <velocity:u8>
        // FF - ASEQ_OP_END

        // Delay is calculated as (num_seconds * ticks_per_quarter_note * (bpm / 60))
        // Where num_seconds = 9, ticks_per_quarter_note = 96, and bpm = 60

        u16 delay = 864;

        // Write Long encoded numbers a la MIDI
        chanCuccoPtr[7] = 0x80 | (delay & 0x7f00) >> 8;
        chanCuccoPtr[8] = delay & 0xFF;
    }
}
