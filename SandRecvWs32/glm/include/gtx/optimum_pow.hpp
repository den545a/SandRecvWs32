/// @ref gtx_optimum_pow
/// @file glm/gtx/optimum_pow.hpp
///
/// @see core (dependence)
///
/// @defgroup gtx_optimum_pow GLM_GTX_optimum_pow
/// @ingroup gtx
///
/// Include <glm/include/gtx/optimum_pow.hpp> to use the features of this extension.
///
/// Integer exponentiation of power functions.

#pragma once

// Dependency:
#include "glm/include/glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_optimum_pow is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#elif GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_optimum_pow extension included")
#endif

namespace glm
{
	/// @addtogroup gtx_optimum_pow
	/// @{

	/// Returns x raised to the power of 2.
	///
	/// @see gtx_optimum_pow
	template<typename genType>
	GLM_FUNC_DECL genType pow2(genType const& x);

	/// Returns x raised to the power of 3.
	///
	/// @see gtx_optimum_pow
	template<typename genType>
	GLM_FUNC_DECL genType pow3(genType const& x);

	/// Returns x raised to the power of 4.
	///
	/// @see gtx_optimum_pow
	template<typename genType>
	GLM_FUNC_DECL genType pow4(genType const& x);

	/// @}
}//namespace glm

#include "glm/include/gtx/optimum_pow.inl"
