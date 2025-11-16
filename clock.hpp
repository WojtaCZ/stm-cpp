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

#ifndef STMCPP_CLOCK_H
#define STMCPP_CLOCK_H

#include <cstdint>
#include <tuple>
#include <array>

#include <stmcpp/register.hpp>
#include <stmcpp/units.hpp>
#include <stmcpp/error.hpp>

#include <stmcpp/stmheader.hpp>
#include "stmcpp-config.hpp"

extern "C" void SysTick_Handler();

namespace stmcpp::clock {
    using namespace stmcpp;
    using namespace stmcpp::units;

    enum class error {
        systick_used_uninitialized,
        pll_lock_timeout
    };

    static stmcpp::error::handler<error, "stmcpp::clock"> errorHandler;

    enum class peripheral : std::uint16_t {
        mdma        = (0x0000) | RCC_AHB3ENR_MDMAEN_Pos,
        dma2d       = (0x0000) | RCC_AHB3ENR_DMA2DEN_Pos,
        jpgdec      = (0x0000) | RCC_AHB3ENR_JPGDECEN_Pos,
        fmc         = (0x0000) | RCC_AHB3ENR_FMCEN_Pos,
        qspi        = (0x0000) | RCC_AHB3ENR_QSPIEN_Pos,
        sdmmc1      = (0x0000) | RCC_AHB3ENR_SDMMC1EN_Pos, 

        dma1        = (0x0100) | RCC_AHB1ENR_DMA1EN_Pos,
        dma2        = (0x0100) | RCC_AHB1ENR_DMA2EN_Pos,
        adc12       = (0x0100) | RCC_AHB1ENR_ADC12EN_Pos,
        eth1mac     = (0x0100) | RCC_AHB1ENR_ETH1MACEN_Pos,
        eth1tx      = (0x0100) | RCC_AHB1ENR_ETH1TXEN_Pos,
        eth1rx      = (0x0100) | RCC_AHB1ENR_ETH1RXEN_Pos,
        usb1otg     = (0x0100) | RCC_AHB1ENR_USB1OTGHSEN_Pos,
        usb1ulpi    = (0x0100) | RCC_AHB1ENR_USB1OTGHSULPIEN_Pos,
        usb2otg     = (0x0100) | RCC_AHB1ENR_USB2OTGHSEN_Pos,
        usb2ulpi    = (0x0100) | RCC_AHB1ENR_USB2OTGHSULPIEN_Pos,

        dcmi        = (0x0200) | RCC_AHB2ENR_DCMIEN_Pos,
        crypt       = (0x0200) | RCC_AHB2ENR_CRYPEN_Pos,
        hash        = (0x0200) | RCC_AHB2ENR_HASHEN_Pos,
        rng         = (0x0200) | RCC_AHB2ENR_RNGEN_Pos,
        sdmmc2      = (0x0200) | RCC_AHB2ENR_SDMMC2EN_Pos,
        sram1       = (0x0200) | RCC_AHB2ENR_SRAM1EN_Pos,
        sram2       = (0x0200) | RCC_AHB2ENR_SRAM2EN_Pos,
        sram3       = (0x0200) | RCC_AHB2ENR_SRAM3EN_Pos,

        gpioa       = (0x0300) | RCC_AHB4ENR_GPIOAEN_Pos,
        gpiob       = (0x0300) | RCC_AHB4ENR_GPIOBEN_Pos,
        gpioc       = (0x0300) | RCC_AHB4ENR_GPIOCEN_Pos,
        gpiod       = (0x0300) | RCC_AHB4ENR_GPIODEN_Pos,
        gpioe       = (0x0300) | RCC_AHB4ENR_GPIOEEN_Pos,
        gpiof       = (0x0300) | RCC_AHB4ENR_GPIOFEN_Pos,
        gpiog       = (0x0300) | RCC_AHB4ENR_GPIOGEN_Pos,
        gpioh       = (0x0300) | RCC_AHB4ENR_GPIOHEN_Pos,
        gpioi       = (0x0300) | RCC_AHB4ENR_GPIOIEN_Pos,
        gpioj       = (0x0300) | RCC_AHB4ENR_GPIOJEN_Pos,
        gpiok       = (0x0300) | RCC_AHB4ENR_GPIOKEN_Pos,
        crc         = (0x0300) | RCC_AHB4ENR_CRCEN_Pos,
        bdma        = (0x0300) | RCC_AHB4ENR_BDMAEN_Pos,
        adc3        = (0x0300) | RCC_AHB4ENR_ADC3EN_Pos,
        hsem        = (0x0300) | RCC_AHB4ENR_HSEMEN_Pos,
        backupram   = (0x0300) | RCC_AHB4ENR_BKPRAMEN_Pos,

        ltdc        = (0x0400) | RCC_APB3ENR_LTDCEN_Pos,
        wwdg1       = (0x0400) | RCC_APB3ENR_WWDG1EN_Pos,

        tim2        = (0x0500) | RCC_APB1LENR_TIM2EN_Pos,
        tim3        = (0x0500) | RCC_APB1LENR_TIM3EN_Pos,
        tim4        = (0x0500) | RCC_APB1LENR_TIM4EN_Pos,
        tim5        = (0x0500) | RCC_APB1LENR_TIM5EN_Pos,
        tim6        = (0x0500) | RCC_APB1LENR_TIM6EN_Pos,
        tim7        = (0x0500) | RCC_APB1LENR_TIM7EN_Pos,
        tim12       = (0x0500) | RCC_APB1LENR_TIM12EN_Pos,
        tim13       = (0x0500) | RCC_APB1LENR_TIM13EN_Pos,
        tim14       = (0x0500) | RCC_APB1LENR_TIM14EN_Pos,
        lptim1      = (0x0500) | RCC_APB1LENR_LPTIM1EN_Pos,
        spi2        = (0x0500) | RCC_APB1LENR_SPI2EN_Pos,
        spi3        = (0x0500) | RCC_APB1LENR_SPI3EN_Pos,
        spdifrx     = (0x0500) | RCC_APB1LENR_SPDIFRXEN_Pos,
        usart2      = (0x0500) | RCC_APB1LENR_USART2EN_Pos,
        usart3      = (0x0500) | RCC_APB1LENR_USART3EN_Pos,
        uart4       = (0x0500) | RCC_APB1LENR_UART4EN_Pos,
        uart5       = (0x0500) | RCC_APB1LENR_UART5EN_Pos,
        i2c1        = (0x0500) | RCC_APB1LENR_I2C1EN_Pos,
        i2c2        = (0x0500) | RCC_APB1LENR_I2C2EN_Pos,
        i2c3        = (0x0500) | RCC_APB1LENR_I2C3EN_Pos,
        hdmicec     = (0x0500) | RCC_APB1LENR_CECEN_Pos,
        dac12       = (0x0500) | RCC_APB1LENR_DAC12EN_Pos,
        uart7       = (0x0500) | RCC_APB1LENR_UART7EN_Pos,
        uart8       = (0x0500) | RCC_APB1LENR_UART8EN_Pos,

        crs         = (0x0600) | RCC_APB1HENR_CRSEN_Pos,
        swpen       = (0x0600) | RCC_APB1HENR_SWPMIEN_Pos,
        opamp       = (0x0600) | RCC_APB1HENR_OPAMPEN_Pos,
        mdios       = (0x0600) | RCC_APB1HENR_MDIOSEN_Pos,
        fdcan       = (0x0600) | RCC_APB1HENR_FDCANEN_Pos,

        tim1        = (0x0700) | RCC_APB2ENR_TIM1EN_Pos,
        tim8        = (0x0700) | RCC_APB2ENR_TIM8EN_Pos,
        usart1      = (0x0700) | RCC_APB2ENR_USART1EN_Pos,
        usart6      = (0x0700) | RCC_APB2ENR_USART6EN_Pos,
        spi1        = (0x0700) | RCC_APB2ENR_SPI1EN_Pos,
        spi4        = (0x0700) | RCC_APB2ENR_SPI4EN_Pos,
        tim15       = (0x0700) | RCC_APB2ENR_TIM15EN_Pos,
        tim16       = (0x0700) | RCC_APB2ENR_TIM16EN_Pos,
        tim17       = (0x0700) | RCC_APB2ENR_TIM17EN_Pos,
        spi5        = (0x0700) | RCC_APB2ENR_SPI5EN_Pos,
        sai1        = (0x0700) | RCC_APB2ENR_SAI1EN_Pos,
        sai2        = (0x0700) | RCC_APB2ENR_SAI2EN_Pos,
        sai3        = (0x0700) | RCC_APB2ENR_SAI3EN_Pos,
        dfsdm1      = (0x0700) | RCC_APB2ENR_DFSDM1EN_Pos,
        hrtim       = (0x0700) | RCC_APB2ENR_HRTIMEN_Pos,

        syscfg      = (0x0800) | RCC_APB4ENR_SYSCFGEN_Pos,
        lpuart1     = (0x0800) | RCC_APB4ENR_LPUART1EN_Pos,
        spi6        = (0x0800) | RCC_APB4ENR_SPI6EN_Pos,
        i2c4        = (0x0800) | RCC_APB4ENR_I2C4EN_Pos,
        lptim2      = (0x0800) | RCC_APB4ENR_LPTIM2EN_Pos,
        lptim3      = (0x0800) | RCC_APB4ENR_LPTIM3EN_Pos,
        lptim4      = (0x0800) | RCC_APB4ENR_LPTIM4EN_Pos,
        lptim5      = (0x0800) | RCC_APB4ENR_LPTIM5EN_Pos,
        comp12      = (0x0800) | RCC_APB4ENR_COMP12EN_Pos,
        vrefbuf     = (0x0800) | RCC_APB4ENR_VREFEN_Pos,
        rtcapb      = (0x0800) | RCC_APB4ENR_RTCAPBEN_Pos,
        sai4        = (0x0800) | RCC_APB4ENR_SAI4EN_Pos
    };
    
    constexpr std::array<std::uint8_t, 9> peripheralRegisterMap_ = {
        offsetof(RCC_TypeDef, AHB3ENR), offsetof(RCC_TypeDef, AHB1ENR), offsetof(RCC_TypeDef, AHB2ENR), offsetof(RCC_TypeDef, AHB4ENR), 
        offsetof(RCC_TypeDef, APB3ENR), offsetof(RCC_TypeDef, APB1LENR), offsetof(RCC_TypeDef, APB1HENR), offsetof(RCC_TypeDef, APB2ENR), offsetof(RCC_TypeDef, APB4ENR)
    };

    template <typename ...Args>
    constexpr void enablePeripherals(Args... peripherals) {
        (enablePeripheral(peripherals), ...);
    }

    constexpr void enablePeripheral(peripheral peripheral) {
        std::uint8_t regOffset_ = peripheralRegisterMap_[static_cast<std::uint16_t>(peripheral) >> 8];
        (*reinterpret_cast<std::uint32_t *>(RCC_BASE + regOffset_)) |= (0b1 << (static_cast<std::uint16_t>(peripheral) & 0xFF));
    } 

    namespace domain {
        enum class d1cpre : uint8_t {
            div1    = 0b0000,
            div2    = 0b1000,
            div4    = 0b1001,
            div8    = 0b1010,
            div16   = 0b1011,
            div64   = 0b1100,
            div128  = 0b1101,
            div256  = 0b1110,
            div512  = 0b1111
        };

        enum class d1ppre : uint8_t {
            div1    = 0b000,
            div2    = 0b100,
            div4    = 0b101,
            div8    = 0b110,
            div16   = 0b111
        };

        enum class hpre : uint8_t {
            div1    = 0b0000,
            div2    = 0b1000,
            div4    = 0b1001,
            div8    = 0b1010,
            div16   = 0b1011,
            div64   = 0b1100,
            div128  = 0b1101,
            div256  = 0b1110,
            div512  = 0b1111
        };

        enum class d2ppre1 : uint8_t {
            div1    = 0b000,
            div2    = 0b100,
            div4    = 0b101,
            div8    = 0b110,
            div16   = 0b111
        };

        enum class d2ppre2 : uint8_t {
            div1    = 0b000,
            div2    = 0b100,
            div4    = 0b101,
            div8    = 0b110,
            div16   = 0b111
        };

        enum class d3ppre : uint8_t {
            div1    = 0b000,
            div2    = 0b100,
            div4    = 0b101,
            div8    = 0b110,
            div16   = 0b111
        };

        enum class source : uint8_t {
            hsi     = 0b000,
            csi     = 0b001,
            hse     = 0b010,
            pll1    = 0b011
        };

        class domain{
            public:
                constexpr domain(clock::domain::d1cpre d1cpre, clock::domain::d1ppre d1ppre, clock::domain::hpre hpre, clock::domain::d2ppre1 d2ppre1, clock::domain::d2ppre2 d2ppre2, clock::domain::d3ppre d3ppre, clock::domain::source source = clock::domain::source::hsi){
                        reg::write(std::ref(RCC->D1CFGR), 
                            static_cast<uint8_t>(d1cpre) << RCC_D1CFGR_D1CPRE_Pos |
                            static_cast<uint8_t>(d1ppre) << RCC_D1CFGR_D1PPRE_Pos |
                            static_cast<uint8_t>(hpre)   << RCC_D1CFGR_HPRE_Pos 
                        );

                        reg::write(std::ref(RCC->D2CFGR), 
                            static_cast<uint8_t>(d2ppre1) << RCC_D2CFGR_D2PPRE1_Pos |
                            static_cast<uint8_t>(d2ppre2) << RCC_D2CFGR_D2PPRE2_Pos 
                        );

                        reg::write(std::ref(RCC->D3CFGR), static_cast<uint8_t>(d3ppre) << RCC_D3CFGR_D3PPRE_Pos);

                        reg::change(std::ref(RCC->CFGR), 0x03, static_cast<uint8_t>(source), RCC_CFGR_SW_Pos);
                }

                clock::domain::source sourceStatus() const {
                    return static_cast<clock::domain::source>(reg::read(std::ref(RCC->CFGR), 0x07, RCC_CFGR_SWS_Pos));
                }
        };
    
    }

    namespace pll {
        enum class peripheral : uint32_t {
            pll1 = offsetof(RCC_TypeDef, PLL1DIVR),
            pll2 = offsetof(RCC_TypeDef, PLL2DIVR),
            pll3 = offsetof(RCC_TypeDef, PLL3DIVR)
        };

        enum class inputRange : uint8_t {
            f1_2MHz     = 0b00,
            f2_4MHz     = 0b01,
            f4_8MHz     = 0b10,
            f8_16MHz    = 0b11
        };

        enum class vcoRange : uint8_t {
            f192_960MHz     = 0b0,
            f150_420MHz     = 0b1,
        };

        enum class clkSource : uint8_t {
            hsi,
            csi,
            hse
        };

        static void setSource(clkSource source) {
            reg::change(std::ref(RCC->PLLCKSELR), 0x03, static_cast<uint8_t>(source));
        }

        template<peripheral Peripheral, unsigned int M, unsigned int N, unsigned int P, unsigned int Q, unsigned int R, unsigned int Fraction = 0>
        class pll{
            private:
                constexpr auto getPllIdx_() const {
                    return ((static_cast<unsigned int>(Peripheral) - offsetof(RCC_TypeDef, PLL1DIVR)) / 8);
                }

            public:
                constexpr pll(clock::pll::inputRange inputRange = clock::pll::inputRange::f1_2MHz, clock::pll::vcoRange vcoRange = clock::pll::vcoRange::f192_960MHz) {
                    static_assert(range_div_m.first < range_div_m.second, "Bottom range bound must be smaller than the top one!");
                    static_assert(range_div_n.first < range_div_n.second, "Bottom range bound must be smaller than the top one!");
                    static_assert(range_div_p.first < range_div_p.second, "Bottom range bound must be smaller than the top one!");
                    static_assert(range_div_q.first < range_div_q.second, "Bottom range bound must be smaller than the top one!");
                    static_assert(range_div_r.first < range_div_r.second, "Bottom range bound must be smaller than the top one!");

                    static_assert(M >= clock::pll::range_div_m.first && M <= clock::pll::range_div_m.second, "The M divider is out of range.");
                    static_assert(N >= clock::pll::range_div_n.first && N <= clock::pll::range_div_n.second, "The N divider is out of range.");
                    static_assert((P >= clock::pll::range_div_p.first && P <= clock::pll::range_div_p.second) || P == 0, "The P divider is out of range.");
                    static_assert((Q >= clock::pll::range_div_q.first && Q <= clock::pll::range_div_q.second) || Q == 0, "The Q divider is out of range.");
                    static_assert((R >= clock::pll::range_div_r.first && R <= clock::pll::range_div_r.second) || R == 0, "The R divider is out of range.");

                    static_assert(!((Peripheral == peripheral::pll1) && (P % 2)), "The P divider for PLL1 must be an even number!");

                    volatile uint32_t * const pllCfgrAdd_ = reinterpret_cast<volatile uint32_t *>(RCC_BASE + static_cast<std::uint32_t>(Peripheral));
                    volatile uint32_t * const  pllFracAdd_ = reinterpret_cast<volatile uint32_t *>(RCC_BASE + static_cast<std::uint32_t>(Peripheral) + 0x004);

                    reg::change(std::ref(RCC->PLLCKSELR), 0x3F, M, (8 * getPllIdx_()) + 4);

                    if constexpr (Fraction > 0){
                        reg::write(std::ref(*pllFracAdd_), Fraction, 3);
                        reg::set(std::ref(RCC->PLLCFGR), RCC_PLLCFGR_PLL1FRACEN, 4 * getPllIdx_());
                    }

                    reg::change(std::ref(RCC->PLLCFGR), 0x0E,
                    (
                        static_cast<uint8_t>(inputRange) << RCC_PLLCFGR_PLL1RGE_Pos |
                        static_cast<uint8_t>(vcoRange) << RCC_PLLCFGR_PLL1VCOSEL_Pos 
                    ), 4 * getPllIdx_());
                    
                    reg::write(std::ref(*pllCfgrAdd_), (N - 1) << RCC_PLL1DIVR_N1_Pos);

                    if constexpr (P != 0) {
                        reg::set(std::ref(*pllCfgrAdd_), (P - 1) << RCC_PLL1DIVR_P1_Pos);
                    } else stmcpp::reg::clear(std::ref(RCC->PLLCFGR), RCC_PLLCFGR_DIVP1EN, 3 * getPllIdx_());
                    
                    if constexpr (Q != 0) {
                        reg::set(std::ref(*pllCfgrAdd_), (Q - 1) << RCC_PLL1DIVR_Q1_Pos);
                    } else stmcpp::reg::clear(std::ref(RCC->PLLCFGR), RCC_PLLCFGR_DIVQ1EN, 3 * getPllIdx_());

                    if constexpr (R != 0) {
                        reg::set(std::ref(*pllCfgrAdd_), (R - 1) << RCC_PLL1DIVR_R1_Pos);
                    } else stmcpp::reg::clear(std::ref(RCC->PLLCFGR), RCC_PLLCFGR_DIVR1EN, 3 * getPllIdx_());

                };

                void enable() const {
                    reg::set(std::ref(RCC->CR), 0x01, (2 * getPllIdx_()) + RCC_CR_PLL1ON_Pos);
                }

                void disable() const {
                    reg::set(std::ref(RCC->CR), 0x01, (2 * getPllIdx_()) + RCC_CR_PLL1ON_Pos);
                }

                bool isLocked() const {
                    return static_cast<bool>(reg::read(std::ref(RCC->CR), 0x01, (2 * getPllIdx_()) + RCC_CR_PLL1RDY_Pos));
                }
                
        };    
    }    
    
    namespace systick {
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
}

#endif
