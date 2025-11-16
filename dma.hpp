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

#ifndef STMCPP_DMA_H
#define STMCPP_DMA_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <stmcpp/register.hpp>
#include <stmcpp/stmheader.hpp>

namespace stmcpp::dma{
    using namespace stmcpp;

    enum class peripheral : std::uint32_t {
        dma1 = DMA1_BASE,
        dma2 = DMA2_BASE
    };

    enum class stream : std::uint32_t {
        stream0 = 0x010UL,
        stream1 = 0x028UL,
        stream2 = 0x040UL,
        stream3 = 0x058UL,
        stream4 = 0x070UL,
        stream5 = 0x088UL,
        stream6 = 0x0A0UL,
        stream7 = 0x0B8UL,
    };

    enum class priority : std::uint8_t {
        low = 0b00,
        medium = 0b01,
        high = 0b10,
        veryHigh = 0b11
    };

    enum class mode : std::uint8_t {
        periph2mem = 0b00,
        mem2periph = 0b01,
        mem2mem = 0b10
    };

    enum class datasize : std::uint8_t {
        byte = 0b00,
        halfWord = 0b01,
        word = 0b10
    };

    enum class burstsize : std::uint8_t {
        single = 0b00,
        incremental4 = 0b01,
        incremental8 = 0b10,
        incremental16 = 0b11
    };

    enum class flowController : std::uint8_t {
        dma = 0b0,
        peripheral = 0b1
    };

    enum class pincOffset : std::uint8_t {
        psize = 0b0,
        word = 0b1
    };

    enum class targetMem : std::uint8_t {
        mem0 = 0b0,
        mem1 = 0b1
    };

    enum class interrupt : std::uint8_t{
        transferComplete        = 0x20,
        transferHalfComplete    = 0x10,
        transferError           = 0x08,
        directModeError         = 0x04,
        fifoError               = 0x01
    };

    enum class fifoStat : std::uint8_t {
        empty = 0b100,
        quarter = 0b000,
        half = 0b001,
        threeQuarter = 0b010, 
        almostFull = 0b011,
        full = 0b101
    };

    enum class fifoTreshold : std::uint8_t {
        quarter = 0b00,
        half = 0b01,
        threeQuarter = 0b10, 
        full = 0b11
    };

    template<peripheral Peripheral, stream Stream>
    class dma {
        private:
            DMA_Stream_TypeDef * const streamHandle_ = reinterpret_cast<DMA_Stream_TypeDef *>(static_cast<std::uint32_t>(Peripheral) + static_cast<std::uint32_t>(Stream));
            DMA_TypeDef * const dmaHandle_ = reinterpret_cast<DMA_TypeDef *>(static_cast<std::uint32_t>(Peripheral));

        public:
            dma(mode mode, datasize psize, bool pincrement, std::uint32_t paddress, datasize msize, bool mincrement, std::uint32_t m0address, std::uint32_t m1address, std::uint16_t numofdata,
                priority priority = priority::low, bool circular = false, pincOffset pincOffset = pincOffset::psize,
                bool doublebuffer = false, bool bufferedtransfers = false, flowController flowController = flowController::dma,
                burstsize pburst = burstsize::single, burstsize mburst = burstsize::single
                ) {

                reg::write(std::ref(streamHandle_->PAR), paddress);
                reg::write(std::ref(streamHandle_->M0AR), m0address);
                reg::write(std::ref(streamHandle_->M1AR), m1address);
                reg::write(std::ref(streamHandle_->NDTR), numofdata);
            
                reg::write(std::ref(streamHandle_->CR),
                    //Interrupts are not enabled here and the channel is not yet being enabled
                    ((static_cast<std::uint8_t>(flowController) & 0b1) << DMA_SxCR_PFCTRL_Pos) |
                    ((static_cast<std::uint8_t>(mode) & 0b11) << DMA_SxCR_DIR_Pos) | 
                    ((static_cast<std::uint8_t>(circular) & 0b1) << DMA_SxCR_CIRC_Pos) |
                    ((static_cast<std::uint8_t>(pincrement) & 0b1) << DMA_SxCR_PINC_Pos) |
                    ((static_cast<std::uint8_t>(mincrement) & 0b1) << DMA_SxCR_MINC_Pos) |
                    ((static_cast<std::uint8_t>(psize) & 0b11) << DMA_SxCR_PSIZE_Pos) | 
                    ((static_cast<std::uint8_t>(msize) & 0b11) << DMA_SxCR_MSIZE_Pos) | 
                    ((static_cast<std::uint8_t>(pincOffset) & 0b1) << DMA_SxCR_PINC_Pos) |
                    ((static_cast<std::uint8_t>(priority) & 0b11) << DMA_SxCR_PL_Pos) | 
                    ((static_cast<std::uint8_t>(doublebuffer) & 0b1) << DMA_SxCR_DBM_Pos) |
                    //Current target is not set here
                    ((static_cast<std::uint8_t>(bufferedtransfers) & 0b1) << DMA_SxCR_TRBUFF_Pos) |
                    ((static_cast<std::uint8_t>(pburst) & 0b11) << DMA_SxCR_PBURST_Pos) | 
                    ((static_cast<std::uint8_t>(mburst) & 0b11) << DMA_SxCR_MBURST_Pos)  
                );

            }

            void enable() const {
                reg::set(std::ref(streamHandle_->CR), DMA_SxCR_EN);
            }

            void disable() const {
                reg::clear(std::ref(streamHandle_->CR), DMA_SxCR_EN);
            }

            void setTargetMemory(targetMem memory) const {
                reg::change(std::ref(streamHandle_->CR), 0x01, static_cast<std::uint8_t>(memory), DMA_SxCR_CT_Pos);
            }

            uint8_t getTargetMemory() const {
                return reg::read(std::ref(streamHandle_->CR), 0x01, DMA_SxCR_CT_Pos);
            }

            void setNumberOfData(uint16_t numofdata) const {
                reg::write(std::ref(streamHandle_->NDTR), numofdata);
            }

            void enableInterrupt(const std::vector<interrupt> interrupts) const {

                std::uint8_t mask_ = 0;

                for (auto i : interrupts) {
                    if (i == interrupt::fifoError) {
                        //If the fifo interrupt should be enabled, set it right away
                        reg::set(std::ref(streamHandle_->FCR), DMA_SxFCR_FEIE);
                    } else {
                        switch (i) {
                            case interrupt::transferHalfComplete:
                                mask_ |= DMA_SxCR_HTIE;
                                break;
                            case interrupt::transferComplete:
                                mask_ |= DMA_SxCR_TCIE;
                                break;
                            case interrupt::transferError:
                                mask_ |= DMA_SxCR_TEIE;
                                break;
                            case interrupt::directModeError:
                                mask_ |= DMA_SxCR_DMEIE;
                                break;
                            default:
                                break;
                        }
                    }
                }

                //Apply the mask to the control register
                reg::set(std::ref(streamHandle_->CR), mask_);
            }

            void enableInterrupt(interrupt interrupt) const {
                if (interrupt == interrupt::fifoError) {
                    //If the fifo interrupt should be enabled, set it
                    reg::set(std::ref(streamHandle_->FCR), DMA_SxFCR_FEIE);
                } else {
                    //If the interrupt in the CR should be set, set it
                    switch (interrupt) {
                        case interrupt::transferHalfComplete:
                            reg::set(std::ref(streamHandle_->CR), DMA_SxCR_HTIE);
                            break;
                        case interrupt::transferComplete:
                            reg::set(std::ref(streamHandle_->CR), DMA_SxCR_TCIE);
                            break;
                        case interrupt::transferError:
                            reg::set(std::ref(streamHandle_->CR), DMA_SxCR_TEIE);
                            break;
                        case interrupt::directModeError:
                            reg::set(std::ref(streamHandle_->CR), DMA_SxCR_DMEIE);
                            break;
                        default:
                            break;
                    }
                }
            }

            void disableInterrupt(const std::vector<interrupt> interrupts) const {

                std::uint8_t mask_ = 0;

                for (auto i : interrupts) {
                    if (i == interrupt::fifoError) {
                        //If the fifo interrupt should be enabled, set it right away
                        reg::clear(std::ref(streamHandle_->FCR), DMA_SxFCR_FEIE);
                    } else {
                        switch (i) {
                            case interrupt::transferHalfComplete:
                                mask_ |= DMA_SxCR_HTIE;
                                break;
                            case interrupt::transferComplete:
                                mask_ |= DMA_SxCR_TCIE;
                                break;
                            case interrupt::transferError:
                                mask_ |= DMA_SxCR_TEIE;
                                break;
                            case interrupt::directModeError:
                                mask_ |= DMA_SxCR_DMEIE;
                                break;
                            default:
                                break;
                        }
                    }
                }

                //Apply the mask to the control register
                reg::clear(std::ref(streamHandle_->CR), mask_);
            }

            void disableInterrupt(interrupt interrupt) const {
                if (interrupt == interrupt::fifoError) {
                    //If the fifo interrupt should be enabled, set it
                    reg::clear(std::ref(streamHandle_->FCR), DMA_SxFCR_FEIE);
                } else {
                    //If the interrupt in the CR should be set, set it
                    switch (interrupt) {
                        case interrupt::transferHalfComplete:
                            reg::clear(std::ref(streamHandle_->CR), DMA_SxCR_HTIE);
                            break;
                        case interrupt::transferComplete:
                            reg::clear(std::ref(streamHandle_->CR), DMA_SxCR_TCIE);
                            break;
                        case interrupt::transferError:
                            reg::clear(std::ref(streamHandle_->CR), DMA_SxCR_TEIE);
                            break;
                        case interrupt::directModeError:
                            reg::clear(std::ref(streamHandle_->CR), DMA_SxCR_DMEIE);
                            break;
                        default:
                            break;
                    }
                }
            }

            void clearInterruptFlag(interrupt interrupt) const {
                switch (Stream) {
                    case stream::stream0:
                        reg::set(std::ref(dmaHandle_->LIFCR), static_cast<uint32_t>(interrupt), 0);
                        break;
                    case stream::stream1:
                        reg::set(std::ref(dmaHandle_->LIFCR), static_cast<uint32_t>(interrupt), 6);
                        break;
                    case stream::stream2:
                        reg::set(std::ref(dmaHandle_->LIFCR), static_cast<uint32_t>(interrupt), 16);
                        break;
                    case stream::stream3:
                        reg::set(std::ref(dmaHandle_->LIFCR), static_cast<uint32_t>(interrupt), 22);
                        break;
                    case stream::stream4:
                        reg::set(std::ref(dmaHandle_->HIFCR), static_cast<uint32_t>(interrupt), 0);
                        break;
                    case stream::stream5:
                        reg::set(std::ref(dmaHandle_->HIFCR), static_cast<uint32_t>(interrupt), 6);
                        break;
                    case stream::stream6:
                        reg::set(std::ref(dmaHandle_->HIFCR), static_cast<uint32_t>(interrupt), 16);
                        break;
                    case stream::stream7:
                        reg::set(std::ref(dmaHandle_->HIFCR), static_cast<uint32_t>(interrupt), 22);
                        break;
                    default:
                        break;
                }
            }

            bool getInterruptFlag(interrupt interrupt) const {
                switch (Stream) {
                    case stream::stream0:
                        return static_cast<bool>(reg::read(std::ref(dmaHandle_->LISR), static_cast<uint32_t>(interrupt), 0));
                        break;
                    case stream::stream1:
                        return static_cast<bool>(reg::read(std::ref(dmaHandle_->LISR), static_cast<uint32_t>(interrupt), 5));
                        break;
                    case stream::stream2:
                        return static_cast<bool>(reg::read(std::ref(dmaHandle_->LISR), static_cast<uint32_t>(interrupt), 10));
                        break;
                    case stream::stream3:
                        return static_cast<bool>(reg::read(std::ref(dmaHandle_->LISR), static_cast<uint32_t>(interrupt), 15));
                        break;
                    case stream::stream4:
                        return static_cast<bool>(reg::read(std::ref(dmaHandle_->HISR), static_cast<uint32_t>(interrupt), 0));
                        break;
                    case stream::stream5:
                        return static_cast<bool>(reg::read(std::ref(dmaHandle_->HISR), static_cast<uint32_t>(interrupt), 5));
                        break;
                    case stream::stream6:
                        return static_cast<bool>(reg::read(std::ref(dmaHandle_->HISR), static_cast<uint32_t>(interrupt), 10));
                        break;
                    case stream::stream7:
                        return static_cast<bool>(reg::read(std::ref(dmaHandle_->HISR), static_cast<uint32_t>(interrupt), 15));
                        break;
                    default:
                        break;
                }
            }

            void setFifoTreshold(fifoTreshold treshold) const {
                reg::change(std::ref(streamHandle_->FCR), 0x03, static_cast<std::uint8_t>(treshold), DMA_SxFCR_FTH_Pos);
            }

            fifoStat getFifoStatus() const {
                return static_cast<fifoStat>(reg::read(std::ref(streamHandle_->FCR), 0x07, DMA_SxFCR_FS_Pos));
            }

            void enableDoubleBuffer() const {
                reg::set(std::ref(streamHandle_->CR), DMA_SxCR_DBM);
            }

            void disableDoubleBuffer() const {
                reg::clear(std::ref(streamHandle_->CR), DMA_SxCR_DBM);
            }
    };
}



#endif
