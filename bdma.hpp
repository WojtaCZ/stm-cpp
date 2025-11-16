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

#ifndef STMCPP_BDMA_H
#define STMCPP_BDMA_H

#include <cstdint>
#include <cstddef>
#include <stmcpp/register.hpp>
#include <stmcpp/stmheader.hpp>

namespace stmcpp::bdma{
    using namespace stmcpp;

    enum class peripheral : std::uint32_t {
        bdma = BDMA_BASE
    };

    enum class channel : std::uint32_t {
        channel0 = 0x008UL,
        channel1 = 0x01CUL,
        channel2 = 0x030UL,
        channel3 = 0x044UL,
        channel4 = 0x058UL,
        channel5 = 0x06CUL,
        channel6 = 0x080UL,
        channel7 = 0x094UL,
    };

    enum class priority : std::uint8_t {
        low = 0b00,
        medium = 0b01,
        high = 0b10,
        veryHigh = 0b11
    };

    enum class mode : std::uint8_t {
        periph2mem = 0b0,
        mem2mem = 0b1,
    };

    enum class dataSize : std::uint8_t {
        byte = 0b00,
        halfWord = 0b01,
        word = 0b10
    };

    enum class pincOffset : std::uint8_t {
        psize = 0b0,
        word = 0b1,
    };

    enum class targetMem : std::uint8_t {
        mem0 = 0b0,
        mem1 = 0b1,
    };

    enum class interrupt : std::uint8_t {
        global                  = 0x01, 
        transferComplete        = 0x02,
        transferHalfComplete    = 0x04,
        transferError           = 0x08
    };


    template<peripheral Peripheral, channel Channel>
    class bdma {
        private:
            BDMA_Channel_TypeDef * const channelHandle_ = reinterpret_cast<BDMA_Channel_TypeDef *>(static_cast<std::uint32_t>(Peripheral) + static_cast<std::uint32_t>(Channel));
            BDMA_TypeDef * const bdmaHandle_ = reinterpret_cast<BDMA_TypeDef *>(static_cast<std::uint32_t>(Peripheral));
        public:
            bdma(mode mode, dataSize psize, bool pincrement, std::uint32_t paddress, dataSize msize, bool mincrement, uint32_t m0address, uint32_t m1address, uint16_t numofdata,
                 priority priority = priority::low, bool circular = false, pincOffset pincoffset = pincOffset::psize, bool doublebuffer = false
                ) {
                reg::write(std::ref(channelHandle_->CCR),
                    //Interrupts are not enabled here and the channel is not yet being enabled
                    ((static_cast<uint8_t>(mode) & 0b01) << BDMA_CCR_DIR_Pos) |
                    ((static_cast<uint8_t>(circular) & 0b1) << BDMA_CCR_CIRC_Pos) |
                    ((static_cast<uint8_t>(pincrement) & 0b1) << BDMA_CCR_PINC_Pos) |
                    ((static_cast<uint8_t>(mincrement) & 0b1) << BDMA_CCR_MINC_Pos) |
                    ((static_cast<uint8_t>(psize) & 0b11) << BDMA_CCR_PSIZE_Pos) | 
                    ((static_cast<uint8_t>(msize) & 0b11) << BDMA_CCR_MSIZE_Pos) | 
                    ((static_cast<uint8_t>(priority) & 0b11) << BDMA_CCR_PL_Pos) | 
                    ((static_cast<uint8_t>(mode) & 0b1) << BDMA_CCR_MEM2MEM_Pos) | 
                    ((static_cast<uint8_t>(doublebuffer) & 0b1) << BDMA_CCR_DBM_Pos) 
                    //Current target is not set here
                );

                reg::write(std::ref(channelHandle_->CPAR), paddress);
                reg::write(std::ref(channelHandle_->CM0AR), m0address);
                reg::write(std::ref(channelHandle_->CM1AR), m1address);
                reg::write(std::ref(channelHandle_->CNDTR), numofdata);
            }

            void enable() const {
                reg::set(std::ref(channelHandle_->CCR), BDMA_CCR_EN);
            }

            void disable() const {
                reg::clear(std::ref(channelHandle_->CCR), BDMA_CCR_EN);
            }

            void setTargetMemory(targetMem memory) const {
                reg::change(std::ref(channelHandle_->CCR), 0x01, static_cast<std::uint8_t>(memory), BDMA_CCR_CT_Pos);
            }

            void setNumberOfData(uint16_t numofdata) const {
                reg::write(std::ref(channelHandle_->CNDTR), numofdata);
            }


            void enableInterrupt(std::vector<interrupt> interrupts) const {

                std::uint8_t mask_ = 0;

                for (auto i : interrupts) {
                    //Merge the interrupts into single mask
                    switch (i) {
                        case interrupt::transferHalfComplete:
                            mask_ |= BDMA_CCR_HTIE;
                            break;
                        case interrupt::transferComplete:
                            mask_ |= BDMA_CCR_TCIE;
                            break;
                        case interrupt::transferError:
                            mask_ |= BDMA_CCR_TEIE;
                        default:
                            break;
                    }
                }
                
                //Apply the mask to the control register
                reg::set(std::ref(channelHandle_->CCR), mask_);
            }

            void enableInterrupt(interrupt interrupt) const {
                switch (interrupt) {
                    case interrupt::transferHalfComplete:
                        reg::set(std::ref(channelHandle_->CCR), BDMA_CCR_HTIE);
                        break;
                    case interrupt::transferComplete:
                        reg::set(std::ref(channelHandle_->CCR), BDMA_CCR_TCIE);
                        break;
                    case interrupt::transferError:
                        reg::set(std::ref(channelHandle_->CCR), BDMA_CCR_TEIE); 
                    default:
                        break;
                }
            }

            void disableInterrupt(std::vector<interrupt> interrupts) const {

                std::uint8_t mask_ = 0;

                for (auto i : interrupts) {
                    //Merge the interrupts into single mask
                    switch (i) {
                        case interrupt::transferHalfComplete:
                            mask_ |= BDMA_CCR_HTIE;
                            break;
                        case interrupt::transferComplete:
                            mask_ |= BDMA_CCR_TCIE;
                            break;
                        case interrupt::transferError:
                            mask_ |= BDMA_CCR_TEIE;
                        default:
                            break;
                    }   
                }
                
                //Apply the mask to the control register
                reg::set(std::ref(channelHandle_->CCR), mask_);
            }

            void disableInterrupt(interrupt interrupt) const {
                switch (interrupt) {
                    case interrupt::transferHalfComplete:
                        reg::clear(std::ref(channelHandle_->CCR), BDMA_CCR_HTIE);
                        break;
                    case interrupt::transferComplete:
                        reg::clear(std::ref(channelHandle_->CCR), BDMA_CCR_TCIE);
                        break;
                    case interrupt::transferError:
                        reg::clear(std::ref(channelHandle_->CCR), BDMA_CCR_TEIE); 
                    default:
                        break;
                }
            }

            void clearInterruptFlag(interrupt interrupt) const {
                switch (Channel) {
                    case channel::channel0:
                        reg::set(std::ref(bdmaHandle_->IFCR), static_cast<uint32_t>(interrupt), 0);
                    case channel::channel1:
                        reg::set(std::ref(bdmaHandle_->IFCR), static_cast<uint32_t>(interrupt), 4);
                    case channel::channel2:
                        reg::set(std::ref(bdmaHandle_->IFCR), static_cast<uint32_t>(interrupt), 8);
                    case channel::channel3:
                        reg::set(std::ref(bdmaHandle_->IFCR), static_cast<uint32_t>(interrupt), 12);
                    case channel::channel4:
                        reg::set(std::ref(bdmaHandle_->IFCR), static_cast<uint32_t>(interrupt), 16);
                    case channel::channel5:
                        reg::set(std::ref(bdmaHandle_->IFCR), static_cast<uint32_t>(interrupt), 20);
                    case channel::channel6:
                        reg::set(std::ref(bdmaHandle_->IFCR), static_cast<uint32_t>(interrupt), 24);
                    case channel::channel7:
                        reg::set(std::ref(bdmaHandle_->IFCR), static_cast<uint32_t>(interrupt), 28);
                    default:
                        break;
                }
            }

            bool getInterruptFlag(interrupt interrupt) const {
                switch (Channel) {
                    case channel::channel0:
                        return static_cast<bool>(reg::read(std::ref(bdmaHandle_->ISR), static_cast<uint32_t>(interrupt), 0));
                    case channel::channel1:
                        return static_cast<bool>(reg::read(std::ref(bdmaHandle_->ISR), static_cast<uint32_t>(interrupt), 4));
                    case channel::channel2:
                        return static_cast<bool>(reg::read(std::ref(bdmaHandle_->ISR), static_cast<uint32_t>(interrupt), 8));
                    case channel::channel3:
                        return static_cast<bool>(reg::read(std::ref(bdmaHandle_->ISR), static_cast<uint32_t>(interrupt), 12));
                    case channel::channel4:
                        return static_cast<bool>(reg::read(std::ref(bdmaHandle_->ISR), static_cast<uint32_t>(interrupt), 16));
                    case channel::channel5:
                        return static_cast<bool>(reg::read(std::ref(bdmaHandle_->ISR), static_cast<uint32_t>(interrupt), 20));
                    case channel::channel6:
                        return static_cast<bool>(reg::read(std::ref(bdmaHandle_->ISR), static_cast<uint32_t>(interrupt), 24));
                    case channel::channel7:
                        return static_cast<bool>(reg::read(std::ref(bdmaHandle_->ISR), static_cast<uint32_t>(interrupt), 28));
                    default:
                        break;
                }
            }
    };
}



#endif
