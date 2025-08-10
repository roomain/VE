#pragma once
/***********************************************
* @headerfile CapabilitiesVisitor.h
* @date 31 / 07 / 2025
* @author Roomain
************************************************/
#include "Capabilities/VE_CapabilitiesVisitor.h"

class CapabilitiesVisitor : public VE_CapabilitiesVisitor
{
private:
	int m_indentationCount = 0;

protected:
	/*@brief create a tree node*/
	void displayNode(const std::string& a_name) override;
	/*@brief go to parent node*/
	void popNode() override;
	/*@brief add node data*/
	void displayNodeCapability(const std::string& a_capability, const VarCapability& a_value) override;

};