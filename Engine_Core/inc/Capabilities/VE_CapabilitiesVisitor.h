#pragma once
/***********************************************
* @headerfile VE_CapabilitiesVisitor.h
* @date 09 / 07 / 2025
* @author Roomain
************************************************/
#include <string>
#include <format>
#include <variant>
#include "core_globals.h"

class VE_Capabilities;
class VE_InstanceCapabilities;
class VE_DeviceCapabilities;

struct VkPhysicalDeviceLimits;
struct VkPhysicalDeviceSparseProperties;
struct VkPhysicalDeviceFeatures;

using VarCapability = std::variant<const char*, int, unsigned int, float, double>;

/*@brief interface for cpabilities visitor*/
/*
* The aim of this visitor is to display capabilities into a tree.
* Nodes represents a category that contains capabilities
*/
class VE_CORE_LIB VE_CapabilitiesVisitor
{
private:
	static std::string versionToString(const unsigned int a_version);

	template<typename SizeType>
	static void memorySize(const SizeType& a_size, float& a_computedSize, std::string& a_unit)
	{
		static constexpr float kilo = 1024;
		static constexpr float mega = 1024 * kilo;
		static constexpr float giga = 1024 * mega;

		if (a_computedSize = a_size / giga; a_computedSize > 0)
		{
			a_unit = "Go";
		}
		else if (a_computedSize = a_size / giga; a_computedSize > 0)
		{
			a_unit = "Mo";
		}
		else if (a_computedSize = a_size / giga; a_computedSize > 0)
		{
			a_unit = "Ko";
		}
		else
		{
			a_computedSize = static_cast<float>(a_size);
			a_unit = "o";
		}
	}

	template<typename SizeType>
	static std::string memorySize(const SizeType& a_size)
	{
		float size;
		std::string unit;
		VE_CapabilitiesVisitor::memorySize(a_size, size, unit);
		return std::format("{0} {1}", size, unit).c_str();
	}

	void visit(const VkPhysicalDeviceLimits& a_limits);
	void visit(const VkPhysicalDeviceSparseProperties& a_limits);
	void visit(const VkPhysicalDeviceFeatures& a_features);

protected:
	/*@brief create a tree node*/
	virtual void displayNode(const std::string& a_name) = 0;
	/*@brief go to parent node*/
	virtual void popNode() = 0;
	/*@brief add node data*/
	virtual void displayNodeCapability(const std::string& a_capability, const VarCapability& a_value) = 0;

public:
	void visit(const VE_Capabilities& a_capabilities);
	void visit(const VE_InstanceCapabilities& a_capabilities);
	void visit(const VE_DeviceCapabilities& a_capabilities);
};