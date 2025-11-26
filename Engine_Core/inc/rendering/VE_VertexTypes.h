#pragma once
/***********************************************
* @headerfile VE_VertexTypes.h
* @date 22 / 09 / 2025
* @author Roomain
************************************************/
#include <vector>
#include "glm/glm.hpp"

struct VE_VertexTangent
{
    glm::vec3 m_vertex;
    glm::vec3 m_tangent;
};

static constexpr uint32_t VertexTangentSize = sizeof(VE_VertexTangent);


struct VE_VertexNormal
{
    glm::vec3 m_vertex;
    glm::vec3 m_normal;
};

static constexpr uint32_t VertexNormalSize = sizeof(VE_VertexNormal);

