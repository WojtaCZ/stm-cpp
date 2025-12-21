#ifndef STMCPP_REGISTER_WAITS_H
#define STMCPP_REGISTER_WAITS_H

#include <stmcpp/register.hpp>
#include <stmcpp/systick.hpp>
#include <functional>

namespace stmcpp::reg {

    template <typename A = regbase, typename M, typename V>
    constexpr void waitForBitsEqual(std::reference_wrapper<A> address, M mask, V value, std::function<void()> onTimeout = nullptr, stmcpp::units::duration timeout = 1000_ms) {
        // Now valid because systick.hpp is included above
        auto timestamp_ = stmcpp::systick::getDuration();
        
        while (stmcpp::systick::getDuration() < (timestamp_ + timeout)) {
            if(read(address, static_cast<regbase>(mask)) == static_cast<regbase>(value)) { 
                return;
            }
        }
        if(onTimeout){
            onTimeout();
        }
    }

    template <typename A = regbase, typename M>
    constexpr void waitForBitSet(std::reference_wrapper<A> address, M mask, std::function<void()> onTimeout = nullptr, stmcpp::units::duration timeout = 1000_ms) {
        waitForBitsEqual(address, mask, mask, onTimeout, timeout);
    }

    template <typename A = regbase, typename M>
    constexpr void waitForBitClear(std::reference_wrapper<A> address, M mask, std::function<void()> onTimeout = nullptr, stmcpp::units::duration timeout = 1000_ms) {
        waitForBitsEqual(address, mask, 0, onTimeout, timeout);
    }

}

#endif