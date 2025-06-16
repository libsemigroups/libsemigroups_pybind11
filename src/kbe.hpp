//
// libsemigroups_pybind11
// Copyright (C) 2025 James D. Mitchell
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

#ifndef SRC_KBE_HPP_
#define SRC_KBE_HPP_

namespace libsemigroups {
  template <typename FroidurePinType>
  struct ElementStateful {
    using element_type = typename FroidurePinType::element_type;
    using state_type   = typename FroidurePinType::state_type;

    ElementStateful()                                  = default;
    ElementStateful(ElementStateful const&)            = default;
    ElementStateful(ElementStateful&&)                 = default;
    ElementStateful& operator=(ElementStateful const&) = default;
    ElementStateful& operator=(ElementStateful&&)      = default;

    ElementStateful(element_type const& lmnt, state_type* sttptr)
        : element(lmnt), state_ptr(sttptr) {}

    element_type element;
    state_type*  state_ptr;
  };

}  // namespace libsemigroups
#endif  // SRC_KBE_HPP_
