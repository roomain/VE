#pragma once
/***********************************************
* @headerfile VE_InstanceCapabilities.h
* @date 09 / 07 / 2025
* @author Roomain
************************************************/
#include "Capabilities/VE_Capabilities.h"
#include "Capabilities/VE_DeviceCapabilities.h"
#include "core_globals.h"


struct VE_Parameter;
#pragma warning(push)
#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 )

/*@brief vulkan instance capabilities*/
class VE_CORE_LIB VE_InstanceCapabilities : public VE_Capabilities
{
private:
	std::vector<VE_DeviceCapabilities> m_devicesCap;

public:
	VE_InstanceCapabilities() = default;
	~VE_InstanceCapabilities() override = default;
	void readCapabilities();
	void readCapabilities(const VkInstance a_instance);
	void visit(VE_CapabilitiesVisitor& a_visitor)const override;
	[[nodiscard]] bool check(const VE_Parameter& a_parameters)const noexcept;

	using Device_const_iterator = std::vector<VE_DeviceCapabilities>::const_iterator;
	[[nodiscard]] Device_const_iterator deviceBegin()const noexcept;
	[[nodiscard]] Device_const_iterator deviceEnd()const noexcept;


	[[nodiscard]] std::vector<int> findSuitableDevice(const VE_DeviceParameter& a_devParameters, const VkSurfaceKHR a_surface = VK_NULL_HANDLE)const noexcept;

};

#pragma warning(pop)