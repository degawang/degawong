#pragma once


#define		DEGA_ALLIGN_NUM		16
#define 	DEGA_EXPORT_DLL 	_declspec(dllexport)
#define 	DEGA_IMPORT_DLL 	_declspec(dllimport)

#ifdef __GNUC__
#define DEGA_FORCE_INLINE __attribute__((__always_inline__)) inline
#else
#define DEGA_FORCE_INLINE __forceinline
#endif
