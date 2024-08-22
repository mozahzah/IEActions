// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEActions.hpp"
#include <cstdint>
#include <iostream>
#include <memory>

void OnVolumeChange(float Volume, void* UserData)
{
    std::cout << "Master audio output volume equals " << Volume << " \n";
}

void OnMuteChange(bool bMute, void* UserData)
{
    if (bMute)
    {
        std::cout << "Muted your master audio output. \n";
    }
    else
    {
        std::cout << "Un-muted your master audio output. \n";
    }
}

int main()
{
    const std::unique_ptr<IEAction_Volume> VolumeAction = IEAction::GetVolumeAction();
    const std::unique_ptr<IEAction_Mute> MuteAction = IEAction::GetMuteAction();

    if (VolumeAction && MuteAction)
    {
        float Volume = 0.0f;
        std::cout << "Set system volume (0.0 to 1.0): \n";
        std::cin >> Volume;
        VolumeAction->SetVolume(Volume);

        int bMute = 0;
        std::cout << "Set system mute (0 for unmute, 1 for mute): \n";
        std::cin >> bMute;
        MuteAction->SetMute(bMute ? true : false);

        std::cout << "Test callbacks by changing your system volume and mute value on your desktop or press Enter to end the demo....\n";
        const uint32_t VolumeCallbackID = VolumeAction->RegisterVolumeChangeCallback(&OnVolumeChange, nullptr);
        const uint32_t MuteCallbackID = MuteAction->RegisterMuteChangeCallback(&OnMuteChange, nullptr);
        std::cin.ignore();
        std::cin.get();

        std::cout << "IEActions Example finished. View the GitHub Wiki for more." << std::endl;

        VolumeAction->UnregisterVolumeChangeCallback(VolumeCallbackID);
        MuteAction->UnregisterMuteChangeCallback(MuteCallbackID);
    }
    return 0;
}