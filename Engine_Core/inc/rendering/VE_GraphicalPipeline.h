#pragma once
/***********************************************
* @headerfile VE_GraphicalPipeline.h
* @date 17 / 09 / 2025
* @author Roomain
************************************************/
#include <vector>
#include "rendering/VE_Pipeline.h"


struct PipelineContext
{
	uint32_t m_ctrlPoints = 1;
	uint32_t m_viewportCount = 1;
	uint32_t m_scissorCount = 1;
	std::vector<VkPushConstantRange> m_pushConstant;
	std::vector<VkDynamicState> m_dynStates;
};

/*@brief base class of graphica pipeline*/
class VE_GraphicalPipeline : public VE_Pipeline
{
protected:
	explicit VE_GraphicalPipeline(const VE_DeviceContext& a_ctxt, const std::string_view& a_cacheFile);

public:
	~VE_GraphicalPipeline() override = default;
	virtual bool setup(const VE_ShaderPtr& a_shader, const PipelineContext& a_renderingCtxt) = 0;
};