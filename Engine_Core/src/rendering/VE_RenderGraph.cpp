#include "pch.h"
#include "rendering/VE_RenderGraph.h"


VE_RenderGraph::VE_RenderGraph(const VE_DeviceContext& a_ctxt) : VulkanObject<VE_DeviceContext>(a_ctxt),
	m_renderGraph{.m_renderCtx  = a_ctxt}
{
	// todo
}

VE_RenderGraph::~VE_RenderGraph()
{
	// todo
}

bool VE_RenderGraph::registerComponent(const std::shared_ptr<VE_IComponent>& a_component)
{
	if (a_component->hasFlag<RenderingFlagBit::IS_RENDERING>())
	{
		auto pipeline = a_component->pipeline();
		if (!pipeline)
		{
			a_component->createPipeline(m_vkCtxt);
			pipeline = a_component->pipeline();
			// register
		}


		if (a_component->hasFlag<RenderingFlagBit::IS_EDITABLE>())
		{
			// in edit rendering command
		}
		else
		{
			// in static rendering command
		}
	}
	else
	{
		// remove from rendering commands
	}
	
	return false;
}
