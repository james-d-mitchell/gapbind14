//
// gapbind14 package for GAP
// Copyright (C) 2018 James D. Mitchell
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

#ifndef GAPBIND14_SRC_TO_GAP_HPP_
#define GAPBIND14_SRC_TO_GAP_HPP_

#include <iostream>  // for debugging, FIXME remove this
#include <string>    // for string

#include "gap-include.hpp"

namespace gapbind14 {
  using gap_tnum_type            = UInt;

  // For TCppType -> Obj
  template <typename TCppType, typename = void> struct to_gap;
  // For Obj -> TCppType
  template <typename TCppType, typename = void> struct to_cpp;

  ////////////////////////////////////////////////////////////////////////
  // Strings
  ////////////////////////////////////////////////////////////////////////

  template <typename TCppType>
  struct to_gap<TCppType,
                std::enable_if_t<
                    std::is_same<std::string, std::decay_t<TCppType>>::value>> {
    using cpp_type                          = TCppType;
    static gap_tnum_type constexpr gap_type = T_STRING;

    Obj operator()(TCppType&& str) {
      Obj ret;
      C_NEW_STRING(ret, str.length(), str.c_str());
      return ret;
    }
  };

  template <typename TCppType>
  struct to_cpp<TCppType,
                std::enable_if_t<
                    std::is_same<std::string, std::decay_t<TCppType>>::value>> {
    using cpp_type                          = TCppType;
    static gap_tnum_type constexpr gap_type = T_STRING;

    TCppType operator()(Obj o) {
      if (TNUM_OBJ(o) != T_STRING) {
        ErrorQuit("expected string but got %s!", (Int) TNAM_OBJ(o), 0L);
      }
      return std::string(CSTR_STRING(o), GET_LEN_STRING(o));
    }
  };

  ////////////////////////////////////////////////////////////////////////
  // Integers
  ////////////////////////////////////////////////////////////////////////

  template <typename TCppType>
  struct to_gap<TCppType, std::enable_if_t<std::is_integral<TCppType>::value>> {
    using cpp_type                          = TCppType;
    static gap_tnum_type constexpr gap_type = T_INT;

    Obj operator()(TCppType&& i) {
      return INTOBJ_INT(i);
    }
  };

  template <typename TCppType>
  struct to_cpp<TCppType, std::enable_if_t<std::is_integral<TCppType>::value>> {
    using cpp_type                          = TCppType;
    static gap_tnum_type constexpr gap_type = T_INT;

    Int operator()(Obj o) {
      if (TNUM_OBJ(o) != T_INT) {
        ErrorQuit("expected int but got %s!", (Int) TNAM_OBJ(o), 0L);
      }
      return INT_INTOBJ(o);
    }
  };


}  // namespace gapbind14
#endif  // GAPBIND14_SRC_TO_GAP_HPP_
