#pragma once
/***********************************************
* @headerfile TRttiDefinition.h
* @date 04 / 04 / 2025
* @author Romain Crespel
************************************************/
#include "RttiDefinition.h"

/*@brief templated Rtti definition*/
template <typename DefinedClass>
class TRttiDefinition : public RttiDefinition
{
public:
	TRttiDefinition() = delete;
	TRttiDefinition(const std::string_view& a_name, const unsigned int a_version) :
		RttiDefinition(a_name, a_version) {
	}
	TRttiDefinition(const std::string_view& a_name, const unsigned int a_version,
		const std::vector<RttiDefinitionPtr>& a_vParents) :
		RttiDefinition(a_name, a_version, a_vParents) {
	}

	/*@brief size of described class*/
	[[nodiscard]] virtual size_t classSize()const noexcept override { return sizeof(DefinedClass); }

	/*@brief create class*/
	std::shared_ptr<DefinedClass> create()
	{
		return std::make_shared<DefinedClass>();
	}
};

template<typename DefinedClass>
using TRttiDefinitionPtr = std::shared_ptr<TRttiDefinition<DefinedClass>>;