#pragma once
/***********************************************
* @headerfile VE_MeshPipeline.h
* @date 27 / 09 / 2025
* @author Roomain
************************************************/
#include "rendering/pipelines/VE_GraphicalPipeline.h"
#include "utils/VulkanDefines.h"
#include "rendering/VE_VertexTypes.h"

/*@brief base class for mesh pipeline*/
class VE_MeshPipeline : public VE_GraphicalPipeline
{
protected:
	static inline VertexInputBindingDescArray<1> s_vertexInputDesc = {
		VkVertexInputBindingDescription
		{
			.binding = 0,
			.stride = MeshVertexSize,
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
		}
	};

	explicit VE_MeshPipeline(const VE_DeviceContext& a_ctxt, const std::string_view& a_cacheFile);

public:
	~VE_MeshPipeline() override = default;
	virtual bool setup(const VE_ShaderPtr& a_shader, const PipelineContext& a_renderingCtxt) override;
};