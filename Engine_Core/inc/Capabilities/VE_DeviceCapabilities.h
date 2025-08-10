#pragma once
/***********************************************
* @headerfile VE_InstanceCapabilities.h
* @date 09 / 07 / 2025
* @author Roomain
************************************************/
#include "Capabilities/VE_Capabilities.h"
#include "parameters/VE_QueueCreateInfo.h"

struct VE_DeviceParameter;
struct VE_DeviceFeatures;
struct VE_QueueFamilyParameter;

/*@brief vulkan device capabilities*/
class VE_DeviceCapabilities : public VE_Capabilities
{
private:
	uint32_t m_deviceIndex;									/*!< index of device*/
	VkPhysicalDevice m_physDevice;							/*!< physical device handle*/
	VkPhysicalDeviceProperties m_properties;				/*!< porperties of the device*/
	VkPhysicalDeviceFeatures m_features;					/*!< features supported by device*/
	VkPhysicalDeviceMemoryProperties m_memoryProperties;	/*!< device memory properties*/
	std::vector<VkQueueFamilyProperties> m_queueFamilies;	/*!< queue families properties for the device*/

	[[nodiscard]] static bool verifyFeature(const bool a_expected, const bool a_existing, const std::string& a_log);
	[[nodiscard]] bool isCompatible(const int a_familyIndex, const VkQueueFamilyProperties& a_famProp, const VE_QueueFamilyParameter& a_param, const VkSurfaceKHR a_surface = VK_NULL_HANDLE)const noexcept;
public:
	VE_DeviceCapabilities() = delete;
	VE_DeviceCapabilities(const uint32_t a_index, const VkPhysicalDevice a_physDevice);
	~VE_DeviceCapabilities() override = default;
	[[nodiscard]] constexpr  VkPhysicalDevice physicalDevice()const noexcept { return m_physDevice; }

	[[nodiscard]] uint32_t deviceIndex() const noexcept;
	[[nodiscard]] std::string deviceName() const noexcept;
	[[nodiscard]] std::string deviceType() const noexcept;
	[[nodiscard]] uint32_t apiVersion() const noexcept;
	[[nodiscard]] uint32_t driverVersion() const noexcept;
	[[nodiscard]] const VkPhysicalDeviceFeatures& deviceFeatures()const noexcept;
	[[nodiscard]] const VkPhysicalDeviceProperties& deviceProperties()const noexcept;
	[[nodiscard]] const VkPhysicalDeviceMemoryProperties& memoryProperties()const noexcept;
	using Queue_const_iterator = std::vector<VkQueueFamilyProperties>::const_iterator;
	[[nodiscard]] Queue_const_iterator queueBegin()const noexcept;
	[[nodiscard]] Queue_const_iterator queueEnd()const noexcept;
		
	void visit(VE_CapabilitiesVisitor& a_visitor)const override;

	[[nodiscard]] bool check(const VE_DeviceParameter& a_parameters)const noexcept;
	[[nodiscard]] bool check(const VE_DeviceFeatures& a_parameters)const noexcept;
	[[nodiscard]] bool check(const VE_QueueFamilyParameter& a_parameters, const VkSurfaceKHR a_surface = VK_NULL_HANDLE)const noexcept;
	[[nodiscard]] static VkPhysicalDeviceFeatures toVkFeatures(const VE_DeviceFeatures& a_features);
	[[nodiscard]] bool createQueueCreateInfo(std::vector<VE_QueueCreateInfo>& a_queueInfos, int& a_PresentFamIndex,
		const std::vector<VE_QueueFamilyParameter>& a_queueParams, const VkSurfaceKHR a_surface = VK_NULL_HANDLE)const;
	
};
