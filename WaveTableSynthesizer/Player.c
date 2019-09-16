#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "SynthCore.h"
#include "Player.h"

extern unsigned char Score[];
extern unsigned char Score2[];
extern unsigned char Score3[];
uint8_t *ScoreList[3] = {Score, Score2, Score3};

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
                    NoteOnAsm(&(player->synthesizer), temp + 57);
            } while ((temp & 0x80) == 0);

            UpdateNextScoreTick(&(player->decoder));
        }
    }
}

void PlaySchedulerNextScore(Player *player)
{
    player->decoder.status = STATUS_STOP;
    if (player->scheduler.currentScoreIndex < player->scheduler.maxScoreNum)
        player->scheduler.currentScoreIndex++;
    else
        player->scheduler.currentScoreIndex = 0;
}

void PlaySchedulerProcess(Player *player)
{
    if (player->scheduler.currentScoreIndex < player->scheduler.maxScoreNum && player->scheduler.schedulerMode == MODE_ORDER_PLAY)
    {
        if (player->decoder.status == STATUS_STOP)
        {
            PlayScore(player, ScoreList[player->scheduler.currentScoreIndex]);
            if (player->scheduler.currentScoreIndex < player->scheduler.maxScoreNum)
                player->scheduler.currentScoreIndex++;
        }
    }
    else if (player->scheduler.schedulerMode == MODE_ONE_SCORE_LOOP)
    {
        if (player->decoder.status == STATUS_STOP)
            PlayScore(player, ScoreList[player->scheduler.currentScoreIndex]);
    }
    else if (player->scheduler.schedulerMode == MODE_ALL_SCORE_LOOP)
    {
        if (player->decoder.status == STATUS_STOP)
        {

            PlayScore(player, ScoreList[player->scheduler.currentScoreIndex]);
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
    player->scheduler.currentScoreIndex = 0;
    player->scheduler.maxScoreNum = 3;
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
    player->decoder.scorePointer = Score;
    SynthInit(&(player->synthesizer));
}