/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class for managing surfaces and their states.
 *
 * @copyright Horizon is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "surfacemonitor.h"
#include "pal.h"

#ifndef GAME_DLL
void (*MiscFocusLoss)() = nullptr;
void (*MiscFocusRestore)() = nullptr;
SurfaceMonitorClass g_allSurfaces;
#endif

SurfaceMonitorClass::SurfaceMonitorClass()
{
#ifdef BUILD_WITH_DDRAW
    for (int i = 0; i < SURFACE_COUNT; ++i) {
        m_surface[i] = nullptr;
    }
#endif
    m_inFocus = false;
    m_surfacesRestored = false;
}

SurfaceMonitorClass::~SurfaceMonitorClass()
{
    Release();
}

#ifdef BUILD_WITH_DDRAW
void SurfaceMonitorClass::Add_Surface(LPDIRECTDRAWSURFACE new_surface)
{
    if (!Got_Surface_Already(new_surface)) {
        for (int i = 0; i < ARRAY_SIZE(m_surface); ++i) {
            if (m_surface[i] == nullptr) {
                m_surface[i] = new_surface;
                return;
            }
        }
    }
}

void SurfaceMonitorClass::Remove_Surface(LPDIRECTDRAWSURFACE old_surface)
{
    for (int i = 0; i < ARRAY_SIZE(m_surface); ++i) {
        if (m_surface[i] == old_surface) {
            m_surface[i] = nullptr;
            return;
        }
    }
}

bool SurfaceMonitorClass::Got_Surface_Already(LPDIRECTDRAWSURFACE test_surface)
{
    for (int i = 0; i < ARRAY_SIZE(m_surface); ++i) {
        if (m_surface[i] == test_surface) {
            return true;
        }
    }
    return false;
}
#endif

void SurfaceMonitorClass::Restore_Surfaces()
{
    if (m_inFocus) {
#ifdef BUILD_WITH_DDRAW
        for (int i = 0; i < SURFACE_COUNT; ++i) {
            if (m_surface[i] && m_surface[i]->Restore()) {
                if (MiscFocusLoss != nullptr) {
                    MiscFocusLoss();
                }
                return;
            }
        }
#endif
        if (MiscFocusRestore != nullptr) {
            MiscFocusRestore();
        }
        m_surfacesRestored = true;
        Set_Palette(g_currentPalette);
    }
}

void SurfaceMonitorClass::Set_Surface_Focus(bool in_focus)
{
    m_inFocus = in_focus;
}

void SurfaceMonitorClass::Release()
{
#ifdef BUILD_WITH_DDRAW
    for (int i = 0; i < SURFACE_COUNT; ++i) {
        if (m_surface[i]) {
            m_surface[i]->Release();
            m_surface[i] = nullptr;
        }
    }
#endif
}
