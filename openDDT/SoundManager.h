#pragma once
#include "GameObject.h"
#include "SDL_mixer.h"
using std::string;

enum Sound
{
    _shoot_sound=0,
    _explode_sound,
    _sound_total,
};

class SoundManager;
typedef SoundManager TheSoundManager;

class SoundManager :
    public GameObject
{
public:
    static SoundManager* Instance()
    {
        if (m_pInstance == 0)
        {
            m_pInstance = new SoundManager();
        }
        return m_pInstance;
    }
    void draw(int angle) {}
    void update(State) {}
    void clean() {}
    bool init();
    void playSound(Sound sound);
private:
    SoundManager() { if (!m_bInitialized) { init(); } }
    static SoundManager* m_pInstance;
    bool m_bInitialized = false;
    Mix_Chunk* m_sound_chunks[_sound_total] = { 0 };
    string m_sound_paths[_sound_total] = { "sounds/shoot.wav","sounds/explode.wav" };
};

