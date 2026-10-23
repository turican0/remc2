#ifndef EMU_PLAT_UNUSED_H
#define EMU_PLAT_UNUSED_H
/* Harness stub. Upstream uses __attribute__((unused)); MSVC has no equivalent
   that works in this position, so the attribute is simply dropped. */
#ifndef UNUSED
#  if defined(__GNUC__) || defined(__clang__)
#    define UNUSED(arg) __attribute__((unused)) arg
#  else
#    define UNUSED(arg) arg
#  endif
#endif
#endif
