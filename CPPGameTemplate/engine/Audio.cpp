/*
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Marmalade.
 *
 * This file consists of source code released by Marmalade under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */

#include "Audio.h"
#include "s3e.h"					// Marmalades main s3e API
#include "IwHashString.h"			// Marmalade API that deals with hashing strings

Audio*  g_pAudio;	// Global reference to our audio object

bool AudioSound::Load(const char* filename)
{
    m_NameHash = IwHashString(filename);
    m_SoundData = CIwSoundWAV::Create(filename, 0, 0);
    if (m_SoundData == 0)
        return false;
    m_SoundSpec = new CIwSoundSpec();
    m_SoundSpec->SetData(m_SoundData);

    return true;
}

AudioSound::~AudioSound()
{
    if (m_SoundSpec != 0)
        delete m_SoundSpec;
    if (m_SoundData != 0)
        delete m_SoundData;
}

//
// Audio class
//
Audio::Audio()
{
    // Initialise IwSound
    IwSoundInit();
}

Audio::~Audio()
{
    for (std::list<AudioSound*>::iterator it = m_Sounds.begin(); it !=  m_Sounds.end(); it++)
        delete *it;
    IwSoundTerminate();
}

AudioSound* Audio::findSound(unsigned int name_hash)
{
    for (std::list<AudioSound*>::iterator it = m_Sounds.begin(); it !=  m_Sounds.end(); it++)
        if ((*it)->m_NameHash == name_hash)
            return *it;
    return 0;
}

void Audio::Update()
{
    IwGetSoundManager()->Update();
}

void Audio::PlayMusic(const char* filename, bool repeat)
{
    // We only support play back of MP3 music
    if (s3eAudioIsCodecSupported(S3E_AUDIO_CODEC_MP3))
        s3eAudioPlay(filename, repeat ? 0 : 1);
}

void Audio::StopMusic()
{
    s3eAudioStop();
}

AudioSound* Audio::PreloadSound(const char* filename)
{
    AudioSound* sound = findSound(IwHashString(filename));
    if (sound == 0)
    {
        sound = new AudioSound();
        if (!sound->Load(filename))
        {
            delete sound;
            return 0;
        }
        m_Sounds.push_back(sound);
    }

    return sound;
}

void Audio::PlaySound(const char* filename)
{
    AudioSound* sound = PreloadSound(filename);
    if (sound != 0)
        sound->m_SoundSpec->Play();
}



