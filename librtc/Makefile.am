AM_CFLAGS        = ${regular_CFLAGS}
AM_CPPFLAGS      = ${regular_CPPFLAGS} -I${top_builddir}/include -I${top_srcdir}/include ${kinclude_CPPFLAGS}

pkgconfig_DATA	= librtc.pc librt4c.pc librt6c.pc

lib_LTLIBRARIES	  = librt4c.la librt6c.la librtc.la

librtc_la_SOURCES =
librtc_la_LIBADD  = librt4c.la librt6c.la
librtc_la_LDFLAGS = -version-info 0:0:0
librt4c_la_SOURCES = librt4c.c
librt4c_la_LDFLAGS = -version-info 1:0:1
librt6c_la_SOURCES = librt6c.c
librt6c_la_LDFLAGS = -version-info 1:0:1
