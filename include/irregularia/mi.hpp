#pragma once

#include <type_traits>
#include "mitraits.hpp"

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
  friend class multiple_int<2*BitWidth+1, typename traits::template next_widest<BackingStorage>>;

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

		static constexpr auto evenIndicesMask = detail::_int_pattern<(IntCount % 2 == 0) ? IntCount-2 : IntCount-1, SmallerBitWidth, SmallerBackingStorage>::value;
		static constexpr auto oddIndicesMask = detail::_int_pattern<(IntCount % 2 == 0) ? IntCount-1 : IntCount-2, SmallerBitWidth, SmallerBackingStorage>::value;

		//Mask all bits other than the sign bits with 1s
		static constexpr auto signMaskShiftedByOne = detail::_sign_pattern<IntCount, SmallerBitWidth, SmallerBackingStorage, BackingStorage>::value;

		//E.g. −2 in 3b is 0b110 -> Mask: 0000; 2 in 3b is 0b010 -> Mask: 1000
		static constexpr auto invertedAndShiftedSignBitMask = (lower_multiple_int::traits::carry_mask |
														static_cast<BackingStorage>(lower_multiple_int::traits::carry_mask) << (IntCount * (SmallerBitWidth+1)))
																	^ traits::carry_mask;
                                  
		//Transfer all small ints with EVEN indices
		value_ |= halfSizeMultipleInt.value_ & evenIndicesMask;

		//Transfer all small ints with ODD indices
		value_ |= ((static_cast<BackingStorage>(halfSizeMultipleInt.value_ & oddIndicesMask)) << ((IntCount % 2 == 0) ? (IntCount-1) : (IntCount)) * (SmallerBitWidth+1));

		//Add sign mask to the result (Negative numbers become now negative again)
		value_ += signMaskShiftedByOne;

		//If the number was a positive number, we have to add the inverted and shifted sign bit mask
		value_ += (((~value_) & traits::int_mask) << 1) & invertedAndShiftedSignBitMask;

		//Reset all carry (overflow)-bits
		value_ &= traits::int_mask;
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

  template<std::size_t SmallerBitWidth, typename SmallerBackingStorage>
  requires(2*SmallerBitWidth + 1 == BitWidth && 2*sizeof(SmallerBackingStorage) == sizeof(BackingStorage))
  explicit operator multiple_int<SmallerBitWidth,SmallerBackingStorage>() const
  {
      using lower_multiple_int = multiple_int<SmallerBitWidth, SmallerBackingStorage>;
      //Mask for one smaller int
      static constexpr auto smallerMultipleInt_IntMask = detail::_int_mask<lower_multiple_int::IntCount, SmallerBitWidth, SmallerBackingStorage>::pattern;
      //Mask for one smaller int carry-bit
      static constexpr auto smallerMultipleInt_CarryMask = detail::_carry_mask<lower_multiple_int::IntCount, SmallerBitWidth, SmallerBackingStorage>::pattern;
      //Mask for smaller int + carry-bit
      static constexpr auto smallerMultipleInt_IntAndCarryMask = smallerMultipleInt_IntMask | smallerMultipleInt_CarryMask;
      //Mask for one bigger int carry-bit
      static constexpr auto biggerMultipleInt_CarryMask = detail::_carry_mask<IntCount, BitWidth, BackingStorage>::pattern;
      //Mask for bigger int all bits are set, but not the unused bits
      static constexpr auto biggerMultipleInt_IntAndCarryMask = traits::int_mask | traits::carry_mask;

      SmallerBackingStorage value = 0;


      for(int i = 0; i < IntCount; ++i)
      {
          value |= ((value_ & (static_cast<BackingStorage>(smallerMultipleInt_IntAndCarryMask) << i*(BitWidth+1))) 
                     >> i*(SmallerBitWidth+1));

          //If the carry-bit is set, set it also in the smaller int
          if(value_ & (biggerMultipleInt_CarryMask << i*(BitWidth+1)))
          {
              value |= (smallerMultipleInt_CarryMask << i*(SmallerBitWidth+1));
          }
          else
          {
              //If the carry-bit should not be set, but it is at the moment, delete it
              if(value & (smallerMultipleInt_CarryMask << i*(SmallerBitWidth+1)))
              {
                  value &= (static_cast<SmallerBackingStorage>(biggerMultipleInt_IntAndCarryMask) ^ 
                           (smallerMultipleInt_CarryMask << i*(SmallerBitWidth+1)));
              }
          }
      }

      return multiple_int<SmallerBitWidth,SmallerBackingStorage>(value);
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