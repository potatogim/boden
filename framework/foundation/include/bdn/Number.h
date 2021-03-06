#ifndef BDN_Number_H_
#define BDN_Number_H_

#include <bdn/config.h>

// needed so that we can provide specializations for std::hash
#include <functional>
#include <cmath>

namespace bdn
{

    /** A template class for representing integer and floating point values as
       objects.

        The template parameter of Number is the simple integer or floating point
       type that the class should use. For example, Number<int64_t> would be a
       64 bit integer, Number<float> would be a floating point value.

        There are also these predefined aliases for using Number with common
        base types:

        - \ref Int
        - \ref UInt
        - \ref Int8
        - \ref UInt8
        - \ref Int16
        - \ref UInt16
        - \ref Int32
        - \ref UInt32
        - \ref Int64
        - \ref UInt64
        - \ref Float
        - \ref Double

        std::hash support
        -----------------

        std::hash can be used to calculate hashes for Number objects of the
       standard types listed above. The resulting hash value is identical to the
       value one would get by applying std::hash directly to the wrapped inner
       integer or floating point value.
    */
    template <typename BaseType> class Number : public Base
    {
      public:
        Number(BaseType value = 0) : _value(value) {}

        /** The simple integer or floating point type that this Number class
           uses. This is the type that was passed as a template parameter.*/
        using SimpleType = BaseType;

        /** Returns the biggest possible value for this integer type.*/
        constexpr static inline BaseType maxValue() { return std::numeric_limits<BaseType>::max(); }

        /** The smallest possible finite value for this integer type.
            For unsigned types this is 0. For signed types it is
            the smallest possible negative number.

            Difference to std::numeric_limits for floating point values
            ----------------------

            Note that for floating point values this is actually the smallest
            possible negative number, i.e. the same as
           std::numeric_limits::lowest(). It is NOT equal to
           std::numeric_limits::min() ( which returns the smallest representable
           *positive* number > 0).
        */
        constexpr static inline BaseType minValue() { return std::numeric_limits<BaseType>::lowest(); }

        /** For floating point types: the special value representing positive
           infinity.

            Integer types do not have an infinity value. For them this is 0.

            Also see hasInfinity() and the global function isFinite().
            */
        constexpr static inline BaseType infinity() { return std::numeric_limits<BaseType>::infinity(); }

        /** For floating point types: the special value representing negative
           infinity.

            Integer types do not have an infinity value. For them this is 0.

            Also see hasInfinity() and the global function isFinite().
            */
        constexpr static inline BaseType negativeInfinity() { return 0 - std::numeric_limits<BaseType>::infinity(); }

        /** The value representing "not a number" (NaN) that can be used
            in safely with normal floating point operations, meaning that it
           does not raise an exception signal when used (it is a "quiet NaN").

            Use isNan() to check if a given value is a NaN.

            Also see hasNan() and the global functions bdn::isFinite() and
           bdn::isNan()

            Note that normally, when the compiler is producing standard
           compliant code, NaN values never compare equal to anything, not even
           other NaN values. However, some compilers support standard-breaking
           optimizations that make the result of such comparisons unreliable. So
           you may want to avoid comparisons with NaN. Also see \ref
           BDN_AGGRESSIVE_FLOAT_OPTIMIZATIONS.

        */
        constexpr static inline BaseType nan() { return std::numeric_limits<BaseType>::quiet_NaN(); }

        /** True if the underlying simple type has a special value to represent
            infinity.

            If this is true then the \ref infinity and \ref negativeInfinity
            constants contain the corresponding values.
            */
        constexpr static inline bool hasInfinity() { return std::numeric_limits<BaseType>::has_infinity; }

        /** True if the underlying simple type has a special value to represent
            NaN ("not a number").

            If this is true then the \ref nan constant contains the
           corresponding value.
            */
        constexpr static inline bool hasNan() { return std::numeric_limits<BaseType>::has_quiet_NaN; }

        /** True if the underlying simple type is signed (i.e. it can have
         * negative values).*/
        constexpr static inline bool isSigned() { return std::numeric_limits<BaseType>::is_signed; }

        /** True if the underlying simple type is an integer.*/
        constexpr static inline bool isInteger() { return std::numeric_limits<BaseType>::is_integer; }

        /** Returns the value of the Number object.*/
        BaseType getValue() const { return _value; }

        /** Sets the value of the integer object.*/
        template <typename ArgType> void setValue(ArgType &&val) { _value = std::forward<ArgType>(val); }

        template <typename ArgType> Number &operator=(ArgType &&arg)
        {
            _value = std::forward<ArgType>(arg);
            return *this;
        }

        /** Returns true if the integer value is not 0.*/
        operator BaseType() const { return _value; }

        template <typename ArgType> bool operator==(ArgType &&otherValue) const
        {
            return (_value == std::forward<ArgType>(otherValue));
        }

        template <typename ArgType> bool operator!=(ArgType &&otherValue) const
        {
            return (_value != std::forward<ArgType>(otherValue));
        }

        template <typename ArgType> bool operator<(ArgType &&otherValue) const
        {
            return (_value < std::forward<ArgType>(otherValue));
        }

        template <typename ArgType> bool operator<=(ArgType &&otherValue) const
        {
            return (_value <= std::forward<ArgType>(otherValue));
        }

        template <typename ArgType> bool operator>(ArgType &&otherValue) const
        {
            return (_value > std::forward<ArgType>(otherValue));
        }

        template <typename ArgType> bool operator>=(ArgType &&otherValue) const
        {
            return (_value >= std::forward<ArgType>(otherValue));
        }

      private:
        BaseType _value;
    };

    // note that signed char and unsigned char
    // are always different types than just plain char
    using SignedChar = Number<signed char>;
    using UnsignedChar = Number<unsigned char>;

    using Short = Number<short>;
    using UShort = Number<unsigned short>;
    using Int = Number<int>;
    using UInt = Number<unsigned>;
    using Long = Number<long>;
    using ULong = Number<unsigned long>;
    using LongLong = Number<long long>;
    using ULongLong = Number<unsigned long long>;

    using Int8 = Number<int8_t>;
    using UInt8 = Number<uint8_t>;
    using Int16 = Number<int16_t>;
    using UInt16 = Number<uint16_t>;
    using Int32 = Number<int32_t>;
    using UInt32 = Number<uint32_t>;
    using Int64 = Number<int64_t>;
    using UInt64 = Number<uint64_t>;
    using Float = Number<float>;
    using Double = Number<double>;
    using LongDouble = Number<long double>;

    template <int byteCount> struct _IntOpImpl;

    template <> struct _IntOpImpl<1>
    {
        static inline uint8_t _rotateBitsLeftImpl(uint8_t val, int bits) { return (val << bits) | (val >> (8 - bits)); }

        static inline uint8_t _rotateBitsRightImpl(uint8_t val, int bits)
        {
            return (val >> bits) | (val << (8 - bits));
        }

        static inline uint8_t _invertByteOrderImpl(uint8_t val)
        {
            // single byte
            return val;
        }
    };

    template <> struct _IntOpImpl<2>
    {
        static inline uint16_t _rotateBitsLeftImpl(uint16_t val, int bits)
        {
            return (val << bits) | (val >> (16 - bits));
        }

        static inline uint16_t _rotateBitsRightImpl(uint16_t val, int bits)
        {
            return (val >> bits) | (val << (16 - bits));
        }

        static inline uint16_t _invertByteOrderImpl(uint16_t val)
        {
#ifdef _MSC_VER
            return _byteswap_ushort(val);

#elif defined(__GNUC__)
            return __builtin_bswap16(val);

#else
            return (val & 0xff) << 8 | (val & 0xff00) >> 8;

#endif
        }
    };

    template <> struct _IntOpImpl<4>
    {
        static inline uint32_t _rotateBitsLeftImpl(uint32_t val, int bits)
        {
#ifdef _MSC_VER
            return _rotl(val, bits);

#else
            return (val << bits) | (val >> (32 - bits));

#endif
        }

        static inline uint32_t _rotateBitsRightImpl(uint32_t val, int bits)
        {
#ifdef _MSC_VER
            return _rotr(val, bits);

#else
            return (val >> bits) | (val << (32 - bits));

#endif
        }

        static inline uint32_t _invertByteOrderImpl(uint32_t val)
        {
#ifdef _MSC_VER
            return _byteswap_ulong(val);

#elif defined(__GNUC__)
            return __builtin_bswap32(val);

#else
            return (val & 0xff) << 24 | (val & 0xff00) << 8 | (val & 0xff0000) >> 8 | (val & 0xff000000) >> 24;

#endif
        }
    };

    template <> struct _IntOpImpl<8>
    {
        static inline uint64_t _rotateBitsLeftImpl(uint64_t val, int bits)
        {
#ifdef _MSC_VER
            return _rotl64(val, bits);

#else
            return (val << bits) | (val >> (64 - bits));

#endif
        }

        static inline uint64_t _rotateBitsRightImpl(uint64_t val, int bits)
        {
#ifdef _MSC_VER
            return _rotr64(val, bits);

#else
            return (val >> bits) | (val << (64 - bits));

#endif
        }

        static inline uint64_t _invertByteOrderImpl(uint64_t val)
        {
#ifdef _MSC_VER
            return _byteswap_uint64(val);

#elif defined(__GNUC__)
            return __builtin_bswap64(val);

#else
            return (val & 0xff) << 56 | (val & 0xff00) << 40 | (val & 0xff0000) << 24 | (val & 0xff000000) << 8 |
                   (val & 0xff00000000) >> 8 | (val & 0xff0000000000) >> 24 | (val & 0xff000000000000) >> 40 |
                   (val & 0xff00000000000000) >> 56;

#endif
        }
    };

    /** Inverts the byte order of the specified integer. This can be used to
        convert between little endian and big endian integer representations.

        This template function can be used with any integer type.

        Example:

        \code

        unsigned short myValue = 0x1234;

        myValue = invertByteOrder( myValue );

        // myValue will now be 0x3412

        \endcode

        */
    template <typename ArgIntType> static inline ArgIntType invertByteOrder(ArgIntType value)
    {
        return static_cast<ArgIntType>(_IntOpImpl<sizeof(ArgIntType)>::_invertByteOrderImpl(value));
    }

    /** Does a bit-rotation operation towards the left on the specified integer
       value.

        This template function can be used with any integer type.

        This function is implemented using compiler intrinsics where available
        to optimize performance.

        The effect is equivalent to the following code (shown here for 32 bit
       values):

        \code

        return (val << bits) | (val >> (32 - bits));

        \endcode

        Example:

        \code

        unsigned short myValue = 0xf000;

        // any integer type (signed or unsigned) can be passed to
       rotateBitsLeft. myValue = rotateBitsLeft(myValue, 4);

        // myValue will now be 0x3003

        \endcode
    */
    template <typename ArgIntType> static inline ArgIntType rotateBitsLeft(ArgIntType value, int bits)
    {
        return static_cast<ArgIntType>(_IntOpImpl<sizeof(ArgIntType)>::_rotateBitsLeftImpl(value, bits));
    }

    /** Does a bit-rotation operation towards the right on the specified integer
       value.

        This template function can be used with any integer type.

        This function is implemented using compiler intrinsics where available
        to optimize performance.

        The effect is equivalent to the following code (shown here for 32 bit
       values):

        \code

        return (val >> bits) | (val << (32 - bits));

        \endcode

        Example:

        \code

        unsigned short myValue = 0x000f;

        // any integer type (signed or unsigned) can be passed to
       rotateBitsLeft. myValue = rotateBitsRight(myValue, 4);

        // myValue will now be 0x3003

        \endcode
    */
    template <typename ArgIntType> static inline ArgIntType rotateBitsRight(ArgIntType value, int bits)
    {
        return static_cast<ArgIntType>(_IntOpImpl<sizeof(ArgIntType)>::_rotateBitsRightImpl(value, bits));
    }

#if BDN_STD_ISNAN_INT_MISSING

    template <bool IsFloatingPoint> struct MscNumberUtilHelper_
    {
        template <typename ValueType> static bool isNan(ValueType value) { return false; }

        template <typename ValueType> static bool isFinite(ValueType value) { return true; }
    };

    template <> struct MscNumberUtilHelper_<true>
    {
        template <typename ValueType> static bool isNan(ValueType value) { return std::isnan(value); }

        template <typename ValueType> static bool isFinite(ValueType value) { return std::isfinite(value); }
    };

#endif

    /** Returns true if the specified number (floating point or integer)
        has the special "not a number" value.

        Note that for integers this always returns false, since for those such a
       special value does not exist.

        Also see Number::nan()
        */
    template <typename ValueType> static inline bool isNan(ValueType value)
    {
#if BDN_STD_ISNAN_INT_MISSING
        // Visual Studio 2017 and below did not provide an implementation of
        // std::isnan for integers (even though C++17 mandates it). So we have
        // to use a workaround here.
        return MscNumberUtilHelper_<!std::numeric_limits<ValueType>::is_integer>::template isNan<ValueType>(value);

#else
        // on other systems we can simply call std::isnan
        return std::isnan(value);

#endif
    }

    /** Returns true if the specified number (floating point or integer)
        is a normal, finite number. I.e. it is not one of the special
        values that represent infinities, NaN ("not a number") etc.

        Since integers do not support any of the mentioned special values this
       always returns true for any floating point value.

        Also see Number::nan(), Number::infinity(), Number::negativeInfinity().
        */
    template <typename ValueType> static inline bool isFinite(ValueType value)
    {
#if BDN_STD_ISFINITE_INT_MISSING
        // Visual Studio 2017 and below did not provide an implementation of
        // std::isfinite for integers (even though C++17 mandates it). So we
        // have to use a workaround here.
        return MscNumberUtilHelper_<!std::numeric_limits<ValueType>::is_integer>::template isFinite<ValueType>(value);

#else
        // on other systems we can simply call std::isnan
        return std::isfinite(value);

#endif
    }
}

namespace std
{

    // specializations of std::hash template for Number objects

    // the fundamental integer types are
    // - signed/unsigned char (which are always different from char without the
    // signed/unsigned prefix)
    // - short
    // - int
    // - long
    // - long long
    // The fixed width types are simply typedefs to these.

    template <> struct hash<bdn::Number<signed char>>
    {
        size_t operator()(const bdn::Number<signed char> &key) const
        {
            return std::hash<signed char>()(key.getValue());
        }
    };

    template <> struct hash<bdn::Number<unsigned char>>
    {
        size_t operator()(const bdn::Number<unsigned char> &key) const
        {
            return std::hash<unsigned char>()(key.getValue());
        }
    };

    template <> struct hash<bdn::Number<short>>
    {
        size_t operator()(const bdn::Number<short> &key) const { return std::hash<short>()(key.getValue()); }
    };

    template <> struct hash<bdn::Number<unsigned short>>
    {
        size_t operator()(const bdn::Number<unsigned short> &key) const
        {
            return std::hash<unsigned short>()(key.getValue());
        }
    };

    template <> struct hash<bdn::Number<int>>
    {
        size_t operator()(const bdn::Number<int> &key) const { return std::hash<int>()(key.getValue()); }
    };

    template <> struct hash<bdn::Number<unsigned int>>
    {
        size_t operator()(const bdn::Number<unsigned int> &key) const
        {
            return std::hash<unsigned int>()(key.getValue());
        }
    };

    template <> struct hash<bdn::Number<long>>
    {
        size_t operator()(const bdn::Number<long> &key) const { return std::hash<long>()(key.getValue()); }
    };

    template <> struct hash<bdn::Number<unsigned long>>
    {
        size_t operator()(const bdn::Number<unsigned long> &key) const
        {
            return std::hash<unsigned long>()(key.getValue());
        }
    };

    template <> struct hash<bdn::Number<long long>>
    {
        size_t operator()(const bdn::Number<long long> &key) const { return std::hash<long long>()(key.getValue()); }
    };

    template <> struct hash<bdn::Number<unsigned long long>>
    {
        size_t operator()(const bdn::Number<unsigned long long> &key) const
        {
            return std::hash<unsigned long long>()(key.getValue());
        }
    };

    template <> struct hash<bdn::Float>
    {
        size_t operator()(const bdn::Float &key) const
        {
            return std::hash<typename bdn::Float::SimpleType>()(key.getValue());
        }
    };

    template <> struct hash<bdn::Double>
    {
        size_t operator()(const bdn::Double &key) const
        {
            return std::hash<typename bdn::Double::SimpleType>()(key.getValue());
        }
    };

    template <> struct hash<bdn::LongDouble>
    {
        size_t operator()(const bdn::LongDouble &key) const
        {
            return std::hash<typename bdn::LongDouble::SimpleType>()(key.getValue());
        }
    };
}

#endif
