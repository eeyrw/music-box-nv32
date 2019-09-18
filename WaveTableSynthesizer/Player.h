#ifndef __ScoreDecoder_H__
#define __ScoreDecoder_H__

#include <stdint.h>
#include "SynthCore.h"

enum PLAY_STATUS
{
    STATUS_STOP = 0,
    STATUS_REDAY_TO_PLAY = 1,
    STATUS_PLAYING = 2
};

enum SCHEDULER_MODE
{
    MODE_ORDER_PLAY = 0,
    MODE_ONE_SCORE_LOOP = 1,
    MODE_ALL_SCORE_LOOP = 2
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
} PlayScheduler;

typedef struct _Player
{
    Synthesizer synthesizer;
    ScoreDecoder decoder;
    PlayScheduler scheduler;
}Player;



extern ScoreListHeader ScoreDataList;

extern void PlayerInit(Player *player);
extern void Player32kProc(Player *player);
extern void PlayerProcess(Player *player);
extern void ScoreDecodeProcess(Player *player);
extern void PlayScore(Player* player, uint8_t *score);
extern void StartPlayScheduler(Player* player);
extern void PlaySchedulerNextScore(Player *player);
extern void PlaySchedulerPreviousScore(Player *player);

extern void UpdateTick(ScoreDecoder *decoder);
extern void UpdateNextScoreTick(ScoreDecoder *decoder);
#endif