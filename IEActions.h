// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Immortal Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include <functional>
#include <memory>
#include <string>

class IEAction_Volume
{
public:
    virtual ~IEAction_Volume() = default;
    virtual float GetVolume() const { return 0.0f; }
    virtual void SetVolume(float Volume) {}
    virtual void RegisterVolumeChangeCallback(const std::function<void(float)>& Callback) {}
};

class IEAction_Mute
{
public:
    virtual ~IEAction_Mute() = default;
    virtual bool GetMute() const { return false; }
    virtual void SetMute(bool bMute) {}
    virtual void RegisterMuteChangeCallback(const std::function<void(bool)>& Callback) {}
};

class IEAction_ConsoleCommand
{
public:
    virtual ~IEAction_ConsoleCommand() = default;
    virtual void ExecuteConsoleCommand(const std::string& ConsoleCommand, float CommandParameterValue = 0.0f);
};

class IEAction_OpenFile
{
public:
    virtual ~IEAction_OpenFile() = default;
    virtual void OpenFile(const std::string& FilePath) {};
};

namespace IEAction
{
    std::unique_ptr<IEAction_Volume> GetVolumeAction();
    std::unique_ptr<IEAction_Mute> GetMuteAction();
    std::unique_ptr<IEAction_ConsoleCommand> GetConsoleCommandAction();
    std::unique_ptr<IEAction_OpenFile> GetOpenFileAction();
}