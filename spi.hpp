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

#ifndef STMCPP_SPI_H
#define STMCPP_SPI_H

#include <stmcpp/stmheader.hpp>
#include <cstdint>

namespace stmcpp::spi {
    using namespace stmcpp;
    
    enum class peripheral : std::uint32_t {
        spi1 = SPI1_BASE,
        spi2 = SPI2_BASE,
        spi3 = SPI3_BASE,
        spi4 = SPI4_BASE,
        spi5 = SPI5_BASE,
        spi6 = SPI6_BASE
    };

    enum class role : std::uint8_t {
        slave   = 0b0, ///< SPI is configured as slave
        master  = 0b1  ///< SPI is configured as master
    };

    enum class mode : std::uint8_t {
        fullDuplex  = 0b00, ///< SPI is configured in full duplex mode
        txSimplex   = 0b01, ///< SPI is configured in transmitter simplex mode
        rxSimplex   = 0b10, ///< SPI is configured in receiver simplex mode
        halfDuplex  = 0b11  ///< SPI is configured in half duplex mode
    };

    enum class direction : std::uint8_t {
        receiver    = 0b0, ///< SPI half-dupex direction is receiver
        transmitter = 0b1  ///< SPI half-dupex direction is transmitter
    };

    enum class protocol : std::uint8_t {
        motorola    = 0b000, ///< SPI is configured to use the motorola protocol
        ti          = 0b001  ///< SPI is configured to use the TI protocol
    };

    enum class clockPol : std::uint8_t {
        idleLow     = 0b0, ///< The SPI clock is low in idle
        idleHigh    = 0b1  ///< The SPI clock is high in idle
    };

    enum class ssOrigin : std::uint8_t {
        ssPad       = 0b0, 
        ssBit       = 0b1  
    };

    enum class ssBehavior : std::uint8_t {
        endOfTransfer       = 0b0, ///< SS is kept at active level till data transfer is completed, it becomes inactive with EOT flag
        interleaved         = 0b1  ///< SPI data frames are interleaved with SS non active pulses when dataIdleness > 1
    };

    enum class afBehavior : std::uint8_t {
        noControl           = 0b0, ///< The peripheral takes no control of GPIOs while it is disabled
        keepControl         = 0b1  ///< The peripheral keeps always control of all associated GPIOs
    };

    enum class clockPhase : std::uint8_t {
        firstTransition     = 0b0, ///< The first clock transition is the first data capture edge
        secondTransition    = 0b1  ///< The second clock transition is the first data capture edge
    };

    enum class bitOrder : std::uint8_t {
        msbFirst    = 0b0, ///< MSB is transmitted first
        lsbFirst    = 0b1  ///< LSB is transmitted first
    };

    enum class ssPol : std::uint8_t {
        activeLow   = 0b0, ///< The SPI slave select is active low
        activeHigh  = 0b1  ///< The SPI slave select is active high
    };
    
    enum class underrunBehav : std::uint8_t {
        pattern             = 0b00, ///< Send out pattern defined by SPI_UDRDR
        repeatLastRx        = 0b01, ///< Repeat lastly received data
        repeatLastTx        = 0b10  ///< Repeat lastly transmitted data
    };

    enum class underrunDet : std::uint8_t {
        beginFrame          = 0b00, ///< Underrun detected at beginning of the data frame
        endFrame            = 0b01, ///< Underrun detected at end of the data frame
        beginSlaveSelect    = 0b10  ///< Underrun detected at beginning of the slave select signal
    };

    enum class crc : std::uint8_t {
        allZeros     = 0b0, ///< All zero calculation pattern 
        allOnes      = 0b1  ///< All one calculation patern
    };

    enum class masterDivider : std::uint8_t {
        div2        = 0b000, ///< The spi master clock is divided by 2
        div4        = 0b001, ///< The spi master clock is divided by 4
        div8        = 0b010, ///< The spi master clock is divided by 8
        div16       = 0b011, ///< The spi master clock is divided by 16
        div32       = 0b100, ///< The spi master clock is divided by 32
        div64       = 0b101, ///< The spi master clock is divided by 64
        div128      = 0b110, ///< The spi master clock is divided by 128
        div256      = 0b111  ///< The spi master clock is divided by 256
    };

    enum class interrupt : std::uint32_t {
        endOfTransfer    = SPI_SR_EOT,   ///< End of transfer
        txFilled         = SPI_SR_TXTF,  ///< Transmission transfer filled
        underrun         = SPI_SR_UDR,   ///< Underrun
        overrun          = SPI_SR_OVR,   ///< Overrun
        crcError         = SPI_SR_CRCE,  ///< CRC error
        tifError         = SPI_SR_TIFRE, ///< TI frame format error
        modeFault        = SPI_SR_MODF,  ///< Mode fault
        additionalReload = SPI_SR_TSERF, ///< Additional number of data
        suspend          = SPI_SR_SUSP,   ///< SPI suspended
        rxPacket         = SPI_SR_RXP,   ///< RX packet is available
        txPacket         = SPI_SR_TXP,   ///< TX packet space is available
        duplexPacket     = SPI_SR_DXP,   ///< RX packet was received and TX packet space is available
        txComplete       = SPI_SR_TXC   ///< RX packet is available
    };

    template<peripheral Peripheral>
        class spi {
            private:
                SPI_TypeDef * const spiHandle_ = reinterpret_cast<SPI_TypeDef *>(static_cast<std::uint32_t>(Peripheral));
                
            public:
                spi(role role, mode mode, uint8_t framesize, masterDivider masterDivider = masterDivider::div2, protocol protocol = protocol::motorola, bitOrder bitOrder = bitOrder::msbFirst, 
                clockPol clockPol = clockPol::idleLow, clockPhase clockPhase = clockPhase::firstTransition, ssOrigin ssOrigin = ssOrigin::ssPad, ssPol ssPol = ssPol::activeLow, bool ssoutputenable = false, 
                ssBehavior ssBehavior = ssBehavior::endOfTransfer , uint8_t fifotreshold = 1, bool enabletxdma = false, bool enablerxdma = false, uint8_t ssidleness = 0, uint8_t dataidleness = 0,
                bool ioswap = false, afBehavior afBehavior = afBehavior::noControl, bool crcenable = false, uint8_t crcsize = 4, bool crcfullsize = false, crc crcrx = crc::allZeros,
                crc crctx = crc::allZeros, bool enablesuspend = false, underrunDet underrunDet = underrunDet::beginFrame, underrunBehav underrunBehav = underrunBehav::pattern) {
                    
                    bool enableSS_ = false;

                    if (role == role::master) {
                        ssOrigin = ssOrigin::ssBit;
                        enableSS_ = true;
                    }

                    reg::write(std::ref(spiHandle_->CR1),
                        ((static_cast<uint8_t>(enableSS_) & 0b1) << SPI_CR1_SSI_Pos) |
                        ((static_cast<uint8_t>(enablesuspend) & 0b1) << SPI_CR1_MASRX_Pos) |
                        ((static_cast<uint8_t>(crcfullsize) & 0b1) << SPI_CR1_CRC33_17_Pos) |
                        ((static_cast<uint8_t>(crcrx) & 0b1) << SPI_CR1_RCRCINI_Pos) |
                        ((static_cast<uint8_t>(crctx) & 0b1) << SPI_CR1_TCRCINI_Pos)
                    );

                    reg::write(std::ref(spiHandle_->CFG1),
                        ((static_cast<uint8_t>(framesize - 1) & 0b11111) << SPI_CFG1_DSIZE_Pos) |
                        ((static_cast<uint8_t>(fifotreshold - 1) & 0b1111) << SPI_CFG1_FTHLV_Pos) |
                        ((static_cast<uint8_t>(underrunBehav) & 0b11) << SPI_CFG1_UDRCFG_Pos) |
                        ((static_cast<uint8_t>(underrunDet) & 0b11) << SPI_CFG1_UDRDET_Pos) |
                        ((static_cast<uint8_t>(enablerxdma) & 0b1) << SPI_CFG1_RXDMAEN_Pos) |
                        ((static_cast<uint8_t>(enabletxdma) & 0b1) << SPI_CFG1_TXDMAEN_Pos) |
                        ((static_cast<uint8_t>(crcsize - 1) & 0b11111) << SPI_CFG1_CRCSIZE_Pos) |
                        ((static_cast<uint8_t>(crcenable) & 0b1) << SPI_CFG1_CRCEN_Pos) |
                        ((static_cast<uint8_t>(masterDivider) & 0b111) << SPI_CFG1_MBR_Pos) 
                    );

                    reg::write(std::ref(spiHandle_->CFG2),
                        ((static_cast<uint8_t>(ssidleness) & 0b1111) << SPI_CFG2_MSSI_Pos) |
                        ((static_cast<uint8_t>(dataidleness) & 0b1111) << SPI_CFG2_MIDI_Pos) |
                        ((static_cast<uint8_t>(ioswap) & 0b1) << SPI_CFG2_IOSWP_Pos) |
                        ((static_cast<uint8_t>(mode) & 0b11) << SPI_CFG2_COMM_Pos) |
                        ((static_cast<uint8_t>(protocol) & 0b111) << SPI_CFG2_SP_Pos) |
                        ((static_cast<uint8_t>(role) & 0b1) << SPI_CFG2_MASTER_Pos) |
                        ((static_cast<uint8_t>(bitOrder) & 0b1) << SPI_CFG2_LSBFRST_Pos) |
                        ((static_cast<uint8_t>(clockPhase) & 0b1) << SPI_CFG2_CPHA_Pos) |
                        ((static_cast<uint8_t>(clockPol) & 0b1) << SPI_CFG2_CPOL_Pos) |
                        ((static_cast<uint8_t>(ssOrigin) & 0b1) << SPI_CFG2_SSM_Pos) |
                        ((static_cast<uint8_t>(ssPol) & 0b1) << SPI_CFG2_SSIOP_Pos) |
                        ((static_cast<uint8_t>(ssoutputenable) & 0b1) << SPI_CFG2_SSOE_Pos) |
                        ((static_cast<uint8_t>(ssBehavior) & 0b1) << SPI_CFG2_SSOM_Pos) |
                        ((static_cast<uint8_t>(afBehavior) & 0b1) << SPI_CFG2_AFCNTR_Pos) 
                    );
                }

                void enable() const {
                    reg::set(std::ref(spiHandle_->CR1), SPI_CR1_SPE);
                }

                void disable() const {
                    reg::clear(std::ref(spiHandle_->CR1), SPI_CR1_SPE);
                }

                void startTransfer() const {
                    reg::set(std::ref(spiHandle_->CR1), SPI_CR1_CSTART);
                }

                void requestSuspend() const {
                    reg::set(std::ref(spiHandle_->CR1), SPI_CR1_CSUSP);
                }

                void setDirection(direction direction) const {
                    if (direction == direction::receiver) {
                        reg::clear(std::ref(spiHandle_->CR1), SPI_CR1_HDDIR);
                    } else reg::set(std::ref(spiHandle_->CR1), SPI_CR1_HDDIR);
                }

                void setInternalSs() const {
                    reg::set(std::ref(spiHandle_->CR1), SPI_CR1_SSI);
                }

                void clearInternalSs() const {
                    reg::clear(std::ref(spiHandle_->CR1), SPI_CR1_SSI);
                }

                void enableCrc() const {
                    reg::set(std::ref(spiHandle_->CFG1), SPI_CFG1_CRCEN);
                }

                void disableCrc() const {
                    reg::clear(std::ref(spiHandle_->CFG1), SPI_CFG1_CRCEN);
                }

                void setCrc(uint32_t crc) const {
                    reg::write(std::ref(spiHandle_->CRCPOLY), crc); 
                }

                void setTxCrc(uint32_t crc) const {
                    reg::write(std::ref(spiHandle_->TXCRC), crc); 
                }
                
                void setRxCrc(uint32_t crc) const {
                    reg::write(std::ref(spiHandle_->RXCRC), crc); 
                }

                void lockIo() const {
                    reg::set(std::ref(spiHandle_->CR1), SPI_CR1_IOLOCK);
                }

                void setNumberOfData(uint16_t number) const {
                    reg::change(std::ref(spiHandle_->CR2), 0xFFFF, number, SPI_CR2_TSIZE_Pos);
                }

                void setNumberOfDataExtension(uint16_t number) const {
                    reg::change(std::ref(spiHandle_->CR2), 0xFFFF, number, SPI_CR2_TSER_Pos);
                }

                void enableRxDma() const {
                    reg::set(std::ref(spiHandle_->CFG1), SPI_CFG1_RXDMAEN);
                }

                void disableRxDma() const {
                    reg::clear(std::ref(spiHandle_->CFG1), SPI_CFG1_RXDMAEN);
                }

                void enableTxDma() const {
                    reg::set(std::ref(spiHandle_->CFG1), SPI_CFG1_TXDMAEN);
                }

                void disableTxDma() const {
                    reg::clear(std::ref(spiHandle_->CFG1), SPI_CFG1_TXDMAEN);
                }

                void enableIoSwap () const {
                    reg::set(std::ref(spiHandle_->CFG2), SPI_CFG2_IOSWP);
                }

                void disableIoSwap () const {
                    reg::clear(std::ref(spiHandle_->CFG2), SPI_CFG2_IOSWP);
                }

                 void enableSoftwareSS () const {
                    reg::set(std::ref(spiHandle_->CFG2), SPI_CFG2_SSM);
                }

                void disableSoftwareSS () const {
                    reg::clear(std::ref(spiHandle_->CFG2), SPI_CFG2_SSM);
                }

                void enableInterrupt(interrupt interrupt) const {
                    //All these three interrupts are affected by this bit
                    if (interrupt == interrupt::endOfTransfer || interrupt == interrupt::suspend || interrupt == interrupt::txComplete) {
                        reg::set(std::ref(spiHandle_->IER), SPI_IER_EOTIE);
                    } else reg::set(std::ref(spiHandle_->IER), static_cast<std::uint32_t>(interrupt));
                    
                }

                void disableInterrupt(interrupt interrupt) const {
                    //All these three interrupts are affected by this bit
                    if (interrupt == interrupt::endOfTransfer || interrupt == interrupt::suspend || interrupt == interrupt::txComplete) {
                        reg::clear(std::ref(spiHandle_->IER), SPI_IER_EOTIE);
                    } else reg::clear(std::ref(spiHandle_->IER), static_cast<std::uint32_t>(interrupt));
                }

                bool getInterruptFlag(interrupt interrupt) const {
                    return static_cast<bool>(reg::read(std::ref(spiHandle_->SR), static_cast<std::uint32_t>(interrupt)));
                }

                void clearInterruptFlag(interrupt interrupt) const {
                    reg::set(std::ref(spiHandle_->IFCR), static_cast<std::uint32_t>(interrupt));
                }

                bool rxFifoNotEmpty() const {
                    return static_cast<bool>(reg::read(std::ref(spiHandle_->SR), SPI_SR_RXWNE));
                }

                std::uint8_t rxFifoFramesAvailable() const {
                    return reg::read(std::ref(spiHandle_->SR), 0x03, SPI_SR_RXPLVL_Pos);
                }

                std::uint16_t remainingNumberOfData() const {
                    return reg::read(std::ref(spiHandle_->SR), 0xFFFF, SPI_SR_CTSIZE_Pos);
                }

                void setUnderrunPattern(uint32_t pattern) const {
                    reg::write(std::ref(spiHandle_->UDRDR), pattern);
                }
    };
}

#endif