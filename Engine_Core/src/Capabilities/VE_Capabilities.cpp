#include <ranges>
#include "pch.h"
#include "Capabilities/VE_Capabilities.h"
#include "Capabilities/VE_CapabilitiesVisitor.h"

void VE_Capabilities::visit(VE_CapabilitiesVisitor& a_visitor)const
{
	a_visitor.visit(*this);
}

VE_Capabilities::Extension_const_iterator VE_Capabilities::extensionBegin()const noexcept
{
	return m_extensions.cbegin();
}

VE_Capabilities::Extension_const_iterator VE_Capabilities::extensionEnd()const noexcept
{
	return m_extensions.cend();
}

VE_Capabilities::Layer_const_iterator VE_Capabilities::layerBegin()const noexcept
{
	return m_layers.cbegin();
}

VE_Capabilities::Layer_const_iterator VE_Capabilities::layerEnd()const noexcept
{
	return m_layers.cend();
}

bool VE_Capabilities::checkExtensions(const std::vector<std::string>& a_extensions)const noexcept
{
	for (const auto& ext : a_extensions)
	{
		if (std::ranges::find_if(m_extensions, [&ext](const auto& a_ext)
			{
				return ext.compare(a_ext.extensionName) == 0;
			}) == m_extensions.cend())
		{
			return false;
		}
	}
	return true;
}

bool VE_Capabilities::checkLayers(const std::vector<std::string>& a_layers)const noexcept
{
	for (const auto& layer : a_layers)
	{
		if (std::ranges::find_if(m_layers, [&layer](const auto& a_lay)
			{
				return layer.compare(a_lay.layerName) == 0;
			}) == m_layers.cend())
		{
			return false;
		}
	}
	return true;
}