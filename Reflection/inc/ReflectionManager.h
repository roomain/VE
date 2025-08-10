#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "notCopiable.h"
#include "ReflectionGlobals.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

class ReflectionClass;
using ReflectionClassPtr = std::shared_ptr<ReflectionClass>;
using ReflectionClassUPtr = std::unique_ptr<ReflectionClass>;
using ReflectionObjectDatabase = std::unordered_map<std::string, ReflectionClassPtr>;

/*@brief singleton providing json deserialisation*/
class REFLECTION_LIB ReflectionManager
{
private:
	ReflectionObjectDatabase m_registeredClass;	/*!< class/struct readed from Json*/
		
	ReflectionManager() = default;

public:
	NOT_COPIABLE(ReflectionManager)
	NOT_MOVABLE(ReflectionManager)
	static ReflectionManager& instance();
	void load(const std::string& a_directory, const std::string& a_profile);
	ReflectionClassPtr getReflection(const std::string& a_classname)const;
	void overrideClass(const std::string& a_classname, const ReflectionClassPtr& a_overrided);
	void resetOverride(const std::string& a_classname);
	void clear();
};
#pragma warning(pop)