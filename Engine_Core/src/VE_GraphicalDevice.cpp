#include "pch.h"
#include "VE_GraphicalDevice.h"
#include "VE_SwapChain.h"

VE_GraphicalDevice::VE_GraphicalDevice(const VE_DeviceContext& a_context, const std::vector<int>& a_queueFamilies, VkSurfaceKHR a_surface, const int a_presentQueueIndex) :
	VE_Device{ a_context,  a_queueFamilies }, m_presentationQueueIndex{ a_presentQueueIndex }, m_surface{ a_surface }
{
}

VE_GraphicalDevice::~VE_GraphicalDevice()
{
	// todo
}

std::shared_ptr<VE_SwapChain> VE_GraphicalDevice::createNewSwapChain()
{
	// because ctor is private
	m_swapChain = std::shared_ptr<VE_SwapChain>(new VE_SwapChain(m_vkCtxt, m_surface));
	return m_swapChain;
}

std::shared_ptr<VE_SwapChain> VE_GraphicalDevice::swapChain()const
{
	return m_swapChain;
}
