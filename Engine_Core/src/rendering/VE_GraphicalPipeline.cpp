#include "pch.h"
#include "rendering/VE_GraphicalPipeline.h"
#include "rendering/VE_Shader.h"

VE_GraphicalPipeline::VE_GraphicalPipeline(const VE_DeviceContext& a_ctxt, const std::string_view& a_cacheFile) :
	VE_Pipeline(a_ctxt, a_cacheFile)
{ }


bool VE_GraphicalPipeline::setup(const VE_ShaderPtr& a_shader)
{
	// push constant ranges

	// pipeline layout
	const VkPipelineLayoutCreateInfo pipelineLayoutCI = Vulkan::Initializers::pipelineLayoutCreateInfo(a_shader->pipelineDescriptorSetLayouts());
	VK_CHECK_EXCEPT(vkCreatePipelineLayout(m_vkCtxt.m_logicalDevice, &pipelineLayoutCI, nullptr, &m_pipelineLayout));

	const auto vertexInputCI = internalCreateVertexInput();
	const auto inputAssemblyCI = internalCreateInputAssembly();
	const auto tesselationCI = internalCreateTesselation();
	const auto viewportCI = internalCreateViewportState();
	const auto rasterCI = internalCreateRasterization();
	const auto multiSampleCI = internalCreateMultisampleState();
	const auto depthStencilCI = internalCreateDepthStencilState();
	const auto colorBlendCI = internalCreateColorBlendState();
	const auto dynamicStateCI = internalCreateDynamicState();

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