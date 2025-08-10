#include "CapabilitiesVisitor.h"
#include <iostream>
#include <ranges>
#include <string>

struct VarCapability_toString_visitor
{
	std::string operator() (const char* a_data)const
	{
		return a_data;
	}

	std::string operator() (const int a_data)const
	{
		return std::to_string(a_data);
	}

	std::string operator() (const unsigned int a_data)const
	{
		return std::to_string(a_data);
	}

	std::string operator() (const float a_data)const
	{
		return std::to_string(a_data);
	}

	std::string operator() (const double& a_data)const
	{
		return std::to_string(a_data);
	}
};

void applyIndentation(const int a_count)
{
#pragma warning(push)
#pragma warning(disable: 4189) // conditional expression is constant
	for (int _ : std::views::iota(0, a_count))
		std::cout << "  ";
#pragma warning(pop)
}

void CapabilitiesVisitor::displayNode(const std::string& a_name)
{
	applyIndentation(m_indentationCount);
	m_indentationCount++;
	std::cout << "-->" << a_name << "\n";
}

void CapabilitiesVisitor::popNode()
{
	m_indentationCount--;
}

void CapabilitiesVisitor::displayNodeCapability(const std::string& a_capability, const VarCapability& a_value)
{
	static VarCapability_toString_visitor visitor;
	applyIndentation(m_indentationCount);
	std::cout << "+" << a_capability << ": " << std::visit(visitor,a_value) << "\n";
}