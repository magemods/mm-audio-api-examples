#include "global.h"
#include "modding.h"
#include "recomputils.h"
#include "audio_api.h"
#include "cseq.h"

static u8* mySong;
static s32 mySongSize;
static s32 mySongSeqId;

RECOMP_CALLBACK("magemods_audio_api", AudioApi_Init) void my_mod_on_init() {

    CSeqContainer* root = cseq_create();
    CSeqSection* seq = cseq_sequence_create(root);
    CSeqSection* chan0 = cseq_channel_create(root);
    CSeqSection* chan1 = cseq_channel_create(root);
    CSeqSection* chan2 = cseq_channel_create(root);
    CSeqSection* layer0 = cseq_layer_create(root);
    CSeqSection* layer1 = cseq_layer_create(root);
    CSeqSection* layer2 = cseq_layer_create(root);
    CSeqSection* label;

    cseq_mutebhv(seq, 0x20);
    cseq_mutescale(seq, 0x32);
    cseq_initchan(seq, 0b00000111);
    label = cseq_label_create(seq);
    cseq_ldchan(seq, 0, chan0);
    cseq_ldchan(seq, 1, chan1);
    cseq_ldchan(seq, 2, chan2);
    cseq_vol(seq, 0xA0);
    cseq_tempo(seq, 0x6E);
    cseq_delay(seq, 0x300);
    // cseq_jump(seq, label); // To repeat the sequence
    cseq_freechan(seq, 0b00000111);
    cseq_section_end(seq);

    // Melody (trumpet)
    cseq_noshort(chan0);
    cseq_ldlayer(chan0, 0, layer0);
    cseq_font(chan0, 2);
    cseq_instr(chan0, 6);
    cseq_delay(chan0, 768);
    cseq_section_end(chan0);

    cseq_loop(layer0, 2);
    cseq_notedv(layer0, 29, 48, 50);
    cseq_notevg(layer0, 27, 48, 0);
    cseq_notedvg(layer0, 25, 96, 50, 128);
    cseq_loopend(layer0);

    cseq_loop(layer0, 4);
    cseq_notedvg(layer0, 25, 24, 50, 128);
    cseq_loopend(layer0);

    cseq_loop(layer0, 4);
    cseq_notedvg(layer0, 27, 24, 50, 128);
    cseq_loopend(layer0);

    cseq_notedv(layer0, 29, 48, 50);
    cseq_notevg(layer0, 27, 48, 0);
    cseq_notedv(layer0, 25, 72, 50);
    cseq_section_end(layer0);

    // Harmony (piano)
    cseq_noshort(chan1);
    cseq_ldlayer(chan1, 0, layer1);
    cseq_font(chan1, 0);
    cseq_instr(chan1, 6);
    cseq_delay(chan1, 768);
    cseq_section_end(chan1);

    cseq_loop(layer1, 2);
    cseq_notedvg(layer1, 29, 96, 50, 128);
    cseq_notedvg(layer1, 25, 96, 50, 128);
    cseq_loopend(layer1);

    cseq_notedvg(layer1, 25, 96, 50, 128);
    cseq_notedvg(layer1, 27, 96, 50, 128);
    cseq_notedvg(layer1, 29, 96, 50, 128);
    cseq_notedvg(layer1, 25, 96, 50, 128);
    cseq_section_end(layer1);

    // Gong
    cseq_noshort(chan2);
    cseq_ldlayer(chan2, 0, layer2);
    cseq_font(chan2, 1);
    cseq_instr(chan2, 4);
    cseq_delay(chan2, 768);
    cseq_section_end(chan2);

    cseq_loop(layer2, 2);
    cseq_ldelay(layer2, 144);
    cseq_notedv(layer2, 25, 48, 50);
    cseq_loopend(layer2);

    cseq_ldelay(layer2, 336);
    cseq_notedv(layer2, 25, 48, 50);
    cseq_section_end(layer2);

    cseq_compile(root, 0);

    mySongSize = root->buffer->size;
    mySong = recomp_alloc(mySongSize);
    Lib_MemCpy(mySong, root->buffer->data, mySongSize);

    cseq_destroy(root);

    AudioTableEntry mySeq = {
        (uintptr_t) mySong,
        mySongSize,
        MEDIUM_CART,
        CACHE_EITHER,
        0, 0, 0,
    };

    mySongSeqId = AudioApi_AddSequence(&mySeq);
    AudioApi_AddSequenceFont(mySongSeqId, 0x24); // font 0
    AudioApi_AddSequenceFont(mySongSeqId, 0x0D); // font 1
    AudioApi_AddSequenceFont(mySongSeqId, 0x03); // font 2
}

RECOMP_HOOK("Player_Update") void onPlayer_Update(Player* this, PlayState* play) {
    if (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_L)) {
        AudioApi_PlayFanfare(mySongSeqId, 0);
    }
}
