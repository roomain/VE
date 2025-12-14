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
#include "bitOperators.h"
#include "TPreviousCurrent.h"

class VE_IComponent;
using VE_IComponentPtr = std::shared_ptr<VE_IComponent>;
using VE_IComponentWPtr = std::weak_ptr<VE_IComponent>;

class VE_GraphicalPipeline;
struct VE_DeviceContext;

enum class RenderingFlagBit
{
	IS_DISABLED = 0,		/*!< indicates component is disabled*/
	IS_RENDERING = 1,		/*!< indicates component is rendered*/
	IS_EDITABLE = 1 << 1	/*!< indicates component is edited*/
};

/*@brief interface class for component (component of an actor)
* A component contains specific part used to render
* an object
*/
class VE_IComponent
{
	friend class VE_Actor;
	friend class VE_RenderGraphTask;
	friend class VE_RenderGraph;

protected:
	
	TPreviousCurrent<RenderingFlagBit> m_renderFlag = TPreviousCurrent(RenderingFlagBit::IS_DISABLED);	/*!< indicate how component is rendering*/
	

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
	[[nodiscard]] inline TPreviousCurrent<RenderingFlagBit> renderingFlags()const { return m_renderFlag; }
	void setRenderFlag(const RenderingFlagBit a_renderFlag) { m_renderFlag = a_renderFlag; }
	template<RenderingFlagBit Flag>
	[[nodiscard]] constexpr bool hasFlag()const { return (m_renderFlag.current() & Flag) == Flag; }
	DEFINE_ALL_ITER(std::vector<VE_IComponentPtr>, m_children);
};