// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <random>
#include <string>

class IEAction_Volume
{
public:
    virtual ~IEAction_Volume() = default;
    virtual float GetVolume() const = 0;
    virtual void SetVolume(float Volume) = 0;

public:
    [[nodiscard]] uint32_t RegisterVolumeChangeCallback(const std::function<void(float, void*)>& Callback, void* UserData);
    void UnregisterVolumeChangeCallback(uint32_t CallbackID);

protected:
    std::map<uint32_t, std::pair<std::function<void(float, void*)>, void*>> m_VolumeChangeCallbacks;
};

class IEAction_Mute
{
public:
    virtual ~IEAction_Mute() = default;
    virtual bool GetMute() const = 0;
    virtual void SetMute(bool bMute) = 0;

public:
    [[nodiscard]] uint32_t RegisterMuteChangeCallback(const std::function<void(bool, void*)>& Callback, void* UserData);
    void UnregisterMuteChangeCallback(uint32_t CallbackID);

protected:
    std::map<uint32_t, std::pair<std::function<void(bool, void*)>, void*>> m_MuteChangeCallbacks;
};

class IEAction_ConsoleCommand
{
public:
    virtual ~IEAction_ConsoleCommand() = default;
    virtual void ExecuteConsoleCommand(const std::string& ConsoleCommand, float CommandParameterValue = 0.0f) = 0;
};

class IEAction_OpenFile
{
public:
    virtual ~IEAction_OpenFile() = default;
    virtual void OpenFile(const std::string& FilePath) = 0;
};

namespace IEAction
{
    std::unique_ptr<IEAction_Volume> GetVolumeAction();
    std::unique_ptr<IEAction_Mute> GetMuteAction();
    std::unique_ptr<IEAction_ConsoleCommand> GetConsoleCommandAction();
    std::unique_ptr<IEAction_OpenFile> GetOpenFileAction();
}