//
//  TradlibState.hpp
//  TradApp
//
//  Created by damien murtagh on 1/10/23.
//

#pragma once

// Using c style enum and struct to maintain compatibility with all of the codebase (c++, objective-c and swift)

enum CurrentAction
{
    tradlibState_CurrentAction_RemoveSilence,
    tradlibState_CurrentAction_ConfigureFilters,
    tradlibState_CurrentAction_CalculateHarmonicity,
    tradlibState_CurrentAction_FindOnsets,
    tradlibState_CurrentAction_DetectNotes,
    
};

enum CurrentActionState
{
    tradlibState_CurrentActionState_Starting,
    tradlibState_CurrentActionState_Running,
    tradlibState_CurrentActionState_FinishedWithSuccess,
    tradlibState_CurrentActionState_FinishedWithError,
    tradlibState_CurrentActionState_Aborted
};

enum ErrorReason
{
    tradlibState_ErrorReason_NoAudioFound,
    tradlibState_ErrorReason_NoSamplesLoaded,
    tradlibState_ErrorReason_NoError
};

struct TradlibState
{
    enum CurrentAction       action;
    enum CurrentActionState  actionState;
    enum ErrorReason         errorReason;
    
    
    // Method
    
    // Results (vectors like odfSigna etc.)
};

