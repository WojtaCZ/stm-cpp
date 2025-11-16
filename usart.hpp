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

#ifndef STMCPP_USART_H
#define STMCPP_USART_H

#include <cstdint>
#include <cstddef>
#include <cmath>

#include <stmcpp/register.hpp>
#include <stmcpp/units.hpp>
#include <stmcpp/stmheader.hpp>

namespace stmcpp::usart {
    using namespace stmcpp;

    enum class peripheral : std::uint32_t {
        usart1 = USART1_BASE,
        usart2 = USART2_BASE,
        usart3 = USART3_BASE,
        uart4 = UART4_BASE,
        uart5 = UART5_BASE,
        usart6 = USART6_BASE,
        uart7 = UART7_BASE,
        uart8 = UART8_BASE
    };

    enum class overSampling : std::uint8_t {
        times16 = 0b0,
        times8  = 0b1
    };

    enum class stopBits : std::uint8_t {
        one         = 0b00,
        half        = 0b01,
        two         = 0b10,
        oneAndHalf  = 0b11
    };

    enum class linBreakLength : std::uint8_t {
        tenBit    = 0b0,
        elevenBit = 0b1
    };

    enum class addressLength : std::uint8_t {
        fourBit  = 0b0,
        sevenBit = 0b1
    };

    enum class rxFifoTreshold : std::uint8_t {
        eight           = 0b000,
        quarter         = 0b001,
        half            = 0b010,
        threeQuarter    = 0b011,
        sevenEights     = 0b100,
        full            = 0b101
    };

    enum class txFifoTreshold : std::uint8_t {
        eight           = 0b000,
        quarter         = 0b001,
        half            = 0b010,
        threeQuarter    = 0b011,
        sevenEights     = 0b100,
        empty           = 0b101
    };
    
    enum class wakeupEvent : std::uint8_t {
        addressMatch  = 0b00,
        startBit      = 0b10,
        rxNotEmpty    = 0b11
    };

    enum class parity : std::uint8_t {
        none  = 0b00,
        even  = 0b10,
        odd   = 0b11
    };
    
    enum class wordLength : std::uint8_t {
        eightBit  = 0b00,
        nineBit   = 0b01,
        sevenBit  = 0b10
    };

    enum class bitOrder : std::uint8_t {
        msbFirst    = 0b1, ///< MSB is transmitted first
        lsbFirst    = 0b0  ///< LSB is transmitted first
    };

    enum class clockPol : std::uint8_t {
        idleLow     = 0b0, ///< The USART clock is low in idle
        idleHigh    = 0b1  ///< The USART clock is high in idle
    };

    enum class clockPhase : std::uint8_t {
        firstTransition     = 0b0, ///< The first clock transition is the first data capture edge
        secondTransition    = 0b1  ///< The second clock transition is the first data capture edge
    };

    enum class driverEnablePol : std::uint8_t {
        activeHigh   = 0b0, 
        activeLow    = 0b1 
    };

    enum class divider : std::uint8_t {
        noDivide    = 0b0000, 
        div2        = 0b0001,
        div4        = 0b0010,
        div6        = 0b0011,
        div8        = 0b0100,
        div10       = 0b0101,
        div12       = 0b0110,
        div16       = 0b0111,
        div32       = 0b1000,
        div64       = 0b1001,
        div128      = 0b1010,
        div256      = 0b1011
    };

    enum class interrupt : std::uint32_t {
        txEmpty         = USART_ICR_TXFECF,
        endOfBlock      = USART_ICR_EOBCF,
        rxTimeout       = USART_ICR_RTOCF,
        characterMatch  = USART_ICR_CMCF,
        parityError     = USART_ICR_PECF,
        framingError    = USART_ICR_FECF,
        noiseDetect     = USART_ICR_NECF,
        overerun        = USART_ICR_ORECF,
        txComplete      = USART_ICR_TCCF,
        idle            = USART_ICR_IDLECF,
        linBreak        = USART_ICR_LBDCF,
        underrun        = USART_ICR_UDRCF,
        lpWakeup        = USART_ICR_WUCF,
        cts             = USART_ICR_CTSCF,
        txBeforeGuard   = USART_ICR_TCBGTCF
    };

    enum class flag : std::uint32_t {
        rxFull          = USART_ISR_RXFF,
        rxTreshold      = USART_ISR_RXFT,
        txTreshold      = USART_ISR_TXFT,
        cts             = USART_ISR_CTS,
        autoBaudError   = USART_ISR_ABRE,
        autoBaudFlag    = USART_ISR_ABRF,
        busy            = USART_ISR_BUSY,
        sendBreak       = USART_ISR_SBKF,
        muteWakeup      = USART_ISR_RWU,
        txEnable        = USART_ISR_TEACK,
        rxEnable        = USART_ISR_REACK,
        rxNotEmpty      = USART_ISR_RXNE_RXFNE,
        txFree          = USART_ISR_TXE_TXFNF
    };

using namespace stmcpp::units;
    template<usart::peripheral Peripheral>
    class uart {
        private:
            USART_TypeDef * const usartHandle_ = reinterpret_cast<USART_TypeDef *>(static_cast<std::uint32_t>(Peripheral));

        public:
            constexpr uart(units::frequency periphClock, usart::divider divider, units::baudrate speed, bool fifomode = false, usart::wordLength wordLength = usart::wordLength::eightBit, usart::overSampling overSampling = usart::overSampling::times16,
                            usart::parity parity = usart::parity::none, usart::stopBits stopBits = usart::stopBits::one, usart::bitOrder bitOrder = usart::bitOrder::lsbFirst
                          ) {
                
                // Work out the divisor and multiplier value based on the oversampling
                constexpr std::array dividerLut_ = {1, 2, 4, 6, 8, 10, 12, 16, 32, 64, 128, 256}; 
                std::uint32_t dividerValue_ = dividerLut_.at(static_cast<std::uint8_t>(divider));
                constexpr std::array multiplierLut_ = {1.0f, 2.0f}; 
                double multiplier_ = multiplierLut_.at(static_cast<std::uint8_t>(overSampling));
                
                // Calculate the baud rate and than round it to get the raw value
                int rawBaud_ = (10 * multiplier_ * (periphClock.toHertz() / dividerValue_)) / speed.toBaud();

                if (rawBaud_ - (rawBaud_ / 10) >= 5) {
                    rawBaud_ = (rawBaud_ / 10) + 1;
                }else rawBaud_ = (rawBaud_ / 10);

                reg::write(std::ref(usartHandle_->CR1),
                    ((static_cast<uint8_t>(fifomode) & 0b1) << USART_CR1_FIFOEN_Pos) |
                    ((static_cast<uint8_t>(wordLength) & 0b10) << (USART_CR1_M1_Pos - 1)) |
                    ((static_cast<uint8_t>(wordLength) & 0b01) << USART_CR1_M0_Pos) |
                    ((static_cast<uint8_t>(overSampling) & 0b1) << USART_CR1_OVER8_Pos) |
                    ((static_cast<uint8_t>(parity) & 0b11) << USART_CR1_PS_Pos)
                );

                reg::write(std::ref(usartHandle_->CR2),
                    ((static_cast<uint8_t>(bitOrder) & 0b1) << USART_CR2_MSBFIRST_Pos) |
                    ((static_cast<uint8_t>(stopBits) & 0b11) << USART_CR2_STOP_Pos) 

                );

                if (overSampling == usart::overSampling::times16) {
                    reg::write(std::ref(usartHandle_->BRR), rawBaud_);
                } else reg::write(std::ref(usartHandle_->BRR), (rawBaud_ & 0xFFFFFFF8) | ((rawBaud_ >> 1) & 0x3));
                
            }

            void enable() const {
                reg::set(std::ref(usartHandle_->CR1), USART_CR1_UE);
            }

            void disable() const {
                reg::clear(std::ref(usartHandle_->CR1), USART_CR1_UE);
            }

            void enableTx() const {
                reg::set(std::ref(usartHandle_->CR1), USART_CR1_TE);
            }

            void disableTx() const {
                reg::clear(std::ref(usartHandle_->CR1), USART_CR1_TE);
            }

            void enableTxDma() const {
                reg::set(std::ref(USART2->CR3), USART_CR3_DMAT);
            }

            void disableTxDma() const {
                reg::clear(std::ref(USART2->CR3), USART_CR3_DMAT);
            }

            void enableRx() const {
                reg::set(std::ref(usartHandle_->CR1), USART_CR1_RE);
            }

            void disableRx() const {
                reg::clear(std::ref(usartHandle_->CR1), USART_CR1_RE);
            }

            void enableRxDma() const {
                reg::set(std::ref(USART2->CR3), USART_CR3_DMAR);
            }

            void disableRxDma() const {
                reg::clear(std::ref(USART2->CR3), USART_CR3_DMAR);
            }

            void transmit(std::uint8_t data) const {
                reg::write(std::ref(usartHandle_->TDR), data);
            }

            void txFifoTreshold(usart::txFifoTreshold treshold) const {
                reg::change(std::ref(usartHandle_->CR3), 0x07, static_cast<uint8_t>(treshold), USART_CR3_TXFTCFG_Pos);
            }

            void rxFifoTreshold(usart::rxFifoTreshold treshold) const {
                reg::change(std::ref(usartHandle_->CR3), 0x07, static_cast<uint8_t>(treshold), USART_CR3_RXFTCFG_Pos);
            }

            void wakeupEvent(usart::wakeupEvent wakeupEvent) const {
                reg::change(std::ref(usartHandle_->CR3), 0x03, static_cast<uint8_t>(wakeupEvent), USART_CR3_WUS_Pos);
            }

            /// @brief Set the USART receiver timeout
            /// @param timeout Timeout in terms of number of bits
            void rxTimeout(std::uint32_t timeout) const { 
                reg::change(std::ref(usartHandle_->RTOR), 0x00FFFFFF, timeout);
            }

            void enableDe(usart::driverEnablePol depolarity = usart::driverEnablePol::activeHigh, std::uint8_t assertion = 0, std::uint8_t deassertion = 0) const {
                reg::set(std::ref(usartHandle_->CR1),
                    ((assertion & 0b11111) << USART_CR1_DEAT_Pos) |
                    ((deassertion & 0b11111) << USART_CR1_DEDT_Pos) 
                );

                reg::set(std::ref(usartHandle_->CR3),
                    (                                  0b1 << USART_CR3_DEM_Pos) |
                    ((static_cast<uint8_t>(depolarity) & 0b1) << USART_CR3_DEP_Pos) 
                );
            }

            void disableDe() const {
                reg::clear(std::ref(usartHandle_->CR1),
                    (0b11111 << USART_CR1_DEAT_Pos) |
                    (0b11111 << USART_CR1_DEDT_Pos) 
                );

                reg::clear(std::ref(usartHandle_->CR3),
                    (0b1 << USART_CR3_DEM_Pos) |
                    (0b1 << USART_CR3_DEP_Pos) 
                );
            }

            void enableCk(usart::clockPol clockPol = usart::clockPol::idleLow, usart::clockPhase clockPhase = usart::clockPhase::firstTransition, bool lastbitclock = false) const {
                reg::set(std::ref(usartHandle_->CR2),
                    (                                  0b1 << USART_CR2_CLKEN_Pos) |
                    ((static_cast<uint8_t>(clockPol) & 0b1) << USART_CR2_CPOL_Pos) |
                    ((static_cast<uint8_t>(clockPhase) & 0b1) << USART_CR2_CPHA_Pos) | 
                    ((static_cast<uint8_t>(lastbitclock) & 0b1) << USART_CR2_LBCL_Pos)
                );
            }

            void disableCk() const {
                reg::clear(std::ref(usartHandle_->CR2),
                    (0b1 << USART_CR2_CLKEN_Pos) |
                    (0b1 << USART_CR2_CPOL_Pos) |
                    (0b1 << USART_CR2_CPHA_Pos) | 
                    (0b1 << USART_CR2_LBCL_Pos)
                );
            }

            void clearInterruptFlag(usart::interrupt interrupt) const {
                reg::set(std::ref(usartHandle_->ICR), static_cast<uint32_t>(interrupt));
            }

            bool getInterruptFlag(usart::interrupt interrupt) const {
                if (interrupt == usart::interrupt::txBeforeGuard) return static_cast<bool>(reg::read(std::ref(usartHandle_->ISR), USART_ISR_TCBGT)); 
                if (interrupt == usart::interrupt::txEmpty) return static_cast<bool>(reg::read(std::ref(usartHandle_->ISR), USART_ISR_TXFE)); 

                return static_cast<bool>(reg::read(std::ref(usartHandle_->ISR), static_cast<uint32_t>(interrupt)));
            }

            bool getStatusFlag(usart::flag flag) const {
                return static_cast<bool>(reg::read(std::ref(usartHandle_->ISR), static_cast<uint32_t>(flag)));
            }
    };
}

#endif