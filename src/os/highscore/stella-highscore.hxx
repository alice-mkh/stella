#pragma once

#include <highscore/libhighscore.h>

G_BEGIN_DECLS

#define STELLA_TYPE_CORE (stella_core_get_type())

G_DECLARE_FINAL_TYPE (StellaCore, stella_core, STELLA, CORE, HsCore)

G_MODULE_EXPORT GType hs_get_core_type (void);

G_END_DECLS
