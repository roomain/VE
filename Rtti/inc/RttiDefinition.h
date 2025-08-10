#pragma once
/***********************************************
* @headerfile RttiDefinition.h
* @date 02 / 04 / 2025
* @author Romain Crespel
************************************************/

#include <vector>
#include <memory>
#include <string>
#include <ranges>

class ClassExtension;
using ClassExtensionPtr = std::shared_ptr<ClassExtension>;

class RttiDefinition;
using RttiDefinitionPtr = std::shared_ptr<RttiDefinition>;

class RttiDefinition : public std::enable_shared_from_this<RttiDefinition>
{
private:
	std::vector<RttiDefinitionPtr> m_vParents;      /*!< parent class definitions*/
	unsigned int m_version;							/*!< version*/
	std::string m_className;						/*!< name of defined class*/
	std::vector<ClassExtensionPtr> m_extensions;    /*!< functional extensions*/
	[[nodiscard]] constexpr bool inheritFrom(const std::shared_ptr<RttiDefinition>& a_pDef)const
	{
		bool bFound = std::ranges::find(m_vParents, a_pDef) != m_vParents.end();
		if (!bFound)
			bFound = std::ranges::find_if(m_vParents, [&a_pDef](auto&& a_curdef)
				{return a_curdef->inheritFrom(a_pDef); }) != m_vParents.end();

		return bFound;
	}

public:
	RttiDefinition() = delete;
	RttiDefinition(const std::string_view& a_name, const unsigned int a_version);
	RttiDefinition(const std::string_view& a_name, const unsigned int a_version,
		const std::vector<std::shared_ptr<RttiDefinition>>& a_vParents);

	/*
	* @brief check is defined class is inherited from class defined by pDef
	* @param pDef class definition to test
	* @brief return true if defined class is inherited class defined by pDef
	*/
	[[nodiscard]] virtual bool isKindOf(const std::shared_ptr<RttiDefinition>& pDef)const noexcept;

	/*
	* @return class name
	*/
	[[nodiscard]] constexpr std::string_view className()const { return m_className; }

	/*
	* @brief check is defined class is same class defined by pDef
	* @param pDef class definition to test
	* @brief return true if defined class is same class defined by pDef
	*/
	[[nodiscard]] inline bool isSame(const std::shared_ptr<RttiDefinition>& pDef)const noexcept
	{
		return pDef.get() == this;
	}

	/*@brief register a Class extension*/
	void registerClassExt(const std::shared_ptr<ClassExtension>& Class);
	/*@brief unregister Class extension*/
	bool unregisterClassExt(const std::shared_ptr<RttiDefinition>& pDef);

	/*@brief find  a Class extension*/
	[[nodiscard]] std::shared_ptr<ClassExtension> getClassExtension(const std::shared_ptr<RttiDefinition>& pDef)const;

	/*@brief find  a Class extension*/
	template<typename Class>
	[[nodiscard]] inline std::shared_ptr<Class> getClassExtension()const
	{
		return std::dynamic_pointer_cast<Class>(getClassExtension(Class::definition()));
	}

	/*@return version number of definition*/
	[[nodiscard]] constexpr unsigned int version()const noexcept { return m_version; }

	/*@brief size of described class*/
	[[nodiscard]] virtual size_t classSize()const noexcept = 0;
};

using RttiDefinitionPtr = std::shared_ptr<RttiDefinition>;