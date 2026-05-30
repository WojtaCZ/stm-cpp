/*
 * STM32G0-compatible ADC abstraction matching the stmcpp library style.
 * Used instead of stmcpp/adc.hpp which contains H7-specific register names.
 */

#ifndef STMCPP_ADC_G0_H
#define STMCPP_ADC_G0_H

#include <cstdint>
#include <array>

#include <stmcpp/register.hpp>
#include <stmcpp/register_waits.hpp>
#include <stmcpp/error.hpp>
#include <stmcpp/stmheader.hpp>

namespace stmcpp::adc {
    using namespace stmcpp;

    enum class error {
        calibration_timeout,
        enable_timeout,
        conversion_timeout
    };

    static stmcpp::error::handler<error, "stmcpp::adc_g0"> errorHandler;

    enum class peripheral : uint32_t {
        adc1 = ADC1_BASE
    };

    // Resolution (ADC_CFGR1 RES bits [4:3])
    enum class resolution : uint8_t {
        twelveBit = 0b00,
        tenBit    = 0b01,
        eightBit  = 0b10,
        sixBit    = 0b11
    };

    // Sampling time for SMP1/SMP2 fields
    enum class samplingTime : uint8_t {
        oneAndHalfClocks          = 0b000,
        threeAndHalfClocks        = 0b001,
        sevenAndHalfClocks        = 0b010,
        twelveAndHalfClocks       = 0b011,
        nineteenAndHalfClocks     = 0b100,
        thirtyNineAndHalfClocks   = 0b101,
        seventyNineAndHalfClocks  = 0b110,
        hundredSixtyAndHalfClocks = 0b111
    };

    // ADC clock source (CFGR2 CKMODE bits [31:30])
    enum class clockMode : uint8_t {
        asyncKernelClock = 0b00,
        pclkDiv2         = 0b01,
        pclkDiv4         = 0b10
    };

    class channel {
        private:
            uint8_t number_;
            samplingTime samplingTime_;

        public:
            constexpr channel(uint8_t number, samplingTime sampleTime)
                : number_(number), samplingTime_(sampleTime) {}

            constexpr uint8_t getNumber() const { return number_; }
            constexpr samplingTime getSamplingTime() const { return samplingTime_; }
    };

    template<peripheral Peripheral>
    class adc {
        private:
            ADC_TypeDef * const adcHandle_ =
                reinterpret_cast<ADC_TypeDef *>(static_cast<uint32_t>(Peripheral));

        public:
            // Configure clock source, enable voltage regulator, run calibration.
            // STM32G071 has no DEEPPWD or ADCALDIF bits.
            adc(clockMode clk = clockMode::pclkDiv4, resolution res = resolution::twelveBit) {
                // Clock source must be set before enabling the ADC
                reg::change(std::ref(adcHandle_->CFGR2),
                    (0b11U << ADC_CFGR2_CKMODE_Pos),
                    (static_cast<uint32_t>(clk) << ADC_CFGR2_CKMODE_Pos));

                // Enable internal voltage regulator
                reg::set(std::ref(adcHandle_->CR), ADC_CR_ADVREGEN);
                // Wait for regulator startup (~20 us; at 64 MHz ~1280 cycles suffice)
                for (volatile uint32_t i = 0; i < 1280; i++) {}

                // Single-ended calibration (G0 has no differential calibration)
                reg::set(std::ref(adcHandle_->CR), ADC_CR_ADCAL);
                reg::waitForBitClear(std::ref(adcHandle_->CR), ADC_CR_ADCAL,
                    []() { errorHandler.hardThrow(error::calibration_timeout); });

                // Resolution + keep last value on overrun
                reg::write(std::ref(adcHandle_->CFGR1),
                    (static_cast<uint32_t>(res) << ADC_CFGR1_RES_Pos) |
                    ADC_CFGR1_OVRMOD);
            }

            // Configure channel selection and sampling time.
            // With CHSELRMOD=0 (default) and SCANDIR=0, channels are converted in
            // ascending channel-number order.
            template<size_t N>
            void setupRegularSequence(const std::array<channel, N>& seq) const {
                static_assert(N <= 18, "STM32G0 ADC supports at most 18 channels");

                reg::write(std::ref(adcHandle_->CHSELR), 0U);
                reg::write(std::ref(adcHandle_->SMPR), 0U);

                for (size_t i = 0; i < N; i++) {
                    uint8_t ch = seq[i].getNumber();
                    // Add channel to the bitfield selection register
                    reg::set(std::ref(adcHandle_->CHSELR), (1U << ch));
                    // All channels share SMP1 (SMPSEL bit = 0 → use SMP1)
                    reg::change(std::ref(adcHandle_->SMPR),
                        (0b111U << ADC_SMPR_SMP1_Pos),
                        (static_cast<uint32_t>(seq[i].getSamplingTime()) << ADC_SMPR_SMP1_Pos));
                }
            }

            void enable() const {
                reg::set(std::ref(adcHandle_->ISR), ADC_ISR_ADRDY);
                reg::set(std::ref(adcHandle_->CR), ADC_CR_ADEN);
                reg::waitForBitSet(std::ref(adcHandle_->ISR), ADC_ISR_ADRDY,
                    []() { errorHandler.hardThrow(error::enable_timeout); });
            }

            void disable() const {
                if (reg::read(std::ref(adcHandle_->CR), ADC_CR_ADSTART)) {
                    reg::set(std::ref(adcHandle_->CR), ADC_CR_ADSTP);
                    reg::waitForBitClear(std::ref(adcHandle_->CR), ADC_CR_ADSTP,
                        []() { errorHandler.hardThrow(error::conversion_timeout); });
                }
                reg::set(std::ref(adcHandle_->CR), ADC_CR_ADDIS);
                reg::waitForBitClear(std::ref(adcHandle_->CR), ADC_CR_ADEN,
                    []() { errorHandler.hardThrow(error::enable_timeout); });
            }

            // Start one single-shot conversion and return the result.
            uint16_t convert() const {
                reg::set(std::ref(adcHandle_->CR), ADC_CR_ADSTART);
                reg::waitForBitSet(std::ref(adcHandle_->ISR), ADC_ISR_EOC,
                    []() { errorHandler.hardThrow(error::conversion_timeout); });
                return static_cast<uint16_t>(adcHandle_->DR); // reading DR clears EOC
            }

            // Scan through N channels in sequence and return all results.
            // N must match the number of channels configured in setupRegularSequence().
            template<size_t N>
            std::array<uint16_t, N> convertAll() const {
                std::array<uint16_t, N> results{};
                reg::set(std::ref(adcHandle_->CR), ADC_CR_ADSTART);
                for (size_t i = 0; i < N; i++) {
                    reg::waitForBitSet(std::ref(adcHandle_->ISR), ADC_ISR_EOC,
                        []() { errorHandler.hardThrow(error::conversion_timeout); });
                    results[i] = static_cast<uint16_t>(adcHandle_->DR);
                }
                return results;
            }

            bool isConverting() const {
                return static_cast<bool>(reg::read(std::ref(adcHandle_->CR), ADC_CR_ADSTART));
            }
    };

}

#endif
