/*
  Copyright (C) 2003, 2004, 2005, 2008, 2011
  Rocky Bernstein <rocky@gnu.org>
  Copyright (C) 2001 Herbert Valerio Riedel <hvr@gnu.org>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifdef HAVE_CONFIG_H
# include "config.h"
# define __CDIO_CONFIG_H__ 1
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "cdio_assert.h"
#include <cdio/cdio.h>
#include <cdio/util.h>
#include "cdio_private.h"

static const char _rcsid[] = "$Id: cdio.c,v 1.14 2008/04/22 15:29:11 karl Exp $";


/*!
  Return the value associatied with key. NULL is returned if obj is NULL
  or "key" does not exist.
 */
const char *
cdio_get_arg (const CdIo *obj, const char key[])
{
  if (obj == NULL) return NULL;
  
  if (obj->op.get_arg) {
    return obj->op.get_arg (obj->env, key);
  } else {
    return NULL;
  }
}

CdIo_t *
cdio_new (generic_img_private_t *p_env, cdio_funcs_t *p_funcs)
{
  CdIo_t *p_new_cdio = calloc(1, sizeof (CdIo_t));

  if (NULL == p_new_cdio) return NULL;
  
  p_new_cdio->env = p_env;      /* This is the private "environment" that
                                   driver-dependent routines use. */
  p_new_cdio->op  = *p_funcs;
  p_env->cdio     = p_new_cdio; /* A way for the driver-dependent routines 
                                   to access the higher-level general cdio 
                                   object. */
  return p_new_cdio;
}

/*!
  Set the arg "key" with "value" in the source device.
*/
driver_return_code_t
cdio_set_arg (CdIo_t *p_cdio, const char key[], const char value[])
{
  if (!p_cdio) return DRIVER_OP_UNINIT;
  if (!p_cdio->op.set_arg) return DRIVER_OP_UNSUPPORTED;
  if (!key) return DRIVER_OP_ERROR;

  return p_cdio->op.set_arg (p_cdio->env, key, value);
}



/* 
 * Local variables:
 *  c-file-style: "gnu"
 *  tab-width: 8
 *  indent-tabs-mode: nil
 * End:
 */
