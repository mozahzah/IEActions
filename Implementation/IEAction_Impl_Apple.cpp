// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "Implementation/IEAction_Impl_Apple.hpp"

#if defined (__APPLE__)
IEAction_Volume_Impl_Apple::IEAction_Volume_Impl_Apple()
{
    m_AudioDeviceID = GetAudioDeviceID();

    const AudioObjectPropertyAddress PropertyAddress =
    {
        kAudioHardwareServiceDeviceProperty_VirtualMainVolume,
        kAudioDevicePropertyScopeOutput,
        kAudioObjectPropertyElementMain
    };
    AudioObjectAddPropertyListener(m_AudioDeviceID, &PropertyAddress, &IEAction_Volume_Impl_Apple::VolumeChangeCallback, this);
}

float IEAction_Volume_Impl_Apple::GetVolume() const
{
    float Volume = 0.0f;
    UInt32 PropertySize = sizeof(float);
    const AudioObjectPropertyAddress PropertyAddress =
    {
        kAudioHardwareServiceDeviceProperty_VirtualMainVolume,
        kAudioDevicePropertyScopeOutput,
        kAudioObjectPropertyElementMain
    };
    
    AudioObjectGetPropertyData(m_AudioDeviceID, &PropertyAddress, 0, NULL, &PropertySize, &Volume);    
    return Volume;
}

void IEAction_Volume_Impl_Apple::SetVolume(float Volume)
{
    uint32_t PropertySize = sizeof(float);
    const AudioObjectPropertyAddress PropertyAddress =
    {
        kAudioHardwareServiceDeviceProperty_VirtualMainVolume,
        kAudioDevicePropertyScopeOutput,
        kAudioObjectPropertyElementMain
    };

    float FinalVolume = std::clamp(Volume, 0.0f, 1.0f);
    AudioObjectSetPropertyData(m_AudioDeviceID, &PropertyAddress, 0, NULL, PropertySize, &FinalVolume);
}

AudioDeviceID IEAction_Volume_Impl_Apple::GetAudioDeviceID()
{
    AudioDeviceID OutputDeviceID;
    uint32_t PropertySize = sizeof(AudioDeviceID);
    const AudioObjectPropertyAddress PropertyAddress =
    {
        kAudioHardwarePropertyDefaultOutputDevice,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };
    
    AudioObjectGetPropertyData(kAudioObjectSystemObject, &PropertyAddress, 0, NULL, &PropertySize, &OutputDeviceID);
    return OutputDeviceID;
}

OSStatus IEAction_Volume_Impl_Apple::VolumeChangeCallback(AudioObjectID ObjectID, uint32_t NumberAddresses, const AudioObjectPropertyAddress* PropertyAddresses, void* UserData)
{
    OSStatus Status;
    if (IEAction_Volume_Impl_Apple* const AppleVolumeAction = static_cast<IEAction_Volume_Impl_Apple*>(UserData))
    {
        for (const std::pair<uint32_t, std::pair<std::function<void(float, void*)>, void*>>& Element : AppleVolumeAction->m_VolumeChangeCallbacks)
        {
            Element.second.first(AppleVolumeAction->GetVolume(), Element.second.second);
        }
        Status = noErr;
    }
    return Status;
}

IEAction_Mute_Impl_Apple::IEAction_Mute_Impl_Apple()
{
    m_AudioDeviceID = GetAudioDeviceID();

    const AudioObjectPropertyAddress PropertyAddress =
    {
        kAudioDevicePropertyMute,
        kAudioDevicePropertyScopeOutput,
        kAudioObjectPropertyElementMain
    };
    AudioObjectAddPropertyListener(m_AudioDeviceID, &PropertyAddress, &IEAction_Mute_Impl_Apple::MuteChangeCallback, this);
}

bool IEAction_Mute_Impl_Apple::GetMute() const
{
    uint32_t PropertySize = sizeof(uint32_t);
    uint32_t Mute = 0;
    const AudioObjectPropertyAddress PropertyAddress =
    {
        kAudioDevicePropertyMute,
        kAudioDevicePropertyScopeOutput,
        kAudioObjectPropertyElementMain
    };
    
    AudioObjectGetPropertyData(m_AudioDeviceID, &PropertyAddress, 0, NULL, &PropertySize, &Mute);    
    return Mute != 0;
}

void IEAction_Mute_Impl_Apple::SetMute(bool bMute)
{
    uint32_t PropertySize = sizeof(uint32_t);
    uint32_t Mute = bMute;
    const AudioObjectPropertyAddress PropertyAddress =
    {
        kAudioDevicePropertyMute,
        kAudioDevicePropertyScopeOutput,
        kAudioObjectPropertyElementMain
    };

    AudioObjectSetPropertyData(m_AudioDeviceID, &PropertyAddress, 0, NULL, PropertySize, &Mute);
}

AudioDeviceID IEAction_Mute_Impl_Apple::GetAudioDeviceID()
{
    AudioDeviceID OutputDeviceID;
    uint32_t PropertySize = sizeof(AudioDeviceID);
    const AudioObjectPropertyAddress PropertyAddress =
    {
        kAudioHardwarePropertyDefaultOutputDevice,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMain
    };
    
    AudioObjectGetPropertyData(kAudioObjectSystemObject, &PropertyAddress, 0, NULL, &PropertySize, &OutputDeviceID);
    return OutputDeviceID;
}

OSStatus IEAction_Mute_Impl_Apple::MuteChangeCallback(AudioObjectID ObjectID, uint32_t NumberAddresses, const AudioObjectPropertyAddress* PropertyAddresses, void* UserData)
{
    OSStatus Status;
    if (IEAction_Mute_Impl_Apple* const AppleMuteAction = static_cast<IEAction_Mute_Impl_Apple*>(UserData))
    {
        for (const std::pair<uint32_t, std::pair<std::function<void(bool, void*)>, void*>>& Element : AppleMuteAction->m_MuteChangeCallbacks)
        {
            Element.second.first(AppleMuteAction->GetMute(), Element.second.second);
        }
        Status = noErr;
    }
    return Status;
}

void IEAction_ConsoleCommand_Impl_Apple::ExecuteConsoleCommand(const std::string& ConsoleCommand, float CommandParameterValue)
{
    std::string FinalConsoleCommand = ConsoleCommand;

    const size_t ValuePosition = FinalConsoleCommand.find("{V}");
    if (ValuePosition != std::string::npos)
    {
        const std::string ValueStr = std::to_string(CommandParameterValue);
        FinalConsoleCommand.replace(ValuePosition, 3, ValueStr);
    }

    system(FinalConsoleCommand.c_str());
}

void IEAction_OpenFile_Impl_Apple::OpenFile(const std::string& FilePath)
{
    std::string OpenFileCommand = "open \"" + FilePath + "\"";
    int result = system(OpenFileCommand.c_str());
}
#endif
