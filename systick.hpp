
#ifndef STMCPP_SYSTICK_H
#define STMCPP_SYSTICK_H

#include <cstdint>
#include <tuple>
#include <array>

#include <stmcpp/register.hpp>
#include <stmcpp/units.hpp>
#include <stmcpp/error.hpp>

#include <stmcpp/stmheader.hpp>
#include "stmcpp-config.hpp"


namespace stmcpp::systick {
    using namespace stmcpp::units;


    enum class error {
        systick_used_uninitialized,
    };

    static stmcpp::error::handler<error, "stmcpp::systick"> errorHandler;


    inline volatile /*static*/ std::uint32_t ticks_ = 0;
    inline /*static*/ duration resolution_ = 1_ms;
    inline /*volatile static*/ bool initialized_ = false; 
    
    static void enable(frequency sysclock, duration resolution = 1_ms) {
        resolution_ = resolution;

        auto reloadVal_ = ((sysclock.toHertz() / resolution_.freq().toHertz()) - 1);
            
        //Zero out the counter
        reg::write(std::ref(SysTick->VAL), 0);

        //Load the reload value
        reg::write(std::ref(SysTick->LOAD), reloadVal_);

        //Start the counter
        reg::set(std::ref(SysTick->CTRL),
                0b1 << SysTick_CTRL_CLKSOURCE_Pos |
                0b1 << SysTick_CTRL_TICKINT_Pos |
                0b1 << SysTick_CTRL_ENABLE_Pos 
        );

        initialized_ = true;
    }

    static void disable() {
        reg::write(std::ref(SysTick->CTRL), 0);
        initialized_ = false;
    }

    static void reconfigure(frequency sysclock, duration resolution = 1_ms){
        disable();
        enable(sysclock, resolution);
    } 

    static inline std::uint32_t getTicks() {
        return ticks_;
    }

    static inline duration getDuration() {
        return resolution_ * ticks_;
    }

    static void waitBlocking(duration time) {
        if(!initialized_) errorHandler.hardThrow(error::systick_used_uninitialized);
        duration timestamp_ = resolution_ * ticks_;
        while(getDuration() < (timestamp_ + time)){;}
    }

    static inline void increment() {
        ++ticks_;
    } 
}   

#endif