/**
 * @file Cosa/Canvas/OffScreen.hh
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2013, Mikael Patel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 *
 * @section Description
 * Off-screen canvas for drawing before copying to the canvas device.
 * Supports only monochrome, 1-bit, pixel in off-screen buffer.
 * 
 * This file is part of the Arduino Che Cosa project.
 */

#ifndef __COSA_CANVAS_OFFSCREEN_HH__
#define __COSA_CANVAS_OFFSCREEN_HH__

#include "Cosa/Canvas.hh"

class OffScreen : public Canvas {
private:
  uint16_t m_count;		// Size of bitmap (bytes)
  uint8_t *m_bitmap;		// Pointer to bitmap buffer
  uint8_t m_free;		// Allocation flag

public:
  /**
   * Construct off-screen canvas with given width, height and depth.
   * A buffer may be given but must be able to hold the bitmap size.
   * @param[in] width of canvas.
   * @param[in] height of canvas.
   * @param[in] buffer for canvas.
   */
  OffScreen(uint8_t width, uint8_t height, uint8_t* buffer = 0) :
    Canvas(width, height),
    m_count((width * height) / CHARBITS),
    m_bitmap(buffer != 0 ? buffer : (uint8_t*) malloc(m_count)),
    m_free(buffer == 0)
  {
  }

  /**
   * Destruct off-screen canvas. Check if buffer should be reclaimed.
   */
  ~OffScreen()
  {
    if (m_free) free(m_bitmap);
  }

  /**
   * Get bitmap for the off-screen canvas.
   * @return bitmap pointer.
   */
  uint8_t* get_bitmap()
  {
    return (m_bitmap);
  }

  /**
   * @override
   * Start interaction with off-screen canvas. Clear buffer.
   * @return true(1) if successful otherwise false(0)
   */
  virtual bool begin();

  /**
   * @override
   * Set pixel according to the current pen color.
   * @param[in] x.
   * @param[in] y.
   */
  virtual void draw_pixel(uint8_t x, uint8_t y)
  {
    uint8_t* bp = &m_bitmap[((y >> 3) * WIDTH) + x];
    uint8_t pos = (y & 0x07); 
    if (get_pen_color() == 0)
      *bp |= (1 << pos);
    else
      *bp &= ~(1 << pos);
  } 

  /**
   * @override
   * Fill rectangle with current color.
   * @param[in] x 
   * @param[in] y
   * @param[in] width
   * @param[in] height
   */
  virtual void fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
  
  /**
   * @override
   * Stop sequence of interaction with off-screen device.
   * @return true(1) if successful otherwise false(0)
   */
  virtual bool end();
};

#endif