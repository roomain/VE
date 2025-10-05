#include "pch.h"
#include "VE_GraphicalDevice.h"
#include "rendering/VE_RenderingCtx.h"

VE_RenderingCtx::VE_RenderingCtx(VE_GraphicalDevicePtr a_device) : m_device{ a_device }
{
	m_swapChain = m_device->createNewSwapChain();
	// test
#pragma warning(push)
#pragma warning( disable : 4189 )// because unsed local variable
	m_swapChain->resize(256, 256);
	unsigned int numImages = m_swapChain->swapBuffersCount();
	VulkanBuffer buffer = allocateStagingBuffer(m_device->context(),
		50 * sizeof(int));
#pragma warning(pop)

}