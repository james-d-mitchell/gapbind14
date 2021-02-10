//
// gapbind14 for GAP
// Copyright (C) 2020 James D. Mitchell
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//

// Include gmp.h *before* switching to C mode, because GMP detects when
// compiled from C++ and then does some things differently, which would
// cause an error if called from within extern "C". But libsing.h
// (indirectly) includes gmp.h ...

#ifndef INCLUDE_GAPBIND14_GAP_INCLUDE_HPP_
#define INCLUDE_GAPBIND14_GAP_INCLUDE_HPP_

#include <gmp.h>

extern "C" {
#include "compiled.h"
}

#endif  // INCLUDE_GAPBIND14_GAP_INCLUDE_HPP_
