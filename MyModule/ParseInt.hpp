/*
	MIT License

	Copyright (c) 2023 RealTimeChris

	Permission is hereby granted, free of charge, to any person obtaining a copy of this
	software and associated documentation files (the "Software"), to deal in the Software
	without restriction, including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software, and to permit
	persons to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or
	substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/
/// Note: Most of the code in this header was sampled from Glaze library: https://github.com/stephenberry/glaze
/// https://github.com/RealTimeChris/jsonifier
/// Nov 13, 2023
#pragma once

#include <type_traits>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <array>

#if defined(__clang__) || (defined(__GNUC__) && defined(__llvm__))
#define PARSEINT_PY_CLANG 1
#elif defined(_MSC_VER)
#define PARSEINT_PY_MSVC 1
#pragma warning(disable : 4820)
#pragma warning(disable : 4371)
#pragma warning(disable : 4324)
#elif defined(__GNUC__) && !defined(__clang__)
#define PARSEINT_PY_GNUCXX 1
#endif

#if defined(PARSEINT_PY_MSVC)
#define PARSEINT_PY_VISUAL_STUDIO 1
#if defined(PARSEINT_PY_CLANG)
#define PARSEINT_PY_CLANG_VISUAL_STUDIO 1
#else
#define PARSEINT_PY_REGULAR_VISUAL_STUDIO 1
#endif
#endif

#define PARSEINT_PY_GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

#if defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
#define PARSEINT_PY_MAC 1
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__gnu_linux__)
#define PARSEINT_PY_LINUX 1
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define PARSEINT_PY_WIN 1
#endif

#if defined(NDEBUG)
#if defined(PARSEINT_PY_MSVC)
#pragma warning(disable : C4820)
#define PARSEINT_PY_NO_INLINE __declspec(noinline)
#define PARSEINT_PY_FLATTEN inline [[msvc::flatten]]
#define PARSEINT_PY_ALWAYS_INLINE [[msvc::forceinline]] inline
#define PARSEINT_PY_MAYBE_ALWAYS_INLINE [[msvc::forceinline]] inline
#define PARSEINT_PY_INLINE inline
#elif defined(PARSEINT_PY_CLANG)
#define PARSEINT_PY_NO_INLINE __attribute__((__noinline__))
#define PARSEINT_PY_FLATTEN inline __attribute__((flatten))
#define PARSEINT_PY_ALWAYS_INLINE __attribute__((always_inline))
#define PARSEINT_PY_MAYBE_ALWAYS_INLINE __attribute__((always_inline))
#define PARSEINT_PY_INLINE inline
#elif defined(PARSEINT_PY_GNUCXX)
#define PARSEINT_PY_NO_INLINE __attribute__((noinline))
#define PARSEINT_PY_FLATTEN inline __attribute__((flatten))
#define PARSEINT_PY_MAYBE_ALWAYS_INLINE inline
#define PARSEINT_PY_ALWAYS_INLINE __attribute__((always_inline))
#define PARSEINT_PY_INLINE inline
#endif
#else
#define PARSEINT_PY_NO_INLINE
#define PARSEINT_PY_FLATTEN
#define PARSEINT_PY_ALWAYS_INLINE
#define PARSEINT_PY_MAYBE_ALWAYS_INLINE
#define PARSEINT_PY_INLINE
#endif

inline constexpr std::array<uint64_t, 20> powerOfTenInt{ 1ull, 10ull, 100ull, 1000ull, 10000ull, 100000ull, 1000000ull, 10000000ull, 100000000ull,
	1000000000ull, 10000000000ull, 100000000000ull, 1000000000000ull, 10000000000000ull, 100000000000000ull, 1000000000000000ull, 10000000000000000ull, 100000000000000000ull,
	1000000000000000000ull, 10000000000000000000ull };

inline constexpr std::array<uint64_t, 20> inversePowerOfTen{ 1000000000000000000ull, 100000000000000000ull, 10000000000000000ull, 1000000000000000ull,
	100000000000000ull, 10000000000000ull, 1000000000000ull, 100000000000ull, 10000000000ull, 1000000000ull, 100000000ull, 10000000ull, 1000000ull, 100000ull, 10000ull,
	1000ull, 100ull, 10ull, 1ull, 0ull };

inline constexpr uint8_t digiTypeDot{ 1 << 4 };
inline constexpr uint8_t digiTypeExp{ 1 << 5 };

inline constexpr std::array<uint8_t, 256> digiTable{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x08,
	0x10, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

PARSEINT_PY_ALWAYS_INLINE constexpr bool isSafeAddition(uint64_t a, uint64_t b) noexcept {
	return a <= (std::numeric_limits<uint64_t>::max)() - b;
}

PARSEINT_PY_ALWAYS_INLINE constexpr bool isSafeMultiplication10(uint64_t a) noexcept {
	constexpr uint64_t b = (std::numeric_limits<uint64_t>::max)() / 10;
	return a <= b;
}

inline constexpr auto eBit{ static_cast<uint8_t>('E' ^ 'e') };
inline constexpr auto f64MinDecExp{ (-324) };
inline constexpr char decimalPoint{ '.' };
inline constexpr char smallE{ 'e' };
inline constexpr char bigE{ 'E' };
inline constexpr char minus{ '-' };
inline constexpr char plus{ '+' };
inline constexpr char zero{ '0' };

inline constexpr std::array<uint64_t, 256> numberSubTable{ []() {
	std::array<uint64_t, 256> returnValues{};
	for (uint64_t x = 0; x < 256; ++x) {
		returnValues[x] = static_cast<uint64_t>(x - zero);
	}
	return returnValues;
}() };

inline constexpr std::array<bool, 256> digitTableBool{ []() {
	std::array<bool, 256> returnValues{};
	returnValues[0x30u] = true;
	returnValues[0x31u] = true;
	returnValues[0x32u] = true;
	returnValues[0x33u] = true;
	returnValues[0x34u] = true;
	returnValues[0x35u] = true;
	returnValues[0x36u] = true;
	returnValues[0x37u] = true;
	returnValues[0x38u] = true;
	returnValues[0x39u] = true;
	return returnValues;
}() };

template<bool isVolatile, std::integral value_type> PARSEINT_PY_ALWAYS_INLINE bool digiFinish(int32_t& exp, value_type& value, uint64_t& sig) {
	if (exp <= -20) [[unlikely]] {
		value = std::remove_cvref_t<decltype(value)>(0);
		return true;
	}

	value = static_cast<std::remove_cvref_t<decltype(value)>>(sig);

	if constexpr (isVolatile) {
		if (exp >= 0) {
			value = value * static_cast<value_type>(powerOfTenInt[exp]);
		}
		else {
			value = value / static_cast<value_type>(powerOfTenInt[-exp]);
		}
	}
	else {
		if (exp >= 0) {
			value *= static_cast<value_type>(powerOfTenInt[exp]);
		}
		else {
			value /= static_cast<value_type>(powerOfTenInt[-exp]);
		}
	}

	return true;
}

template<bool isVolatile, std::integral value_type>
PARSEINT_PY_ALWAYS_INLINE bool digiExpFinish(uint64_t& sig, value_type& value, int32_t& expSig, int32_t& exp) {
	if (sig == 0) {
		value = 0;
		return true;
	}
	if (expSig == 19) {
		value *= static_cast<std::remove_cvref_t<decltype(value)>>(powerOfTenInt[expSig - 1]);
		if constexpr (isVolatile) {
			value = value * static_cast<value_type>(powerOfTenInt[expSig - 1]);
			return bool(value *= 10);
		}
		else {
			value *= static_cast<value_type>(powerOfTenInt[expSig - 1]);
			return bool(value *= 10);
		}
	}
	else if (expSig >= 20) [[unlikely]] {
		return false;
	}
	exp = expSig;
	return digiFinish<isVolatile>(exp, value, sig);
};

template<bool isVolatile, std::integral value_type, typename char_type> PARSEINT_PY_ALWAYS_INLINE bool digiExpMore(bool& expSign, const char_type*& cur,
	const char_type*& tmp, value_type& value, int32_t& expSig, int32_t& expLit, uint64_t& sig, int32_t& exp) {
	expSign = (*++cur == minus);
	cur += (*cur == plus || *cur == minus);
	while (*cur == zero)
		++cur;
	tmp = cur;
	uint8_t c;
	while (uint8_t(c = uint8_t(numberSubTable[*cur])) < 10) {
		++cur;
		expLit = c + uint32_t(expLit) * 10;
	}
	if ((cur - tmp >= 6)) [[unlikely]] {
		if (sig == 0 || expSign) {
			value = 0;
			return true;
		}
		else {
			return false;
		}
	}
	expSig += expSign ? -expLit : expLit;
	return digiExpFinish<isVolatile>(sig, value, expSig, exp);
};

template<bool isVolatile, std::integral value_type, typename char_type> PARSEINT_PY_ALWAYS_INLINE bool digiFracEnd(bool& expSign, const char_type*& cur,
	const char_type*& tmp, value_type& value, int32_t& expSig, int32_t& expLit, uint64_t& sig, int32_t& exp, const char_type*& sigEnd, const char_type*& dotPos) {
	sigEnd = cur;
	expSig = -int32_t((cur - dotPos) - 1);
	if (expSig == 0) [[unlikely]]
		return false;
		if ((eBit | *cur) != smallE) [[likely]] {
			if ((expSig < f64MinDecExp - 19)) [[unlikely]] {
				value = 0;
				return true;
			}
			exp = expSig;
			return digiFinish<isVolatile>(exp, value, sig);
		}
		else {
			return digiExpMore<isVolatile>(expSign, cur, tmp, value, expSig, expLit, sig, exp);
		}
};

template<bool isVolatile, std::integral value_type, typename char_type> PARSEINT_PY_ALWAYS_INLINE bool digiStop(bool& expSign, const char_type*& cur,
	const char_type*& tmp, value_type& value, int32_t& expSig, int32_t& expLit, uint64_t& sig, int32_t& exp, const char_type*& sigEnd, const char_type*& dotPos,
	uint32_t& fracZeros, size_t currentIndex) {
	cur += currentIndex + 1 + fracZeros;
	return digiFracEnd<isVolatile>(expSign, cur, tmp, value, expSig, expLit, sig, exp, sigEnd, dotPos);
};

template<bool isVolatile, std::integral value_type, typename char_type> PARSEINT_PY_ALWAYS_INLINE bool digiFrac(bool& expSign, const char_type*& cur,
	const char_type*& tmp, value_type& value, int32_t& expSig, int32_t& expLit, uint64_t& sig, int32_t& exp, const char_type*& sigEnd, const char_type*& dotPos,
	uint32_t& fracZeros, uint64_t& numTmp, size_t currentIndex) {
	while (currentIndex < 21) {
		if (((numTmp = uint64_t(numberSubTable[cur[currentIndex + 1 + fracZeros]])) <= 9)) [[likely]] {
			sig = numTmp + sig * 10;

		}
		else {
			return digiStop<isVolatile>(expSign, cur, tmp, value, expSig, expLit, sig, exp, sigEnd, dotPos, fracZeros, currentIndex);
		}
		++currentIndex;
	}
	return true;
};

template<bool isVolatile, std::integral value_type, typename char_type> PARSEINT_PY_ALWAYS_INLINE bool digiSepr(bool& expSign, const char_type*& cur,
	const char_type*& tmp, value_type& value, int32_t& expSig, int32_t& expLit, uint64_t& sig, int32_t& exp, const char_type*& sigEnd, const char_type*& dotPos,
	uint32_t& fracZeros, uint64_t& numTmp, size_t currentIndex) {
	if (((digiTable[uint8_t(cur[currentIndex])] & (digiTypeDot | digiTypeExp)) == 0)) [[likely]] {
		cur += currentIndex;
		value = sig;
		return true;
	}
	dotPos = cur + currentIndex;
	if ((cur[currentIndex] == decimalPoint)) [[likely]] {
		if (sig == 0)
			while (cur[fracZeros + currentIndex + 1] == zero)
				++fracZeros;
		return digiFrac<isVolatile>(expSign, cur, tmp, value, expSig, expLit, sig, exp, sigEnd, dotPos, fracZeros, numTmp, currentIndex);
	}
	cur += currentIndex;
	sigEnd = cur;
	return digiExpMore<isVolatile>(expSign, cur, tmp, value, expSig, expLit, sig, exp);
};

template<size_t currentIndex = 1, bool isVolatile, std::integral value_type, typename char_type> PARSEINT_PY_ALWAYS_INLINE bool exprIntg(bool& expSign,
	const char_type*& cur, const char_type*& tmp, value_type& value, int32_t& expSig, int32_t& expLit, uint64_t& sig, int32_t& exp, const char_type*& sigEnd,
	const char_type*& dotPos, uint32_t& fracZeros, uint64_t& numTmp) {
	if constexpr (currentIndex < 21) {
		if ((numTmp = numberSubTable[cur[currentIndex]]) <= 9) [[likely]] {
			if constexpr (std::unsigned_integral<std::remove_cvref_t<value_type>>) {
				auto maxValue{ (std::numeric_limits<value_type>::max() - numTmp) / 10 };
				if (sig > maxValue) {
					return false;
				}
			}
			else {
				auto maxValue{ (-std::numeric_limits<value_type>::min() - numTmp) / 10 };
				if (sig > maxValue) {
					return false;
				}
			}
			sig = numTmp + sig * 10;
			return exprIntg<currentIndex + 1, isVolatile>(expSign, cur, tmp, value, expSig, expLit, sig, exp, sigEnd, dotPos, fracZeros, numTmp);
		}
		else {
			if (currentIndex > 1) {
				if (*cur == zero)
					return false;
			}
			return digiSepr<isVolatile>(expSign, cur, tmp, value, expSig, expLit, sig, exp, sigEnd, dotPos, fracZeros, numTmp, currentIndex);
		};
	}
	return true;
};

template<std::integral value_type, typename char_type> PARSEINT_PY_ALWAYS_INLINE bool parseInt(value_type& value, char_type* cur) noexcept {
	constexpr auto isVolatile = std::is_volatile_v<std::remove_reference_t<decltype(value)>>;
	[[maybe_unused]] const char_type* sigEnd{};
	const char_type* dotPos{};
	uint32_t fracZeros = 0;
	uint64_t sig = uint64_t(numberSubTable[*cur]);
	int32_t exp = 0;
	bool expSign;
	int32_t expSig = 0;
	int32_t expLit = 0;
	uint64_t numTmp;
	const char_type* tmp;

	if (sig > 9) [[unlikely]] {
		return false;
	}

	return exprIntg<1, isVolatile>(expSign, cur, tmp, value, expSig, expLit, sig, exp, sigEnd, dotPos, fracZeros, numTmp);
}