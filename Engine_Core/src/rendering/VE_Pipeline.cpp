#include "pch.h"
#include "rendering/VE_Pipeline.h"
#include "utils/VulkanPipelineInitializers.h"
#include "rendering/VE_Shader.h"

VE_Pipeline::VE_Pipeline(const VE_DeviceContext& a_ctxt) : VulkanObject<VE_DeviceContext>{ a_ctxt }
{
	//
}

VE_Pipeline::~VE_Pipeline()
{
	cleanup();
}

void VE_Pipeline::cleanup()
{
	if (m_pipelineLayout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(m_vkCtxt.m_logicalDevice, m_pipelineLayout, nullptr);
		m_pipelineLayout = VK_NULL_HANDLE;
	}

	if(m_pipeline != VK_NULL_HANDLE )
	{
		vkDestroyPipeline(m_vkCtxt.m_logicalDevice, m_pipeline, nullptr);
		m_pipeline = VK_NULL_HANDLE;
	}
}

void VE_Pipeline::setup(VE_ShaderPtr a_shader)
{
	// push constant ranges

	// pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutCI = Vulkan::Initializers::pipelineLayoutCreateInfo(a_shader->m_descriptorSetLayouts);
	VK_CHECK_EXCEPT(vkCreatePipelineLayout(m_vkCtxt.m_logicalDevice, &pipelineLayoutCI, nullptr, &m_pipelineLayout));


	// VkPipelineShaderStageCreateInfo -> from shader
	
	// VkPipelineVertexInputStateCreateInfo
	
	// VkPipelineInputAssemblyStateCreateInfo
	
	// VkPipelineTessellationStateCreateInfo
	
	// VkPipelineViewportStateCreateInfo
	
	// VkPipelineRasterizationStateCreateInfo
	
	// VkPipelineMultisampleStateCreateInfo
	
	// VkPipelineDepthStencilStateCreateInfo
	
	// VkPipelineColorBlendStateCreateInfo
	
	// VkPipelineDynamicStateCreateInfo
	std::vector<VkDynamicState> dynamic;
	VkPipelineDynamicStateCreateInfo dynamicStateCI = Vulkan::Initializers::pipelineDynamicStateCreateInfo(dynamic);
	
	// pipeline create info
	VkGraphicsPipelineCreateInfo pipelineCI = Vulkan::Initializers::createGraphicPipeline(
		0,
		a_shader->m_shaderStageCreateInfo,
		nullptr, // vertex input state
		nullptr, // input assembly state
		nullptr, // tessellation state
		nullptr, // viewport state
		nullptr, // rasterization state
		nullptr, // multisample state
		nullptr, // depth stencil state
		nullptr, // color blend state
		nullptr, // dynamic state
		m_pipelineLayout,
		VK_NULL_HANDLE, // render pass
		0); // subpass

	// todo
}

void VE_Pipeline::loadFromCache(const VE_PipelineCache& a_cache)
{
	//
}
