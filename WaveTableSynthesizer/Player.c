#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "SynthCore.h"
#include "Player.h"

void Player32kProc(Player *player)
{
    SynthAsm(&(player->synthesizer));
    if (player->decoder.status == STATUS_DECODING)
        player->decoder.currentTick++;
    if (player->synthesizer.decayGenTick < 200)
        player->synthesizer.decayGenTick += 1;
}

void ScoreDecodeProcess(Player *player)
{
    uint8_t temp;

    if (player->decoder.status == STATUS_DECODING)
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
    player->scheduler.switchDirect = SCHEDULER_SCORE_NEXT;
    player->scheduler.status = SCHEDULER_SWITCHING;
    StopDecode(player);
}

void PlaySchedulerPreviousScore(Player *player)
{
    player->scheduler.switchDirect = SCHEDULER_SCORE_PREV;
    player->scheduler.status = SCHEDULER_SWITCHING;
    StopDecode(player);
}



uint8_t *GetScorePhyiscalAddr(uint32_t addr)
{
    return (uint8_t *)((uint32_t)(&ScoreDataList) + addr);
}

void PlaySchedulerProcess(Player *player)
{
    uint32_t *ScoreList = (&(player->scheduler.scoreListHeader)->firstAddr);

    switch (player->scheduler.status)
    {
    case SCHEDULER_READY_TO_SWITCH:
        if (player->decoder.status == STATUS_STOP)
        {
            player->scheduler.switchDirect = SCHEDULER_SCORE_NEXT;
            player->scheduler.status = SCHEDULER_SWITCHING;
            StopDecode(player);
        }
        break;

    case SCHEDULER_SWITCHING:
        player->scheduler.status = player->scheduler.switchDirect;
        break;

    case SCHEDULER_SCORE_PREV:
        player->scheduler.currentScoreIndex--;
        if (player->scheduler.currentScoreIndex < 0)
            player->scheduler.currentScoreIndex = player->scheduler.maxScoreNum - 1;
        PlayScore(player, GetScorePhyiscalAddr(ScoreList[player->scheduler.currentScoreIndex]));
        player->scheduler.status = SCHEDULER_READY_TO_SWITCH;

        break;

    case SCHEDULER_SCORE_NEXT:
        player->scheduler.currentScoreIndex++;
        if (player->scheduler.currentScoreIndex >= player->scheduler.maxScoreNum)
        {
            player->scheduler.status = SCHEDULER_STOP;
            player->scheduler.currentScoreIndex = -1;
        }
        else
        {
            PlayScore(player, GetScorePhyiscalAddr(ScoreList[player->scheduler.currentScoreIndex]));
            player->scheduler.status = SCHEDULER_READY_TO_SWITCH;
        }

        break;

    case SCHEDULER_STOP:
        /* Do nothing */
        break;
    default:
        break;
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
    player->scheduler.scoreListHeader = &ScoreDataList;
    player->scheduler.currentScoreIndex = -1;
    player->scheduler.maxScoreNum = (player->scheduler.scoreListHeader)->scoreCount;
    player->scheduler.schedulerMode = MODE_ORDER_PLAY;
}

void StopPlayScheduler(Player *player)
{
    StopDecode(player);
    player->scheduler.status = SCHEDULER_STOP;
}

void PlayScore(Player *player, uint8_t *score)
{
    player->synthesizer.hwSet(SYNTH_HW_ON);
    player->decoder.currentTick = 0;
    player->decoder.lastScoreTick = 0;

    player->decoder.scorePointer = score;
    UpdateNextScoreTick(&(player->decoder));
    player->decoder.status = STATUS_DECODING;
}

void StopDecode(Player *player)
{
    player->synthesizer.hwSet(SYNTH_HW_OFF);
    player->decoder.status = STATUS_STOP;
    player->decoder.currentTick = 0;
    player->decoder.lastScoreTick = 0;
}

void PlayerInit(Player *player)
{
    player->decoder.status = STATUS_STOP;
    player->decoder.currentTick = 0;
    player->decoder.lastScoreTick = 0;
    player->decoder.scorePointer = NULL;
    SynthInit(&(player->synthesizer));
}