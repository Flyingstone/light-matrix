/**
 * @file test_avx_bpacks.cpp
 *
 * Unit testing of AVX boolean packs
 * 
 * @author Dahua Lin 
 */


#include "simd_test_base.h"
#include <light_mat/simd/avx_bpacks.h>

using namespace lmat;
using namespace lmat::test;


static_assert(simd_bpack<float,  avx_t>::pack_width == 8, "Unexpected pack width");
static_assert(simd_bpack<double, avx_t>::pack_width == 4, "Unexpected pack width");

template<typename T> struct elemwise_construct;

template<> struct elemwise_construct<float>
{
	static simd_bpack<float, avx_t> get(const bool* s)
	{
		return simd_bpack<float, avx_t>(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]);
	}

	static void set(simd_bpack<float, avx_t>& pk, const bool *s)
	{
		pk.set(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]);
	}
};

template<> struct elemwise_construct<double>
{
	static simd_bpack<double, avx_t> get(const bool* s)
	{
		return simd_bpack<double, avx_t>(s[0], s[1], s[2], s[3]);
	}

	static void set(simd_bpack<double, avx_t>& pk, const bool *s)
	{
		pk.set(s[0], s[1], s[2], s[3]);
	}
};


T_CASE( avx_bpack_constructs )
{
	typedef simd_bpack<T, avx_t> bpack_t;
	typedef typename bpack_t::bint_type bint;
	const unsigned int width = bpack_t::pack_width;

	bpack_t pk0 = bpack_t::all_false();
	ASSERT_SIMD_EQ( pk0,  bint(0));

	bpack_t pk1 = bpack_t::all_true();
	ASSERT_SIMD_EQ( pk1,  bint(-1));

	bpack_t pk2( false );
	ASSERT_SIMD_EQ( pk2, bint(0) );

	bpack_t pk3( true );
	ASSERT_SIMD_EQ( pk3, bint(-1) );

	bool s[width];
	for (unsigned i = 0; i < width; ++i) s[i] = (i % 2 == 0);

	bint r[width];
	for (unsigned i = 0; i < width; ++i) r[i] = -bint(s[i]);

	bpack_t pk4 = elemwise_construct<T>::get(s);
	ASSERT_SIMD_EQ( pk4, r );
}


T_CASE( avx_bpack_load_and_store )
{
	typedef simd_bpack<T, avx_t> bpack_t;
	typedef typename bpack_t::bint_type bint;
	const unsigned int width = bpack_t::pack_width;

	bool s[width];
	bint si[width];
	bool r[width];

	for (unsigned i = 0; i < width; ++i)
	{
		s[i] = (i % 2 == 0);
		si[i] = -bint(s[i]);
		r[i] = false;
	}

	bpack_t pk(s);
	ASSERT_SIMD_EQ( pk, si );

	pk.store(r);
	ASSERT_VEC_EQ( width, s, r);
}


T_CASE( avx_bpack_set )
{
	typedef simd_bpack<T, avx_t> bpack_t;
	typedef typename bpack_t::bint_type bint;
	const unsigned int width = bpack_t::pack_width;

	bpack_t pk;

	pk.set( true );
	ASSERT_SIMD_EQ( pk,  bint(-1));

	pk.set( false );
	ASSERT_SIMD_EQ( pk,  bint(0));

	bool s[width];
	for (unsigned i = 0; i < width; ++i) s[i] = (i % 2 == 0);

	bint r[width];
	for (unsigned i = 0; i < width; ++i) r[i] = -bint(s[i]);

	elemwise_construct<T>::set(pk, s);
	ASSERT_SIMD_EQ( pk, r );
}


T_CASE( avx_bpack_to_scalar )
{
	typedef simd_bpack<T, avx_t> bpack_t;
	typedef typename bpack_t::bint_type bint;
	const unsigned int width = bpack_t::pack_width;

	bpack_t pk;
	pk.set( true );
	ASSERT_EQ( pk.to_scalar(), true );

	pk.set( false );
	ASSERT_EQ( pk.to_scalar(), false );

	bool s[width];
	for (unsigned i = 0; i < width; ++i) s[i] = (i % 2 == 0);

	elemwise_construct<T>::set(pk, s);
	ASSERT_EQ( pk.to_scalar(), true );
}


TI_CASE( avx_bpack_extracts )
{
	typedef simd_bpack<T, avx_t> bpack_t;
	typedef typename bpack_t::bint_type bint;
	const unsigned int width = bpack_t::pack_width;

	bool s[width];
	for (unsigned i = 0; i < width; ++i) s[i] = (i % 2 == 0);

	bpack_t pk;
	elemwise_construct<T>::set(pk, s);
	ASSERT_EQ( pk.extract(pos_<I>()), s[I] );

	for (unsigned i = 0; i < width; ++i) s[i] = (i % 3 == 0);

	elemwise_construct<T>::set(pk, s);
	ASSERT_EQ( pk.extract(pos_<I>()), s[I] );
}


AUTO_TPACK( avx_bpack_basic )
{
	ADD_T_CASE_FP( avx_bpack_constructs )
	ADD_T_CASE_FP( avx_bpack_load_and_store )
	ADD_T_CASE_FP( avx_bpack_set )
}

AUTO_TPACK( avx_bpack_elems )
{
	ADD_T_CASE_FP( avx_bpack_to_scalar )

	ADD_TI_CASE( avx_bpack_extracts, float, 0 )
	ADD_TI_CASE( avx_bpack_extracts, float, 1 )
	ADD_TI_CASE( avx_bpack_extracts, float, 2 )
	ADD_TI_CASE( avx_bpack_extracts, float, 3 )
	ADD_TI_CASE( avx_bpack_extracts, float, 4 )
	ADD_TI_CASE( avx_bpack_extracts, float, 5 )
	ADD_TI_CASE( avx_bpack_extracts, float, 6 )
	ADD_TI_CASE( avx_bpack_extracts, float, 7 )

	ADD_TI_CASE( avx_bpack_extracts, double, 0 )
	ADD_TI_CASE( avx_bpack_extracts, double, 1 )
	ADD_TI_CASE( avx_bpack_extracts, double, 2 )
	ADD_TI_CASE( avx_bpack_extracts, double, 3 )
}




