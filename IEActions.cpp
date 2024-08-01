// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEActions.h"

#include "Implementation/IEAction_Impl_Win.h"
#include "Implementation/IEAction_Impl_Apple.h"

namespace IEAction
{
    std::unique_ptr<IEAction_Volume> GetVolumeAction()
    {
#if defined (_WIN32)
        return std::make_unique<IEAction_Volume_Impl_Win>();
#elif defined (__APPLE__)
        return std::make_unique<IEAction_Volume_Impl_Apple>();
#endif
    }

    std::unique_ptr<IEAction_Mute> GetMuteAction()
    {
#if defined (_WIN32)
        return std::make_unique<IEAction_Mute_Impl_Win>();
#elif defined (__APPLE__)
        return std::make_unique<IEAction_Mute_Impl_Apple>();
#endif
    }

    std::unique_ptr<IEAction_ConsoleCommand> GetConsoleCommandAction()
    {
#if defined (_WIN32)
        return std::make_unique<IEAction_ConsoleCommand_Impl_Win>();
#elif defined (__APPLE__)
        return std::make_unique<IEAction_ConsoleCommand_Impl_Apple>();
#endif
    }

    std::unique_ptr<IEAction_OpenFile> GetOpenFileAction()
    {
#if defined (_WIN32)
        return std::make_unique<IEAction_OpenFile_Impl_Win>();
#elif defined (__APPLE__)
        return std::make_unique<IEAction_OpenFile_Impl_Apple>();
#endif
    }
}