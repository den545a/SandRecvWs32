/// @ref gtx_type_trait
/// @file glm/gtx/type_trait.hpp
///
/// @see core (dependence)
///
/// @defgroup gtx_type_trait GLM_GTX_type_trait
/// @ingroup gtx
///
/// Include <glm/include/gtx/type_trait.hpp> to use the features of this extension.
///
/// Defines traits for each type.

#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_type_trait is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#elif GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_type_trait extension included")
#endif

// Dependency:
#include "glm/include/detail/qualifier.hpp"
#include "glm/include/gtc/quaternion.hpp"
#include "glm/include/gtx/dual_quaternion.hpp"

namespace glm
{
	/// @addtogroup gtx_type_trait
	/// @{

	template<typename T>
	struct type
	{
		static bool const is_vec = false;
		static bool const is_mat = false;
		static bool const is_quat = false;
		static length_t const components = 0;
		static length_t const cols = 0;
		static length_t const rows = 0;
	};

	template<length_t L, typename T, qualifier Q>
	struct type<vec<L, T, Q> >
	{
		static bool const is_vec = true;
		static bool const is_mat = false;
		static bool const is_quat = false;
		static length_t const components = L;
	};

	template<length_t C, length_t R, typename T, qualifier Q>
	struct type<mat<C, R, T, Q> >
	{
		static bool const is_vec = false;
		static bool const is_mat = true;
		static bool const is_quat = false;
		static length_t const components = C;
		static length_t const cols = C;
		static length_t const rows = R;
	};

	template<typename T, qualifier Q>
	struct type<qua<T, Q> >
	{
		static bool const is_vec = false;
		static bool const is_mat = false;
		static bool const is_quat = true;
		static length_t const components = 4;
	};

	template<typename T, qualifier Q>
	struct type<tdualquat<T, Q> >
	{
		static bool const is_vec = false;
		static bool const is_mat = false;
		static bool const is_quat = true;
		static length_t const components = 8;
	};

	/// @}
}//namespace glm

#include "glm/include/gtx/type_trait.inl"
