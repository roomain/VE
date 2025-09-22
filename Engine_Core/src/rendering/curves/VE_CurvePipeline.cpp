#include "pch.h"
#include "rendering/curves/VE_CurvePipeline.h"

VkPipelineVertexInputStateCreateInfo VE_CurvePipeline::internalCreateVertexInput() const
{
	return Vulkan::Initializers::pipelineVertexInputStateCreateInfo(VE_CurvePipeline::s_vertexInputDesc);
}

VkPipelineInputAssemblyStateCreateInfo VE_CurvePipeline::internalCreateInputAssembly()const
{
	return Vulkan::Initializers::pipelineInputAssemblyStateCreateInfo(
		VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
		0, VK_TRUE);
}

VkPipelineTessellationStateCreateInfo VE_CurvePipeline::internalCreateTesselation()const
{
	return Vulkan::Initializers::pipelineTessellationStateCreateInfo(0);
}

VkPipelineViewportStateCreateInfo VE_CurvePipeline::internalCreateViewportState()const
{
	return VkPipelineViewportStateCreateInfo{};
}

VkPipelineRasterizationStateCreateInfo VE_CurvePipeline::internalCreateRasterization()const
{
	return VkPipelineRasterizationStateCreateInfo{};
}

VkPipelineMultisampleStateCreateInfo VE_CurvePipeline::internalCreateMultisampleState()const
{
	return VkPipelineMultisampleStateCreateInfo{};
}

VkPipelineDepthStencilStateCreateInfo VE_CurvePipeline::internalCreateDepthStencilState()const
{
	return VkPipelineDepthStencilStateCreateInfo{};
}

VkPipelineColorBlendStateCreateInfo VE_CurvePipeline::internalCreateColorBlendState()const
{
	return VkPipelineColorBlendStateCreateInfo{};
}

VkPipelineDynamicStateCreateInfo VE_CurvePipeline::internalCreateDynamicState()const
{
	return VkPipelineDynamicStateCreateInfo{};
}
