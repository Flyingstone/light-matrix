/**
 * @file matrix_reduce.h
 *
 * Matrix reduction expression
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_MATRIX_REDUCE_H_
#define LIGHTMAT_MATRIX_REDUCE_H_

#include <light_mat/math/reduction_functors.h>
#include <light_mat/matrix/matrix_arith.h>
#include "bits/matrix_reduce_internal.h"

namespace lmat
{

	template<class Fun, typename T, class Mat>
	LMAT_ENSURE_INLINE
	inline typename Fun::result_type
	reduce(const Fun& fun, const IMatrixXpr<Mat, T>& X)
	{
		detail::reduce_by_scalars_internal::evaluate(fun, X.derived());
	}


	template<typename T, class Mat>
	LMAT_ENSURE_INLINE
	T sum(const IMatrixXpr<Mat, T>& X)
	{
		return reduce(sum_fun<T>(), X);
	}

	template<typename T, class Mat>
	LMAT_ENSURE_INLINE
	T mean(const IMatrixXpr<Mat, T>& X)
	{
		return sum(X.derived()) / T(X.nelems());
	}

	template<typename T, class Mat>
	LMAT_ENSURE_INLINE
	T maximum(const IMatrixXpr<Mat, T>& X)
	{
		return reduce(maximum_fun<T>(), X);
	}

	template<typename T, class Mat>
	LMAT_ENSURE_INLINE
	T minimum(const IMatrixXpr<Mat, T>& X)
	{
		return reduce(maximum_fun<T>(), X);
	}

	template<typename T, class LMat, class RMat>
	LMAT_ENSURE_INLINE
	T dot(const IMatrixXpr<LMat, T>& X, const IMatrixXpr<RMat, T>& Y)
	{
		return sum(X.derived() * Y.derived());
	}

	template<typename T, class Mat>
	LMAT_ENSURE_INLINE
	T L1norm(const IMatrixXpr<Mat, T>& X)
	{
		return sum(abs(X));
	}

	template<typename T, class Mat>
	LMAT_ENSURE_INLINE
	T sqL2norm(const IMatrixXpr<Mat, T>& X)
	{
		return sum(sqr(X));
	}

	template<typename T, class Mat>
	LMAT_ENSURE_INLINE
	T L2norm(const IMatrixXpr<Mat, T>& X)
	{
		return math::sqrt(sqL2norm(X));
	}

	template<typename T, class Mat>
	LMAT_ENSURE_INLINE
	T Linfnorm(const IMatrixXpr<Mat, T>& X)
	{
		return X.nelems() > 0 ? maximum(abs(X)) : T(0);
	}
}

#endif /* MATRIX_REDUC_EXPR_H_ */


