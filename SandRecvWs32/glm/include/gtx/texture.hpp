/// @ref gtx_texture
/// @file glm/gtx/texture.hpp
///
/// @see core (dependence)
///
/// @defgroup gtx_texture GLM_GTX_texture
/// @ingroup gtx
///
/// Include <glm/include/gtx/texture.hpp> to use the features of this extension.
///
/// Wrapping mode of texture coordinates.

#pragma once

// Dependency:
#include "glm/include/glm.hpp"
#include "glm/include/gtc/integer.hpp"
#include "glm/include/gtx/component_wise.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_texture is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#elif GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_texture extension included")
#endif

namespace glm
{
	/// @addtogroup gtx_texture
	/// @{

	/// Compute the number of mipmaps levels necessary to create a mipmap complete texture
	///
	/// @param Extent Extent of the texture base level mipmap
	/// @tparam L Integer between 1 and 4 included that qualify the dimension of the vector
	/// @tparam T Floating-point or signed integer scalar types
	/// @tparam Q Value from qualifier enum
	template <length_t L, typename T, qualifier Q>
	T levels(vec<L, T, Q> const& Extent);

	/// @}
}// namespace glm

#include "glm/include/gtx/texture.inl"

