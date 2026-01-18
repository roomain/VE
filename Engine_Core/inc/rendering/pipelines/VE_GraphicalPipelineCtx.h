#pragma once
/***********************************************
* @headerfile VE_GraphicalPipelineCtx.h
* @date 24 / 10 / 2025
* @author Roomain
************************************************/
#include <vulkan/vulkan.hpp>
#include "Reflection_macros.h"
#include "rendering/pipelines/VE_GraphicalPipelineCtx.generated.h"

/*@brief context used to create a pipeline*/
REFLECT_CLASS(VE_GraphicalPipelineCtx)
struct VE_GraphicalPipelineCtx
{
	DESERIALIZE_DECLARE(VE_GraphicalPipelineCtx)
	REFLECT_MEMBER
	VkSampleCountFlagBits m_multisample = VK_SAMPLE_COUNT_1_BIT;
	REFLECT_MEMBER
	uint32_t m_ctrlPoints = 1;										/*!< tesselation control points*/
	REFLECT_MEMBER
	uint32_t m_viewportCount = 1;									/*!< viewport count*/
	REFLECT_MEMBER
	uint32_t m_scissorCount = 1;									/*!< scissor count*/
	REFLECT_MEMBER
	VkBool32 m_blendEnable = VK_FALSE;
	REFLECT_MEMBER
	VkLogicOp m_blendOperator = VK_LOGIC_OP_CLEAR;
	REFLECT_MEMBER
	std::vector<VkPushConstantRange> m_pushConstant;
	REFLECT_MEMBER
	std::vector<VkDynamicState> m_dynStates;
	REFLECT_MEMBER
	std::vector<VkPipelineColorBlendAttachmentState> m_colorAttachments;
};

DESERIALIZE_INLINE_IMPL(VE_GraphicalPipelineCtx)