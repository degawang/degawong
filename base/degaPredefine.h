#pragma once


/* image merge */
#define     DEGA_MERGE_HARDMIX          0X01
#define     DEGA_MERGE_OVERLAY          0X02
#define     DEGA_MERGE_PINLIGHT         0X03
#define     DEGA_MERGE_HARDLIGHT        0X04
#define     DEGA_MERGE_SOFTLIGHT        0X05
#define     DEGA_MERGE_LINERLIGHT       0X06
#define     DEGA_MERGE_VIVLDLIGHT       0X07

/* image filter */
#define     DEGA_FILTER_EDGE            0X01
#define     DEGA_FILTER_SHARP           0X02
#define     DEGA_FILTER_SPHERIZE        0X03

/* image process */
#define     DEGA_PROCESS_RESIZE         0X01

/* calculate base */
#define		DEGA_CONSTANT_PI	        3.14159265358979f

/* platform base */
#ifdef __GNUC__
#define     DEGA_FORCE_INLINE           __attribute__((__always_inline__)) inline
#elif defined(MSVC)
#define     DEGA_FORCE_INLINE           __forceinline
#else
#define     DEGA_FORCE_INLINE
#endif

/* cpp base */
#define		DEGA_ALLIGN_NUM		        16
#define 	DEGA_EXPORT_DLL 	        _declspec(dllexport)
#define 	DEGA_IMPORT_DLL 	        _declspec(dllimport)
#define		DEGA_NEED_ALLIGN		    true