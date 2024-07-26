// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "IEActions.h"

#if defined (__APPLE__)
#include <AudioToolbox/AudioToolbox.h>
#include <CoreAudio/CoreAudio.h>

class IEAction_Volume_Impl_Apple : public IEAction_Volume
{
public:
    IEAction_Volume_Impl_Apple();

public:
    float GetVolume() const override;
    void SetVolume(float Volume) override;
    void RegisterVolumeChangeCallback(const std::function<void(float)>& Callback) override;

private:
    AudioDeviceID GetAudioDeviceID();
    static OSStatus VolumeChangeCallback(AudioObjectID ObjectID, uint32_t NumberAddresses, const AudioObjectPropertyAddress* PropertyAddresses, void* UserData);

private:
    AudioDeviceID m_AudioDeviceID;
    std::function<void(float)> m_OnVolumeChangeCallback;
};

class IEAction_Mute_Impl_Apple : public IEAction_Mute
{
public:
    IEAction_Mute_Impl_Apple();

public:
    bool GetMute() const override;
    void SetMute(bool bMute) override;
    void RegisterMuteChangeCallback(const std::function<void(bool)>& Callback) override;
   
private:
    AudioDeviceID GetAudioDeviceID();
    static OSStatus MuteChangeCallback(AudioObjectID ObjectID, uint32_t NumberAddresses, const AudioObjectPropertyAddress* PropertyAddresses, void* UserData);

private:
    AudioDeviceID m_AudioDeviceID;
    std::function<void(float)> m_OnMuteChangeCallback;
};

class IEAction_OpenFile_Impl_Apple : public IEAction_OpenFile
{
public:
    void OpenFile(const std::string& FilePath) override;
};
#endif