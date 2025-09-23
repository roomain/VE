#include "pch.h"
#include "rendering/VE_Shader.h"
#include "rendering/curves/VE_CurvePipeline.h"


bool VE_CurvePipeline::setup(const VE_ShaderPtr& a_shader, const PipelineContext& a_renderingCtxt)
{
	// pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutCI = Vulkan::Initializers::pipelineLayoutCreateInfo(a_shader->pipelineDescriptorSetLayouts());
	pipelineLayoutCI.pPushConstantRanges = a_renderingCtxt.m_pushConstant.data();
	pipelineLayoutCI.pushConstantRangeCount = static_cast<uint32_t>(a_renderingCtxt.m_pushConstant.size());
	VK_CHECK_EXCEPT(vkCreatePipelineLayout(m_vkCtxt.m_logicalDevice, &pipelineLayoutCI, nullptr, &m_pipelineLayout));

	const auto vertexInputCI = Vulkan::Initializers::pipelineVertexInputStateCreateInfo(VE_CurvePipeline::s_vertexInputDesc);
	const auto inputAssemblyCI = Vulkan::Initializers::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_LINE_STRIP, 0, VK_TRUE);

	const auto tesselationCI = Vulkan::Initializers::pipelineTessellationStateCreateInfo(a_renderingCtxt.m_ctrlPoints);
	const auto viewportCI = Vulkan::Initializers::pipelineViewportStateCreateInfo(a_renderingCtxt.m_viewportCount, a_renderingCtxt.m_scissorCount);
	const auto rasterCI = Vulkan::Initializers::pipelineRasterizationStateCreateInfo(RasterizationSettings{.polygonMode = VK_POLYGON_MODE_LINE	} );
	const auto multiSampleCI = Vulkan::Initializers::pipelineMultisampleStateCreateInfo();//todo
	const auto depthStencilCI = Vulkan::Initializers::pipelineDepthStencilStateCreateInfo(DepthStencilSettings{});
	const auto colorBlendCI = Vulkan::Initializers::pipelineColorBlendStateCreateInfo();// todo

	const auto dynamicStateCI = Vulkan::Initializers::pipelineDynamicStateCreateInfo(a_renderingCtxt.m_dynStates);

	// pipeline create info
	VkGraphicsPipelineCreateInfo pipelineCI = Vulkan::Initializers::createGraphicPipeline(
		0,
		a_shader->pipelineShaderStages(),	// VkPipelineShaderStageCreateInfo -> from shader
		&vertexInputCI,						// vertex input state
		&inputAssemblyCI,					// input assembly state
		&tesselationCI,						// tessellation state
		&viewportCI,						// viewport state
		&rasterCI,							// rasterization state
		&multiSampleCI,						// multisample state
		&depthStencilCI,					// depth stencil state
		&colorBlendCI,						// color blend state
		&dynamicStateCI,					// dynamic state
		m_pipelineLayout,
		VK_NULL_HANDLE, // render pass
		0); // subpass

	vkCreateGraphicsPipelines(m_vkCtxt.m_logicalDevice, m_cache, 1,
		&pipelineCI, nullptr, &m_pipeline);
	return isValid();
}