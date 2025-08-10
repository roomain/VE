#pragma once

/*@brief indicate if object is reflective*/
template<typename Type>
struct Is_reflective
{
private:
	using TrueType = char;
	using FalseType = short;

	// template used for compile time evaluation
	template<typename C>
	static constexpr TrueType& reflective(decltype(&C::deserialize));

	template<typename C>
	static constexpr FalseType& reflective(...);

public:
	enum { value = sizeof(reflective<Type>(0)) == sizeof(Is_reflective<Type>::TrueType) };
};
