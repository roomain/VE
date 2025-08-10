#include "pch.h"
#include "ReflectionManager.h"
#include <filesystem>
#include <ranges>
#include <fstream>
#include <vector>
#include "ReflectionUtils.h"
#include "ReflectionJsonParser.h"
#include "ReflectionException.h"

namespace fs = std::filesystem;

ReflectionManager& ReflectionManager::instance()
{
	static ReflectionManager instance;
	return instance;
}

void ReflectionManager::load(const std::string& a_directory, const std::string& a_profile)
{
	fs::path jsonDir = std::filesystem::current_path();
	jsonDir.append(a_directory);

	auto isJsonFile = [](auto& a_entry) { return a_entry.is_regular_file() &&
		(a_entry.path().extension() == ".json" || a_entry.path().extension() == ".JSON"); };

	if (fs::exists(jsonDir) && fs::is_directory(jsonDir))
	{
		ReflectionJsonParser parser;
		for (const auto& entry : fs::recursive_directory_iterator(jsonDir) | std::views::filter(isJsonFile))
		{
			ParserInfo info{ .filename = entry.path().string() };
			parser.loadFile(entry.path().string(), a_profile, m_registeredClass, info);
		}
	}
}

ReflectionClassPtr ReflectionManager::getReflection(const std::string& a_classname)const
{
	ReflectionClassPtr reflect;
	if (auto iter = m_registeredClass.find(a_classname); iter != m_registeredClass.cend())
		reflect = iter->second;
	return reflect;
}

void ReflectionManager::overrideClass(const std::string& a_classname, const ReflectionClassPtr& a_overrided)
{
	if (auto iter = m_registeredClass.find(a_classname); iter != m_registeredClass.cend())
	{
		iter->second->overrideClass(a_overrided);
	}
	else // calss not registered
	{
		// create temporary
		m_registeredClass[a_classname] = a_overrided;
	}
}

void ReflectionManager::resetOverride(const std::string& a_classname)
{
	if (auto iter = m_registeredClass.find(a_classname); iter != m_registeredClass.cend())
	{
		if (!iter->second->resetOverride())
			m_registeredClass.erase(iter);// it's not a real ovveride but a temporary class
	}
}

void ReflectionManager::clear()
{
	m_registeredClass.clear();
}