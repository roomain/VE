#pragma once
/***********************************************
* @headerfile VE_Camera.h
* @date 08 / 01 / 2025
* @author Roomain
************************************************/
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

/*@brief rendering camera*/
class VE_Camera
{
public:
    enum class Projection
    {
        ORTHOGRAPHIC,
        PERSPECTIVE
	};

private:
	glm::dmat4 m_projection;	/*!< projection matrix*/
	glm::dmat4 m_transform;     /*!< transformation matrix*/
	Projection m_projectionType; /*!< projection type*/

public:

    inline glm::dmat4 matrix()const noexcept { return m_transform; }
    inline glm::dmat4 projection()const noexcept { return m_projection; }

#pragma region Projection
    /*@brief set orthographic projection*/
    void setOrthographic(const double& a_left, const double& a_right,
        const double& a_bottom, const double& a_top,
        const double& a_nearPlane, const double& a_farPlane)
    {
		m_projectionType = Projection::ORTHOGRAPHIC;
        m_projection = glm::ortho(a_left, a_right, a_bottom, a_top, a_nearPlane, a_farPlane);
	}

    /*@brief set perspective: fov is in radian*/
    void setPerspective(const double& a_fovY, const double& a_aspectRatio,
        const double& a_nearPlane, const double& a_farPlane)
    {
        m_projectionType = Projection::PERSPECTIVE;
        m_projection = glm::perspective(a_fovY, a_aspectRatio, a_nearPlane, a_farPlane);
	}

	[[nodiscard]] constexpr Projection projectionType()const noexcept { return m_projectionType; }

    [[nodiscard]] inline glm::dvec3 forward()const
    {
        return glm::normalize(glm::xyz(-m_transform[2]));
    }

    [[nodiscard]] inline glm::dvec3 up()const
    {
        return glm::normalize(glm::xyz(m_transform[1]));
    }

    [[nodiscard]] inline glm::dvec3 right()const
    {
        return glm::normalize(glm::xyz(m_transform[0]));
	}

    [[nodiscard]] glm::dmat4 projectionMatrix()const
    {
        return m_projection;
	}
    
#pragma endregion

    [[nodiscard]] inline glm::dmat4 modelViewMatrix()const
    {
        return glm::inverse(m_transform);
    }

#pragma region Positioning
    [[nodiscard]] inline glm::dmat4 transformMatrix()const
    {
        return m_transform;
	}

    // look at from current position
    inline void lookAt(const glm::dvec3& a_target, const glm::dvec3& a_upVector)
    {
        m_transform = glm::lookAt(position(), a_target, a_upVector);
    }

    inline void lookAt(const glm::dvec3& a_position, const glm::dvec3& a_target, const glm::dvec3& a_upVector)
    {
        m_transform = glm::lookAt(a_position, a_target, a_upVector);
    }

    inline void setPosition(const glm::dvec3& a_position)
    {
        m_transform[3] = glm::dvec4(a_position, m_transform[3][3]);
    }

    inline void setRotation(const glm::dvec3& a_axisAngles)
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        m_transform = glm::recompose(scale, glm::dquat(a_axisAngles), translation, skew, perspective);
    }

    inline void setScale(const glm::dvec3& a_scale)
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        m_transform = glm::recompose(a_scale, rotation, translation, skew, perspective);
    }

    inline void setQuaternion(const glm::dquat& a_quaternion)
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        m_transform = glm::recompose(scale, a_quaternion, translation, skew, perspective);
    }

    [[nodiscard]] inline glm::dvec3 position()const
    {
        return glm::xyz(m_transform[3]) * m_transform[3].w;
    }

    [[nodiscard]] inline glm::dvec3 rotation()const
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        return glm::eulerAngles(rotation);
    }

    [[nodiscard]] glm::dquat quaternion()const
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        return rotation;
    }

    inline void rotateAround(const double& a_angle, const glm::dvec3& a_axis, const glm::dvec3& a_target)
    {
        glm::dvec3 offset = a_target - position();
        m_transform = glm::translate(m_transform, offset);
        m_transform = glm::rotate(m_transform, a_angle, a_axis);
        m_transform = glm::translate(m_transform, -offset);
    }
#pragma endregion
};