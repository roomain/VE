#include "pch.h"
#include "rendering/VE_UniformSampler.h"


VE_UniformSampler::VE_UniformSampler(const uint32_t a_bindingPoint, const VE_DeviceContext& a_ctxt) : VE_Uniform(a_bindingPoint), VulkanObject<VE_DeviceContext>{ a_ctxt }
{
    //
}

VE_UniformSampler::~VE_UniformSampler()
{
	cleanup();
}

void VE_UniformSampler::initialize()
{
	//todo
}

void VE_UniformSampler::cleanup()
{
	//todo
}

void VE_UniformSampler::updateShaderVariable()
{
	//todo
}