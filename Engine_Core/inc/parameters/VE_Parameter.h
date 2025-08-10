#pragma once
#include <string>
#include <vector>

#include "Reflection_macros.h"
#include "vulkan/vulkan.hpp"
#include "bitOperators.h"
#include "VE_Parameter.generated.h"

/*Need to override flags because flag in vulkan are unsigned int*/
using QueueFlag = VkQueueFlagBits;

REFLECT_CLASS(VE_Parameter)
struct VE_Parameter
{
	DESERIALIZE_DECLARE(VE_Parameter)
    REFLECT_MEMBER
	bool bIsDebug = false;
	REFLECT_MEMBER
	std::vector<std::string> extensions;	/*!< vulkan instance extensions properties*/
	REFLECT_MEMBER
	std::vector<std::string> layers;		/*!< vulkan instance desired layer*/
};

DESERIALIZE_INLINE_IMPL(VE_Parameter)



REFLECT_CLASS(VE_QueueFamilyParameter)
struct VE_QueueFamilyParameter
{
	DESERIALIZE_DECLARE(VE_QueueFamilyParameter)
	REFLECT_MEMBER
	bool bPresentationAvailable = false;	/*!< is rendering queue*/
	REFLECT_MEMBER
    QueueFlag flags = static_cast<QueueFlag>(0);  /*!< queue flags*/
	REFLECT_MEMBER
	uint32_t count = 1;			            /*!< number of queue*/
    REFLECT_MEMBER
    float priority = 0;                     /*!< queue priorities*/
};

DESERIALIZE_INLINE_IMPL(VE_QueueFamilyParameter)

REFLECT_CLASS(VE_DeviceFeatures)
struct VE_DeviceFeatures 
{
    DESERIALIZE_DECLARE(VE_DeviceFeatures)
    REFLECT_MEMBER
    bool robustBufferAccess = false;
    REFLECT_MEMBER
    bool fullDrawIndexUint32 = false;
    REFLECT_MEMBER
    bool imageCubeArray = false;
    REFLECT_MEMBER
    bool independentBlend = false;
    REFLECT_MEMBER
    bool geometryShader = false;
    REFLECT_MEMBER
    bool tessellationShader = false;
    REFLECT_MEMBER
    bool sampleRateShading = false;
    REFLECT_MEMBER
    bool dualSrcBlend = false;
    REFLECT_MEMBER
    bool logicOp = false;
    REFLECT_MEMBER
    bool multiDrawIndirect = false;
    REFLECT_MEMBER
    bool drawIndirectFirstInstance = false;
    REFLECT_MEMBER
    bool depthClamp = false;
    REFLECT_MEMBER
    bool depthBiasClamp = false;
    REFLECT_MEMBER
    bool fillModeNonSolid = false;
    REFLECT_MEMBER
    bool depthBounds = false;
    REFLECT_MEMBER
    bool wideLines = false;
    REFLECT_MEMBER
    bool largePoints = false;
    REFLECT_MEMBER
    bool alphaToOne = false;
    REFLECT_MEMBER
    bool multiViewport = false;
    REFLECT_MEMBER
    bool samplerAnisotropy = false;
    REFLECT_MEMBER
    bool textureCompressionETC2 = false;
    REFLECT_MEMBER
    bool textureCompressionASTC_LDR = false;
    REFLECT_MEMBER
    bool textureCompressionBC = false;
    REFLECT_MEMBER
    bool occlusionQueryPrecise = false;
    REFLECT_MEMBER
    bool pipelineStatisticsQuery = false;
    REFLECT_MEMBER
    bool vertexPipelineStoresAndAtomics = false;
    REFLECT_MEMBER
    bool fragmentStoresAndAtomics = false;
    REFLECT_MEMBER
    bool shaderTessellationAndGeometryPointSize = false;
    REFLECT_MEMBER
    bool shaderImageGatherExtended = false;
    REFLECT_MEMBER
    bool shaderStorageImageExtendedFormats = false;
    REFLECT_MEMBER
    bool shaderStorageImageMultisample = false;
    REFLECT_MEMBER
    bool shaderStorageImageReadWithoutFormat = false;
    REFLECT_MEMBER
    bool shaderStorageImageWriteWithoutFormat = false;
    REFLECT_MEMBER
    bool shaderUniformBufferArrayDynamicIndexing = false;
    REFLECT_MEMBER
    bool shaderSampledImageArrayDynamicIndexing = false;
    REFLECT_MEMBER
    bool shaderStorageBufferArrayDynamicIndexing = false;
    REFLECT_MEMBER
    bool shaderStorageImageArrayDynamicIndexing = false;
    REFLECT_MEMBER
    bool shaderClipDistance = false;
    REFLECT_MEMBER
    bool shaderCullDistance = false;
    REFLECT_MEMBER
    bool shaderFloat64 = false;
    REFLECT_MEMBER
    bool shaderInt64 = false;
    REFLECT_MEMBER
    bool shaderInt16 = false;
    REFLECT_MEMBER
    bool shaderResourceResidency = false;
    REFLECT_MEMBER
    bool shaderResourceMinLod = false;
    REFLECT_MEMBER
    bool sparseBinding = false;
    REFLECT_MEMBER
    bool sparseResidencyBuffer = false;
    REFLECT_MEMBER
    bool sparseResidencyImage2D = false;
    REFLECT_MEMBER
    bool sparseResidencyImage3D = false;
    REFLECT_MEMBER
    bool sparseResidency2Samples = false;
    REFLECT_MEMBER
    bool sparseResidency4Samples = false;
    REFLECT_MEMBER
    bool sparseResidency8Samples = false;
    REFLECT_MEMBER
    bool sparseResidency16Samples = false;
    REFLECT_MEMBER
    bool sparseResidencyAliased = false;
    REFLECT_MEMBER
    bool variableMultisampleRate = false;
    REFLECT_MEMBER
    bool inheritedQueries = false;
};
DESERIALIZE_INLINE_IMPL(VE_DeviceFeatures)


REFLECT_CLASS(VE_DeviceParameter)
struct VE_DeviceParameter
{
	DESERIALIZE_DECLARE(VE_DeviceParameter)
	REFLECT_MEMBER
	std::vector<std::string> extensions;			/*!< vulkan device extensions properties*/
	REFLECT_MEMBER
	std::vector<std::string> layers;				/*!< vulkan device desired layer*/
	REFLECT_MEMBER
	std::vector<VE_QueueFamilyParameter> queues;	/*!< vulkan queue family parameters*/
    REFLECT_MEMBER
    VE_DeviceFeatures features;                  /*!< device requested features*/
};

DESERIALIZE_INLINE_IMPL(VE_DeviceParameter)