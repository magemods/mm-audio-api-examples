#include "global.h"
#include "modding.h"
#include "recomputils.h"
#include "audio_api.h"
#include "cseq.h"

u8* mySong;
s32 mySongSize;
s32 mySongSeqId;

RECOMP_CALLBACK("magemods_audio_api", AudioApi_Init) void my_mod_on_init() {
    CSeqContainer* root = cseq_create();
    CSeqSection* seq = cseq_sequence_create(root);
    CSeqSection* chan = cseq_channel_create(root);
    CSeqSection* layer0 = cseq_layer_create(root);
    CSeqSection* layer1 = cseq_layer_create(root);
    CSeqSection* label;

    cseq_mutebhv(seq, 0x20);
    cseq_mutescale(seq, 0x32);
    cseq_initchan(seq, 0b00000001);
    label = cseq_label_create(seq);
    cseq_ldchan(seq, 0, chan);
    cseq_vol(seq, 0xA0);
    cseq_tempo(seq, 0x6E);
    cseq_delay(seq, 0x300);
    // cseq_jump(seq, label); // To repeat the sequence
    cseq_freechan(seq, 0b00000001);
    cseq_section_end(seq);

    cseq_noshort(chan);
    cseq_ldlayer(chan, 0, layer0);
    cseq_ldlayer(chan, 1, layer1);
    cseq_instr(chan, 5);
    cseq_delay(chan, 768);
    cseq_section_end(chan);

    cseq_transpose(layer0, 12);

    cseq_loop(layer1, 2);
    cseq_notedv(layer1, 29, 48, 50);
    cseq_notevg(layer1, 27, 48, 0);
    cseq_notedvg(layer1, 26, 96, 50, 128);
    cseq_loopend(layer1);

    cseq_loop(layer1, 4);
    cseq_notedvg(layer1, 27, 24, 50, 128);
    cseq_loopend(layer1);

    cseq_loop(layer1, 4);
    cseq_notedvg(layer1, 29, 24, 50, 128);
    cseq_loopend(layer1);

    cseq_notedv(layer1, 29, 48, 50);
    cseq_notevg(layer1, 27, 48, 0);
    cseq_notedv(layer1, 26, 72, 50);
    cseq_section_end(layer1);

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
    AudioApi_AddSequenceFont(mySongSeqId, 0x03);
}

RECOMP_HOOK("Player_Update") void onPlayer_Update(Player* this, PlayState* play) {
    if (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_L)) {
        AudioApi_PlayFanfare(mySongSeqId, 0);
    }
}
