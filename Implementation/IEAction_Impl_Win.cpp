// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "Implementation/IEAction_Impl_Win.h"
#include "IEAction_Impl_Win.h"

#if defined (_WIN32)
IEAction_Volume_Impl_Win::IEAction_Volume_Impl_Win()
{
    m_AudioEndpointVolume = GetMainAudioEndpointVolume();
}

float IEAction_Volume_Impl_Win::GetVolume() const
{
    float Volume = -1.0f;
    if (m_AudioEndpointVolume)
    {
        m_AudioEndpointVolume->GetMasterVolumeLevelScalar(&Volume);
    }
    return Volume;
}

void IEAction_Volume_Impl_Win::SetVolume(float Volume)
{
    if (m_AudioEndpointVolume)
    {
        m_AudioEndpointVolume->SetMasterVolumeLevelScalar(std::clamp(Volume, 0.0f, 1.0f), nullptr);
    }
}

void IEAction_Volume_Impl_Win::RegisterVolumeChangeCallback(const std::function<void(float)>& Callback)
{
    m_OnVolumeChangeCallback = Callback;
}

IAudioEndpointVolume* IEAction_Volume_Impl_Win::GetMainAudioEndpointVolume()
{
    IAudioEndpointVolume* EndpointVolume = nullptr;
    HRESULT Result = CoInitialize(NULL);
    if (SUCCEEDED(Result))
    {
        IMMDeviceEnumerator* DeviceEnumerator = nullptr;
        Result = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&DeviceEnumerator));
        if (SUCCEEDED(Result) && DeviceEnumerator)
        {
            IMMDevice* Device = nullptr;
            Result = DeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &Device);
            if (SUCCEEDED(Result) && Device)
            {
                Result = Device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (void**)&EndpointVolume);
                if (FAILED(Result))
                {
                    Device->Release();
                    DeviceEnumerator->Release();
                    CoUninitialize();
                }
            }
            else
            {
                DeviceEnumerator->Release();
                CoUninitialize();
            }
        }
        else
        {
            CoUninitialize();
        }
    }
    return EndpointVolume;
}

IEAction_Mute_Impl_Win::IEAction_Mute_Impl_Win()
{
    m_AudioEndpointVolume = GetMainAudioEndpointVolume();
}

bool IEAction_Mute_Impl_Win::GetMute() const
{
    BOOL bMute;
    if (m_AudioEndpointVolume)
    {
        m_AudioEndpointVolume->GetMute(&bMute);
    }
    return bMute;
}

void IEAction_Mute_Impl_Win::SetMute(bool bMute)
{
    if (m_AudioEndpointVolume)
    {
        m_AudioEndpointVolume->SetMute(bMute, nullptr);
    }
}

void IEAction_Mute_Impl_Win::RegisterMuteChangeCallback(const std::function<void(bool)>& Callback)
{
    m_OnMuteChangeCallback = Callback;
}

IAudioEndpointVolume* IEAction_Mute_Impl_Win::GetMainAudioEndpointVolume()
{
    IAudioEndpointVolume* EndpointVolume = nullptr;
    HRESULT Result = CoInitialize(NULL);
    if (SUCCEEDED(Result))
    {
        IMMDeviceEnumerator* DeviceEnumerator = nullptr;
        Result = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&DeviceEnumerator));
        if (SUCCEEDED(Result) && DeviceEnumerator)
        {
            IMMDevice* Device = nullptr;
            Result = DeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &Device);
            if (SUCCEEDED(Result) && Device)
            {
                Result = Device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (void**)&EndpointVolume);
                if (FAILED(Result))
                {
                    Device->Release();
                    DeviceEnumerator->Release();
                    CoUninitialize();
                }
            }
            else
            {
                DeviceEnumerator->Release();
                CoUninitialize();
            }
        }
        else
        {
            CoUninitialize();
        }
    }
    return EndpointVolume;
}

void IEAction_ConsoleCommand_Impl_Win::ExecuteConsoleCommand(const std::string& ConsoleCommand, float CommandParameterValue)
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

void IEAction_OpenFile_Impl_Win::OpenFile(const std::string& FilePath)
{
    SHELLEXECUTEINFOA ShExecInfo = {0};
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = "open"; 
    ShExecInfo.lpFile = FilePath.c_str();
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_SHOWNORMAL;
    ShExecInfo.hInstApp = NULL;

    if (!ShellExecuteExA(&ShExecInfo))
    {
        ShExecInfo.fMask = SEE_MASK_INVOKEIDLIST;
        ShExecInfo.lpVerb = "openas";
        ShellExecuteExA(&ShExecInfo);
    }
}
#endif