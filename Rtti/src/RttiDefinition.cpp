#include "RttiDefinition.h"
#include "ClassExtension.h"


RttiDefinition::RttiDefinition(const std::string_view& a_name, const unsigned int a_version) :
	m_version{ a_version }, m_className{ a_name } {
}

RttiDefinition::RttiDefinition(const std::string_view& a_name, const unsigned int a_version,
	const std::vector<std::shared_ptr<RttiDefinition>>& a_vParents) :
	m_version{ a_version }, m_className{ a_name }, m_vParents{ a_vParents } {
}

bool RttiDefinition::isKindOf(const std::shared_ptr<RttiDefinition>& pDef)const noexcept
{
	bool bIsKindOf = isSame(pDef);
	if (!bIsKindOf)
		bIsKindOf = inheritFrom(pDef);
	return bIsKindOf;
}

void RttiDefinition::registerClassExt(const std::shared_ptr<ClassExtension>& a_Class)
{
	if (a_Class)
	{
		std::erase_if(m_extensions, [&a_Class](const auto& a_curProt)
			{
				return a_curProt->isKindOf(a_Class->isA()) || a_Class->isKindOf(a_curProt->isA());
			});

		m_extensions.emplace_back(a_Class);
	}
}

bool RttiDefinition::unregisterClassExt(const std::shared_ptr<RttiDefinition>& a_pDef)
{
	return m_extensions.size() != std::erase_if(m_extensions, [&a_pDef](const auto& a_curProt)
		{
			return a_curProt->isKindOf(a_pDef);
		});
}

std::shared_ptr<ClassExtension> RttiDefinition::getClassExtension(const std::shared_ptr<RttiDefinition>& pDef)const
{
	std::shared_ptr<ClassExtension> pClassExt;
	auto iter = std::ranges::find_if(m_extensions, [&pDef](auto a_pExt) {return a_pExt->isKindOf(pDef); });
	if (iter != m_extensions.end())
	{
		pClassExt = *iter;
	}
	else
	{
		for (const auto& pCurClass : m_vParents)
		{
			pClassExt = pCurClass->getClassExtension(pDef);
			if (pClassExt)
				break;
		}
	}
	return pClassExt;
}