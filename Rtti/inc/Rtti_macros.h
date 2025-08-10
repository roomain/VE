#pragma once
/***********************************************
* @headerfile Rtti_macros.h
* @date 04 / 04 / 2025
* @author Romain Crespel
************************************************/
#include <tuple>
#include <format>
#include "TRttiDefinition.h"
#include "RttiException.h"

/*@brief macro used to create Rtti definitions*/

template<typename ...Types>
class DefinitionsStub {};

/*@brief check if RTTI definition classes are defined for Type and Other ... and get them definition*/
template<typename Type, typename ...Other>
constexpr void getAndCheckDefinition(std::vector<RttiDefinitionPtr>& a_vDef, DefinitionsStub<Type, Other ...>&& a_trash)
{
	if (!Type::definition())
		throw RttiException(RttiException::Type::parent_not_defined, std::format("Classname {}", typeid(Type).name()));
	a_vDef.push_back(Type::definition());
	getAndCheckDefinition(a_vDef, DefinitionsStub<Other ...>());
}

template<typename Type>
constexpr void getAndCheckDefinition(std::vector<RttiDefinitionPtr>& a_vDef, DefinitionsStub<Type>&& a_trash)
{
	if (!Type::definition())
		throw RttiException(RttiException::Type::parent_not_defined, std::format("Classname {}", typeid(Type).name()));
	a_vDef.push_back(Type::definition());
}


/*@brief Generate static member for RTTI*/
#define BASE_RTTI_STATIC(version, classname) \
private: \
	static inline std::shared_ptr<TRttiDefinition<classname>> s_definition; \
\
public: \
\
	[[nodiscard]] static inline RttiDefinitionPtr definition()noexcept { return s_definition; }\
	\
	static void releaseDef(){s_definition.reset();}

//------------------------------------------------------------------------------------------------------------------------------------
/*@brief Generate base member functions for RTTI*/
#define BASE_DEFINITION_MACRO(version, classname) \
public:\
	template<typename Type>\
	[[nodiscard]] constexpr bool isTypeOf()const \
	{\
		return std::is_same<Type, classname>::value;\
	}\
\
	[[nodiscard]] virtual inline std::shared_ptr<RttiDefinition> isA()const \
	{\
		if(!classname::s_definition)\
			throw RttiException(RttiException::Type::null_definition); \
		return classname::s_definition; \
	}\
\
	[[nodiscard]] virtual inline bool isKindOf(const std::shared_ptr<RttiDefinition>& a_def)const \
	{\
		if(!classname::s_definition)\
			throw RttiException(RttiException::Type::null_definition); \
		return classname::s_definition->isKindOf(a_def); \
	}\
\
	template<typename KindType> \
	[[nodiscard]] inline bool isKindOf()const \
	{\
		if(!classname::s_definition)\
			throw RttiException(RttiException::Type::null_definition); \
		return classname::s_definition->isKindOf(KindType::definition()); \
	}\
\
	[[nodiscard]] virtual inline ClassExtensionPtr getClassExtension(const std::shared_ptr<RttiDefinition>& a_def)const \
	{\
		if(!classname::s_definition)\
			throw RttiException(RttiException::Type::null_definition); \
		return classname::s_definition->getClassExtension(a_def); \
	}\
\
	template<typename Type>\
	[[nodiscard]] Type* const cast()const\
	{\
		Type* pRet = nullptr; \
		if (isKindOf(Type::definition()))\
		{\
			pRet = const_cast<Type*>(dynamic_cast<const Type*>(this));\
		}\
		return pRet;\
	}\

//------------------------------------------------------------------------------------------------------------------------------------

/*@brief declare RTTI for a simple class*/
#define DECLARE_RTTI(version, classname) \
BASE_RTTI_STATIC(version, classname)\
BASE_DEFINITION_MACRO(version, classname)\
public: \
	static void initDef()\
	{\
		if(!s_definition) \
			s_definition = std::make_shared<TRttiDefinition<classname>>(std::string(#classname), version); \
	}

/*@brief declare RTTI for a derived class*/
#define DECLARE_RTTI_DERIVED(version, classname, ...) \
BASE_RTTI_STATIC(version, classname)\
BASE_DEFINITION_MACRO(version, classname)\
public: \
	static void initDef()\
	{\
		std::vector<RttiDefinitionPtr> vParentDefs;\
		getAndCheckDefinition(vParentDefs, DefinitionsStub<__VA_ARGS__>());\
		s_definition = std::make_shared< TRttiDefinition<classname>>(std::string(#classname), version, vParentDefs); \
	}
