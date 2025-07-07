#include "global.h"
#include "modding.h"
#include "recomputils.h"
#include "audio_api.h"

INCBIN(attack1, "src/audio/attack1.raw");
INCBIN(attack2, "src/audio/attack2.raw");
INCBIN(attack3, "src/audio/attack3.raw");


RECOMP_CALLBACK("magemods_audio_api", AudioApi_Init) void my_mod_on_init_attack_test() {

    // Build SFX # 1
    AdpcmLoop attack1_sample_LOOP = {
        { 0, (attack1_end - attack1) / 2, 0, 0 }, {}
    };
    Sample attack1_sample = {
        0, CODEC_S16, MEDIUM_CART, false, false,
        attack1_end - attack1,
        attack1,
        &attack1_sample_LOOP,
        NULL
    };
    SoundEffect attack1_sfx = {
        { &attack1_sample, 1.0f },
    };

    // Build SFX # 2
    AdpcmLoop attack2_sample_LOOP = {
        { 0, (attack2_end - attack2) / 2, 0, 0 }, {}
    };
    Sample attack2_sample = {
        0, CODEC_S16, MEDIUM_CART, false, false,
        attack2_end - attack2,
        attack2,
        &attack2_sample_LOOP,
        NULL
    };
    SoundEffect attack2_sfx = {
        { &attack2_sample, 1.0f },
    };

    // Build SFX # 3
    AdpcmLoop attack3_sample_LOOP = {
        { 0, (attack3_end - attack3) / 2, 0, 0 }, {}
    };
    Sample attack3_sample = {
        0, CODEC_S16, MEDIUM_CART, false, false,
        attack3_end - attack3,
        attack3,
        &attack3_sample_LOOP,
        NULL
    };
    SoundEffect attack3_sfx = {
        { &attack3_sample, 1.0f },
    };

    // Link's attacks are in soundfont 0x00 with sfxId 28-33
    // Replace them with our new SoundEffect objects, also changing the tuning for a few of them so
    // that they play at different pitches.

    AudioApi_ReplaceSoundEffect(0x00, 28, &attack1_sfx);

    AudioApi_ReplaceSoundEffect(0x00, 29, &attack2_sfx);

    attack1_sfx.tunedSample.tuning = 1.1f;
    AudioApi_ReplaceSoundEffect(0x00, 30, &attack1_sfx);

    attack2_sfx.tunedSample.tuning = 1.1f;
    AudioApi_ReplaceSoundEffect(0x00, 31, &attack2_sfx);

    attack1_sfx.tunedSample.tuning = 1.2f;
    AudioApi_ReplaceSoundEffect(0x00, 32, &attack1_sfx);

    AudioApi_ReplaceSoundEffect(0x00, 33, &attack3_sfx);
}
