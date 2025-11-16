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

#ifndef STMCPP_UNITS_H
#define STMCPP_UNITS_H

#include <cstdint>
#include <cstddef>
#include <cmath>


#include <cstdint>
#include <limits>

#include <type_traits>
#include <cstdint>

namespace stmcpp::units {

	template<typename StorageType, typename ConcreteType>
	struct unit {
        using StorageType_ = StorageType;
        StorageType_ raw_ = 0;

        constexpr ConcreteType operator+ (ConcreteType rhs) const { 
            return { static_cast<StorageType_>(raw_ + rhs.raw_) }; 
        }

        constexpr ConcreteType& operator+= (ConcreteType rhs) { 
            raw_ += rhs.raw_;
            return static_cast<ConcreteType&>(*this);
        }
        
        constexpr ConcreteType operator- () const { 
            return { static_cast<StorageType_>(-raw_) };
        }        

        constexpr ConcreteType operator- (ConcreteType rhs) const {
            return { static_cast<StorageType_>(raw_ - rhs.raw_) };
        }

        constexpr ConcreteType& operator-= (ConcreteType rhs) { 
            raw_ -= rhs.raw_;
            return static_cast<ConcreteType&>(*this);
        }

        template<typename Scalar>
        constexpr friend ConcreteType operator* (Scalar s, unit q) {
            return { static_cast<StorageType_>(q.raw_ * s) };
        }

        template<typename Scalar>
        constexpr friend ConcreteType operator* (unit q, Scalar s) {
            return { static_cast<StorageType_>(q.raw_ * s) };
        }

        template<typename Scalar> 
        constexpr ConcreteType& operator*=(Scalar s) {
            raw_ *= s;
            return static_cast<ConcreteType&>(*this);
        }

        template<typename Scalar>
        constexpr std::enable_if_t<std::is_arithmetic_v<Scalar>, ConcreteType> operator/ (Scalar s) const {
            return { static_cast<StorageType_>(raw_ / s) };
        }
        
        constexpr float operator/ (unit q) const {
            return raw_ / static_cast<float>(q.raw_);
        }

        template<typename Scalar>
        constexpr ConcreteType& operator/=(Scalar s) {
            raw_ /= s;
            return static_cast<ConcreteType&>(*this);
        }

        constexpr friend bool operator<     (ConcreteType lhs, ConcreteType rhs) { return lhs.raw_ < rhs.raw_; }
        constexpr friend bool operator>     (ConcreteType lhs, ConcreteType rhs) { return lhs.raw_ > rhs.raw_; }
        constexpr friend bool operator==    (ConcreteType lhs, ConcreteType rhs) { return lhs.raw_ == rhs.raw_; }
        constexpr friend bool operator!=    (ConcreteType lhs, ConcreteType rhs) { return lhs.raw_ != rhs.raw_; }
        constexpr friend bool operator>=    (ConcreteType lhs, ConcreteType rhs) { return lhs.raw_ >= rhs.raw_; }
        constexpr friend bool operator<=    (ConcreteType lhs, ConcreteType rhs) { return lhs.raw_ <= rhs.raw_; }
	};

    /*
        Current unit (stored in uA or pA)
    */

    // Change the storage type based on the range required
    #ifdef STMCPP_UNITS_VOLTAGE_HIGHRANGE
        using voltageStorage_ = std::int64_t; // Allow for Megavolt to Picovolt range (value stored in picovolts)
        static constexpr auto voltageScaleFactor_ = 1'000'000;
    #else
        using voltageStorage_ = std::int32_t; // Allow for Kilovolt to Microvolt range (value stored in microvolts)
        static constexpr auto voltageScaleFactor_ = 1;
    #endif

	struct voltage : public unit<voltageStorage_, voltage> {
		template<typename T>
		static constexpr voltage fromVolts(T volts) {
            return { static_cast<StorageType_>(volts * (1'000'000ll * voltageScaleFactor_)) };
        }

        template<typename T>
		static constexpr voltage fromMilliVolts(T millivolts) {
            return { static_cast<StorageType_>(millivolts * (1'000 * voltageScaleFactor_)) };
        }

        template<typename T>
		static constexpr voltage fromMicroVolts(T microvolts) {
            return { static_cast<StorageType_>(microvolts * voltageScaleFactor_) };
        }

        constexpr float toMicroVolts() const {
            return raw_ / (1.0f * voltageScaleFactor_);
        }

		constexpr float toMilliVolts() const {
            return raw_ / (1'000.0f * voltageScaleFactor_);
        }

		constexpr float toVolts() const {
            return raw_ / (1'000'000.0f * voltageScaleFactor_);
        }

        #ifdef STMCPP_UNITS_VOLTAGE_HIGHRANGE
            template<typename T>
            static constexpr voltage fromKiloVolts(T kilovolts) {
                return { static_cast<StorageType_>(kilovolts * (1'000'000'000ll * voltageScaleFactor_)) };
            }

            template<typename T>
            static constexpr voltage fromNanoVolts(T nanovolts) {
                return { static_cast<StorageType_>(nanovolts * (voltageScaleFactor_ / 1'000)) };
            }

            template<typename T>
            static constexpr voltage fromPicoVolts(T picovolts) {
                return { static_cast<StorageType_>(picovolts *  (voltageScaleFactor_ / 1'000'000)) };
            }

            constexpr float toKiloVolts() const {
                return raw_ / (1'000'000'000.0f * voltageScaleFactor_);
            }

            constexpr float toNanoVolts() const {
                return raw_ / (1.0f * (voltageScaleFactor_ / 1'000ll));
            }

            constexpr StorageType_ toPicoVolts() const {
                return raw_;
            }
        #endif

	};

    constexpr voltage operator""_V(unsigned long long voltage) { return voltage::fromVolts(voltage); }
    constexpr voltage operator""_mV(unsigned long long voltage) { return voltage::fromMilliVolts(voltage); }
    constexpr voltage operator""_uV(unsigned long long voltage) { return voltage::fromMicroVolts(voltage); }
    
    static_assert((1_V + 111_mV) - 1111000_uV == 0_V, "Voltage units check.");

    #ifdef STMCPP_UNITS_VOLTAGE_HIGHRANGE
        constexpr voltage operator""_kV(unsigned long long voltage) { return voltage::fromKiloVolts(voltage); }
        constexpr voltage operator""_nV(unsigned long long voltage) { return voltage::fromNanoVolts(voltage); }
        constexpr voltage operator""_pV(unsigned long long voltage) { return voltage::fromPicoVolts(voltage); }

        static_assert((123_kV + 456_V + 789_mV + 123_uV + 456_nV) - 123456789123456000_pV == 0_V, "Voltage units check.");
    #endif

    /*
        Current unit (stored in uA or pA)
    */

    // Change the storage type based on the range required
    #ifdef STMCPP_UNITS_CURRENT_HIGHRANGE
        using currentStorage_ = std::int64_t; // Allow for Megaampere to Picoampere range (value stored in picoampere)
        static constexpr auto currentScaleFactor_ = 1'000'000;
    #else
        using currentStorage_ = std::int32_t; // Allow for Kiloampere to Microampere range (value stored in microampere)
        static constexpr auto currentScaleFactor_ = 1;
    #endif

	struct current : public unit<currentStorage_, current> {
		template<typename T>
		static constexpr current fromAmperes(T amperes) {
            return { static_cast<StorageType_>(amperes * (1'000'000ll * currentScaleFactor_)) };
        }

        template<typename T>
		static constexpr current fromMilliAmperes(T milliamperes) {
            return { static_cast<StorageType_>(milliamperes * (1'000 * currentScaleFactor_)) };
        }

        template<typename T>
		static constexpr current fromMicroAmperes(T microamperes) {
            return { static_cast<StorageType_>(microamperes * currentScaleFactor_) };
        }

        constexpr float toMicroAmperes() const {
            return raw_ / (1.0f * currentScaleFactor_);
        }

		constexpr float toMilliAmperes() const {
            return raw_ / (1'000.0f * currentScaleFactor_);
        }

		constexpr float toAmperes() const {
            return raw_ / (1'000'000.0f * currentScaleFactor_);
        }

        #ifdef STMCPP_UNITS_CURRENT_HIGHRANGE
            template<typename T>
            static constexpr current fromKiloAmperes(T kiloamperes) {
                return { static_cast<StorageType_>(kiloamperes * (1'000'000'000ll * currentScaleFactor_)) };
            }

            template<typename T>
            static constexpr current fromNanoAmperes(T nanoamperes) {
                return { static_cast<StorageType_>(nanoamperes * (currentScaleFactor_ / 1'000)) };
            }

            template<typename T>
            static constexpr current fromPicoAmperes(T picoamperes) {
                return { static_cast<StorageType_>(picoamperes *  (currentScaleFactor_ / 1'000'000)) };
            }

            constexpr float toKiloAmperes() const {
                return raw_ / (1'000'000'000.0f * currentScaleFactor_);
            }

            constexpr float toNanoAmperes() const {
                return raw_ / (1.0f * (currentScaleFactor_ / 1'000ll));
            }

            constexpr StorageType_ toPicoAmperes() const {
                return raw_;
            }
        #endif

	};

    constexpr current operator""_A(unsigned long long current) { return current::fromAmperes(current); }
    constexpr current operator""_mA(unsigned long long current) { return current::fromMilliAmperes(current); }
    constexpr current operator""_uA(unsigned long long current) { return current::fromMicroAmperes(current); }
    
    static_assert((123_A + 456_mA) - 123456000_uA == 0_A, "Current units check.");

    #ifdef STMCPP_UNITS_CURRENT_HIGHRANGE
        constexpr current operator""_kA(unsigned long long voltage) { return current::fromKiloAmperes(current); }
        constexpr current operator""_nA(unsigned long long voltage) { return current::fromNanoAmperes(current); }
        constexpr current operator""_pA(unsigned long long voltage) { return current::fromPicoAmperes(current); }

        static_assert((123_kA + 456_A + 789_mA + 123_uA + 456_nA) - 123456789123456000_pA == 0_A, "Current units check.");
    #endif

    /*
        Duration unit (stored in us or ps)
    */

    // Change the storage type based on the range required
    #ifdef STMCPP_UNITS_DURATION_HIGHRANGE
        using durationStorage_ = std::uint64_t; // Allow for higher range: ~213days @ 1ps resolution
        static constexpr auto durationScaleFactor_ = 1'000'000;
    #else
        using durationStorage_ = std::uint32_t; // Allow for a range of ~35minutes @ 1us resolution
        static constexpr auto durationScaleFactor_ = 1;
    #endif

    struct frequency;

	struct duration : unit<durationStorage_, duration> {
        static constexpr duration fromMicroSeconds(unsigned microseconds) {
            return { microseconds * durationScaleFactor_};
        }

		static constexpr duration fromMilliSeconds(unsigned milliseconds) {
            return { milliseconds * (1'000 * durationScaleFactor_) };
        }

		static constexpr duration fromSeconds(unsigned seconds) {
            return { seconds * (1'000'000 * durationScaleFactor_) };
        }

        constexpr float toMicroSeconds() const {
            return raw_ / (1.0f * durationScaleFactor_);
        }

		constexpr float toMilliSeconds() const {
            return raw_ / (1'000.0f * durationScaleFactor_);
        }

		constexpr float toSeconds() const {
            return raw_ / (1'000'000.0f * durationScaleFactor_);
        }

        constexpr frequency freq() const;

        #ifdef STMCPP_UNITS_DURATION_HIGHRANGE
            static constexpr duration fromNanoSeconds(unsigned nanoseconds) {
                return { microseconds * (durationScaleFactor_ / 1'000)};
            }

            static constexpr duration fromPicoSeconds(unsigned picoseconds) {
                return { microseconds * (durationScaleFactor_ / 1'000)};
            }

            constexpr float toDays() const {
                return raw_ / (24.0f * 60.0f * 60.0f * durationScaleFactor_);
            }

            constexpr float toHours() const {
                return raw_ / (60.0f * 60.0f * durationScaleFactor_);
            }

            constexpr float toMinutes() const {
                return raw_ / (60.0f * durationScaleFactor_);
            }

            constexpr float toNanoSeconds() const {
                return raw_ / (1.0f * (voltageScaleFactor_ / 1'000ll));
            }

            constexpr durationStorage_ toPicoSeconds() const {
                return raw_;
            }
        #endif
	};

    constexpr duration operator""_us(unsigned long long duration) { return duration::fromMicroSeconds(duration); }
    constexpr duration operator""_ms(unsigned long long duration) { return duration::fromMilliSeconds(duration); }
    constexpr duration operator""_s(unsigned long long duration) { return duration::fromSeconds(duration); }

    static_assert((123_s + 456_ms) - 123456000_us == 0_s, "Duration units check.");

    #ifdef STMCPP_UNITS_DURATION_HIGHRANGE
        constexpr duration operator""_ns(unsigned long long duration) { return duration::fromNanoSeconds(duration); }
        constexpr duration operator""_ps(unsigned long long duration) { return duration::fromPicoSeconds(duration); }

        static_assert((123_s + 456_ms + 789_us + 123_ns) - 123456789123000_ps == 0_s, "Duration units check.");
    #endif

    /*
        Frequency unit (stored in hertz)
    */
    struct frequency : unit<std::uint32_t, frequency> {
		template<typename T>
		static constexpr frequency fromHertz(T hertz) {
            return { static_cast<StorageType_>(hertz) };
        }

		template<typename T>
		static constexpr frequency fromKiloHertz(T kilohertz) {
            return { static_cast<StorageType_>(kilohertz * 1'000) };
        }

		template<typename T>
		static constexpr frequency fromMegaHertz(T megahertz) {
            return { static_cast<StorageType_>(megahertz * 1'000'000) };
        }

        template<typename T>
		static constexpr frequency fromGigaHertz(T gigahertz) {
            return { static_cast<StorageType_>(gigahertz * 1'000'000'000) };
        }

		constexpr StorageType_ toHertz() const { 
            return raw_;
        }

        constexpr float toKiloHertz() const { 
            return raw_ / 1'000.0f;
        }

        constexpr float toMegaHertz() const { 
            return raw_ / 1'000'000.0f;
        }

		constexpr duration period() const;
	};

    constexpr frequency operator""_Hz(unsigned long long frequency) { return frequency::fromHertz(frequency); }
    constexpr frequency operator""_kHz(unsigned long long frequency) { return frequency::fromKiloHertz(frequency); }
    constexpr frequency operator""_MHz(unsigned long long frequency) { return frequency::fromMegaHertz(frequency); }

    static_assert((123_MHz + 456_kHz) - 123456000_Hz == 0_Hz, "Frequency units check.");

    /*
        Baudrate unit (stored in baud)
    */
    struct baudrate : unit<std::uint32_t, baudrate> {
		template<typename T>
		static constexpr baudrate fromBaud(T baud) {
            return { static_cast<StorageType_>(baud) };
        }

		template<typename T>
		static constexpr baudrate fromKiloBaud(T kilobaud) {
            return { static_cast<StorageType_>(kilobaud * 1'000) };
        }

        template<typename T>
		static constexpr baudrate fromMegaBaud(T megabaud) {
            return { static_cast<StorageType_>(megabaud * 1'000'000) };
        }

		constexpr StorageType_ toBaud() const { 
            return raw_;
        }

        constexpr float toKiloBaud() const { 
            return raw_ / 1'000.0f;
        }

        constexpr float toMegaBaud() const { 
            return raw_ / 1'000'000.0f;
        }
	};

    constexpr baudrate operator""_Bd(unsigned long long baudrate) { return baudrate::fromBaud(baudrate); }
    constexpr baudrate operator""_kBd(unsigned long long baudrate) { return baudrate::fromKiloBaud(baudrate); }
    constexpr baudrate operator""_MBd(unsigned long long baudrate) { return baudrate::fromMegaBaud(baudrate); }

    static_assert((123_MBd + 456_kBd) - 123456000_Bd == 0_Bd, "Baudrate units check.");


    /*
        Functions for some unit conversions
    */
    constexpr frequency duration::freq() const { 
        return frequency::fromHertz((1'000'000 * durationScaleFactor_) / raw_);
    }

    constexpr duration frequency::period() const { 
        return duration::fromMicroSeconds(1'000'000 / raw_);
    }
}

#endif