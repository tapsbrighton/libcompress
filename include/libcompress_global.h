//
// Created by taps on 7/11/2021.
//

#ifndef LIBCOMPRESS_LIBCOMPRESS_GLOBAL_H
#define LIBCOMPRESS_LIBCOMPRESS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMPRESS_LIBRARY)
#  define COMPRESS_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define COMPRESS_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

#endif //LIBCOMPRESS_LIBCOMPRESS_GLOBAL_H
