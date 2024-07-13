#include "SoundManager.h"
#include "SDL_mixer.h"
SoundManager* SoundManager::m_pInstance = 0;
bool SoundManager::init()
{
    if (m_bInitialized)
    {
        return true;
    }
    //初始化SDL_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        return false;
    }
    //加载音效
    for (int i = 0; i < _sound_total; i++)
    {
        m_sound_chunks[i] = Mix_LoadWAV(m_sound_paths[i].c_str());
        if (!m_sound_chunks[i])
        {
            return false;
        }
    }

    m_bInitialized = true;
    return true;
}

void SoundManager::playSound(Sound sound)
{
    Mix_PlayChannel(-1, m_sound_chunks[sound], 0);
}