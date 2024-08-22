// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "Implementation/IEAction_Impl_Win.hpp"

#if defined (_WIN32)

static std::atomic_bool bCallbackGuard(false);

IEAudioEndpointVolumeCallback::IEAudioEndpointVolumeCallback() : m_RefCount(1)
{
    m_AudioEndpointVolume = GetMainAudioEndpointVolume();
    if (m_AudioEndpointVolume)
    {
        m_AudioEndpointVolume->RegisterControlChangeNotify(this);
    }
}

HRESULT STDMETHODCALLTYPE IEAudioEndpointVolumeCallback::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = static_cast<IAudioEndpointVolumeCallback*>(this);
    AddRef();
    return S_OK;
}

ULONG STDMETHODCALLTYPE IEAudioEndpointVolumeCallback::AddRef(void)
{
    return InterlockedIncrement(&m_RefCount);
}

ULONG STDMETHODCALLTYPE IEAudioEndpointVolumeCallback::Release(void)
{
    ULONG ulRef = InterlockedDecrement(&m_RefCount);
    if (ulRef == 0)
    {
        if (m_AudioEndpointVolume)
        {
            m_AudioEndpointVolume->UnregisterControlChangeNotify(this);
        }
        delete this;
    }
    return ulRef;
}

IAudioEndpointVolume* IEAudioEndpointVolumeCallback::GetMainAudioEndpointVolume()
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

HRESULT STDMETHODCALLTYPE IEAction_Volume_Impl_Win::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
{
    HRESULT Result = E_FAIL;
    if (pNotify)
    {
        for (const std::pair<uint32_t, std::pair<std::function<void(float, void*)>, void*>>& Element : m_VolumeChangeCallbacks)
        {
            Element.second.first(pNotify->fMasterVolume, Element.second.second);
        }
        Result = S_OK;
    }
    return Result;
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

HRESULT STDMETHODCALLTYPE IEAction_Mute_Impl_Win::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
{
    HRESULT Result = E_FAIL;
    if (pNotify)
    {
        for (const std::pair<uint32_t, std::pair<std::function<void(float, void*)>, void*>>& Element : m_MuteChangeCallbacks)
        {
            Element.second.first(pNotify->bMuted, Element.second.second);
        }
        Result = S_OK;
    }
    return Result;
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