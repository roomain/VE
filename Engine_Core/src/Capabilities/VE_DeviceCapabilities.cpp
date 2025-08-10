#include <ranges>
#include <format>
#include "utils/vulkan_enum_to_string.h"
#include "enumerate.h"
#include "pch.h"

VE_DeviceCapabilities::VE_DeviceCapabilities(const uint32_t a_index, const VkPhysicalDevice a_physDevice) : VE_Capabilities(),
m_deviceIndex{ a_index }, m_physDevice { a_physDevice }
{
	enumerate(&vkEnumerateDeviceExtensionProperties, m_extensions, m_physDevice, nullptr);
	enumerate(&vkEnumerateDeviceLayerProperties, m_layers, m_physDevice);
	vkGetPhysicalDeviceProperties(m_physDevice, &m_properties);
	vkGetPhysicalDeviceFeatures(m_physDevice, &m_features);
	enumerate(&vkGetPhysicalDeviceQueueFamilyProperties, m_queueFamilies, m_physDevice);	
	vkGetPhysicalDeviceMemoryProperties(m_physDevice, &m_memoryProperties);
}

uint32_t VE_DeviceCapabilities::deviceIndex() const noexcept
{
	return m_deviceIndex;
}

std::string VE_DeviceCapabilities::deviceName() const noexcept
{
	return m_properties.deviceName;
}

std::string VE_DeviceCapabilities::deviceType() const noexcept
{
	return to_string(m_properties.deviceType);
}

uint32_t VE_DeviceCapabilities::apiVersion() const noexcept
{
	return m_properties.apiVersion;
}

uint32_t VE_DeviceCapabilities::driverVersion() const noexcept
{
	return m_properties.driverVersion;
}

const VkPhysicalDeviceFeatures& VE_DeviceCapabilities::deviceFeatures()const noexcept
{
	return m_features;
}

const VkPhysicalDeviceProperties& VE_DeviceCapabilities::deviceProperties()const noexcept
{
	return m_properties;
}

const VkPhysicalDeviceMemoryProperties& VE_DeviceCapabilities::memoryProperties()const noexcept
{
	return m_memoryProperties;
}

VE_DeviceCapabilities::Queue_const_iterator VE_DeviceCapabilities::queueBegin()const noexcept
{
	return m_queueFamilies.cbegin();
}

VE_DeviceCapabilities::Queue_const_iterator VE_DeviceCapabilities::queueEnd()const noexcept
{
	return m_queueFamilies.cend();
}

void VE_DeviceCapabilities::visit(VE_CapabilitiesVisitor& a_visitor)const
{
	a_visitor.visit(*this);
}

bool VE_DeviceCapabilities::check(const VE_DeviceParameter& a_parameters)const noexcept
{
	bool bCheck = true;
	// check 
	Logger::info("Check device parameters---");
	Logger::info("---Check layers");
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
	Logger::info("---Check extensions");
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

	bCheck &= check(a_parameters.features);


	Logger::info("Check Queues---");
	for (const auto& queueParam : a_parameters.queues)
	{
		Logger::info(std::format("---Check queue parameters"));
		bCheck &= check(queueParam);
	}
	return bCheck;
}

bool VE_DeviceCapabilities::verifyFeature(const bool a_expected, const bool a_existing, const std::string& a_log)
{
	if (a_expected && !a_existing)
	{
		Logger::info(a_log);
		return false;
	}
	return true;
}

bool VE_DeviceCapabilities::check(const VE_DeviceFeatures& a_parameters)const noexcept
{
	Logger::info("Check features---");
	bool bCheck = VE_DeviceCapabilities::verifyFeature(a_parameters.robustBufferAccess,
		m_features.robustBufferAccess, "robustBufferAccess");

	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.multiDrawIndirect, m_features.multiDrawIndirect, "multiDrawIndirect");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.multiDrawIndirect, m_features.multiDrawIndirect, "multiDrawIndirect");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.drawIndirectFirstInstance, m_features.drawIndirectFirstInstance, "drawIndirectFirstInstance");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.depthClamp, m_features.depthClamp, "depthClamp");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.depthBiasClamp, m_features.depthBiasClamp, "depthBiasClamp");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.fillModeNonSolid, m_features.fillModeNonSolid, "fillModeNonSolid");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.depthBounds, m_features.depthBounds, "depthBounds");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.wideLines, m_features.wideLines, "wideLines");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.largePoints, m_features.largePoints, "largePoints");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.alphaToOne, m_features.alphaToOne, "alphaToOne");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.multiViewport, m_features.multiViewport, "multiViewport");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.samplerAnisotropy, m_features.samplerAnisotropy, "samplerAnisotropy");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.textureCompressionETC2, m_features.textureCompressionETC2, "textureCompressionETC2");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.textureCompressionASTC_LDR, m_features.textureCompressionASTC_LDR, "textureCompressionASTC_LDR");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.textureCompressionBC, m_features.textureCompressionBC, "textureCompressionBC");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.occlusionQueryPrecise, m_features.occlusionQueryPrecise, "occlusionQueryPrecise");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.pipelineStatisticsQuery, m_features.pipelineStatisticsQuery, "pipelineStatisticsQuery");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.vertexPipelineStoresAndAtomics, m_features.vertexPipelineStoresAndAtomics, "vertexPipelineStoresAndAtomics");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.fragmentStoresAndAtomics, m_features.fragmentStoresAndAtomics, "fragmentStoresAndAtomics");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderTessellationAndGeometryPointSize, m_features.shaderTessellationAndGeometryPointSize, "shaderTessellationAndGeometryPointSize");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderImageGatherExtended, m_features.shaderImageGatherExtended, "shaderImageGatherExtended");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderStorageImageExtendedFormats, m_features.shaderStorageImageExtendedFormats, "shaderStorageImageExtendedFormats");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderStorageImageMultisample, m_features.shaderStorageImageMultisample, "shaderStorageImageMultisample");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderStorageImageReadWithoutFormat, m_features.shaderStorageImageReadWithoutFormat, "shaderStorageImageReadWithoutFormat");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderStorageImageWriteWithoutFormat, m_features.shaderStorageImageWriteWithoutFormat, "shaderStorageImageWriteWithoutFormat");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderUniformBufferArrayDynamicIndexing, m_features.shaderUniformBufferArrayDynamicIndexing, "shaderUniformBufferArrayDynamicIndexing");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderSampledImageArrayDynamicIndexing, m_features.shaderSampledImageArrayDynamicIndexing, "shaderSampledImageArrayDynamicIndexing");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderStorageBufferArrayDynamicIndexing, m_features.shaderStorageBufferArrayDynamicIndexing, "shaderStorageBufferArrayDynamicIndexing");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderStorageImageArrayDynamicIndexing, m_features.shaderStorageImageArrayDynamicIndexing, "shaderStorageImageArrayDynamicIndexing");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderClipDistance, m_features.shaderClipDistance, "shaderClipDistance");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderCullDistance, m_features.shaderCullDistance, "shaderCullDistance");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderFloat64, m_features.shaderFloat64, "shaderFloat64");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderInt64, m_features.shaderInt64, "shaderInt64");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderInt16, m_features.shaderInt16, "shaderInt16");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderResourceResidency, m_features.shaderResourceResidency, "shaderResourceResidency");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.shaderResourceMinLod, m_features.shaderResourceMinLod, "shaderResourceMinLod");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.sparseBinding, m_features.sparseBinding, "sparseBinding");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.sparseResidencyBuffer, m_features.sparseResidencyBuffer, "sparseResidencyBuffer");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.sparseResidencyImage2D, m_features.sparseResidencyImage2D, "sparseResidencyImage2D");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.sparseResidencyImage3D, m_features.sparseResidencyImage3D, "sparseResidencyImage3D");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.sparseResidency2Samples, m_features.sparseResidency2Samples, "sparseResidency2Samples");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.sparseResidency4Samples, m_features.sparseResidency4Samples, "sparseResidency4Samples");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.inheritedQueries,
		m_features.inheritedQueries, "inheritedQueries");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.variableMultisampleRate,
		m_features.variableMultisampleRate, "variableMultisampleRate");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.sparseResidencyAliased,
		m_features.sparseResidencyAliased, "sparseResidencyAliased");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.sparseResidency16Samples,
		m_features.sparseResidency16Samples, "sparseResidency16Samples");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.sparseResidency8Samples,
		m_features.sparseResidency8Samples, "sparseResidency8Samples");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.sparseResidency4Samples,
		m_features.sparseResidency4Samples, "sparseResidency4Samples");

	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.multiDrawIndirect,
		m_features.multiDrawIndirect, "multiDrawIndirect");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.logicOp,
		m_features.logicOp, "logicOp");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.dualSrcBlend,
		m_features.dualSrcBlend, "dualSrcBlend");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.sampleRateShading,
		m_features.sampleRateShading, "sampleRateShading");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.tessellationShader,
		m_features.tessellationShader, "tessellationShader");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.geometryShader,
		m_features.geometryShader, "geometryShader");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.independentBlend,
		m_features.independentBlend, "independentBlend");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.imageCubeArray,
		m_features.imageCubeArray, "imageCubeArray");
	bCheck &= VE_DeviceCapabilities::verifyFeature(a_parameters.fullDrawIndexUint32,
		m_features.fullDrawIndexUint32, "fullDrawIndexUint32");



	return bCheck;
}

bool VE_DeviceCapabilities::isCompatible(const int a_familyIndex, const VkQueueFamilyProperties& a_famProp, const VE_QueueFamilyParameter& a_param, const VkSurfaceKHR a_surface)const noexcept
{
	if ((a_famProp.queueFlags & static_cast<VkQueueFlags>(a_param.flags)) == static_cast<VkQueueFlags>(a_param.flags) &&
		a_famProp.queueCount >= a_param.count)
	{
		if (a_param.bPresentationAvailable && (a_surface != VK_NULL_HANDLE))
		{
			VkBool32 supported = false;
			VK_CHECK_LOG(vkGetPhysicalDeviceSurfaceSupportKHR(m_physDevice, a_familyIndex, a_surface, &supported))
			if (supported)
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool VE_DeviceCapabilities::check(const VE_QueueFamilyParameter& a_parameters, const VkSurfaceKHR a_surface)const noexcept
{
	int index = 0;
	for (const auto& queue : m_queueFamilies)
	{
		if (isCompatible(index, queue, a_parameters, a_surface))
		{
			return true;
		}
		++index;
	}
	return false;
}

VkPhysicalDeviceFeatures VE_DeviceCapabilities::toVkFeatures(const VE_DeviceFeatures& a_features)
{
	return VkPhysicalDeviceFeatures
	{
		a_features.robustBufferAccess,
		a_features.fullDrawIndexUint32,
		a_features.imageCubeArray,
		a_features.independentBlend,
		a_features.geometryShader,
		a_features.tessellationShader,
		a_features.sampleRateShading,
		a_features.dualSrcBlend,
		a_features.logicOp,
		a_features.multiDrawIndirect,
		a_features.drawIndirectFirstInstance,
		a_features.depthClamp,
		a_features.depthBiasClamp,
		a_features.fillModeNonSolid,
		a_features.depthBounds,
		a_features.wideLines,
		a_features.largePoints,
		a_features.alphaToOne,
		a_features.multiViewport,
		a_features.samplerAnisotropy,
		a_features.textureCompressionETC2,
		a_features.textureCompressionASTC_LDR,
		a_features.textureCompressionBC,
		a_features.occlusionQueryPrecise,
		a_features.pipelineStatisticsQuery,
		a_features.vertexPipelineStoresAndAtomics,
		a_features.fragmentStoresAndAtomics,
		a_features.shaderTessellationAndGeometryPointSize,
		a_features.shaderImageGatherExtended,
		a_features.shaderStorageImageExtendedFormats,
		a_features.shaderStorageImageMultisample,
		a_features.shaderStorageImageReadWithoutFormat,
		a_features.shaderStorageImageWriteWithoutFormat,
		a_features.shaderUniformBufferArrayDynamicIndexing,
		a_features.shaderSampledImageArrayDynamicIndexing,
		a_features.shaderStorageBufferArrayDynamicIndexing,
		a_features.shaderStorageImageArrayDynamicIndexing,
		a_features.shaderClipDistance,
		a_features.shaderCullDistance,
		a_features.shaderFloat64,
		a_features.shaderInt64,
		a_features.shaderInt16,
		a_features.shaderResourceResidency,
		a_features.shaderResourceMinLod,
		a_features.sparseBinding,
		a_features.sparseResidencyBuffer,
		a_features.sparseResidencyImage2D,
		a_features.sparseResidencyImage3D,
		a_features.sparseResidency2Samples,
		a_features.sparseResidency4Samples,
		a_features.sparseResidency8Samples,
		a_features.sparseResidency16Samples,
		a_features.sparseResidencyAliased,
		a_features.variableMultisampleRate,
		a_features.inheritedQueries
	};
}

// faire queue info
bool VE_DeviceCapabilities::createQueueCreateInfo(std::vector<VE_QueueCreateInfo>& a_queueInfos, int& a_PresentFamIndex,
	const std::vector<VE_QueueFamilyParameter>& a_queueParams, const VkSurfaceKHR a_surface)const
{
	a_PresentFamIndex = -1;
	for (const auto& param : a_queueParams)
	{
		int numQueueRequested = param.count;
		uint32_t familyIndex = 0;
		for (const auto& famProps : m_queueFamilies)
		{
			auto iter = std::ranges::find_if(a_queueInfos, [familyIndex](const auto& a_info)
				{
					return a_info.m_familyIndex == familyIndex;
				});
			//compute available queue
			int availableQueues = famProps.queueCount;

			bool alreadyExists = iter != a_queueInfos.cend();
			if (alreadyExists)
				availableQueues -= static_cast<int>(iter->m_priorities.size());

			// has available queues check compatibility
			if ((availableQueues > 0) && VE_DeviceCapabilities::isCompatible(familyIndex, famProps, param, a_surface))
			{
				// get the presentation family
				if (a_surface && param.bPresentationAvailable)
					a_PresentFamIndex = static_cast<int>(familyIndex);

				int numActiveQueue = std::min(numQueueRequested, availableQueues);
				numQueueRequested -= numActiveQueue;
				if (!alreadyExists)
				{
					std::vector<float> priorities(numActiveQueue, param.priority);
					a_queueInfos.emplace_back(familyIndex, priorities);
				}
				else
				{
					iter->m_priorities.insert(iter->m_priorities.end(), numActiveQueue, param.priority);
				}
			}
			++familyIndex;

			if (numQueueRequested <= 0)
				break;// all queues found
		}
	
		if (numQueueRequested > 0)
			return false;// missing queues
	}
	return true;
}
