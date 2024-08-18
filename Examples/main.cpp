// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEActions.h"
#include <iostream>

void OnVolumeChange(float Volume, void* UserData)
{
    std::cout << "You have set your master audio output volume to " << Volume << ".\n";
}

void OnMuteChange(bool bMute, void* UserData)
{
    if (bMute)
    {
        std::cout << "You have muted your master audio output. \n";
    }
    else
    {
        std::cout << "You have un-muted your master audio output. \n";
    }
}

int main()
{
    const std::unique_ptr<IEAction_Volume> VolumeAction = IEAction::GetVolumeAction();
    const std::unique_ptr<IEAction_Mute> MuteAction = IEAction::GetMuteAction();

    if (VolumeAction)
    {
        const uint32_t CallbackID = VolumeAction->RegisterVolumeChangeCallback(&OnVolumeChange, nullptr);

        float Volume = 1.0f;
        std::cout << "Set system volume 0 to 1: ";
        std::cin >> Volume;
        VolumeAction->SetVolume(Volume);
    }

    if (MuteAction)
    {
        const uint32_t CallbackID = MuteAction->RegisterMuteChangeCallback(&OnMuteChange, nullptr);

        int bMute = 0;
        std::cout << "Set system mute 0 or 1: ";
        std::cin >> bMute;
        MuteAction->SetMute(bMute ? true : false);
    }

    std::cout << "IEActions Example finished view the github Wiki for more.";
    return 0;
}