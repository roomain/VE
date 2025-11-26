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

bool VE_RenderGraphregisterComponent(const std::shared_ptr<VE_IComponent>& a_component)
{
	// todo
	return false;
}
