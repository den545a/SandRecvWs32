/// @ref gtx_normal
/// @file glm/gtx/normal.hpp
///
/// @see core (dependence)
/// @see gtx_extented_min_max (dependence)
///
/// @defgroup gtx_normal GLM_GTX_normal
/// @ingroup gtx
///
/// Include <glm/include/gtx/normal.hpp> to use the features of this extension.
///
/// Compute the normal of a triangle.

#pragma once

// Dependency:
#include "glm/include/glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_normal is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#elif GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_normal extension included")
#endif

namespace glm
{
	/// @addtogroup gtx_normal
	/// @{

	/// Computes triangle normal from triangle points.
	///
	/// @see gtx_normal
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> triangleNormal(vec<3, T, Q> const& p1, vec<3, T, Q> const& p2, vec<3, T, Q> const& p3);

	/// @}
}//namespace glm

#include "glm/include/gtx/normal.inl"
