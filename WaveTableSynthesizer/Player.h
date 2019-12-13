#ifndef __ScoreDecoder_H__
#define __ScoreDecoder_H__

#include "SynthCore.h"

#define pSynthesizer    0
#define pScoreDecoder   (pSynthesizer+SynthesizerSize)
#define pCurrentTick    0
#define pLastScoreTick  (pCurrentTick+4)
#define pStatus         (pLastScoreTick+4)
#define constSTATUS_DECODING 2

#ifndef __ASSEMBLER__

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

enum DECODER_STATUS
{
    STATUS_STOP = 0,
    STATUS_REDAY_TO_DECODE = 1,
    STATUS_DECODING = constSTATUS_DECODING
};

enum SCHEDULER_MODE
{
    MODE_ORDER_PLAY = 0,
};

enum SCHEDULER_STATUS
{
    SCHEDULER_READY_TO_SWITCH = 0,
    SCHEDULER_SWITCHING,
    SCHEDULER_SCORE_PREV,
    SCHEDULER_SCORE_NEXT,
    SCHEDULER_STOP,
};

typedef struct _ScoreListHeader
{
    char identifer[4];
    uint32_t scoreCount;
    uint32_t firstAddr;
}ScoreListHeader;

typedef struct _ScoreDecoder
{
    uint32_t currentTick;
    uint32_t lastScoreTick;
    uint32_t status;
    uint8_t *scorePointer;
} ScoreDecoder;

typedef struct _PlayScheduler
{
    uint32_t schedulerMode;
    int32_t currentScoreIndex;
    uint32_t maxScoreNum;
    ScoreListHeader* scoreListHeader;
    uint32_t status;
    uint32_t switchDirect;
} PlayScheduler;

typedef struct _Player
{
    Synthesizer synthesizer;
    ScoreDecoder decoder;
    PlayScheduler scheduler;
}Player;



extern ScoreListHeader ScoreDataList;
extern Player *GlobalPlayerPtr;

extern void PlayerInit(Player *player);
extern void Player32kProc(Player *player);
extern void PlayerProcess(Player *player);
extern void ScoreDecodeProcess(Player *player);
extern void PlayScore(Player* player, uint8_t *score);
extern void StopDecode(Player *player);
extern void StartPlayScheduler(Player* player);
extern void StopPlayScheduler(Player *player);
extern void PlaySchedulerNextScore(Player *player);
extern void PlaySchedulerPreviousScore(Player *player);
extern void SchedulerSetIntialRandomSeed(Player *player,uint8_t randomSeed);
extern void UpdateTick(ScoreDecoder *decoder);
extern void UpdateNextScoreTick(ScoreDecoder *decoder);

#ifdef __cplusplus
} //end extern "C"
#endif

#else
.extern GlobalPlayerPtr;
#endif

#endif