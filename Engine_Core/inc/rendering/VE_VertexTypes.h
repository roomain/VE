#pragma once
/***********************************************
* @headerfile VE_VertexTypes.h
* @date 22 / 09 / 2025
* @author Roomain
************************************************/
#include "glm/glm.hpp"

struct VE_CurveVertex
{
    glm::vec3 m_vertex;
    glm::vec3 m_tangent;
};

static constexpr uint32_t CurveVertexSize = sizeof(VE_CurveVertex);


struct VE_MeshVertex
{
    glm::vec3 m_vertex;
    glm::vec3 m_normal;
};

static constexpr uint32_t MeshVertexSize = sizeof(VE_MeshVertex);

template<typename VertexType>
struct VE_Shape
{
    std::vector<VertexType> m_vertexBuffer; /*!< mesh vertex buffer data*/
    std::vector<int> m_indexBuffer;         /*!< mesh index buffer data*/
};