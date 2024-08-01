// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEActions.h"
#include <iostream>

int main()
{
    if (const std::unique_ptr<IEAction_Volume> VolumeAction = IEAction::GetVolumeAction())
    {
        float Volume = 1.0f;
        std::cout << "Set system volume 0 to 1: ";
        std::cin >> Volume;
        VolumeAction->SetVolume(Volume);
        std::cout << "You have set your master audio output volume to " << VolumeAction->GetVolume() << ".\n";
    }

    if (const std::unique_ptr<IEAction_Mute> MuteAction = IEAction::GetMuteAction())
    {
        int bMute = 0;
        std::cout << "Set system mute 0 or 1: ";
        std::cin >> bMute;
        MuteAction->SetMute(bMute ? true : false);

        if (MuteAction->GetMute())
        {
            std::cout << "You have muted your master audio output. \n";
        }
        else
        {
            std::cout << "You have un-muted your master audio output. \n";
        }
    }

    std::cout << "IEActions Example finished view the github Wiki for more.";
    return 0;
}