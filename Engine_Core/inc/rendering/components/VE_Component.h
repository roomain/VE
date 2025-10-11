#pragma once
/***********************************************
* @headerfile VE_GraphicalDevice.h
* @date 07 / 10 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>
#include "iterators.h"

class VE_Component;
using VE_ComponentPtr = std::shared_ptr<VE_Component>;
using VE_ComponentWPtr = std::weak_ptr<VE_Component>;

/*@brief Base class for component (component of an actor)
* A component contains specific part used to render
* an object
*/
class VE_Component
{
	friend class VE_Actor;
protected:
	
	bool m_bEnabled = true;					/*!< indicate if write command and update are allowed*/
	bool m_bRenderEnable = true;			/*!< indicate if write command is enabled*/

	VE_ComponentWPtr m_parent;				/*!< parent component*/
	std::vector<VE_ComponentPtr> m_children;/*!< children components*/

	/*@brief write rendering commands*/
	virtual void writeCommands(VkCommandBuffer& a_cmdBuffer)const = 0;
	/*@brief update call each frame by parent component*/
	virtual void update(const float a_elapsed) = 0;

public:
	[[nodiscard]] inline VE_ComponentWPtr parent()const { return m_parent; }
	[[nodiscard]] constexpr size_t childCount()const { return m_children.size(); }
	[[nodiscard]] inline VE_ComponentPtr childAt(const int a_index)const { return m_children.at(a_index); }
	DEFINE_ALL_ITER(std::vector<VE_ComponentPtr>, m_children);
};