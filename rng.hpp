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

#ifndef STMCPP_RNG_H
#define STMCPP_RNG_H

#include <stmcpp/stmheader.hpp>
#include <cstdint>

#include <stmcpp/register.hpp>
#include <stmcpp/clock.hpp>
#include <stmcpp/error.hpp>


namespace stmcpp::rng {
    using namespace stmcpp;
    using namespace stmcpp::units;

    enum class error {
        timeout
    };

    static stmcpp::error::handler<error, "stmcpp::rng"> errorHandler;

    enum class status {
        seedErrorInterrupt = RNG_SR_SEIS,
        clockErrorInterrupt = RNG_SR_CEIS,
        seedError = RNG_SR_SECS,
        clockError = RNG_SR_CECS,
        dataReady = RNG_SR_DRDY
    };

    static void enableInterrupt() {
        reg::set(std::ref(RNG->CR), RNG_CR_IE);
    }

    static void disableInterrupt() {
        reg::clear(std::ref(RNG->CR), RNG_CR_IE);
    }

    static void enableClockErrorDetection() {
        reg::set(std::ref(RNG->CR), RNG_CR_CED);
    }

    static void disableClockErrorDetection() {
        reg::clear(std::ref(RNG->CR), RNG_CR_CED);
    }

    static void enable() {
        reg::set(std::ref(RNG->CR), RNG_CR_RNGEN);
    }

    static void disable() {
        reg::clear(std::ref(RNG->CR), RNG_CR_RNGEN);
    }

    static uint32_t getData() {
        // Wait for the data to be ready
        stmcpp::reg::waitForBitSet(std::ref(RNG->SR), RNG_SR_DRDY, []() { errorHandler.hardThrow(error::timeout); });

        return reg::read(std::ref(RNG->DR));
    }

    static bool getStatusFlag(status flag) {
        return static_cast<bool>(reg::read(std::ref(RNG->SR), static_cast<std::uint32_t>(flag)));
    }
    
}

#endif