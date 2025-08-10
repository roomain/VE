#pragma once
/***********************************************
* @headerfile VulkanPipelineInitializers.h
* @date 07 / 05 / 2025
* @author Roomain
************************************************/
#include "vulkan/vulkan.h"

/*@brief functions for structure initialisation*/
namespace Vulkan::Initializers
{
    template<typename DescriptorSetContainer>
	[[nodiscard]] constexpr VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo(
		const DescriptorSetContainer& a_pSetLayouts)
	{
		return VkPipelineLayoutCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.pNext = nullptr,
		.setLayoutCount = static_cast<uint32_t>(a_pSetLayouts.size()),
		.pSetLayouts = a_pSetLayouts.data()};
	}

	[[nodiscard]] constexpr VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo(
		const VkDescriptorSetLayout& a_pSetLayouts)
	{
		return VkPipelineLayoutCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.pNext = nullptr,
		.setLayoutCount = 1,
		.pSetLayouts = &a_pSetLayouts 
		};
	}

	[[nodiscard]] constexpr VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo(
		const VkShaderStageFlagBits a_stage, const VkShaderModule a_shaderModule)
	{
		return VkPipelineShaderStageCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.stage = a_stage,
		.module = a_shaderModule,
		.pName = "main"
		};
	}

	[[nodiscard]] constexpr VkVertexInputBindingDescription vertexInputBindingDescription(
		uint32_t a_binding,
		uint32_t a_stride,
		VkVertexInputRate a_inputRate)
	{
		return VkVertexInputBindingDescription{
		.binding = a_binding,
		.stride = a_stride,
		.inputRate = a_inputRate };
	}

	[[nodiscard]] constexpr VkVertexInputAttributeDescription vertexInputAttributeDescription(
		uint32_t a_binding,
		uint32_t a_location,
		VkFormat a_format,
		uint32_t a_offset)
	{
		return VkVertexInputAttributeDescription{
		.location = a_location,
		.binding = a_binding,
		.format = a_format,
		.offset = a_offset };
	}

	template<typename VertexInputBindingDescContainer, typename VertexInputAttribDescContainer>
	[[nodiscard]] constexpr VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo(
		const VertexInputBindingDescContainer& a_vertexBindingDescriptions,
		const VertexInputAttribDescContainer& a_vertexAttributeDescriptions
	)
	{
		return VkPipelineVertexInputStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.pNext = nullptr,
		.vertexBindingDescriptionCount = static_cast<uint32_t>(a_vertexBindingDescriptions.size()),
		.pVertexBindingDescriptions = a_vertexBindingDescriptions.data(),
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(a_vertexAttributeDescriptions.size()),
		.pVertexAttributeDescriptions = a_vertexAttributeDescriptions.data() };
	}
    [[nodiscard]] constexpr VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(
		VkPrimitiveTopology a_topology,
		VkPipelineInputAssemblyStateCreateFlags a_flags,
		VkBool32 a_primitiveRestartEnable)
	{
		return VkPipelineInputAssemblyStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags,
		.topology = a_topology,
		.primitiveRestartEnable = a_primitiveRestartEnable };
	}

	[[nodiscard]] constexpr VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(
		const RasterizationSettings& a_rasterSettings,
		VkPipelineRasterizationStateCreateFlags a_flags = 0)
	{
		return VkPipelineRasterizationStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags,
		.depthClampEnable = VK_FALSE,
		.polygonMode = a_rasterSettings.polygonMode,
		.cullMode = a_rasterSettings.cullFlags,
		.frontFace = a_rasterSettings.frontFace,
		.lineWidth = 1.0f };
	}

	[[nodiscard]] constexpr VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState(
		VkColorComponentFlags a_colorWriteMask,
		VkBool32 a_blendEnable)
	{
		return VkPipelineColorBlendAttachmentState{
		.blendEnable = a_blendEnable,
		.colorWriteMask = a_colorWriteMask };
	}


	[[nodiscard]] constexpr VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
		uint32_t a_attachmentCount,
		const VkPipelineColorBlendAttachmentState* a_pAttachments)
	{
		return VkPipelineColorBlendStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.pNext = nullptr,
		.attachmentCount = a_attachmentCount,
		.pAttachments = a_pAttachments };
	}

	template<typename PipelinColorBlendAttachContainer>
	[[nodiscard]] constexpr VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
		const PipelinColorBlendAttachContainer& a_pAttachments)
	{
		return VkPipelineColorBlendStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.pNext = nullptr,
		.attachmentCount = static_cast<uint32_t>(a_pAttachments.size()),
		.pAttachments = a_pAttachments.data() };
	}

	[[nodiscard]] constexpr VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
		const DepthStencilSettings& a_depthSettings)
	{
		return VkPipelineDepthStencilStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.pNext = nullptr,
		.depthTestEnable = a_depthSettings.depthTestEnable,
		.depthWriteEnable = a_depthSettings.depthWriteEnable,
		.depthCompareOp = a_depthSettings.depthCompareOp,
		.back{.compareOp = VK_COMPARE_OP_ALWAYS} };
	}

	[[nodiscard]] constexpr VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(
		uint32_t a_viewportCount,
		uint32_t a_scissorCount,
		VkPipelineViewportStateCreateFlags a_flags = 0)
	{
		return VkPipelineViewportStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags,
		.viewportCount = a_viewportCount,
		.scissorCount = a_scissorCount };
	}

	[[nodiscard]] constexpr VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo(
		VkSampleCountFlagBits a_rasterizationSamples,
		VkPipelineMultisampleStateCreateFlags a_flags = 0)
	{
		return VkPipelineMultisampleStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags,
		.rasterizationSamples = a_rasterizationSamples };
	}

	[[nodiscard]] constexpr VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
		const VkDynamicState* a_pDynamicStates,
		uint32_t a_dynamicStateCount,
		VkPipelineDynamicStateCreateFlags a_flags = 0)
	{
		return VkPipelineDynamicStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags,
		.dynamicStateCount = a_dynamicStateCount,
		.pDynamicStates = a_pDynamicStates };
	}

	template<typename DynamicStateContainer>
	[[nodiscard]] constexpr VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
		const DynamicStateContainer& a_pDynamicStates,
		VkPipelineDynamicStateCreateFlags a_flags = 0)
	{
		return VkPipelineDynamicStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = a_flags,
		.dynamicStateCount = static_cast<uint32_t>(a_pDynamicStates.size()),
		.pDynamicStates = a_pDynamicStates.data() };
	}

	[[nodiscard]] constexpr VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo(uint32_t a_patchControlPoints)
	{
		return VkPipelineTessellationStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
		.pNext = nullptr,
		.patchControlPoints = a_patchControlPoints };
	}

	[[nodiscard]] constexpr VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(
		VkPolygonMode a_polygonMode,
		VkCullModeFlags a_cullMode,
		VkFrontFace a_frontFace,
		VkPipelineRasterizationStateCreateFlags a_flags = 0,
		VkBool32 a_depthClamp = VK_FALSE)
	{
		return VkPipelineRasterizationStateCreateInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = a_flags,
			.depthClampEnable = a_depthClamp,
			.polygonMode = a_polygonMode,
			.cullMode = a_cullMode,
			.frontFace = a_frontFace,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0,
			.depthBiasClamp = 0,
			.depthBiasSlopeFactor = 0,
			.lineWidth = 1.0f };
	}


	[[nodiscard]] constexpr VkGraphicsPipelineCreateInfo createGraphicPipeline(
		const VkPipelineCreateFlags a_flags,
		const std::vector<VkPipelineShaderStageCreateInfo>& a_shaderStages,
		const VkPipelineVertexInputStateCreateInfo* a_pVertexInputState,
		const VkPipelineInputAssemblyStateCreateInfo* a_pInputAssemblyState,
		const VkPipelineTessellationStateCreateInfo* a_pTessellationState,
		const VkPipelineViewportStateCreateInfo* a_pViewportState,
		const VkPipelineRasterizationStateCreateInfo* a_pRasterizationState,
		const VkPipelineMultisampleStateCreateInfo* a_pMultisampleState,
		const VkPipelineDepthStencilStateCreateInfo* a_pDepthStencilState,
		const VkPipelineColorBlendStateCreateInfo* a_pColorBlendState,
		const VkPipelineDynamicStateCreateInfo* a_pDynamicState,
		VkPipelineLayout a_layout,
		VkRenderPass     a_renderPass,
		uint32_t         a_subpass,
		VkPipeline       a_basePipelineHandle = VK_NULL_HANDLE,
		int32_t          a_basePipelineIndex = -1)
	{
		return VkGraphicsPipelineCreateInfo{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = nullptr,
			.flags = a_flags,
			.stageCount = static_cast<uint32_t>(a_shaderStages.size()),
			.pStages = a_shaderStages.data(),
			.pVertexInputState = a_pVertexInputState,
			.pInputAssemblyState = a_pInputAssemblyState,
			.pTessellationState = a_pTessellationState,
			.pViewportState = a_pViewportState,
			.pRasterizationState = a_pRasterizationState,
			.pMultisampleState = a_pMultisampleState,
			.pDepthStencilState = a_pDepthStencilState,
			.pColorBlendState = a_pColorBlendState,
			.pDynamicState = a_pDynamicState,
			.layout = a_layout,
			.renderPass = a_renderPass,
			.subpass = a_subpass,
			.basePipelineHandle = a_basePipelineHandle,
			.basePipelineIndex = a_basePipelineIndex

		};
	}

	[[nodiscard]] constexpr VkComputePipelineCreateInfo computePipelineCreateInfo(
		VkPipelineLayout layout,
		VkPipelineCreateFlags flags = 0)
	{
		return VkComputePipelineCreateInfo{
		.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
		.pNext = nullptr,
		.flags = flags,
		.layout = layout };
	}
    

	template<typename AttachmentDescContainer, typename SubpassDependContainer>
	[[nodiscard]] constexpr VkRenderPassCreateInfo createRenderPass(
		const VkRenderPassCreateFlags a_flag,
		const AttachmentDescContainer& a_attachment,
		const VkSubpassDescription& a_subpass,
		const SubpassDependContainer& a_subpassesDepends)
	{
		return VkRenderPassCreateInfo{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.pNext = nullptr,
			.flags = a_flag,
			.attachmentCount = static_cast<uint32_t>(a_attachment.size()),
			.pAttachments = a_attachment.data(),
			.subpassCount = 1,
			.pSubpasses = &a_subpass,
			.dependencyCount = static_cast<uint32_t>(a_subpassesDepends.size()),
			.pDependencies = a_subpassesDepends.data(),
		};
	}

	template<typename ColorAttach>
	[[nodiscard]] constexpr VkSubpassDescription createGraphicSubpassDesc(const ColorAttach& a_colorAttach, const VkAttachmentReference* a_depthReference)
	{
		return VkSubpassDescription{
			.flags = 0,
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.inputAttachmentCount = 0,
			.pInputAttachments = nullptr,
			.colorAttachmentCount = static_cast<uint32_t>(a_colorAttach.size()),
			.pColorAttachments = a_colorAttach.data(),
			.pResolveAttachments = nullptr,
			.pDepthStencilAttachment = a_depthReference,
			.preserveAttachmentCount = 0,
			.pPreserveAttachments = nullptr
		};
	}

}