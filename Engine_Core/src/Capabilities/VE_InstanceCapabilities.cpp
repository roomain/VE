#include "pch.h"
#include <ranges>
#include <format>
#include "Capabilities/VE_InstanceCapabilities.h"
#include "enumerate.h"

void VE_InstanceCapabilities::readCapabilities()
{
	enumerate(&vkEnumerateInstanceExtensionProperties, m_extensions, nullptr);
	enumerate(&vkEnumerateInstanceLayerProperties, m_layers);

	auto appInfo = Vulkan::Initializers::applicationInfo("VulkanEngine", 0, "VulkanEngine", 0);
	auto instanceInfo = Vulkan::Initializers::instanceCreateInfo(&appInfo);
	VkInstance tempInstance;
	VK_CHECK_EXCEPT(vkCreateInstance(&instanceInfo, nullptr, &tempInstance))
	readCapabilities(tempInstance);
	vkDestroyInstance(tempInstance, nullptr);
}

void VE_InstanceCapabilities::readCapabilities(const VkInstance a_instance)
{
	m_devicesCap.clear();

	std::vector<VkPhysicalDevice> vDevices;
	enumerate(&vkEnumeratePhysicalDevices, vDevices, a_instance);

	uint32_t deviceIndex = 0;
	for (const auto dev : vDevices)
	{
		m_devicesCap.emplace_back(deviceIndex, dev);
		++deviceIndex;
	}
}

void VE_InstanceCapabilities::visit(VE_CapabilitiesVisitor& a_visitor)const
{
	a_visitor.visit(*this);

	for (const auto& dev : m_devicesCap)
	{
		a_visitor.visit(dev);
	}
}

bool VE_InstanceCapabilities::check(const VE_Parameter& a_parameters)const noexcept
{
	bool bCheck = true;
	// check 
	Logger::info("Check parameters---");
	Logger::info("---Check instance layers");
	for (const auto& layer : a_parameters.layers)
	{
		if (std::ranges::find_if(m_layers, [layer](const auto& localLayer)
			{
				return std::string(localLayer.layerName) == layer;
			}) == m_layers.cend())
		{
			Logger::error(std::format("layer {0} not fpund", layer));
			bCheck = false;
		}
	}
	Logger::info("---Check instance extensions");
	for (const auto& ext : a_parameters.extensions)
	{
		if (std::ranges::find_if(m_extensions, [ext](const auto& localExt)
			{
				return std::string(localExt.extensionName) == ext;
			}) == m_extensions.cend())
		{
			Logger::error(std::format("extension {0} not fpund", ext));
			bCheck = false;
		}
	}
	return bCheck;
}

VE_InstanceCapabilities::Device_const_iterator VE_InstanceCapabilities::deviceBegin()const noexcept
{
	return m_devicesCap.cbegin();
}

VE_InstanceCapabilities::Device_const_iterator VE_InstanceCapabilities::deviceEnd()const noexcept
{
	return m_devicesCap.cend();
}


std::vector<int> VE_InstanceCapabilities::findSuitableDevice(const VE_DeviceParameter& a_devParameters, const VkSurfaceKHR a_surface)const noexcept
{
	std::vector<int> suitableDevices;
	for (const auto& dev : m_devicesCap)
	{
		if (dev.checkExtensions(a_devParameters.extensions) &&
			dev.checkLayers(a_devParameters.layers) &&
			dev.check(a_devParameters.features))
		{
			for (const auto& queueParam : a_devParameters.queues)
			{
				if (dev.check(queueParam, a_surface))
				{
					suitableDevices.emplace_back(dev.deviceIndex());
					break;
				}
			}
		}
	}
	return suitableDevices;
}