#pragma once
/***********************************************
* @headerfile VE_InstanceCapabilities.h
* @date 09 / 07 / 2025
* @author Roomain
************************************************/
#include "Capabilities/VE_Capabilities.h"
#include "Capabilities/VE_DeviceCapabilities.h"

struct VE_Parameter;

/*@brief vulkan instance capabilities*/
class VE_InstanceCapabilities : public VE_Capabilities
{
private:
	std::vector<VE_DeviceCapabilities> m_devicesCap;

public:
	VE_InstanceCapabilities();
	~VE_InstanceCapabilities() override = default;
	void readCapabilities(const VkInstance a_instance);
	void visit(VE_CapabilitiesVisitor& a_visitor)const override;
	[[nodiscard]] bool check(const VE_Parameter& a_parameters)const noexcept;

	using Device_const_iterator = std::vector<VE_DeviceCapabilities>::const_iterator;
	[[nodiscard]] Device_const_iterator deviceBegin()const noexcept;
	[[nodiscard]] Device_const_iterator deviceEnd()const noexcept;


	[[nodiscard]] std::vector<int> findSuitableDevice(const VE_DeviceParameter& a_devParameters, const VkSurfaceKHR a_surface = VK_NULL_HANDLE)const noexcept;

};

