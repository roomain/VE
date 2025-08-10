#include "pch.h"
#include <format>
#include "VE_Application.h"
#include "enumerate.h"

VE_InstanceCapabilities VE_Application::m_capabilities;

const VE_InstanceCapabilities& VE_Application::capabilities()
{
	return VE_Application::m_capabilities;
}

VkBool32 VE_Application::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT a_messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT /*a_messageTypes*/,
	const VkDebugUtilsMessengerCallbackDataEXT* a_pCallbackData,
	void*)
{
	switch (a_messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		Logger::info(a_pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		Logger::warning(a_pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		Logger::error(a_pCallbackData->pMessage);
		break;
	default:
		break;
	}
	return true;
}

VE_Application::VE_Application(const VE_Parameter& a_parameter, const CreateSurfaceCallback a_surfCreate) :
	m_surfaceCreateCallback{ a_surfCreate }
{
	auto appInfo = Vulkan::Initializers::applicationInfo("VulkanEngine", 0, "VulkanEngine", 
		ENGINE_VERSION, VK_API_VERSION_1_3);
	auto instanceInfo = Vulkan::Initializers::instanceCreateInfo(&appInfo);
	instanceInfo.enabledLayerCount = static_cast<uint32_t>(a_parameter.layers.size());
	auto tempLayers = vStringToChar(a_parameter.layers);
	instanceInfo.ppEnabledLayerNames = tempLayers.data();
	instanceInfo.enabledExtensionCount = static_cast<uint32_t>(a_parameter.extensions.size());
	auto tempExt = vStringToChar(a_parameter.extensions);
	instanceInfo.ppEnabledExtensionNames = tempExt.data();

	VK_CHECK_EXCEPT(vkCreateInstance(&instanceInfo, nullptr, &m_vulkanInstance))
	
	enumerate(&vkEnumeratePhysicalDevices, m_physicalDevices, m_vulkanInstance);
	if (a_parameter.bIsDebug)
	{
		auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_vulkanInstance, "vkCreateDebugUtilsMessengerEXT");
		if (vkCreateDebugUtilsMessengerEXT)
		{
			VkDebugUtilsMessengerCreateInfoEXT messCI = Vulkan::Initializers::messageCallbackCreateInfo();
			messCI.pUserData = this;
			messCI.pfnUserCallback = &VE_Application::debugCallback;
			VK_CHECK_EXCEPT(vkCreateDebugUtilsMessengerEXT(m_vulkanInstance, &messCI, nullptr, &m_debugMessenger))
		}
	}

	if (m_vulkanInstance != VK_NULL_HANDLE)
	{
		m_capabilities.readCapabilities(m_vulkanInstance);
	}
}

VE_Application::~VE_Application()
{
	// todo
	m_computeDevices.clear();
	m_graphicalDevices.clear();
	vkDestroyInstance(m_vulkanInstance, nullptr);
}

uint32_t VE_Application::numGraphicalDevices()const noexcept
{
	return static_cast<uint32_t>(m_graphicalDevices.size());
}

void VE_Application::createVulkanDevice(VE_DeviceContext& a_ctxt, std::vector<int>& a_useQueuesFamilies, int& a_presentFamQueueIndex, const int a_devIndex, const VE_DeviceParameter& a_devParameters, VkSurfaceKHR a_surface)const
{
	a_ctxt = VE_InstanceContext{ m_vulkanInstance };
	auto features = VE_DeviceCapabilities::toVkFeatures(a_devParameters.features);
	// get queue create info
	a_presentFamQueueIndex = -1;
	std::vector<VE_QueueCreateInfo> a_queueInfos;
	if (auto devIter = (m_capabilities.deviceBegin() + a_devIndex);
		!devIter->createQueueCreateInfo(a_queueInfos, a_presentFamQueueIndex, a_devParameters.queues, a_surface))
	{
		throw Exception(std::format("Device {} hasn't enougth queues.", devIter->deviceName()));
	}

	a_ctxt.m_deviceIndex = a_devIndex;
	a_ctxt.m_physicalDevice = m_physicalDevices[a_devIndex];

	std::vector<VkDeviceQueueCreateInfo> queuesCreateInfo;
	for (const auto& queueInfo : a_queueInfos)
	{
		a_useQueuesFamilies.emplace_back(queueInfo.m_familyIndex);
		queuesCreateInfo.emplace_back(Vulkan::Initializers::queueCreateInfo(queueInfo.m_familyIndex,
			static_cast<uint32_t>(queueInfo.m_priorities.size()),
			queueInfo.m_priorities));
	}

	VkDeviceCreateInfo devInfo = Vulkan::Initializers::deviceCreateInfo(queuesCreateInfo, &features, 0/*VK_QUEUE_GRAPHICS_BIT*/);
	
	// device layers
	devInfo.enabledLayerCount = static_cast<uint32_t>(a_devParameters.layers.size());
	auto tempLayers = vStringToChar(a_devParameters.layers);
	devInfo.ppEnabledLayerNames = tempLayers.data();

	// device extensions
	//    enable layers for VMA
	auto extended = a_devParameters.extensions;
	extended.emplace_back(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME);
	extended.emplace_back(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME);

	devInfo.enabledExtensionCount = static_cast<uint32_t>(extended.size());
	auto tempExt = vStringToChar(extended);
	devInfo.ppEnabledExtensionNames = tempExt.data();

	VK_CHECK_EXCEPT(vkCreateDevice(a_ctxt.m_physicalDevice, &devInfo, nullptr, &a_ctxt.m_logicalDevice))
	
}

std::shared_ptr<VE_GraphicalDevice> VE_Application::createGraphicalDevice(const DeviceChoice& a_choice, const VE_DeviceParameter& a_devParameters)
{
	std::shared_ptr<VE_GraphicalDevice> device;
	if (m_surfaceCreateCallback)
	{
		VkSurfaceKHR surface = VK_NULL_HANDLE;
		m_surfaceCreateCallback(m_vulkanInstance, nullptr, &surface);
		if (int chosenIndex = chooseDevice(a_choice, a_devParameters, surface); chosenIndex >= 0)
		{
			VE_DeviceContext devCtxt{ VE_InstanceContext{m_vulkanInstance} };
			std::vector<int> availableQueueFamilies;
			int presentationQueueFamilyIndex = -1;
			createVulkanDevice(devCtxt, availableQueueFamilies, presentationQueueFamilyIndex, chosenIndex, a_devParameters, surface);
			// because ctor is private
			device = std::shared_ptr<VE_GraphicalDevice>(new VE_GraphicalDevice(devCtxt, availableQueueFamilies, surface, presentationQueueFamilyIndex));
			m_graphicalDevices.emplace_back(device);
		}
	}
	else
	{
		Logger::error("Missing surface creation callback");
	}
	return device;
}

std::shared_ptr<VE_GraphicalDevice> VE_Application::getGraphicalDevice(const uint32_t a_index)const
{
	return m_graphicalDevices[a_index];
}


uint32_t VE_Application::numComputeDevices()const noexcept
{
	return static_cast<uint32_t>(m_computeDevices.size());
}

std::shared_ptr<VE_ComputeDevice> VE_Application::createComputeDevice(const DeviceChoice& a_choice, const VE_DeviceParameter& a_devParameters)
{
	std::shared_ptr<VE_ComputeDevice> device;
	if ( int chosenIndex = chooseDevice(a_choice, a_devParameters); chosenIndex >= 0)
	{
		// todo
	}
	return device;
}

std::shared_ptr<VE_ComputeDevice> VE_Application::getComputeDevice(const uint32_t a_index)const
{
	return m_computeDevices[a_index];
}
