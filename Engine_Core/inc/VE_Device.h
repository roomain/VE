#pragma once
/***********************************************
* @headerfile VE_Device.h
* @date 15 / 07 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <vector>
#include "vulkan/vulkan.hpp"
#include "utils/VulkanContext.h"
#include "VE_QueueFamily.h"
#include "core_globals.h"

#pragma warning(push)
#pragma warning(disable:4251) // disable warning for dll export/import


class VE_CORE_LIB VE_Device : public VulkanObject<VE_DeviceContext>
{
	friend class VE_Application;

protected:	
	std::vector<VE_QueueFamily>	m_queues; /*!< queues availables/used by device*/

	void createMemoryAllocator();
	explicit VE_Device(const VE_DeviceContext& a_context, const std::vector<int>& a_queueFamilies);

public:
	virtual ~VE_Device() override;


	VkQueue createQueue(const VkQueueFlags a_flag);

};
#pragma warning(pop)