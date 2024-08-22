// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "IEActions.hpp"

#if defined (_WIN32)
#include <atlbase.h> 
#include <commctrl.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>

class IEAudioEndpointVolumeCallback : public IAudioEndpointVolumeCallback
{
public:
    IEAudioEndpointVolumeCallback();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
    ULONG STDMETHODCALLTYPE AddRef(void) override;
    ULONG STDMETHODCALLTYPE Release(void) override;

private:
    IAudioEndpointVolume* GetMainAudioEndpointVolume();
    
protected:
    CComPtr<IAudioEndpointVolume> m_AudioEndpointVolume;

private:
    LONG m_RefCount;
};

class IEAction_Volume_Impl_Win : public IEAction_Volume, public IEAudioEndpointVolumeCallback
{
public:
    ~IEAction_Volume_Impl_Win() override = default;
    float GetVolume() const override;
    void SetVolume(float Volume) override;

public:
    HRESULT STDMETHODCALLTYPE OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify) override;
};

class IEAction_Mute_Impl_Win : public IEAction_Mute, public IEAudioEndpointVolumeCallback
{
public:
    ~IEAction_Mute_Impl_Win() override = default;
    bool GetMute() const override;
    void SetMute(bool bMute) override;

public:
    HRESULT STDMETHODCALLTYPE OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify) override;
};

class IEAction_ConsoleCommand_Impl_Win : public IEAction_ConsoleCommand
{
public:
    void ExecuteConsoleCommand(const std::string& ConsoleCommand, float CommandParameterValue = 0.0f) override;
};

class IEAction_OpenFile_Impl_Win : public IEAction_OpenFile
{
public:
    void OpenFile(const std::string& FilePath) override;
};
#endif