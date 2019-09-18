#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "SynthCore.h"
#include "Player.h"


void Player32kProc(Player *player)
{
    SynthAsm(&(player->synthesizer));
    player->decoder.currentTick++;
    if (player->synthesizer.decayGenTick < 200)
        player->synthesizer.decayGenTick += 1;
}

void ScoreDecodeProcess(Player *player)
{
    uint8_t temp;

    if (player->decoder.status == STATUS_PLAYING)
    {
        if ((player->decoder.currentTick >> 8) >= player->decoder.lastScoreTick)
        {
            do
            {
                temp = *(player->decoder.scorePointer);
                player->decoder.scorePointer++;
                if (temp == 0xFF)
                    player->decoder.status = STATUS_STOP;
                else
                    NoteOnAsm(&(player->synthesizer), temp);
            } while ((temp & 0x80) == 0);

            UpdateNextScoreTick(&(player->decoder));
        }
    }
}

void PlaySchedulerNextScore(Player *player)
{
    player->decoder.status = STATUS_STOP;
    if (!(player->scheduler.currentScoreIndex < player->scheduler.maxScoreNum))
        player->scheduler.currentScoreIndex = 0;
}

void PlaySchedulerPreviousScore(Player *player)
{
    player->decoder.status = STATUS_STOP;
    player->scheduler.currentScoreIndex-=2;
    if (player->scheduler.currentScoreIndex < 0)
        player->scheduler.currentScoreIndex = player->scheduler.maxScoreNum-1;
}

uint8_t* GetScorePhyiscalAddr(uint32_t addr)
{
    return (uint8_t*)((uint32_t)(&ScoreDataList)+addr);
}

void PlaySchedulerProcess(Player *player)
{
    uint32_t* ScoreList=(&(player->scheduler.scoreListHeader)->firstAddr);
    if (player->scheduler.currentScoreIndex < player->scheduler.maxScoreNum && player->scheduler.schedulerMode == MODE_ORDER_PLAY)
    {
        if (player->decoder.status == STATUS_STOP)
        {
            PlayScore(player, GetScorePhyiscalAddr(ScoreList[player->scheduler.currentScoreIndex]));
            if (player->scheduler.currentScoreIndex < player->scheduler.maxScoreNum)
                player->scheduler.currentScoreIndex++;
        }
    }
    else if (player->scheduler.schedulerMode == MODE_ONE_SCORE_LOOP)
    {
        if (player->decoder.status == STATUS_STOP)
            PlayScore(player, GetScorePhyiscalAddr(ScoreList[player->scheduler.currentScoreIndex]));
    }
    else if (player->scheduler.schedulerMode == MODE_ALL_SCORE_LOOP)
    {
        if (player->decoder.status == STATUS_STOP)
        {

            PlayScore(player, GetScorePhyiscalAddr(ScoreList[player->scheduler.currentScoreIndex]));
            if (player->scheduler.currentScoreIndex < player->scheduler.maxScoreNum)
                player->scheduler.currentScoreIndex++;
            else
                player->scheduler.currentScoreIndex = 0;
        }
    }
}

void PlayerProcess(Player *player)
{
    SynthGenEnvelopeProcess(&(player->synthesizer));
    ScoreDecodeProcess(player);
    PlaySchedulerProcess(player);
}

void UpdateNextScoreTick(ScoreDecoder *decoder)
{
    uint32_t tempU32;
    uint8_t temp;
    tempU32 = decoder->lastScoreTick;
    do
    {
        temp = *(decoder->scorePointer);
        decoder->scorePointer++;
        tempU32 += temp;
    } while (temp == 0xFF);
    decoder->lastScoreTick = tempU32;
}

void StartPlayScheduler(Player *player)
{
    player->scheduler.scoreListHeader=&ScoreDataList;
    player->scheduler.currentScoreIndex = 0;
    player->scheduler.maxScoreNum = (player->scheduler.scoreListHeader)->scoreCount;
    player->scheduler.schedulerMode = MODE_ORDER_PLAY;
}

void PlayScore(Player *player, uint8_t *score)
{
    player->decoder.currentTick = 0;
    player->decoder.lastScoreTick = 0;

    player->decoder.scorePointer = score;
    UpdateNextScoreTick(&(player->decoder));
    player->decoder.status = STATUS_PLAYING;
}

void PlayerInit(Player *player)
{
    player->decoder.status = STATUS_STOP;
    player->decoder.currentTick = 0;
    player->decoder.lastScoreTick = 0;
    player->decoder.scorePointer = NULL;
    SynthInit(&(player->synthesizer));
}