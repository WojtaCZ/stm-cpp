/* 
 * This file is part of the stmcpp distribution (https://github.com/WojtaCZ/stmcpp).
 * Copyright (c) 2024 Vojtech Vosahlo.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STMCPP_REGISTER_H
#define STMCPP_REGISTER_H

#include <cstdint>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <functional>

#include <stmcpp/units.hpp>

/*
 * The following file contains the helper function templates
 * used for generic register access throughout the rest of this project.
 */



namespace stmcpp::reg {
    using namespace stmcpp::units;

    //Use 32 bits as a register size
    using regbase = std::uint32_t;

    template <typename A = regbase, typename V>
    constexpr void write(std::reference_wrapper<A> address, V value, unsigned int bitshift = 0) {
        address.get() = (value << bitshift); 
    }

    template <typename A = regbase>
    [[nodiscard]]
    constexpr A read(std::reference_wrapper<A> address) {
        return address.get();
    }

    template <typename A = regbase, typename M>
    [[nodiscard]]
    constexpr A read(std::reference_wrapper<A> address, M mask, unsigned int bitshift = 0) {
        return (address.get() & (mask << bitshift)) >> bitshift;
    }

    template <typename A = regbase, typename M>
    constexpr void change(std::reference_wrapper<A> address, M mask, typename std::type_identity<M>::type value, unsigned int bitshift = 0) {
        address.get() = (address.get() & ~(mask << bitshift)) | ((value & mask) << bitshift);
    }

    template <typename A = regbase, typename M>
    constexpr void set(std::reference_wrapper<A> address, M mask, unsigned int bitshift = 0) {
        address.get() |= (mask << bitshift);
    }

    template <typename A = regbase, typename M>
    constexpr void clear(std::reference_wrapper<A> address, M mask, unsigned int bitshift = 0) {
        address.get() &= ~(mask << bitshift);
    }

    template <typename A = regbase, typename M>
    constexpr void toggle(std::reference_wrapper<A> address, M mask, unsigned int bitshift = 0) {
        address.get() ^= (mask << bitshift);
    }

    /*template <typename A = regbase, typename M>
    constexpr void waitForBitState(std::reference_wrapper<A> address, M mask, bool state, std::function<void()> onTimeout = nullptr, duration timeout = 1000_ms) {
        duration timestamp_ = stmcpp::clock::systick::getDuration();
        
        while (stmcpp::clock::systick::getDuration() < (timestamp_ + timeout)) {
            if(static_cast<bool>(read(address, static_cast<regbase>(mask))) == state) { 
                return;
            }
        }
        // Call the timeout handler if timeout occured
        if(onTimeout){
            onTimeout();
        }
    }*/

}

#endif
