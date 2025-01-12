#ifndef _WINMM_PROXY_H
#define _WINMM_PROXY_H

#ifdef _MSC_VER
#define EXPORT(exp_name, target_name) __pragma(comment(linker, "/export:" #exp_name "=C:\\Windows\\System32\\" #target_name))
#else
#define EXPORT(exp_name, target_name) asm(".section .drectve\n\t.ascii \" -export:" #exp_name "= c:/windows/system32/" #target_name "\"")
#endif

EXPORT(CloseDriver, winmm.CloseDriver);
EXPORT(DefDriverProc, winmm.DefDriverProc);
EXPORT(DriverCallback, winmm.DriverCallback);
EXPORT(DrvGetModuleHandle, winmm.DrvGetModuleHandle);
EXPORT(GetDriverModuleHandle, winmm.GetDriverModuleHandle);
EXPORT(OpenDriver, winmm.OpenDriver);
EXPORT(PlaySound, winmm.PlaySound);
EXPORT(PlaySoundA, winmm.PlaySoundA);
EXPORT(PlaySoundW, winmm.PlaySoundW);
EXPORT(SendDriverMessage, winmm.SendDriverMessage);
EXPORT(WOWAppExit, winmm.WOWAppExit);
EXPORT(auxGetDevCapsA, winmm.auxGetDevCapsA);
EXPORT(auxGetDevCapsW, winmm.auxGetDevCapsW);
EXPORT(auxGetNumDevs, winmm.auxGetNumDevs);
EXPORT(auxGetVolume, winmm.auxGetVolume);
EXPORT(auxOutMessage, winmm.auxOutMessage);
EXPORT(auxSetVolume, winmm.auxSetVolume);
EXPORT(joyConfigChanged, winmm.joyConfigChanged);
EXPORT(joyGetDevCapsA, winmm.joyGetDevCapsA);
EXPORT(joyGetDevCapsW, winmm.joyGetDevCapsW);
EXPORT(joyGetNumDevs, winmm.joyGetNumDevs);
EXPORT(joyGetPos, winmm.joyGetPos);
EXPORT(joyGetPosEx, winmm.joyGetPosEx);
EXPORT(joyGetThreshold, winmm.joyGetThreshold);
EXPORT(joyReleaseCapture, winmm.joyReleaseCapture);
EXPORT(joySetCapture, winmm.joySetCapture);
EXPORT(joySetThreshold, winmm.joySetThreshold);
EXPORT(mciDriverNotify, winmm.mciDriverNotify);
EXPORT(mciDriverYield, winmm.mciDriverYield);
EXPORT(mciExecute, winmm.mciExecute);
EXPORT(mciFreeCommandResource, winmm.mciFreeCommandResource);
EXPORT(mciGetCreatorTask, winmm.mciGetCreatorTask);
EXPORT(mciGetDeviceIDA, winmm.mciGetDeviceIDA);
EXPORT(mciGetDeviceIDFromElementIDA, winmm.mciGetDeviceIDFromElementIDA);
EXPORT(mciGetDeviceIDFromElementIDW, winmm.mciGetDeviceIDFromElementIDW);
EXPORT(mciGetDeviceIDW, winmm.mciGetDeviceIDW);
EXPORT(mciGetDriverData, winmm.mciGetDriverData);
EXPORT(mciGetErrorStringA, winmm.mciGetErrorStringA);
EXPORT(mciGetErrorStringW, winmm.mciGetErrorStringW);
EXPORT(mciGetYieldProc, winmm.mciGetYieldProc);
EXPORT(mciLoadCommandResource, winmm.mciLoadCommandResource);
EXPORT(mciSendCommandA, winmm.mciSendCommandA);
EXPORT(mciSendCommandW, winmm.mciSendCommandW);
EXPORT(mciSendStringA, winmm.mciSendStringA);
EXPORT(mciSendStringW, winmm.mciSendStringW);
EXPORT(mciSetDriverData, winmm.mciSetDriverData);
EXPORT(mciSetYieldProc, winmm.mciSetYieldProc);
EXPORT(midiConnect, winmm.midiConnect);
EXPORT(midiDisconnect, winmm.midiDisconnect);
EXPORT(midiInAddBuffer, winmm.midiInAddBuffer);
EXPORT(midiInClose, winmm.midiInClose);
EXPORT(midiInGetDevCapsA, winmm.midiInGetDevCapsA);
EXPORT(midiInGetDevCapsW, winmm.midiInGetDevCapsW);
EXPORT(midiInGetErrorTextA, winmm.midiInGetErrorTextA);
EXPORT(midiInGetErrorTextW, winmm.midiInGetErrorTextW);
EXPORT(midiInGetID, winmm.midiInGetID);
EXPORT(midiInGetNumDevs, winmm.midiInGetNumDevs);
EXPORT(midiInMessage, winmm.midiInMessage);
EXPORT(midiInOpen, winmm.midiInOpen);
EXPORT(midiInPrepareHeader, winmm.midiInPrepareHeader);
EXPORT(midiInReset, winmm.midiInReset);
EXPORT(midiInStart, winmm.midiInStart);
EXPORT(midiInStop, winmm.midiInStop);
EXPORT(midiInUnprepareHeader, winmm.midiInUnprepareHeader);
EXPORT(midiOutCacheDrumPatches, winmm.midiOutCacheDrumPatches);
EXPORT(midiOutCachePatches, winmm.midiOutCachePatches);
EXPORT(midiOutClose, winmm.midiOutClose);
EXPORT(midiOutGetDevCapsA, winmm.midiOutGetDevCapsA);
EXPORT(midiOutGetDevCapsW, winmm.midiOutGetDevCapsW);
EXPORT(midiOutGetErrorTextA, winmm.midiOutGetErrorTextA);
EXPORT(midiOutGetErrorTextW, winmm.midiOutGetErrorTextW);
EXPORT(midiOutGetID, winmm.midiOutGetID);
EXPORT(midiOutGetNumDevs, winmm.midiOutGetNumDevs);
EXPORT(midiOutGetVolume, winmm.midiOutGetVolume);
EXPORT(midiOutLongMsg, winmm.midiOutLongMsg);
EXPORT(midiOutMessage, winmm.midiOutMessage);
EXPORT(midiOutOpen, winmm.midiOutOpen);
EXPORT(midiOutPrepareHeader, winmm.midiOutPrepareHeader);
EXPORT(midiOutReset, winmm.midiOutReset);
EXPORT(midiOutSetVolume, winmm.midiOutSetVolume);
EXPORT(midiOutShortMsg, winmm.midiOutShortMsg);
EXPORT(midiOutUnprepareHeader, winmm.midiOutUnprepareHeader);
EXPORT(midiStreamClose, winmm.midiStreamClose);
EXPORT(midiStreamOpen, winmm.midiStreamOpen);
EXPORT(midiStreamOut, winmm.midiStreamOut);
EXPORT(midiStreamPause, winmm.midiStreamPause);
EXPORT(midiStreamPosition, winmm.midiStreamPosition);
EXPORT(midiStreamProperty, winmm.midiStreamProperty);
EXPORT(midiStreamRestart, winmm.midiStreamRestart);
EXPORT(midiStreamStop, winmm.midiStreamStop);
EXPORT(mixerClose, winmm.mixerClose);
EXPORT(mixerGetControlDetailsA, winmm.mixerGetControlDetailsA);
EXPORT(mixerGetControlDetailsW, winmm.mixerGetControlDetailsW);
EXPORT(mixerGetDevCapsA, winmm.mixerGetDevCapsA);
EXPORT(mixerGetDevCapsW, winmm.mixerGetDevCapsW);
EXPORT(mixerGetID, winmm.mixerGetID);
EXPORT(mixerGetLineControlsA, winmm.mixerGetLineControlsA);
EXPORT(mixerGetLineControlsW, winmm.mixerGetLineControlsW);
EXPORT(mixerGetLineInfoA, winmm.mixerGetLineInfoA);
EXPORT(mixerGetLineInfoW, winmm.mixerGetLineInfoW);
EXPORT(mixerGetNumDevs, winmm.mixerGetNumDevs);
EXPORT(mixerMessage, winmm.mixerMessage);
EXPORT(mixerOpen, winmm.mixerOpen);
EXPORT(mixerSetControlDetails, winmm.mixerSetControlDetails);
EXPORT(mmDrvInstall, winmm.mmDrvInstall);
EXPORT(mmGetCurrentTask, winmm.mmGetCurrentTask);
EXPORT(mmTaskBlock, winmm.mmTaskBlock);
EXPORT(mmTaskCreate, winmm.mmTaskCreate);
EXPORT(mmTaskSignal, winmm.mmTaskSignal);
EXPORT(mmTaskYield, winmm.mmTaskYield);
EXPORT(mmioAdvance, winmm.mmioAdvance);
EXPORT(mmioAscend, winmm.mmioAscend);
EXPORT(mmioClose, winmm.mmioClose);
EXPORT(mmioCreateChunk, winmm.mmioCreateChunk);
EXPORT(mmioDescend, winmm.mmioDescend);
EXPORT(mmioFlush, winmm.mmioFlush);
EXPORT(mmioGetInfo, winmm.mmioGetInfo);
EXPORT(mmioInstallIOProcA, winmm.mmioInstallIOProcA);
EXPORT(mmioInstallIOProcW, winmm.mmioInstallIOProcW);
EXPORT(mmioOpenA, winmm.mmioOpenA);
EXPORT(mmioOpenW, winmm.mmioOpenW);
EXPORT(mmioRead, winmm.mmioRead);
EXPORT(mmioRenameA, winmm.mmioRenameA);
EXPORT(mmioRenameW, winmm.mmioRenameW);
EXPORT(mmioSeek, winmm.mmioSeek);
EXPORT(mmioSendMessage, winmm.mmioSendMessage);
EXPORT(mmioSetBuffer, winmm.mmioSetBuffer);
EXPORT(mmioSetInfo, winmm.mmioSetInfo);
EXPORT(mmioStringToFOURCCA, winmm.mmioStringToFOURCCA);
EXPORT(mmioStringToFOURCCW, winmm.mmioStringToFOURCCW);
EXPORT(mmioWrite, winmm.mmioWrite);
EXPORT(mmsystemGetVersion, winmm.mmsystemGetVersion);
EXPORT(sndPlaySoundA, winmm.sndPlaySoundA);
EXPORT(sndPlaySoundW, winmm.sndPlaySoundW);
EXPORT(timeBeginPeriod, winmm.timeBeginPeriod);
EXPORT(timeEndPeriod, winmm.timeEndPeriod);
EXPORT(timeGetDevCaps, winmm.timeGetDevCaps);
EXPORT(timeGetSystemTime, winmm.timeGetSystemTime);
EXPORT(timeGetTime, winmm.timeGetTime);
EXPORT(timeKillEvent, winmm.timeKillEvent);
EXPORT(timeSetEvent, winmm.timeSetEvent);
EXPORT(waveInAddBuffer, winmm.waveInAddBuffer);
EXPORT(waveInClose, winmm.waveInClose);
EXPORT(waveInGetDevCapsA, winmm.waveInGetDevCapsA);
EXPORT(waveInGetDevCapsW, winmm.waveInGetDevCapsW);
EXPORT(waveInGetErrorTextA, winmm.waveInGetErrorTextA);
EXPORT(waveInGetErrorTextW, winmm.waveInGetErrorTextW);
EXPORT(waveInGetID, winmm.waveInGetID);
EXPORT(waveInGetNumDevs, winmm.waveInGetNumDevs);
EXPORT(waveInGetPosition, winmm.waveInGetPosition);
EXPORT(waveInMessage, winmm.waveInMessage);
EXPORT(waveInOpen, winmm.waveInOpen);
EXPORT(waveInPrepareHeader, winmm.waveInPrepareHeader);
EXPORT(waveInReset, winmm.waveInReset);
EXPORT(waveInStart, winmm.waveInStart);
EXPORT(waveInStop, winmm.waveInStop);
EXPORT(waveInUnprepareHeader, winmm.waveInUnprepareHeader);
EXPORT(waveOutBreakLoop, winmm.waveOutBreakLoop);
EXPORT(waveOutClose, winmm.waveOutClose);
EXPORT(waveOutGetDevCapsA, winmm.waveOutGetDevCapsA);
EXPORT(waveOutGetDevCapsW, winmm.waveOutGetDevCapsW);
EXPORT(waveOutGetErrorTextA, winmm.waveOutGetErrorTextA);
EXPORT(waveOutGetErrorTextW, winmm.waveOutGetErrorTextW);
EXPORT(waveOutGetID, winmm.waveOutGetID);
EXPORT(waveOutGetNumDevs, winmm.waveOutGetNumDevs);
EXPORT(waveOutGetPitch, winmm.waveOutGetPitch);
EXPORT(waveOutGetPlaybackRate, winmm.waveOutGetPlaybackRate);
EXPORT(waveOutGetPosition, winmm.waveOutGetPosition);
EXPORT(waveOutGetVolume, winmm.waveOutGetVolume);
EXPORT(waveOutMessage, winmm.waveOutMessage);
EXPORT(waveOutOpen, winmm.waveOutOpen);
EXPORT(waveOutPause, winmm.waveOutPause);
EXPORT(waveOutPrepareHeader, winmm.waveOutPrepareHeader);
EXPORT(waveOutReset, winmm.waveOutReset);
EXPORT(waveOutRestart, winmm.waveOutRestart);
EXPORT(waveOutSetPitch, winmm.waveOutSetPitch);
EXPORT(waveOutSetPlaybackRate, winmm.waveOutSetPlaybackRate);
EXPORT(waveOutSetVolume, winmm.waveOutSetVolume);
EXPORT(waveOutUnprepareHeader, winmm.waveOutUnprepareHeader);
EXPORT(waveOutWrite, winmm.waveOutWrite);

#endif
