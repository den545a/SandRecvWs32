/// @ref gtx_orthonormalize
/// @file glm/gtx/orthonormalize.hpp
///
/// @see core (dependence)
/// @see gtx_extented_min_max (dependence)
///
/// @defgroup gtx_orthonormalize GLM_GTX_orthonormalize
/// @ingroup gtx
///
/// Include <glm/include/gtx/orthonormalize.hpp> to use the features of this extension.
///
/// Orthonormalize matrices.

#pragma once

// Dependency:
#include "glm/include/vec3.hpp"
#include "glm/include/mat3x3.hpp"
#include "glm/include/geometric.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_orthonormalize is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#elif GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_orthonormalize extension included")
#endif

namespace glm
{
	/// @addtogroup gtx_orthonormalize
	/// @{

	/// Returns the orthonormalized matrix of m.
	///
	/// @see gtx_orthonormalize
	template<typename T, qualifier Q>
	GLM_FUNC_DECL mat<3, 3, T, Q> orthonormalize(mat<3, 3, T, Q> const& m);

	/// Orthonormalizes x according y.
	///
	/// @see gtx_orthonormalize
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> orthonormalize(vec<3, T, Q> const& x, vec<3, T, Q> const& y);

	/// @}
}//namespace glm

#include "glm/include/gtx/orthonormalize.inl"
