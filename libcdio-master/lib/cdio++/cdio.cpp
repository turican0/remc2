/* -*- C++ -*-
  $Id: cdio.cpp,v 1.2 2008/04/20 13:44:31 karl Exp $

  Copyright (C) 2006, 2008 Rocky Bernstein <rocky@gnu.org>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <sys/types.h>
#include <cdio++/cdio.hpp>

void possible_throw_device_exception(driver_return_code_t drc) 
  {
    switch (drc) {
    case DRIVER_OP_SUCCESS:
      return;
    case DRIVER_OP_ERROR:
      throw DriverOpError();
    case DRIVER_OP_UNSUPPORTED:
      throw DriverOpUnsupported();
    case DRIVER_OP_UNINIT:
      throw DriverOpUninit();
    case DRIVER_OP_NOT_PERMITTED:
      throw DriverOpNotPermitted();
    case DRIVER_OP_BAD_PARAMETER:
      throw DriverOpBadParameter();
    case DRIVER_OP_BAD_POINTER:
      throw DriverOpBadPointer();
    case DRIVER_OP_NO_DRIVER:
      throw DriverOpNoDriver();
    default:
      throw DriverOpException(drc);
    }
  }
