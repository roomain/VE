#include "pch.h"
#include <format>
#include "Capabilities/VE_CapabilitiesVisitor.h"
#include "Capabilities/VE_Capabilities.h"
#include "Capabilities/VE_InstanceCapabilities.h"
#include "Capabilities/VE_DeviceCapabilities.h"
#include "utils/vulkan_enum_to_string.h"


std::string VE_CapabilitiesVisitor::versionToString(const unsigned int a_version)
{
	return  std::to_string(a_version >> 22) + "." + std::to_string((a_version >> 12) & 0x3ff) + "." + std::to_string(a_version & 0xfff);
}

void VE_CapabilitiesVisitor::visit(const VE_Capabilities& a_capabilities)
{
	displayNode("Layers");
	for (auto layerIt = a_capabilities.layerBegin(); layerIt != a_capabilities.layerEnd(); ++layerIt)
	{
		displayNodeCapability("Name", layerIt->layerName);
		displayNodeCapability("Description", layerIt->description);
		displayNodeCapability("Impl version", VE_CapabilitiesVisitor::versionToString(layerIt->implementationVersion).c_str());
		displayNodeCapability("Spec version", VE_CapabilitiesVisitor::versionToString(layerIt->specVersion).c_str());
	}
	popNode();

	displayNode("Extensions");
	for (auto extIt = a_capabilities.extensionBegin(); extIt != a_capabilities.extensionEnd(); ++extIt)
	{
		displayNodeCapability("Name", extIt->extensionName);
		displayNodeCapability("Spec version", VE_CapabilitiesVisitor::versionToString(extIt->specVersion).c_str());

	}
	popNode();
}

void VE_CapabilitiesVisitor::visit(const VE_InstanceCapabilities& a_capabilities)
{
	displayNode("Instance");
	visit(static_cast<const VE_Capabilities&>(a_capabilities));
	//
	popNode();
}

void VE_CapabilitiesVisitor::visit(const VE_DeviceCapabilities& a_capabilities)
{
	displayNode(std::format("Device {0}", a_capabilities.deviceIndex()));
	const auto& devProp = a_capabilities.deviceProperties();
	displayNodeCapability("Type", to_string(devProp.deviceType).c_str());
	displayNodeCapability("Device Id", std::to_string(devProp.deviceID).c_str());
	displayNodeCapability("Vendor Id", std::to_string(devProp.vendorID).c_str());
	displayNodeCapability("Name", a_capabilities.deviceName().c_str());
	displayNodeCapability("Name", a_capabilities.deviceName().c_str());
	displayNodeCapability("Driver version", VE_CapabilitiesVisitor::versionToString(a_capabilities.driverVersion()).c_str());
	displayNodeCapability("API version", VE_CapabilitiesVisitor::versionToString(a_capabilities.driverVersion()).c_str());
    
	visit(devProp.limits);
	visit(devProp.sparseProperties);

	visit(static_cast<const VE_Capabilities&>(a_capabilities));

    visit(a_capabilities.deviceFeatures());
	// memory
	const auto& memProp = a_capabilities.memoryProperties();
	displayNode("Memory");
	for (uint32_t index = 0; index < memProp.memoryTypeCount; ++index)
	{
		displayNodeCapability("Mem type flags", Flag<VkMemoryPropertyFlagBits>::to_string(memProp.memoryTypes[index].propertyFlags).c_str());
		displayNodeCapability("Mem type Heap index", std::to_string(memProp.memoryTypes[index].heapIndex).c_str());
	}

	for (uint32_t index = 0; index < memProp.memoryHeapCount; ++index)
	{
		displayNodeCapability("Mem heap flags", Flag<VkMemoryHeapFlagBits>::to_string(memProp.memoryHeaps[index].flags).c_str());
		displayNodeCapability("Mem heap size", VE_CapabilitiesVisitor::memorySize(memProp.memoryHeaps[index].size).c_str());
	}
	popNode();
	//
	popNode();
}

void VE_CapabilitiesVisitor::visit(const VkPhysicalDeviceFeatures& a_features)
{
    displayNode("features");
    displayNodeCapability("Robust buffer access", static_cast<bool>(a_features.robustBufferAccess));
    displayNodeCapability("Full draw index uint32", static_cast<bool>(a_features.fullDrawIndexUint32));
    displayNodeCapability("Image cube array", static_cast<bool>(a_features.imageCubeArray));
    displayNodeCapability("Independent blend", static_cast<bool>(a_features.independentBlend));
    displayNodeCapability("Geometry shader", static_cast<bool>(a_features.geometryShader));
    displayNodeCapability("Teselation shader", static_cast<bool>(a_features.tessellationShader));
    displayNodeCapability("Sample rate shading", static_cast<bool>(a_features.sampleRateShading));
    displayNodeCapability("Dual src blend", static_cast<bool>(a_features.dualSrcBlend));
    displayNodeCapability("Logic Op", static_cast<bool>(a_features.logicOp));
    displayNodeCapability("Multi draw indirect", static_cast<bool>(a_features.multiDrawIndirect));
    displayNodeCapability("Draw indirect first instance", static_cast<bool>(a_features.drawIndirectFirstInstance));
    displayNodeCapability("Depth clamp", static_cast<bool>(a_features.depthClamp));
    displayNodeCapability("Depth bias clamp", static_cast<bool>(a_features.depthBiasClamp));
    displayNodeCapability("Fill mode non solid", static_cast<bool>(a_features.fillModeNonSolid));
    displayNodeCapability("Depthh bounds", static_cast<bool>(a_features.depthBounds));
    displayNodeCapability("Wide lines", static_cast<bool>(a_features.wideLines));
    displayNodeCapability("Large points", static_cast<bool>(a_features.largePoints));
    displayNodeCapability("Alpha to one", static_cast<bool>(a_features.alphaToOne));
    displayNodeCapability("Multi viewport", static_cast<bool>(a_features.multiViewport));
    displayNodeCapability("Sampler anisotropy", static_cast<bool>(a_features.samplerAnisotropy));
    displayNodeCapability("Texture compression ETC2", static_cast<bool>(a_features.textureCompressionETC2));
    displayNodeCapability("Texture compression ASTC_LDR", static_cast<bool>(a_features.textureCompressionASTC_LDR));
    displayNodeCapability("Texture compression BC", static_cast<bool>(a_features.textureCompressionBC));
    displayNodeCapability("Occlusion query precise", static_cast<bool>(a_features.occlusionQueryPrecise));
    displayNodeCapability("Pipeline statics query", static_cast<bool>(a_features.pipelineStatisticsQuery));
    displayNodeCapability("Vertex pipeline stores and atomics", static_cast<bool>(a_features.vertexPipelineStoresAndAtomics));
    displayNodeCapability("Fragment stores and atomics", static_cast<bool>(a_features.fragmentStoresAndAtomics));
    displayNodeCapability("Shader tesselation and geometry point size", static_cast<bool>(a_features.shaderTessellationAndGeometryPointSize));
    displayNodeCapability("Shader image gather extended", static_cast<bool>(a_features.shaderImageGatherExtended));
    displayNodeCapability("Shader storage image extended formats", static_cast<bool>(a_features.shaderStorageImageExtendedFormats));
    displayNodeCapability("Shader storage image multisample", static_cast<bool>(a_features.shaderStorageImageMultisample));
    displayNodeCapability("Shader storage image read without format", static_cast<bool>(a_features.shaderStorageImageReadWithoutFormat));
    displayNodeCapability("Shader storage image write without format", static_cast<bool>(a_features.shaderStorageImageWriteWithoutFormat));
    displayNodeCapability("Shader uniform buffer array dynamic indexing", static_cast<bool>(a_features.shaderUniformBufferArrayDynamicIndexing));
    displayNodeCapability("Shader ampled image array dynamic indexing", static_cast<bool>(a_features.shaderSampledImageArrayDynamicIndexing));
    displayNodeCapability("Shader storage buffer array dynamic indexing", static_cast<bool>(a_features.shaderStorageBufferArrayDynamicIndexing));
    displayNodeCapability("Shader storage image array dynamic indexing", static_cast<bool>(a_features.shaderStorageImageArrayDynamicIndexing));
    displayNodeCapability("Shader clip distance", static_cast<bool>(a_features.shaderClipDistance));
    displayNodeCapability("Shader cull distance", static_cast<bool>(a_features.shaderCullDistance));
    displayNodeCapability("Shader Float 64", static_cast<bool>(a_features.shaderFloat64));
    displayNodeCapability("Shader Int 64", static_cast<bool>(a_features.shaderInt64));
    displayNodeCapability("Shader Int 16", static_cast<bool>(a_features.shaderInt16));
    displayNodeCapability("Shader resource residency", static_cast<bool>(a_features.shaderResourceResidency));
    displayNodeCapability("Shader resource min lod", static_cast<bool>(a_features.shaderResourceMinLod));
    displayNodeCapability("Sparse binding", static_cast<bool>(a_features.sparseBinding));
    displayNodeCapability("Sparse residency buffer", static_cast<bool>(a_features.sparseResidencyBuffer));
    displayNodeCapability("Sparse residency image 2D", static_cast<bool>(a_features.sparseResidencyImage2D));
    displayNodeCapability("Sparse residency image 3D", static_cast<bool>(a_features.sparseResidencyImage3D));
    displayNodeCapability("Sparse residency 2 samples", static_cast<bool>(a_features.sparseResidency2Samples));
    displayNodeCapability("Sparse residency 4 samples", static_cast<bool>(a_features.sparseResidency4Samples));
    displayNodeCapability("Sparse residency 8 samples", static_cast<bool>(a_features.sparseResidency8Samples));
    displayNodeCapability("Sparse residency 16 samples", static_cast<bool>(a_features.sparseResidency16Samples));
    displayNodeCapability("Sparse residency aliased", static_cast<bool>(a_features.sparseResidencyAliased));
    displayNodeCapability("Variable multisample rate", static_cast<bool>(a_features.variableMultisampleRate));
    displayNodeCapability("Inherited queries", static_cast<bool>(a_features.inheritedQueries));
    popNode();
}

void VE_CapabilitiesVisitor::visit(const VkPhysicalDeviceLimits& a_limits)
{
	displayNode("Limits");
    displayNodeCapability("Max image dim 1D", a_limits.maxImageDimension1D);
    displayNodeCapability("Max image dim 2D", a_limits.maxImageDimension2D);
    displayNodeCapability("Max image dim 3D", a_limits.maxImageDimension3D);
    displayNodeCapability("Max image dim cube",a_limits.maxImageDimensionCube);
    displayNodeCapability("Max image array layer",a_limits.maxImageArrayLayers);
    displayNodeCapability("Max texel buffer element",a_limits.maxTexelBufferElements);
    displayNodeCapability("Max uniform buffer range",a_limits.maxUniformBufferRange);
    displayNodeCapability("Max storage buffer range",a_limits.maxStorageBufferRange);
    displayNodeCapability("Max push constant size",a_limits.maxPushConstantsSize);
    displayNodeCapability("Max memory allocation count",a_limits.maxMemoryAllocationCount);
    displayNodeCapability("Max sampler allocation count",a_limits.maxSamplerAllocationCount);
    displayNodeCapability("Buffer image granularity",VE_CapabilitiesVisitor::memorySize(a_limits.bufferImageGranularity).c_str());
    displayNodeCapability("Sparse address size",VE_CapabilitiesVisitor::memorySize(a_limits.sparseAddressSpaceSize).c_str());
    displayNodeCapability("Max bound descriptor sets",a_limits.maxBoundDescriptorSets);
    displayNodeCapability("Max per stage descriptor samplers",a_limits.maxPerStageDescriptorSamplers);
    displayNodeCapability("Max per stage descriptor uniform buffers",a_limits.maxPerStageDescriptorUniformBuffers);
    displayNodeCapability("Max per stage descriptor storage buffers",a_limits.maxPerStageDescriptorStorageBuffers);
    displayNodeCapability("Max per stage descriptor sampled images",a_limits.maxPerStageDescriptorSampledImages);
    displayNodeCapability("Max per stage descriptor storage images",a_limits.maxPerStageDescriptorStorageImages);
    displayNodeCapability("Max per stage descriptor input attachment",a_limits.maxPerStageDescriptorInputAttachments);
    displayNodeCapability("Max per stage resources",a_limits.maxPerStageResources);
    displayNodeCapability("Max descriptor set samplers ",a_limits.maxDescriptorSetSamplers);
    displayNodeCapability("Max descriptor set uniform buffers",a_limits.maxDescriptorSetUniformBuffers);
    displayNodeCapability("Max descriptor set uniform buffers dynamic",a_limits.maxDescriptorSetUniformBuffersDynamic);
    displayNodeCapability("Max descriptor set storage buffers",a_limits.maxDescriptorSetStorageBuffers);
    displayNodeCapability("Max descriptor set storage buffers dynamic",a_limits.maxDescriptorSetStorageBuffersDynamic);
    displayNodeCapability("Max descriptor set sampled images",a_limits.maxDescriptorSetSampledImages);
    displayNodeCapability("Max descriptor set storage images",a_limits.maxDescriptorSetStorageImages);
    displayNodeCapability("Max descriptor set input attachment",a_limits.maxDescriptorSetInputAttachments);
    displayNodeCapability("Max vertex input attributes",a_limits.maxVertexInputAttributes);
    displayNodeCapability("Max vertex input bindings",a_limits.maxVertexInputBindings);
    displayNodeCapability("Max vertex input attibute offset",a_limits.maxVertexInputAttributeOffset);
    displayNodeCapability("Max vertex input binding stride",a_limits.maxVertexInputBindingStride);
    displayNodeCapability("Max vertex output components",a_limits.maxVertexOutputComponents);
    displayNodeCapability("Max tesselation generation level",a_limits.maxTessellationGenerationLevel);
    displayNodeCapability("Max tesselation patch size",a_limits.maxTessellationPatchSize);
    displayNodeCapability("Max tesselation control per vertex input components",a_limits.maxTessellationControlPerVertexInputComponents);
    displayNodeCapability("Max tesselation control per vertex output components",a_limits.maxTessellationControlPerVertexOutputComponents);
    displayNodeCapability("Max tesselation control per patch output components",a_limits.maxTessellationControlPerPatchOutputComponents);
    displayNodeCapability("Max tesselation control total output components",a_limits.maxTessellationControlTotalOutputComponents);
    displayNodeCapability("Max tesselation evaluation input components",a_limits.maxTessellationEvaluationInputComponents);
    displayNodeCapability("Max tesselation evaluation output components",a_limits.maxTessellationEvaluationOutputComponents);
    displayNodeCapability("Max geometry shader invocations",a_limits.maxGeometryShaderInvocations);
    displayNodeCapability("Max geometry input components",a_limits.maxGeometryInputComponents);
    displayNodeCapability("Max geometry output components",a_limits.maxGeometryOutputComponents);
    displayNodeCapability("Max geometry output vertices",a_limits.maxGeometryOutputVertices);
    displayNodeCapability("Max geometry total output components",a_limits.maxGeometryTotalOutputComponents);
    displayNodeCapability("Max fragment input components",a_limits.maxFragmentInputComponents);
    displayNodeCapability("Max fragment output attachments",a_limits.maxFragmentOutputAttachments);
    displayNodeCapability("Max fragment dual src attachments",a_limits.maxFragmentDualSrcAttachments);
    displayNodeCapability("Max fragment combined output resources",a_limits.maxFragmentCombinedOutputResources);
    displayNodeCapability("Max compute shared memory size", VE_CapabilitiesVisitor::memorySize(a_limits.maxComputeSharedMemorySize).c_str());
    displayNodeCapability("Max compute workgroup[0]",a_limits.maxComputeWorkGroupCount[0]);
    displayNodeCapability("Max compute workgroup[1]", a_limits.maxComputeWorkGroupCount[1]);
    displayNodeCapability("Max compute workgroup[2]", a_limits.maxComputeWorkGroupCount[2]);
    displayNodeCapability("Max compute workgroup invocation",a_limits.maxComputeWorkGroupInvocations);
    displayNodeCapability("Max compute workgroup size [0]",a_limits.maxComputeWorkGroupSize[0]);
    displayNodeCapability("Max compute workgroup size [1]", a_limits.maxComputeWorkGroupSize[1]);
    displayNodeCapability("Max compute workgroup size [2]", a_limits.maxComputeWorkGroupSize[2]);
    displayNodeCapability("Sub pixel precision bits",a_limits.subPixelPrecisionBits);
    displayNodeCapability("Sub texel precision bits",a_limits.subTexelPrecisionBits);
    displayNodeCapability("Mipmap precision bits",a_limits.mipmapPrecisionBits);
    displayNodeCapability("Max draw indexed value",a_limits.maxDrawIndexedIndexValue);
    displayNodeCapability("Max draw indirect count",a_limits.maxDrawIndirectCount);
    displayNodeCapability("Max sampler lod bias",a_limits.maxSamplerLodBias);
    displayNodeCapability("Max sampler anisotropy",a_limits.maxSamplerAnisotropy);
    displayNodeCapability("Max viewports",a_limits.maxViewports);
    displayNodeCapability("Max viewport dimension [0]", a_limits.maxViewportDimensions[0]);
    displayNodeCapability("Max viewport dimension [1]", a_limits.maxViewportDimensions[1]);
    displayNodeCapability("viewport bounds range [0]",a_limits.viewportBoundsRange[0]);
    displayNodeCapability("viewport bounds range [1]", a_limits.viewportBoundsRange[1]);
    displayNodeCapability("viewport subpixel bits",a_limits.viewportSubPixelBits);
    displayNodeCapability("Min memory map alignment", VE_CapabilitiesVisitor::memorySize(a_limits.minMemoryMapAlignment).c_str());
    displayNodeCapability("Min texel buffer offset alignment",VE_CapabilitiesVisitor::memorySize(a_limits.minTexelBufferOffsetAlignment).c_str());
    displayNodeCapability("Min uniform buffers offset alignment",VE_CapabilitiesVisitor::memorySize(a_limits.minUniformBufferOffsetAlignment).c_str());
    displayNodeCapability("Min storage buffer alignment",VE_CapabilitiesVisitor::memorySize(a_limits.minStorageBufferOffsetAlignment).c_str());
    displayNodeCapability("Min texel offset",a_limits.minTexelOffset);
    displayNodeCapability("Max texel offset",a_limits.maxTexelOffset);
    displayNodeCapability("Min texel gather offset",a_limits.minTexelGatherOffset);
    displayNodeCapability("Max texel gather offset",a_limits.maxTexelGatherOffset);
    displayNodeCapability("Min interpolation offset",a_limits.minInterpolationOffset);
    displayNodeCapability("Max interpolation offset",a_limits.maxInterpolationOffset);
    displayNodeCapability("Sub pixel interpolation offset bits",a_limits.subPixelInterpolationOffsetBits);
    displayNodeCapability("Max frame buffer width",a_limits.maxFramebufferWidth);
    displayNodeCapability("Max frame buffer height",a_limits.maxFramebufferHeight);
    displayNodeCapability("Max frame buffer layers",a_limits.maxFramebufferLayers);
    displayNodeCapability("Frame buffer color sample count",Flag<VkSampleCountFlagBits>::to_string(a_limits.framebufferColorSampleCounts).c_str());
    displayNodeCapability("Frame buffer depth sample count",Flag<VkSampleCountFlagBits>::to_string(a_limits.framebufferDepthSampleCounts).c_str());
    displayNodeCapability("Frame buffer stencil sample count",Flag<VkSampleCountFlagBits>::to_string(a_limits.framebufferStencilSampleCounts).c_str());
    displayNodeCapability("Frame buffer ne attachment sample count",Flag<VkSampleCountFlagBits>::to_string(a_limits.framebufferNoAttachmentsSampleCounts).c_str());
    displayNodeCapability("Max color attachments",a_limits.maxColorAttachments);
    displayNodeCapability("Sampled image color sample count",Flag<VkSampleCountFlagBits>::to_string(a_limits.sampledImageColorSampleCounts).c_str());
    displayNodeCapability("Sampled image integer sample count",Flag<VkSampleCountFlagBits>::to_string(a_limits.sampledImageIntegerSampleCounts).c_str());
    displayNodeCapability("Sampled image depth sample count",Flag<VkSampleCountFlagBits>::to_string(a_limits.sampledImageDepthSampleCounts).c_str());
    displayNodeCapability("Sampled image stencil sample count",Flag<VkSampleCountFlagBits>::to_string(a_limits.sampledImageStencilSampleCounts).c_str());
    displayNodeCapability("Storage image sample counts",Flag<VkSampleCountFlagBits>::to_string(a_limits.storageImageSampleCounts).c_str());
    displayNodeCapability("Max sample mask words",a_limits.maxSampleMaskWords);
    displayNodeCapability("Timestamp compute and graphics",static_cast<bool>(a_limits.timestampComputeAndGraphics));
    displayNodeCapability("Timestamp period",a_limits.timestampPeriod);
    displayNodeCapability("Max clip distance",a_limits.maxClipDistances);
    displayNodeCapability("Max cull distance",a_limits.maxCullDistances);
    displayNodeCapability("Max combined clip and cull distances",a_limits.maxCombinedClipAndCullDistances);
    displayNodeCapability("Discrete queue priorities",a_limits.discreteQueuePriorities);
    displayNodeCapability("Point size range[0]",a_limits.pointSizeRange[0]);
    displayNodeCapability("Point size range[1]", a_limits.pointSizeRange[1]);
    displayNodeCapability("Line width range[0]",a_limits.lineWidthRange[0]);
    displayNodeCapability("Line width range[1]", a_limits.lineWidthRange[1]);
    displayNodeCapability("Point size granularity",a_limits.pointSizeGranularity);
    displayNodeCapability("Line width granularity",a_limits.lineWidthGranularity);
    displayNodeCapability("Strict lines",static_cast<bool>(a_limits.strictLines));
    displayNodeCapability("Standard sample locations",static_cast<bool>(a_limits.standardSampleLocations));
    displayNodeCapability("Optimal buffer copy offset alignment",VE_CapabilitiesVisitor::memorySize(a_limits.optimalBufferCopyOffsetAlignment).c_str());
    displayNodeCapability("Optimal buffer copy row pitch aligment",VE_CapabilitiesVisitor::memorySize(a_limits.optimalBufferCopyRowPitchAlignment).c_str());
    displayNodeCapability("Non coherent atom size",VE_CapabilitiesVisitor::memorySize(a_limits.nonCoherentAtomSize).c_str());
	popNode();
}

void VE_CapabilitiesVisitor::visit(const VkPhysicalDeviceSparseProperties& a_sparse)
{
	displayNode("Sparse props");
    displayNodeCapability("Residency standard 2D block shape", static_cast<bool>(a_sparse.residencyStandard2DBlockShape));
    displayNodeCapability("Residency standard 2D multisample block shape", static_cast<bool>(a_sparse.residencyStandard2DMultisampleBlockShape));
    displayNodeCapability("Residency standard 3D block shape", static_cast<bool>(a_sparse.residencyStandard3DBlockShape));
    displayNodeCapability("Residency aligned mip size", static_cast<bool>(a_sparse.residencyAlignedMipSize));
    displayNodeCapability("Residency non resident strict", static_cast<bool>(a_sparse.residencyNonResidentStrict));
	popNode();
}