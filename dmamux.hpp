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

#ifndef STMCPP_DMAMUX_H
#define STMCPP_DMAMUX_H

#include <cstdint>
#include <cstddef>
#include <stmcpp/register.hpp>
#include <stmcpp/stmheader.hpp>

namespace stmcpp::dmamux1 {
    using namespace stmcpp;
    
    enum class channel : std::uint32_t {
        channel0    = 0x000UL,
        channel1    = 0x004UL,
        channel2    = 0x008UL,
        channel3    = 0x00CUL,
        channel4    = 0x010UL,
        channel5    = 0x014UL,
        channel6    = 0x018UL,
        channel7    = 0x01CUL,
        channel8    = 0x020UL,
        channel9    = 0x024UL,
        channel10   = 0x028UL,
        channel11   = 0x02CUL,
        channel12   = 0x030UL,
        channel13   = 0x034UL,
        channel14   = 0x038UL,
        channel15   = 0x03CUL
    };  

    enum class request : std::uint8_t {
        dmamux1_req_gen0    = 1,
        dmamux1_req_gen1    = 2,
        dmamux1_req_gen2    = 3,
        dmamux1_req_gen3    = 4,
        dmamux1_req_gen4    = 5,
        dmamux1_req_gen5    = 6,
        dmamux1_req_gen6    = 7,
        dmamux1_req_gen7    = 8,
        adc1_dma            = 9,
        adc2_dma            = 10,
        tim1_ch1            = 11,
        tim1_ch2            = 12,
        tim1_ch3            = 13,
        tim1_ch4            = 14,
        tim1_up             = 15,
        tim1_trig           = 16,
        tim1_com            = 17,
        tim2_ch1            = 18,
        tim2_ch2            = 19,
        tim2_ch3            = 20,
        tim2_ch4            = 21,
        tim2_up             = 22,                
        tim3_ch1            = 23,
        tim3_ch2            = 24,   
        tim3_ch3            = 25,
        tim3_ch4            = 26,
        tim3_up             = 27,
        tim3_trig           = 28,
        tim4_ch1            = 29,
        tim4_ch2            = 30,
        tim4_ch3            = 31,
        tim4_up             = 32,
        i2c1_rx_dma         = 33,
        i2c1_tx_dma         = 34,
        i2c2_rx_dma         = 35,
        i2c2_tx_dma         = 36,
        spi1_rx_dma         = 37,
        spi1_tx_dma         = 38,
        spi2_rx_dma         = 39,
        spi2_tx_dma         = 40,
        usart1_rx_dma       = 41,
        usart1_tx_dma       = 42,
        usart2_rx_dma       = 43,
        usart2_tx_dma       = 44,
        usart3_rx_dma       = 45,
        usart3_tx_dma       = 46,
        tim8_ch1            = 47,
        tim8_ch2            = 48,
        tim8_ch3            = 49,
        tim8_ch4            = 50,
        tim8_up             = 51,
        tim8_trig           = 52,
        tim8_com            = 53,
        //54 = Reserved
        tim5_ch1            = 55,
        tim5_ch2            = 56,
        tim5_ch3            = 57,
        tim5_ch4            = 58,
        tim5_up             = 59,
        tim5_trig           = 60,
        spi3_rx_dma         = 61,
        spi3_tx_dma         = 62,
        uart4_rx_dma        = 63,
        uart4_tx_dma        = 64,
        uart5_rx_dma        = 65,
        uart5_tx_dma        = 66,
        dac_ch1_dma         = 67,
        dac_ch2_dma         = 68,
        tim6_up             = 69,
        tim7_up             = 70,
        usart6_rx_dma       = 71,
        usart6_tx_dma       = 72,
        i2c3_rx_dma         = 73,
        i2c3_tx_dma         = 74,
        dcmi_dma            = 75,
        cryp_in_dma         = 76,
        cryp_out_dma        = 77,
        hash_in_dma         = 78,
        uart7_rx_dma        = 79,
        uart7_tx_dma        = 80,
        uart8_rx_dma        = 81,
        uart8_tx_dma        = 82,
        spi4_rx_dma         = 83,
        spi4_tx_dma         = 84,
        spi5_rx_dma         = 85,
        spi5_tx_dma         = 86,
        sai1a_dma           = 87,
        sai1b_dma           = 88,
        sai2a_dma           = 89,
        sai2b_dma           = 90,
        swpmi_rx_dma        = 91,
        swpmi_tx_dma        = 92,
        spdifrx_dat_dma     = 93,
        spdifrx_ctrl_dma    = 94,
        hr_req_1            = 95,
        hr_req_2            = 96,
        hr_req_3            = 97,
        hr_req_4            = 98,
        hr_req_5            = 99,
        hr_req_6            = 100,
        dfsdm1_dma0         = 101,
        dfsdm1_dma1         = 102,
        dfsdm1_dma2         = 103,
        dfsdm1_dma3         = 104,
        tim15_ch1           = 105,
        tim15_up            = 106,
        tim15_trig          = 107,
        tim15_com           = 108,
        tim16_ch1           = 109,
        tim16_up            = 110,
        tim17_ch1           = 111,
        tim17_up            = 112,
        sai3_a_dma          = 113,
        sai3_b_dma          = 114,
        adc3_dma            = 115
    };
    
    enum class trigger : std::uint8_t {
        dmamux1_evt0        = 0,
        dmamux1_evt1        = 1,
        dmamux1_evt2        = 2,
        lptim1_out          = 3,
        lptim2_out          = 4,
        lptim3_out          = 5,
        extit0              = 6,
        tim12_trgo          = 7
    };

    enum class sync : std::uint8_t {
        dmamux1_evt0        = 0,
        dmamux1_evt1        = 1,
        dmamux1_evt2        = 2,
        lptim1_out          = 3,
        lptim2_out          = 4,
        lptim3_out          = 5,
        extit0              = 6,
        tim12_trgo          = 7
    };

    enum class polarity : std::uint8_t {
        noevent     = 0b00,
        rising      = 0b01,
        falling     = 0b10,
        both        = 0b11
    };

    enum class generator : std::uint32_t {
       gen0 = 0x100UL,
       gen1 = 0x104UL,
       gen2 = 0x108UL,
       gen3 = 0x10CUL,
       gen4 = 0x110UL,
       gen5 = 0x114UL,
       gen6 = 0x118UL,
       gen7 = 0x11CUL
    };

    template<channel Channel>
    class dmamux {
        private:
            DMAMUX_Channel_TypeDef * const channelHandle_ = reinterpret_cast<DMAMUX_Channel_TypeDef *>(static_cast<std::uint32_t>(DMAMUX1_BASE) + static_cast<std::uint32_t>(Channel));
            DMAMUX_ChannelStatus_TypeDef * const channelStatusHandle_ = reinterpret_cast<DMAMUX_ChannelStatus_TypeDef *>(static_cast<std::uint32_t>(DMAMUX1_ChannelStatus_BASE));

            constexpr auto getChannelIdx_() const {
                return static_cast<std::uint32_t>(Channel)/4;
            }

        public:
            dmamux(request request, std::uint8_t numreq = 1, bool syncenable = false, sync sync = sync::dmamux1_evt0, polarity polarity = polarity::noevent, bool eventenable = false, bool intenable = false) {
                //Write the channel configuration
                reg::write(std::ref(channelHandle_->CCR),
                    ((static_cast<std::uint8_t>(request) & 0b01111111) << DMAMUX_CxCR_DMAREQ_ID_Pos) |
                    ((static_cast<std::uint8_t>(intenable) & 0b1) << DMAMUX_CxCR_SOIE_Pos) | 
                    ((static_cast<std::uint8_t>(eventenable) & 0b1) << DMAMUX_CxCR_EGE_Pos) |
                    ((static_cast<std::uint8_t>(syncenable) & 0b1) << DMAMUX_CxCR_SE_Pos) |
                    ((static_cast<std::uint8_t>(polarity) & 0b11) << DMAMUX_CxCR_SPOL_Pos) |
                    (((static_cast<std::uint8_t>(numreq) - 1) & 0b11111) << DMAMUX_CxCR_NBREQ_Pos) |
                    ((static_cast<std::uint8_t>(sync) & 0b111) << DMAMUX_CxCR_SYNC_ID_Pos) 
                );
            }

            void setNumReq(uint8_t number) const {
                reg::change(std::ref(channelHandle_->CCR), 0x1F, (static_cast<std::uint8_t>(number) - 1), DMAMUX_CxCR_NBREQ_Pos);
            }

            void enableSync() const {
                reg::set(std::ref(channelHandle_->CCR), DMAMUX_CxCR_SE);
            }

            void disableSync() const {
                reg::clear(std::ref(channelHandle_->CCR), DMAMUX_CxCR_SE);
            }

            void setPolarity(polarity polarity) const {
                reg::change(std::ref(channelHandle_->CCR), 0x03, static_cast<std::uint8_t>(polarity), DMAMUX_CxCR_SPOL_Pos);
            }

            void enableEvent() const {
                reg::set(std::ref(channelHandle_->CCR), DMAMUX_CxCR_EGE);
            }

            void disableEvent() const {
                reg::clear(std::ref(channelHandle_->CCR), DMAMUX_CxCR_EGE);
            }

            void enableInterrupt() const {
                reg::set(std::ref(channelHandle_->CCR), DMAMUX_CxCR_SOIE);
            }

            void disableInterrupt() const {
                reg::clear(std::ref(channelHandle_->CCR), DMAMUX_CxCR_SOIE);
            }

            bool getInterruptFlag() const {
                return static_cast<bool>(reg::read(std::ref(channelStatusHandle_->CSR), 0b1 << getChannelIdx_()));
            }

            void clearInterruptFlag() const {
                reg::write(std::ref(channelStatusHandle_->CSR), 0b1 << getChannelIdx_());
            }
        };

        template<generator Generator>
        class reqgen {
            private:
                DMAMUX_RequestGen_TypeDef * const reqgenHandle_ = reinterpret_cast<DMAMUX_RequestGen_TypeDef *>(static_cast<std::uint32_t>(DMAMUX1_BASE) + static_cast<std::uint32_t>(Generator));
                DMAMUX_RequestGenStatus_TypeDef * const reqgenStatusHandle_ = reinterpret_cast<DMAMUX_RequestGenStatus_TypeDef *>(static_cast<std::uint32_t>(DMAMUX1_RequestGenStatus_BASE));

                constexpr auto getGenIdx_() const {
                    return (static_cast<std::uint32_t>(Generator) - 0x100UL) / 4;
                }

            public:
                reqgen(trigger trigger, polarity polarity, std::uint8_t numreq = 1, bool intenable = false) {                   
                    //Write the channel configuration
                    reg::write(std::ref(reqgenHandle_->RGCR), 
                        ((static_cast<std::uint8_t>(trigger) & 0b111) << DMAMUX_RGxCR_SIG_ID_Pos) |
                        ((static_cast<std::uint8_t>(intenable) & 0b1) << DMAMUX_RGxCR_OIE_Pos) | 
                        //Generator is not enabled here
                        ((static_cast<std::uint8_t>(polarity) & 0b11) << DMAMUX_RGxCR_GPOL_Pos) |
                        (((static_cast<std::uint8_t>(numreq) - 1) & 0b11111) << DMAMUX_RGxCR_GNBREQ_Pos) 
                    );
                }

                void enable() const {
                    reg::set(std::ref(reqgenHandle_->RGCR), DMAMUX_RGxCR_GE);
                }

                void disable() const {
                    reg::clear(std::ref(reqgenHandle_->RGCR), DMAMUX_RGxCR_GE);
                }

                void enableInterrupt() const {
                    reg::set(std::ref(reqgenHandle_->RGCR), DMAMUX_RGxCR_OIE);
                }

                void disableInterrupt() const {
                    reg::clear(std::ref(reqgenHandle_->RGCR), DMAMUX_RGxCR_OIE);
                }

                bool getInterruptFlag() const {
                    return static_cast<bool>(reg::read(std::ref(reqgenStatusHandle_->RGSR), 0b1 << getGenIdx_()));
                }

                void clearInterruptFlag() const {
                    reg::write(std::ref(reqgenStatusHandle_->RGCFR), 0b1 << getGenIdx_());
                }
    };
}

namespace stmcpp::dmamux2 {

    enum class channel : std::uint32_t {
        channel0    = 0x000UL,
        channel1    = 0x004UL,
        channel2    = 0x008UL,
        channel3    = 0x00CUL,
        channel4    = 0x010UL,
        channel5    = 0x014UL,
        channel6    = 0x018UL,
        channel7    = 0x01CUL
    };  

    enum class request : std::uint8_t {
        dmamux2_req_gen0    = 1,
        dmamux2_req_gen1    = 2,
        dmamux2_req_gen2    = 3,
        dmamux2_req_gen3    = 4,
        dmamux2_req_gen4    = 5,
        dmamux2_req_gen5    = 6,
        dmamux2_req_gen6    = 7,
        dmamux2_req_gen7    = 8,
        lpuart1_rx_dma      = 9,
        lpuart1_tx_dma      = 10,
        spi6_rx_dma         = 11,
        spi6_tx_dma         = 12,
        i2c4_rx_dma         = 13,
        i2c4_tx_dma         = 14,
        sai4_a_dma          = 15,
        sai4_b_dma          = 16,
        adc3_dma            = 17
    };
    
    enum class trigger : std::uint8_t {
        dmamux2_evt0        = 0,
        dmamux2_evt1        = 1,
        dmamux2_evt2        = 2,
        dmamux2_evt3        = 3,
        dmamux2_evt4        = 4,
        dmamux2_evt5        = 5,
        dmamux2_evt6        = 6,
        lpuart_rx_wkup      = 7,
        lpuart_tx_wkup      = 8,
        lptim2_wkup         = 9,
        lptim2_out          = 10,
        lptim3_wkup         = 11,
        lptim3_out          = 12,
        lptim4_ait          = 13,
        lptim5_ait          = 14,
        i2c4_wkup           = 15,
        spi6_wkup           = 16,
        comp1_out           = 17,
        comp2_out           = 18,
        rtc_wkup            = 19,
        syscfg_exti0_mux    = 20,
        syscfg_exti2_mux    = 21,
        i2c4_event_it       = 22,
        spi6_it             = 23,
        lpuart1_it_t        = 24,
        lpuart1_it_r        = 25,
        adc3_it             = 26,
        adc3_awd1           = 27,
        bdma_ch0_it         = 28,
        bdma_ch1_it         = 29
    };

    enum class sync : std::uint8_t {
        dmamux2_evt0        = 0,
        dmamux2_evt1        = 1,
        dmamux2_evt2        = 2,
        dmamux2_evt3        = 3,
        dmamux2_evt4        = 4,
        dmamux2_evt5        = 5,
        lpuart_rx_wkup      = 6,
        lpuart_tx_wkup      = 7,
        lptim2_out          = 8,
        lptim3_out          = 9,
        i2c4_wkup           = 10,
        spi6_wkup           = 11,
        comp1_out           = 12,
        rtc_wkup            = 13,
        syscfg_exti0_mux    = 14,
        syscfg_exti2_mux    = 15
    };

    enum class polarity : std::uint8_t {
        noevent     = 0b00,
        rising      = 0b01,
        falling     = 0b10,
        both        = 0b11
    };

    enum class generator : std::uint32_t {
       gen0 = 0x100UL,
       gen1 = 0x104UL,
       gen2 = 0x108UL,
       gen3 = 0x10CUL,
       gen4 = 0x110UL,
       gen5 = 0x114UL,
       gen6 = 0x118UL,
       gen7 = 0x11CUL
    };

    template<channel Channel>
    class dmamux {
        private:
            DMAMUX_Channel_TypeDef * const channelHandle_ = reinterpret_cast<DMAMUX_Channel_TypeDef *>(static_cast<uint32_t>(DMAMUX2_BASE) + static_cast<std::uint32_t>(Channel));
            DMAMUX_ChannelStatus_TypeDef * const channelStatusHandle_ = reinterpret_cast<DMAMUX_ChannelStatus_TypeDef *>(static_cast<std::uint32_t>(DMAMUX2_ChannelStatus_BASE));

            constexpr auto getChannelIdx_() const {
                return static_cast<std::uint32_t>(Channel)/4;
            }
            
        public:
            dmamux(request request, std::uint8_t numreq = 1, bool syncenable = false, sync sync = sync::dmamux2_evt0, polarity polarity = polarity::noevent, bool eventenable = false, bool intenable = false) {
                //Write the channel configuration
                reg::write(std::ref(channelHandle_->CCR),
                    ((static_cast<std::uint8_t>(request) & 0b01111111) << DMAMUX_CxCR_DMAREQ_ID_Pos) |
                    ((static_cast<std::uint8_t>(intenable) & 0b1) << DMAMUX_CxCR_SOIE_Pos) | 
                    ((static_cast<std::uint8_t>(eventenable) & 0b1) << DMAMUX_CxCR_EGE_Pos) |
                    ((static_cast<std::uint8_t>(syncenable) & 0b1) << DMAMUX_CxCR_SE_Pos) |
                    ((static_cast<std::uint8_t>(polarity) & 0b11) << DMAMUX_CxCR_SPOL_Pos) |
                    (((static_cast<std::uint8_t>(numreq) - 1) & 0b11111) << DMAMUX_CxCR_NBREQ_Pos) |
                    ((static_cast<std::uint8_t>(sync) & 0b111) << DMAMUX_CxCR_SYNC_ID_Pos) 
                );
            }

            void setNumReq(std::uint8_t number) const {
                reg::change(std::ref(channelHandle_->CCR), 0x1F, (static_cast<std::uint8_t>(number) - 1), DMAMUX_CxCR_NBREQ_Pos);
            }

            void enableSync() const {
                reg::set(std::ref(channelHandle_->CCR), DMAMUX_CxCR_SE);
            }

            void disableSync() const {
                reg::clear(std::ref(channelHandle_->CCR), DMAMUX_CxCR_SE);
            }

            void setPolarity(polarity polarity) const {
                reg::change(std::ref(channelHandle_->CCR), 0x03, static_cast<std::uint8_t>(polarity), DMAMUX_CxCR_SPOL_Pos);
            }

            void enableEvent() const {
                reg::set(std::ref(channelHandle_->CCR), DMAMUX_CxCR_EGE);
            }

            void disableEvent() const {
                reg::clear(std::ref(channelHandle_->CCR), DMAMUX_CxCR_EGE);
            }

            void enableInterrupt() const {
                reg::set(std::ref(channelHandle_->CCR), DMAMUX_CxCR_SOIE);
            }

            void disableInterrupt() const {
                reg::clear(std::ref(channelHandle_->CCR), DMAMUX_CxCR_SOIE);
            }

            bool getInterruptFlag() const {
                return static_cast<bool>(reg::read(std::ref(channelStatusHandle_->CSR), 0b1 << getChannelIdx_()));
            }

            void clearInterruptFlag() const {
                reg::write(std::ref(channelStatusHandle_->CSR), 0b1 << getChannelIdx_());
            }
    };

    template<generator Generator>
        class reqgen{
            private:
                DMAMUX_RequestGen_TypeDef * const reqgenHandle_ = reinterpret_cast<DMAMUX_RequestGen_TypeDef *>(static_cast<std::uint32_t>(DMAMUX2_BASE) + static_cast<std::uint32_t>(Generator));
                DMAMUX_RequestGenStatus_TypeDef * const reqgenStatusHandle_ = reinterpret_cast<DMAMUX_RequestGenStatus_TypeDef *>(static_cast<std::uint32_t>(DMAMUX2_RequestGenStatus_BASE));

                constexpr auto getGenIdx_() const {
                    return (static_cast<std::uint32_t>(Generator) - 0x100UL) / 4;
                }

            public:
                reqgen(trigger trigger, polarity polarity, uint8_t numreq = 1, bool intenable = false){                   
                    //Write the channel configuration
                    reg::write(std::ref(reqgenHandle_->RGCR), 
                        ((static_cast<std::uint8_t>(trigger) & 0b111) << DMAMUX_RGxCR_SIG_ID_Pos) |
                        ((static_cast<std::uint8_t>(intenable) & 0b1) << DMAMUX_RGxCR_OIE_Pos) | 
                        //Generator is not enabled here
                        ((static_cast<std::uint8_t>(polarity) & 0b11) << DMAMUX_RGxCR_GPOL_Pos) |
                        (((static_cast<std::uint8_t>(numreq) - 1) & 0b11111) << DMAMUX_RGxCR_GNBREQ_Pos) 
                    );
                }

                void enable() const {
                    reg::set(std::ref(reqgenHandle_->RGCR), DMAMUX_RGxCR_GE);
                }

                void disable() const {
                    reg::clear(std::ref(reqgenHandle_->RGCR), DMAMUX_RGxCR_GE);
                }

                void enableInterrupt() const {
                    reg::set(std::ref(reqgenHandle_->RGCR), DMAMUX_RGxCR_OIE);
                }

                void disableInterrupt() const {
                    reg::clear(std::ref(reqgenHandle_->RGCR), DMAMUX_RGxCR_OIE);
                }

                bool getInterruptFlag() const {
                    return static_cast<bool>(reg::read(std::ref(reqgenStatusHandle_->RGSR), 0b1 << getGenIdx_()));
                }

                void clearInterruptFlag() const {
                    reg::write(std::ref(reqgenStatusHandle_->RGCFR), 0b1 << getGenIdx_());
                }
    };
}


#endif
