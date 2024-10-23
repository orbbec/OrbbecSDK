#ifndef OB_EXPORT_H
#define OB_EXPORT_H

#ifdef OB_STATIC_DEFINE
#  define OB_EXPORT
#  define OB_NO_EXPORT
#else
#  ifndef OB_EXPORT
#    ifdef OrbbecSDK_EXPORTS
        /* We are building this library */
#      ifdef _WIN32
#        define OB_EXPORT __declspec(dllexport)
#      else
#        define OB_EXPORT __attribute__((visibility("default")))
#      endif
#    else
        /* We are using this library */
#      ifdef _WIN32
#        define OB_EXPORT __declspec(dllimport)
#      else
#        define OB_EXPORT __attribute__((visibility("default")))
#      endif
#    endif
#  endif

#  ifndef OB_NO_EXPORT
#    ifdef _WIN32
#      define OB_NO_EXPORT
#    else
#      define OB_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#  endif
#endif

#ifndef OB_DEPRECATED
#  ifdef _WIN32
#    define OB_DEPRECATED __declspec(deprecated)
#  else
#    define OB_DEPRECATED __attribute__((__deprecated__))
#  endif
#endif

#ifndef OB_DEPRECATED_EXPORT
#  define OB_DEPRECATED_EXPORT OB_EXPORT OB_DEPRECATED
#endif

#ifndef OB_DEPRECATED_NO_EXPORT
#  define OB_DEPRECATED_NO_EXPORT OB_NO_EXPORT OB_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef OB_NO_DEPRECATED
#    define OB_NO_DEPRECATED
#  endif
#endif

#endif /* OB_EXPORT_H */
