#pragma once

#ifdef __GNUC__
#define DEGA_FORCE_INLINE __attribute__((__always_inline__)) inline
#elif defined(MSVC)
#define DEGA_FORCE_INLINE __forceinline
#else
#define DEGA_FORCE_INLINE
#endif


#define		DEGA_ALLIGN_NUM		16
#define 	DEGA_EXPORT_DLL 	_declspec(dllexport)
#define 	DEGA_IMPORT_DLL 	_declspec(dllimport)

#define     DEGA_FILTER_EDGE    0X00
#define     DEGA_FILTER_SHARP   0X01
