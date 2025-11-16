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

#ifndef STMCPP_DAC_H
#define STMCPP_DAC_H

#include <stmcpp/stmheader.hpp>
#include <cstdint>

#include <stmcpp/register.hpp>
#include <stmcpp/units.hpp>
#include <stmcpp/clock.hpp>
#include <stmcpp/error.hpp>


namespace stmcpp::dac {
    using namespace stmcpp;
    using namespace stmcpp::units;

    enum class error {
        timeout
    };

    static stmcpp::error::handler<error, "stmcpp::dac"> errorHandler;
    
    enum class channel {
        ch1,
        ch2
    };

    enum class trigger {
        software = 0b0000,
        trig1    = 0b0001,
        trig2    = 0b0010,
        trig3    = 0b0011,
        trig4    = 0b0100,
        trig5    = 0b0101,
        trig6    = 0b0110,
        trig7    = 0b0100,
        trig8    = 0b0100,
        trig9    = 0b0100,
        trig10   = 0b0100,
        trig11   = 0b0100,
        trig12   = 0b0100,
        trig13   = 0b0100,
        trig14   = 0b0100,
        trig15   = 0b0100,
    };

    enum class waveGeneration {
        noWave      = 0b00,
        noise       = 0b01,
        triangle    = 0b10
    };

    enum class waveAmplitude {
        bit1  = 0b0000,
        bit2  = 0b0001,
        bit3  = 0b0010,
        bit4  = 0b0011,
        bit5  = 0b0100,
        bit6  = 0b0101,
        bit7  = 0b0110,
        bit8  = 0b0111,
        bit9  = 0b1000,
        bit10 = 0b1001,
        bit11 = 0b1010,
        bit12 = 0b1011      
    };

    enum class bufferMode {
        externalBuffered = 0b000,
        externalInternalBuffered = 0b001,
        externalUnbuffered = 0b010,
        internalUnuffered = 0b011
    };

    enum class mode {
        normal = 0b0000,
        sampleAndHold = 0b1000
    };

    enum class status {
        busy = DAC_SR_BWST1,
        calibration = DAC_SR_CAL_FLAG1,
        underrun = DAC_SR_DMAUDR1
    };

    template<channel Channel>
    class dac {
        public:
            dac(bufferMode bufferMode = bufferMode::externalBuffered, mode mode = mode::normal, waveGeneration generation = waveGeneration::noWave, waveAmplitude amplitude = waveAmplitude::bit1, bool triggerEnable = false, trigger trigger = trigger::software) {
                
                if constexpr (Channel == channel::ch1) { 
                    reg::write(std::ref(DAC1->CR), 
                        (static_cast<uint32_t>(triggerEnable) << DAC_CR_TEN1_Pos) | 
                        (static_cast<uint32_t>(trigger) << DAC_CR_TSEL1_Pos) | 
                        (static_cast<uint32_t>(generation) << DAC_CR_WAVE1_Pos) | 
                        (static_cast<uint32_t>(amplitude) << DAC_CR_MAMP1_Pos)
                    );
                } else {
                    reg::write(std::ref(DAC1->CR), 
                        (static_cast<uint32_t>(triggerEnable) << DAC_CR_TEN2_Pos) | 
                        (static_cast<uint32_t>(trigger) << DAC_CR_TSEL2_Pos) | 
                        (static_cast<uint32_t>(generation) << DAC_CR_WAVE2_Pos) | 
                        (static_cast<uint32_t>(amplitude) << DAC_CR_MAMP2_Pos)
                    );
                }

                if constexpr (Channel == channel::ch1) {
                    reg::change(std::ref(DAC1->MCR), 0b111, static_cast<uint8_t>(bufferMode) | static_cast<uint8_t>(mode), DAC_MCR_MODE1_Pos);
                } else reg::change(std::ref(DAC1->MCR), 0b111, static_cast<uint8_t>(bufferMode) | static_cast<uint8_t>(mode), DAC_MCR_MODE2_Pos);
            }
        
            void enable() {
                if constexpr (Channel == channel::ch1) {
                    reg::set(std::ref(DAC1->CR), DAC_CR_EN1);
                } else reg::set(std::ref(DAC1->CR), DAC_CR_EN2);
            }

            void disable() {
                if constexpr (Channel == channel::ch1) {
                    reg::clear(std::ref(DAC1->CR), DAC_CR_EN1);
                } else reg::clear(std::ref(DAC1->CR), DAC_CR_EN2);
            }

            void enableUnderrunInterrupt() {
                if constexpr (Channel == channel::ch1) {
                    reg::set(std::ref(DAC1->CR), DAC_CR_DMAUDRIE1);
                } else reg::set(std::ref(DAC1->CR), DAC_CR_DMAUDRIE2);
            }

            void disableUnderrunInterrupt() {
                if constexpr (Channel == channel::ch1) {
                    reg::clear(std::ref(DAC1->CR), DAC_CR_DMAUDRIE1);
                } else reg::clear(std::ref(DAC1->CR), DAC_CR_DMAUDRIE2);
            }

            void trigger() const {
                if constexpr (Channel == channel::ch1) {
                    reg::clear(std::ref(DAC1->SWTRIGR), DAC_SWTRIGR_SWTRIG1);
                } else reg::clear(std::ref(DAC1->SWTRIGR), DAC_SWTRIGR_SWTRIG2);
            }

            void setValue(uint16_t data) {
                if constexpr (Channel == channel::ch1) {
                    reg::write(std::ref(DAC1->DHR12R1), data);
                } else reg::write(std::ref(DAC1->DHR12R2), data);
            }

            uint16_t getOutput() {
                if constexpr (Channel == channel::ch1) {
                    return reg::read(std::ref(DAC1->DOR1)) & 0x0FFF;
                } else return reg::read(std::ref(DAC1->DOR2)) & 0x0FFF;
            }

            bool getStatusFlag(status flag) {
                if constexpr (Channel == channel::ch1) {
                    return static_cast<bool>(reg::read(std::ref(DAC1->SR), static_cast<std::uint32_t>(flag)));
                } else return static_cast<bool>(reg::read(std::ref(DAC1->SR), static_cast<std::uint32_t>(flag) << 16));
            }

    };
}

#endif