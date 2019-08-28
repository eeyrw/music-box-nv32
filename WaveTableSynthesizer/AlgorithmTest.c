//#ifdef RUN_TEST
#include "SynthCore.h"
#include "Player.h"
#include <stdint.h>
#include <stdio.h>

#define TEST_LOOP_NUN 10000

Synthesizer synthesizerC;
Synthesizer synthesizerASM;


void TestInit(void)
{
    SynthInit(&synthesizerC);
    SynthInit(&synthesizerASM);
}

int32_t abs_s32(int32_t num)
{
    return num>0?num:-num;
}

void PrintParameters(Synthesizer* synth)
{
    SoundUnit* soundUnits = synth->SoundUnitList;
    printf("MixOut:%d\n",synth->mixOut);
    printf("LastSoundUnit:%d\n",synth->lastSoundUnit);
    printf("%12s","Chn Val");
    for(uint8_t k=0;k<POLY_NUM;k++)
    {
        printf("%6d ",soundUnits[k].val);
    }
    printf("\n");

    printf("%12s","Chn Sample");
    for(uint8_t k=0;k<POLY_NUM;k++)
    {
        printf("%6d ",soundUnits[k].sampleVal);
    }
    printf("\n");

    printf("%12s","Chn EnvLevel");
    for(uint8_t k=0;k<POLY_NUM;k++)
    {
        printf("%6d ",soundUnits[k].envelopeLevel);
    }
    printf("\n");


    printf("%12s","Chn WavePos");
    for(uint8_t k=0;k<POLY_NUM;k++)
    {
        printf("%6lu ",soundUnits[k].wavetablePos);
    }
    printf("\n");

    printf("%12s","Chn EnvlPos");
    for(uint8_t k=0;k<POLY_NUM;k++)
    {
        printf("%6d ",soundUnits[k].envelopePos);
    }
    printf("\n");

    printf("%12s","Chn NoteIncr");
    for(uint8_t k=0;k<POLY_NUM;k++)
    {
        printf("%6x ",soundUnits[k].increment);
    }
    printf("\n");
       
}

void TestUpdateTickFunc(void)
{
    uint32_t i;
    Player player;
    PlayerInit(&player);
    printf("~~~~~~~Start testing updateTickFunc.~~~~~~~\n");
    for(i=0;i<0xffff;i++)
    {
        if(i!=player.currentTick)
        {
            printf("UpdateTickFunc get wrong in %ld loop.\n",i);
            break;
        }
        UpdateTick(&player);
    }
    if(i==player.currentTick)
        printf("UpdateTickFunc passed the test.\n");

}

uint8_t SynthParamterCompare(Synthesizer* synthA,Synthesizer* synthB)
{
    uint32_t error=0;
    SoundUnit* sa=synthA->SoundUnitList;
    SoundUnit* sb=synthB->SoundUnitList;

    if(abs_s32(synthA->mixOut-synthB->mixOut)>POLY_NUM)
        error++;
    for(uint32_t k=0;k<POLY_NUM;k++)
    {
        if(abs_s32(sa[k].val-sb[k].val)>2)
            error++;
        if(sa[k].sampleVal!=sb[k].sampleVal)
            error++;
        if(sa[k].envelopeLevel!=sb[k].envelopeLevel)
            error++;
        if(sa[k].envelopePos!=sb[k].envelopePos)
            error++;
        if(sa[k].wavetablePos!=sb[k].wavetablePos)
            error++;
        if(sa[k].increment!=sb[k].increment)
            error++;
    }
    if(error>0)
    {
        printf("%lu error(s) found:\n",error);
        printf("Synth C:\n");
        PrintParameters(&synthesizerC);
        printf("Synth ASM:\n");
        PrintParameters(&synthesizerASM);
    }
    else
    {
        printf("Passed.\n");
    }
    return error;
    
}
extern Player mainPlayer;

void TestSynth(void)
{
    uint32_t errorTimes=0;
    printf("~~~~~~~Start testing synthesizer.~~~~~~~\n");
    for(uint32_t i=40;i<40+POLY_NUM;i++)
    {
        NoteOnC(&synthesizerC,i%56);
        NoteOnAsm(&synthesizerASM,i%56);
    }
    printf("After NoteOn Test:\n");
        printf("Synth C:\n");
        PrintParameters(&synthesizerC);
        printf("Synth ASM:\n");
        PrintParameters(&synthesizerASM);

    for(uint32_t i=0;i<TEST_LOOP_NUN;i++)
    {
        SynthAsm(&synthesizerASM);
        SynthC(&synthesizerC);
        GenDecayEnvlopeC(&synthesizerC);
        GenDecayEnvlopeAsm(&synthesizerASM);
        printf("=============%lu==============\n",i);
        if(SynthParamterCompare(&synthesizerC,&synthesizerASM)>0)
        errorTimes++;

        if(errorTimes>6)
        break;
    }
}

void TestProcess(void)
{
    TestInit();
    TestUpdateTickFunc();
    TestSynth();
}
//#endif