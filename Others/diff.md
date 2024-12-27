# Modules/Audio/Public/AudioManager.cpp

```c++
(721行)  在 bool AudioManager::InitFMOD(FMOD_OUTPUTTYPE desireOutput)

if(!m_FMODSystem) // not loaded yet
{
    + #if PLATFORM_OPENHARMONY
    +     m_MaEngine = (ma_engine*)malloc(sizeof(ma_engine));
    +     engine_init_buf_vfs(m_MaEngine);
    + #endif
    #if PLATFORM_METRO
    FMOD::FMODInvokeOnUIThread = _FMODInvokeOnUIThread;
    #endif
}
```


# Module/Audio/Public/AudioManager.h
```c++
(396行)，在 public:
    inline FMOD::System* GetFMODSystem() const {return m_FMODSystem;}
    + #if PLATFORM_OPENHARMONY
    +       inline ma_engine* GetMaEngine() const {return m_MaEngine;}
    + #endif
```