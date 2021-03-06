// LAF OS Library
// Copyright (C) 2012-2017  David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef OS_SKIA_SKIA_SYSTEM_INCLUDED
#define OS_SKIA_SKIA_SYSTEM_INCLUDED
#pragma once

#include "base/base.h"
#include "gfx/size.h"
#include "os/common/system.h"
#include "os/skia/skia_display.h"
#include "os/skia/skia_surface.h"

#ifdef _WIN32
  #include "os/win/system.h"
  #define SkiaSystemBase WindowSystem
#elif __APPLE__
  #include "os/osx/system.h"
  #define SkiaSystemBase OSXSystem
#else
  #include "os/x11/system.h"
  #define SkiaSystemBase X11System
#endif

#include "SkGraphics.h"

namespace os {

class SkiaSystem : public SkiaSystemBase {
public:
  SkiaSystem()
    : m_defaultDisplay(nullptr)
    , m_gpuAcceleration(false) {
    SkGraphics::Init();
  }

  ~SkiaSystem() {
    SkGraphics::Term();
  }

  Capabilities capabilities() const override {
    return Capabilities(
      int(Capabilities::MultipleDisplays) |
      int(Capabilities::CanResizeDisplay) |
      int(Capabilities::DisplayScale) |
      int(Capabilities::CustomNativeMouseCursor)
    // TODO enable this when the GPU support is ready
#if 0 // SK_SUPPORT_GPU
      | int(Capabilities::GpuAccelerationSwitch)
#endif
      );
  }

  bool gpuAcceleration() const override {
    return m_gpuAcceleration;
  }

  void setGpuAcceleration(bool state) override {
    m_gpuAcceleration = state;
  }

  gfx::Size defaultNewDisplaySize() override {
    gfx::Size sz;
#ifdef _WIN32
    sz.w = GetSystemMetrics(SM_CXMAXIMIZED);
    sz.h = GetSystemMetrics(SM_CYMAXIMIZED);
    sz.w -= GetSystemMetrics(SM_CXSIZEFRAME)*4;
    sz.h -= GetSystemMetrics(SM_CYSIZEFRAME)*4;
    sz.w = MAX(0, sz.w);
    sz.h = MAX(0, sz.h);
#endif
    return sz;
  }

  Display* defaultDisplay() override {
    return m_defaultDisplay;
  }

  Display* createDisplay(int width, int height, int scale) override {
    SkiaDisplay* display = new SkiaDisplay(width, height, scale);
    if (!m_defaultDisplay)
      m_defaultDisplay = display;
    return display;
  }

  Surface* createSurface(int width, int height) override {
    SkiaSurface* sur = new SkiaSurface;
    sur->create(width, height);
    return sur;
  }

  Surface* createRgbaSurface(int width, int height) override {
    SkiaSurface* sur = new SkiaSurface;
    sur->createRgba(width, height);
    return sur;
  }

  Surface* loadSurface(const char* filename) override {
    return SkiaSurface::loadSurface(filename);
  }

  Surface* loadRgbaSurface(const char* filename) override {
    return loadSurface(filename);
  }

  void setTranslateDeadKeys(bool state) override {
    if (m_defaultDisplay)
      m_defaultDisplay->setTranslateDeadKeys(state);
  }

private:
  SkiaDisplay* m_defaultDisplay;
  bool m_gpuAcceleration;
};

} // namespace os

#endif
