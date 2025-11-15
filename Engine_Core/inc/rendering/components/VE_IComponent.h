#pragma once
/***********************************************
* @headerfile VE_IComponent.h
* @date 07 / 10 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>
#include "iterators.h"

class VE_IComponent;
using VE_IComponentPtr = std::shared_ptr<VE_IComponent>;
using VE_IComponentWPtr = std::weak_ptr<VE_IComponent>;

class VE_GraphicalPipeline;
struct VE_DeviceContext;


/*@brief interface class for component (component of an actor)
* A component contains specific part used to render
* an object
*/
class VE_IComponent
{
	friend class VE_Actor;
	friend class VE_RenderGraphTask;
protected:
	
	bool m_bEnabled = true;						/*!< indicate if write command and update are allowed*/
	bool m_bRenderEnable = true;				/*!< indicate if write command is enabled*/

	VE_IComponentWPtr m_parent;					/*!< parent component*/
	std::vector<VE_IComponentPtr> m_children;	/*!< children components*/

	/*@brief get device context*/
	const virtual VE_DeviceContext& context()const = 0;

	/*@brief write rendering commands*/
	virtual void writeCommands(VkCommandBuffer& a_cmdBuffer) = 0;

	/*@brief update call each frame by parent component*/
	virtual void update(const float a_elapsed) = 0;

	/*@brief get pipeline of component class*/
	virtual std::shared_ptr<VE_GraphicalPipeline> pipeline()const = 0;
	/*@brief create the pipeline of component class*/
	virtual void createPipeline(const VE_DeviceContext& a_ctxt) = 0;

public:
	VE_IComponent() = default;
	[[nodiscard]] inline VE_IComponentWPtr parent()const { return m_parent; }
	[[nodiscard]] constexpr size_t childCount()const { return m_children.size(); }
	[[nodiscard]] inline VE_IComponentPtr childAt(const int a_index)const { return m_children.at(a_index); }
	[[nodiscard]] virtual constexpr bool isInvalid()const noexcept = 0;
	DEFINE_ALL_ITER(std::vector<VE_IComponentPtr>, m_children);
};