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

#ifndef STMCPP_ADC_H
#define STMCPP_ADC_H

#include <stmcpp/stmheader.hpp>
#include <cstdint>

#include <stmcpp/register.hpp>
#include <stmcpp/units.hpp>
#include <stmcpp/clock.hpp>
#include <stmcpp/error.hpp>


namespace stmcpp::adc {
    using namespace stmcpp;
    using namespace stmcpp::units;

    enum class error {
        ldo_timeout,
        calibration_timeout,
        enable_timeout,
        disable_timeout,
        regular_stop_timeout,
        injected_stop_timeout,
        channel_out_of_range,
        sequence_out_of_range,
        trigger_event_out_of_range,
        timeout
    };

    static stmcpp::error::handler<error, "stmcpp::adc"> errorHandler;
    
    enum class peripheral : std::uint32_t {
        adc1 = ADC1_BASE,
        adc2 = ADC2_BASE,
        adc3 = ADC3_BASE
    };

    enum class interrupt : std::uint32_t {
        adcReady = ADC_IER_ADRDYIE_Msk,
        endOfSampling = ADC_IER_EOSMPIE_Msk,
        endOfConversion = ADC_IER_EOCIE_Msk,
        endOfSequence = ADC_IER_EOSIE_Msk,
        overrun = ADC_IER_OVRIE_Msk,
        endOfInjectedConversion = ADC_IER_JEOCIE_Msk,
        endOfInjectedSequence = ADC_IER_JEOSIE_Msk,
        analogWatchdog1 = ADC_IER_AWD1IE_Msk,
        analogWatchdog2 = ADC_IER_AWD2IE_Msk,
        analogWatchdog3 = ADC_IER_AWD3IE_Msk,
        injectedContextQueueOverflow = ADC_IER_JQOVFIE_Msk
    };

    enum class status : std::uint32_t {
        ldoReady = ADC_ISR_LDORDY_Msk
    };

    enum class calibration {
        singleEnded,
        differential
    };

    enum class resolution {
        eightBit = 0b111,
        tenBit = 0b011,
        twelweBit = 0b010,
        fourteenBit = 0b001,
        sixteenBit = 0b000
    };

    enum class hardwareTrigEdge {
        none = 0b00,
        rising = 0b01,
        falling = 0b10,
        both = 0b11
    };

    enum class dataManegment {
        storeInDR = 0b00,
        oneShotDMA = 0b01,
        DFSDM = 0b10,
        circularDMA = 0b11
    };

    class channel {
        public:
            enum class samplingTime {
                oneAndHalfClocks = 0b000,
                twoAndHalfClocks = 0b001,
                eightAndHalfClocks = 0b010,
                sixteenAndHalfClocks = 0b011,
                thirtyTwoAndHalfClocks = 0b100,
                sixtyFourAndHalfClocks = 0b101,
                threeHundretEightySevenAndHalfClocks = 0b110,
                eightHundretTenAndHalfClocks = 0b111

            };

        private:
            uint8_t number_;
            samplingTime samplingTime_;
            
        public:
            constexpr channel(uint8_t number, samplingTime samplingTime) : number_{number}, samplingTime_{samplingTime} {
                /*if (number > 19) {
                    errorHandler.hardThrow(error::channel_out_of_range);
                    return;
                }*/

                //number_ = number;
            }

            constexpr uint8_t getNumber() const {
                return number_;
            }

            constexpr samplingTime getSamplingTime() const {
                return samplingTime_;
            }
    };

    template<peripheral Peripheral>
    class adc {
        private:
            ADC_TypeDef * const adcHandle_ = reinterpret_cast<ADC_TypeDef *>(static_cast<std::uint32_t>(Peripheral));

        public:
            adc(resolution resolution, dataManegment dataManegment = dataManegment::storeInDR, bool regularOversampled = false, bool injectedOversampled = false, uint16_t oversampling = 1, uint8_t leftShift = 0, uint8_t rightShift = 0, bool boost = false, bool continuous = false, bool overrun = false) {
                // Make sure to disable deep powerdown
                reg::clear(std::ref(adcHandle_->CR), ADC_CR_DEEPPWD);
                // Enable the ADC voltage regulator
                reg::set(std::ref(adcHandle_->CR), ADC_CR_ADVREGEN);
                // Wait for it to be ready
                reg::waitForBitSet(std::ref(adcHandle_->ISR), status::ldoReady, []() { errorHandler.hardThrow(error::ldo_timeout); });

                // Enable boost mode if needed
                if (boost) reg::set(std::ref(adcHandle_->CR), ADC_CR_BOOST_Msk);

                reg::write(std::ref(adcHandle_->CFGR),
                    ((static_cast<uint8_t>(continuous) & 0b1) << ADC_CFGR_CONT_Pos) | 
                    ((static_cast<uint8_t>(overrun) & 0b1) << ADC_CFGR_OVRMOD_Pos) |
                    ((static_cast<uint8_t>(resolution) & 0b111) << ADC_CFGR_RES_Pos) |
                    ((static_cast<uint8_t>(dataManegment) & 0b11) << ADC_CFGR_DMNGT_Pos)
                );

                reg::write(std::ref(adcHandle_->CFGR2),
                    ((static_cast<uint8_t>(leftShift) & 0b1111) << ADC_CFGR2_LSHIFT_Pos) | 
                    ((static_cast<uint16_t>(oversampling-1) & 0x3FF) << ADC_CFGR2_OVSR_Pos) |
                    ((static_cast<uint8_t>(rightShift) & 0b1111) << ADC_CFGR2_OVSS_Pos) |
                    ((static_cast<uint8_t>(regularOversampled) & 0b1) << ADC_CFGR2_ROVSE_Pos) |
                    ((static_cast<uint8_t>(injectedOversampled) & 0b1) << ADC_CFGR2_JOVSE_Pos) 
                );
            }

            void enable() const {
                // Enable the ADC and wait for it to be ready
                reg::set(std::ref(adcHandle_->ISR), ADC_ISR_ADRDY);
                reg::set(std::ref(adcHandle_->CR), ADC_CR_ADEN);
                reg::waitForBitSet(std::ref(adcHandle_->ISR), interrupt::adcReady, []() { errorHandler.hardThrow(error::enable_timeout); });
            }

            void disable() const {
                // If there is a conversion ongoing, stop it
                if (reg::read(std::ref(adcHandle_->CR), ADC_CR_ADSTART | ADC_CR_JADSTART)) {
                    reg::set(std::ref(adcHandle_->CR), ADC_CR_ADSTP | ADC_CR_JADSTP);
                    reg::waitForBitClear(std::ref(adcHandle_->CR), ADC_CR_ADSTP | ADC_CR_JADSTP, []() { errorHandler.hardThrow(error::disable_timeout); });
                }

                reg::set(std::ref(adcHandle_->CR), ADC_CR_ADDIS);
                reg::waitForBitClear(std::ref(adcHandle_->CR), ADC_CR_ADEN, []() { errorHandler.hardThrow(error::disable_timeout); });
            }

            void calibrate(calibration calib, bool enableLinearity = false) const {
                // Disable the ADC
                disable();
                // Make sure to disable deep powerdown
                reg::clear(std::ref(adcHandle_->CR), ADC_CR_DEEPPWD);
                // And check if the LDO is ready
                reg::waitForBitSet(std::ref(adcHandle_->ISR), status::ldoReady, []() { errorHandler.hardThrow(error::ldo_timeout); });

                // Set up DIFF/Single Ended calibration
                if (calib == calibration::singleEnded){
                    reg::clear(std::ref(adcHandle_->CR), ADC_CR_ADCALDIF);
                } else reg::set(std::ref(adcHandle_->CR), ADC_CR_ADCALDIF);
                
                // Enable or disable linearity calibration
                if (enableLinearity) {
                    reg::set(std::ref(adcHandle_->CR), ADC_CR_ADCALLIN);
                } else reg::clear(std::ref(adcHandle_->CR), ADC_CR_ADCALLIN);

                // Start the calibration and wait for it to finish
                reg::set(std::ref(adcHandle_->CR), ADC_CR_ADCAL);
                reg::waitForBitClear(std::ref(adcHandle_->CR), ADC_CR_ADCAL, []() { errorHandler.hardThrow(error::calibration_timeout); });
            }

            void enableInterrupt(interrupt interrupt) const {
                reg::set(std::ref(adcHandle_->IER), static_cast<std::uint32_t>(interrupt));              
            }

            void disableInterrupt(interrupt interrupt) const {
                reg::clear(std::ref(adcHandle_->IER), static_cast<std::uint32_t>(interrupt));
            }

            bool getInterruptFlag(interrupt interrupt) const {
                return static_cast<bool>(reg::read(std::ref(adcHandle_->ISR), static_cast<std::uint32_t>(interrupt)));
            }

            bool getStatusFlag(status interrupt) const {
                return static_cast<bool>(reg::read(std::ref(adcHandle_->ISR), static_cast<std::uint32_t>(interrupt)));
            }

            void clearInterruptFlag(interrupt interrupt) const {
                reg::set(std::ref(adcHandle_->ISR), static_cast<std::uint32_t>(interrupt) & 0x07FD);
            }

            void startRegular() const {
                reg::set(std::ref(adcHandle_->CR), ADC_CR_ADSTART);
            }

            void stopRegular() const {
                reg::set(std::ref(adcHandle_->CR), ADC_CR_ADSTP);
                reg::waitForBitClear(std::ref(adcHandle_->CR), ADC_CR_ADSTP, []() { errorHandler.hardThrow(error::regular_stop_timeout); });
            }

            bool isConvertingRegular() const {
                return static_cast<bool>(reg::read(std::ref(adcHandle_->CR), ADC_CR_ADSTART)); 
            }

            void startInjected() const {
                reg::set(std::ref(adcHandle_->CR), ADC_CR_JADSTART);
            }

            void stopInjected() const {
                reg::set(std::ref(adcHandle_->CR), ADC_CR_JADSTP);
                reg::waitForBitClear(std::ref(adcHandle_->CR), ADC_CR_ADSTP, []() { errorHandler.hardThrow(error::injected_stop_timeout); });
            }

            bool isConvertingInjected() const {
                return static_cast<bool>(reg::read(std::ref(adcHandle_->CR), ADC_CR_JADSTART)); 
            }

            template<size_t N>
            void setupRegularSequence(const std::array<channel, N> & sequence, std::uint8_t triggerEvent = 0, hardwareTrigEdge edge = hardwareTrigEdge::none) const{
                if (N > 16) {
                    static_assert(N <= 16, "The number of channels in the sequence must be less than or equal to 16");
                    errorHandler.hardThrow(error::sequence_out_of_range);
                    return;
                }

                reg::change(std::ref(adcHandle_->CFGR), 0b11, static_cast<uint8_t>(edge), ADC_CFGR_EXTEN_Pos);
                reg::change(std::ref(adcHandle_->CFGR), 0b11111, static_cast<uint8_t>(triggerEvent), ADC_CFGR_EXTSEL_Pos);

                // Set up the number of channels in the sequence
                reg::write(std::ref(adcHandle_->SQR1), sequence.size());

                reg::write(std::ref(adcHandle_->PCSEL), 0);

                for(size_t i = 0; i < sequence.size(); i++) {
                    channel::samplingTime channelSamplingTime_ = sequence.at(i).getSamplingTime();
                    std::uint8_t  channelNumber_ = sequence.at(i).getNumber();

                    if (i < 4){
                        reg::change(std::ref(adcHandle_->SQR1), 0b11111, channelNumber_, (i + 1) * 6);
                    } else if (i < 9) {
                        reg::change(std::ref(adcHandle_->SQR2), 0b11111, channelNumber_, (i - 4) * 6);
                    } else if (i < 14) {
                        reg::change(std::ref(adcHandle_->SQR3), 0b11111, channelNumber_, (i - 9) * 6);
                    } else {
                        reg::change(std::ref(adcHandle_->SQR4), 0b11111, channelNumber_, (i - 14) * 6);
                    }

                    if (channelNumber_ < 10) {
                        reg::change(std::ref(adcHandle_->SMPR1), 0b111, static_cast<uint8_t>(channelSamplingTime_), channelNumber_ * 3);
                    } else {
                        reg::change(std::ref(adcHandle_->SMPR2), 0b111, static_cast<uint8_t>(channelSamplingTime_), (channelNumber_ - 10) * 3);
                    } 

                    reg::set(std::ref(adcHandle_->PCSEL), 0b1, channelNumber_);

                }
            }

            void setupInjectedSequence(std::vector<channel> & sequence, std::uint8_t triggerEvent = 0, hardwareTrigEdge edge = hardwareTrigEdge::none){
                
                reg::write(std::ref(adcHandle_->PCSEL), 0);

                if (sequence.size() > 4) {
                    errorHandler.hardThrow(error::sequence_out_of_range);
                    return;
                }

                if (triggerEvent > 31) {
                    errorHandler.hardThrow(error::trigger_event_out_of_range);
                    return;
                }

                // Set up the number of channels in the sequence and trigger event
                reg::write(std::ref(adcHandle_->JSQR), 0b11, sequence.size() | (triggerEvent << ADC_JSQR_JEXTSEL_Pos) | (static_cast<uint8_t>(edge) << ADC_JSQR_JEXTEN_Pos));
                for(int i = 0; i < sequence.size(); i++) {
                    reg::change(std::ref(adcHandle_->JSQR), 0b11111, sequence.at(i).getNumber(), (i * 6) + 9);
                    reg::set(std::ref(adcHandle_->PCSEL), 0b1, sequence.at(i).getNumber());
                }
            }


    };
}

#endif