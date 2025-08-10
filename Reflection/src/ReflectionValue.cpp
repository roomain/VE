#include "pch.h"
#include "ReflectionValue.h"

void ReflectionValue::affect(const bool a_value)
{
	m_variantData = a_value;
}

void ReflectionValue::affect(const int a_value)
{
	m_variantData = a_value;
}

void ReflectionValue::affect(const unsigned int a_value)
{
	m_variantData = a_value;
}

void ReflectionValue::affect(const double& a_value)
{
	m_variantData = a_value;
}

void ReflectionValue::affect(const std::string& a_value)
{
	m_variantData = a_value;
}

void ReflectionValue::affect(const ReflectionClassPtr& a_value)
{
	m_variantData = a_value;
}

void ReflectionValue::affect(std::vector<VariantValue>&& a_value)
{
	m_variantData = std::move(a_value);
}