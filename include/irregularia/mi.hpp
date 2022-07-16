#pragma once

#include <type_traits>
#include "mitraits.hpp"

template<class>
struct width2;

template<>
struct width2<uint8_t>
{
    using type=uint16_t;
};

template<>
struct width2<uint16_t>
{
    using type=uint32_t;
};

template<>
struct width2<uint32_t>
{
    using type=uint64_t;
};

template<>
struct width2<uint64_t>
{
    //If we are in the biggest uint-datatype, stop the recursion of the
    //friend class multiple_int<2*BitWidth+1, typename width2<BackingStorage>::type> clausel
    using type=uint64_t;  
};

namespace irregularia
{

template<std::size_t BitWidth, typename BackingStorage>
requires std::is_integral_v<BackingStorage> && std::is_unsigned_v<BackingStorage>
struct multiple_int
{
public:
  static constexpr int IntCount = (8 * sizeof(BackingStorage)) / (BitWidth + 1);

  using traits = detail::_multiple_int_traits<IntCount, BitWidth, BackingStorage>;

  friend class std::numeric_limits<multiple_int<BitWidth, BackingStorage>>;
  //We cannot access the private member in the conversion constructor because we
  //are here in a template class. For this reason, we need to use the "friend class" clausel
  friend class multiple_int<2*BitWidth+1, typename width2<BackingStorage>::type>;

private:
  multiple_int(typename traits::int_type value)
      : value_ {value}
  {
  }

public:
  template<std::size_t SmallerBitWidth, typename SmallerBackingStorage>
  requires(2*SmallerBitWidth + 1 == BitWidth && 2*sizeof(SmallerBackingStorage) == sizeof(BackingStorage))
  multiple_int(multiple_int<SmallerBitWidth, SmallerBackingStorage> halfSizeMultipleInt)
      : value_ {0}
  {
    using lower_multiple_int = multiple_int<SmallerBitWidth, SmallerBackingStorage>;
    // Mask for one smaller int
    static constexpr auto smallerMultipleInt_IntMask =
        detail::_int_mask<lower_multiple_int::IntCount, SmallerBitWidth, SmallerBackingStorage>::pattern;
    // Mask for one smaller int carry-bit
    static constexpr auto smallerMultipleInt_CarryMask =
        detail::_carry_mask<lower_multiple_int::IntCount, SmallerBitWidth, SmallerBackingStorage>::pattern;
    // Mask for one smaller int sign-"bit"
    static constexpr auto smallerMultipleInt_SignMask =
        static_cast<SmallerBackingStorage>(1) << SmallerBitWidth - 1;
    // Mask for one bigger int
    static constexpr auto biggerMultipleInt_IntMask =
        detail::_int_mask<IntCount, BitWidth, BackingStorage>::pattern;
    // Mask for one bigger int carry-bit
    static constexpr auto biggerMultipleInt_CarryMask =
        detail::_carry_mask<IntCount, BitWidth, BackingStorage>::pattern;
    // Mask for all bits being 1 above the smallerMultipleInt_IntMask (sets all bits to 1,
    // if the sign-"bit" of the smaller int is set)
    static constexpr auto biggerMultipleInt_SignMask =
        biggerMultipleInt_IntMask ^ smallerMultipleInt_IntMask;

    // Place the values of the smaller ints with even indices in the first half of the
    // (bigger) internal storage and the values of the smaller ints with odd indices in
    // the second half
    for (int half = 0; half < 2; ++half)  // half == 0: First half; half == 1: Second half
    {
      for (int i = half;
           i < multiple_int<SmallerBitWidth, SmallerBackingStorage>::IntCount;
           i += 2)
      {
        const int shiftAmount = i * (SmallerBitWidth + 1);
        // If we are in the first half, we do not need to shift the values;
        // if we are in the second half, we have to shift it to the half minus the
        // shiftAmount because we already shifted it by this amount
        const int halfShiftAmount =
            half ? (lower_multiple_int::IntCount - 1) * (BitWidth + 1) - shiftAmount : 0;

        value_ |= (static_cast<BackingStorage>(halfSizeMultipleInt.value_)
                   & (smallerMultipleInt_IntMask << i * (SmallerBitWidth + 1))) << halfShiftAmount;

        // Extend "sign" bit, if it is set
        if (halfSizeMultipleInt.value_ & (smallerMultipleInt_SignMask << shiftAmount)) {
          value_ |= (biggerMultipleInt_SignMask << shiftAmount) << halfShiftAmount;
        }

        // Set the carry-bit at the right position, if it is set
        if (halfSizeMultipleInt.value_ & (smallerMultipleInt_CarryMask << shiftAmount)) {
          value_ |= (biggerMultipleInt_CarryMask << shiftAmount) << halfShiftAmount;
        }
      }
    }
  }

private:

  typename traits::int_type value_;

public:
 
  template <typename IndivStorage, std::size_t AtMostIntCount>
  requires (
    AtMostIntCount <= IntCount &&
    AtMostIntCount > 0 &&
    (sizeof(IndivStorage) * 8) >= BitWidth
  )
  static auto encode(const std::array<IndivStorage, AtMostIntCount> &input) {
  
    //Create a mask with #bit-width bits set to one
    static auto mask = (static_cast<unsigned int>(1) << BitWidth) - 1;

    BackingStorage value_ = 0;

    if constexpr (IntCount > 1) {

      for (std::size_t i = 0; i < (IntCount - 1); ++i) {
  
        //Insert value
        value_ |= (input[i] & mask);
        //Shift by bit width + 1 (carry bit)
        value_ <<= (BitWidth + 1);
  
      }
      
    }
    
    //Don't shift the last value
    value_ |= (input[IntCount - 1] & mask);
    
    return multiple_int<BitWidth, BackingStorage> { value_ };

  }

  std::array<int, IntCount> decode() {

    //Create a mask with #bit-width bits set to one
    static auto mask = (static_cast<unsigned int>(1) << BitWidth) - 1;

    std::array<int, IntCount> data;
    
    if constexpr (IntCount > 1) {

      for (std::size_t i = 0; i < IntCount; ++i) {
  
        auto val = ((value_ >> (i * (BitWidth + 1))) & mask);
  
        //During encoding numbers are inserted in reverse order,
        //decode them in reverse order to correct that
        if ((val >> (BitWidth - 1)) != 0) { data[IntCount - i - 1] = (~(mask) | val); }
        else { data[IntCount - i - 1] = val; }
  
      }
      
    }

    return data;

  }

  auto intv() const -> typename traits::int_type
  {
    return this->value_ & traits::int_mask;
  }

  auto carry() const -> typename traits::int_type
  {
    return this->value_ & traits::carry_mask;
  }

  auto operator+(irregularia::multiple_int<BitWidth, BackingStorage> rhs)
      -> irregularia::multiple_int<BitWidth, BackingStorage>
  {
    using value_type = irregularia::multiple_int<BitWidth, BackingStorage>;

    // 1 for handle, 0 for zero out unconditionally
    // Will cause compilation error if not defined
    if constexpr (IRREGULARIA_BIT_CARRY_POLICY == 0) {
      // No need to use intv, as the carry bits are never on, therefore they can
      // never bleed into the LSB of the following integer
      auto sum = static_cast<typename traits::int_type>(this->value_ + rhs.value_);
      auto mi_sum = value_type {sum};
      mi_sum.value_ &= traits::int_mask;

      return mi_sum;
    } else {
      // Use intv instead of the raw value to avoid adding carry bits, which
      // would "bleed" their overflow into the LSB of the following integer
      auto sum = static_cast<typename traits::int_type>(this->intv() + rhs.intv());
      auto mi_sum = value_type {sum};
      // If carry bit was set on neither lhs nor rhs, and no carry bits are set
      // in their sum,  no carry bits are set

      // If carry bit was set on neither lhs nor rhs, and carry bits are set in
      // their sum, retain them

      // If carry bit was set on lhs and / or rhs, and no carry bit is set in
      // their sum, retain it

      // If carry bit was set on lhs and / or rhs, and carry bits are
      // set in their sum, retain them

      // => if carry bits are set anywhere, they must be propagated
      mi_sum.value_ |= (this->value_ | rhs.value_) & traits::carry_mask;
      return mi_sum;
    }
  }
};

};  // namespace irregularia