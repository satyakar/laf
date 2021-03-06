// LAF OS Library
// Copyright (C) 2018  David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "os/skia/resize_surface.h"

namespace os {

ResizeSurface::ResizeSurface()
  : m_snapshot(nullptr)
{
}

ResizeSurface::~ResizeSurface()
{
  reset();
}

void ResizeSurface::create(Display* display)
{
  if (m_snapshot) {
    m_snapshot->dispose();
    m_snapshot = nullptr;
  }

  const SkiaSurface* surface =
    static_cast<SkiaSurface*>(display->getSurface());

  m_snapshot = new SkiaSurface;
  m_snapshot->create(surface->width(),
                     surface->height());
  m_snapshot->drawSurface(surface, 0, 0);
}

void ResizeSurface::reset()
{
  if (!m_snapshot)
    return;
  m_snapshot->dispose();
  m_snapshot = nullptr;
}

void ResizeSurface::draw(Display* display)
{
  if (!m_snapshot)
    return;

  SkiaSurface* surface =
    static_cast<SkiaSurface*>(display->getSurface());
  surface->drawSurface(
    m_snapshot,
    gfx::Rect(0, 0, m_snapshot->width(), m_snapshot->height()),
    gfx::Rect(0, 0, surface->width(), surface->height()));
}

} // namespace os
