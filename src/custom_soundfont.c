#include "global.h"
#include "modding.h"
#include "recomputils.h"
#include "audio_api.h"

INCBIN(inst, "src/audio/inst-bflat.raw");
INCBIN(drum, "src/audio/drum.raw");

// Pull some values from soundfont_3, these definitions aren't necessary if you're making a completely
// custom font.

extern u8 SampleBank_0_Timpani_Off[];

EnvelopePoint Inst_Env[] = {
    ENVELOPE_POINT(    1, 32700),
    ENVELOPE_POINT(    1, 32700),
    ENVELOPE_POINT(32700, 32700),
    ENVELOPE_HANG(),
};

EnvelopePoint Drum_Env[] = {
    ENVELOPE_POINT(    2, 32700),
    ENVELOPE_POINT(  298,     0),
    ENVELOPE_POINT(    1,     0),
    ENVELOPE_HANG(),
};

s16 SF3_Timpani_BOOK[] = {
    (s16)0, (s16)2, (s16)0, (s16)2,
    (s16)0xF8FE, (s16)0xF2E3, (s16)0xED9A, (s16)0xE910, (s16)0xE532, (s16)0xE1F0, (s16)0xDF3B, (s16)0xDD04,
    (s16)0x0EF8, (s16)0x1501, (s16)0x1A2F, (s16)0x1E99, (s16)0x2251, (s16)0x2569, (s16)0x27F0, (s16)0x29F6,
    (s16)0xFE92, (s16)0xFEF0, (s16)0xFF77, (s16)0xFFCB, (s16)0xFFF1, (s16)0xFFFE, (s16)0x0001, (s16)0x0001,
    (s16)0x05F3, (s16)0x02FE, (s16)0x0129, (s16)0x0054, (s16)0x0009, (s16)0xFFF8, (s16)0xFFF8, (s16)0xFFFC,
};

AdpcmLoop SF3_Timpani_LOOP = {
    { 45298, 59996, 0xFFFFFFFF, 60017 },
    {
        (s16)0xFF3F, (s16)0xFFCA, (s16)0x0058, (s16)0x00E3,
        (s16)0x016D, (s16)0x01FB, (s16)0x0289, (s16)0x0313,
        (s16)0x03A0, (s16)0x042F, (s16)0x04BF, (s16)0x054B,
        (s16)0x05DC, (s16)0x066B, (s16)0x06FD, (s16)0x0790,
    },
};

Sample SF3_Timpani_HEADER = {
    0, CODEC_ADPCM, 0, true, false,
    0x0083E0,
    SampleBank_0_Timpani_Off,
    &SF3_Timpani_LOOP,
    &SF3_Timpani_BOOK,
};

#define SF3_DRUM_4_ENTRY(tuning) \
    { \
        242, \
        74, \
            false,                         \
        { &SF3_Timpani_HEADER, (tuning) }, \
        Drum_Env, \
    }

Drum SF3_DRUM_4[31] = {
    SF3_DRUM_4_ENTRY(0.3535529971122741699219f),
    SF3_DRUM_4_ENTRY(0.3745769858360290527344f),
    SF3_DRUM_4_ENTRY(0.3968499898910522460938f),
    SF3_DRUM_4_ENTRY(0.4204480051994323730469f),
    SF3_DRUM_4_ENTRY(0.4454489946365356445312f),
    SF3_DRUM_4_ENTRY(0.4719370007514953613281f),
    SF3_DRUM_4_ENTRY(0.5000000000000000000000f),
    SF3_DRUM_4_ENTRY(0.5297319889068603515625f),
    SF3_DRUM_4_ENTRY(0.5612310171127319335938f),
    SF3_DRUM_4_ENTRY(0.5946040153503417968750f),
    SF3_DRUM_4_ENTRY(0.6299610137939453125000f),
    SF3_DRUM_4_ENTRY(0.6674200296401977539062f),
    SF3_DRUM_4_ENTRY(0.7071070075035095214844f),
    SF3_DRUM_4_ENTRY(0.7491539716720581054688f),
    SF3_DRUM_4_ENTRY(0.7937009930610656738281f),
    SF3_DRUM_4_ENTRY(0.8408970236778259277344f),
    SF3_DRUM_4_ENTRY(0.8908990025520324707031f),
    SF3_DRUM_4_ENTRY(0.9438750147819519042969f),
    SF3_DRUM_4_ENTRY(1.0000000000000000000000f),
    SF3_DRUM_4_ENTRY(1.0594630241394042968750f),
    SF3_DRUM_4_ENTRY(1.1224620342254638671875f),
    SF3_DRUM_4_ENTRY(1.1892069578170776367188f),
    SF3_DRUM_4_ENTRY(1.2599209547042846679688f),
    SF3_DRUM_4_ENTRY(1.3348400592803955078125f),
    SF3_DRUM_4_ENTRY(1.4142140150070190429688f),
    SF3_DRUM_4_ENTRY(1.4983069896697998046875f),
    SF3_DRUM_4_ENTRY(1.5874010324478149414062f),
    SF3_DRUM_4_ENTRY(1.6817929744720458984375f),
    SF3_DRUM_4_ENTRY(1.7817980051040649414062f),
    SF3_DRUM_4_ENTRY(1.8877489566802978515625f),
    SF3_DRUM_4_ENTRY(2.0000000000000000000000f),
};


RECOMP_CALLBACK("magemods_audio_api", AudioApi_Init) void my_mod_on_init_drum_test() {

    // Build a custom instrument
    AdpcmLoop inst_sample_LOOP = {
        { 0, (inst_end - inst) / 2, 0, 0 }, {}
    };

    Sample inst_sample = {
        0, CODEC_S16, MEDIUM_CART, false, false,
        inst_end - inst,
        inst,
        &inst_sample_LOOP,
        NULL
    };

    Instrument inst = {
        false,
        INSTR_SAMPLE_LO_NONE,
        INSTR_SAMPLE_HI_NONE,
        251,
        Inst_Env,
        INSTR_SAMPLE_NONE,
        { &inst_sample, 0.890899f },
        INSTR_SAMPLE_NONE,
    };

    // Build a custom drum
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

    Drum drum = {
        251, 74, false,
        { &drum_sample, 1.0f },
        Drum_Env,
    };


    // Create a new empty soundfont
    s32 newFontId = AudioApi_CreateEmptySoundFont();

    // Add 16 copies of our custom instrument
    // normally you'd want to add unique instruments here
    for (s32 i = 0; i < 16; i++) {
        AudioApi_AddInstrument(newFontId, &inst);
    }

    // Add 33 copies of our custom drum with tuning ranging from 0.40f - 2.0f
    // normally you'd want to add several unique drums here
    for (s32 i = 0; i < 33; i++) {
        drum.tunedSample.tuning = 0.40f + 0.05f * i;
        AudioApi_AddDrum(newFontId, &drum);
    }

    // Copy the timpani from soundfont 0x03 using the various tuning values
    for (s32 i = 0; i < 31; i++) {
        AudioApi_AddDrum(newFontId, &SF3_DRUM_4[i]);
    }

    // Replace clock town's soundfont with our custom one
    AudioApi_ReplaceSequenceFont(NA_BGM_CLOCK_TOWN_DAY_1, 0, newFontId);
}
