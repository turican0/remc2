//############################################################################
//##                                                                        ##
//##  M3D.CPP: Wrapper for 3D abstraction providers                         ##
//##                                                                        ##
//##  16-bit protected-mode source compatible with MSC 7.0                  ##
//##  32-bit protected-mode source compatible with MSC 11.0/Watcom 10.6     ##
//##                                                                        ##
//##  Version 1.00 of 25-Aug-98: Initial                                    ##
//##                                                                        ##
//##  Author: John Miles                                                    ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#include <math.h>

#include "rib.h"
#include "mss.h"
#include "imssapi.h"

#define MSS_EPSILON 0.0001F

#define DEGS_TO_DIAMS(degs) ((degs) / 114.591559F)
#define DIAMS_TO_DEGS(degs) ((degs) * 114.591559F)

//
// Service object positions every 100 milliseconds
//

#define SERVICE_MSECS 100

typedef MSS_STRUCT ROOM_PARAMS
{
  F32 level;
  F32 time;
  F32 damping;
  F32 predelay;
} ROOM_PARAMS;


#if 1

//
// MSS 6 values
//

static ROOM_PARAMS rooms[EAX_ENVIRONMENT_COUNT]  =
{
  { /*EAX_ENVIRONMENT_GENERIC,*/          0.0F,    1.493F,  0.5F,    0.011F },
  { /*EAX_ENVIRONMENT_PADDEDCELL,*/       0.25F,   0.1F,    0.1F,    0.002F },
  { /*EAX_ENVIRONMENT_ROOM,*/             0.417F,  0.4F,    0.666F,  0.003F },
  { /*EAX_ENVIRONMENT_BATHROOM,*/         0.75F,   1.499F,  0.166F,  0.011F },
  { /*EAX_ENVIRONMENT_LIVINGROOM,*/       0.208F,  0.478F,  0.1F,    0.004F },
  { /*EAX_ENVIRONMENT_STONEROOM,*/        0.3F,    2.309F,  0.68F,   0.017F },
  { /*EAX_ENVIRONMENT_AUDITORIUM,*/       0.403F,  4.279F,  0.5F,    0.03F  },
  { /*EAX_ENVIRONMENT_CONCERTHALL,*/      0.5F,    3.961F,  0.5F,    0.029F },
  { /*EAX_ENVIRONMENT_CAVE,*/             0.5F,    2.886F,  1.304F,  0.022F },
  { /*EAX_ENVIRONMENT_ARENA,*/            0.361F,  7.284F,  0.332F,  0.03F  },
  { /*EAX_ENVIRONMENT_HANGAR,*/           0.5F,    10.0F,   0.3F,    0.03F  },
  { /*EAX_ENVIRONMENT_CARPETEDHALLWAY,*/  0.153F,  0.259F,  0.55F,   0.03F  },
  { /*EAX_ENVIRONMENT_HALLWAY,*/          0.361F,  1.493F,  0.50F,   0.011F },
  { /*EAX_ENVIRONMENT_STONECORRIDOR,*/    0.444F,  2.697F,  0.638F,  0.02F  },
  { /*EAX_ENVIRONMENT_ALLEY,*/            0.25F,   1.752F,  0.776F,  0.011F },
  { /*EAX_ENVIRONMENT_FOREST,*/           0.111F,  3.145F,  0.472F,  0.088F },
  { /*EAX_ENVIRONMENT_CITY,*/             0.111F,  2.767F,  0.224F,  0.011F },
  { /*EAX_ENVIRONMENT_MOUNTAINS,*/        0.2F,    0.8F,    0.2F,    0.300F },
  { /*EAX_ENVIRONMENT_QUARRY,*/           0.55F,   3.00F,   0.55F,   0.043F },
  { /*EAX_ENVIRONMENT_PLAIN,*/            0.12F,   0.8F,    0.18F,   0.100F },
  { /*EAX_ENVIRONMENT_PARKINGLOT,*/       0.208F,  1.652F,  1.5F,    0.012F },
  { /*EAX_ENVIRONMENT_SEWERPIPE,*/        0.652F,  2.886F,  0.25F,   0.021F },
  { /*EAX_ENVIRONMENT_UNDERWATER,*/       1.0F,    1.499F,  0.1F,    0.011F },
  { /*EAX_ENVIRONMENT_DRUGGED,*/          0.875F,  8.392F,  1.388F,  0.011F },
  { /*EAX_ENVIRONMENT_DIZZY,*/            0.139F,  10.00F,  1.5F,    0.011F },
  { /*EAX_ENVIRONMENT_PSYCHOTIC,*/        0.486F,  7.563F,  2.5F,    0.011F }
};

#endif

#if 0

//
// EAX 1 values, observed on Creative SB Audigy 2 ZS (WDM)
// Predelay is not supported by EAX 1, so all of these parameters are 0
//

static ROOM_PARAMS rooms[EAX_ENVIRONMENT_COUNT]  =
{
  { /*EAX_ENVIRONMENT_GENERIC,*/         0.500032F, 1.490000F, 0.502175F, 0.000000F },
  { /*EAX_ENVIRONMENT_PADDEDCELL,*/      0.250032F, 0.170000F, 0.402581F, 0.000000F },
  { /*EAX_ENVIRONMENT_ROOM,*/            0.417347F, 0.400000F, 0.663819F, 0.000000F },
  { /*EAX_ENVIRONMENT_BATHROOM,*/        0.653129F, 1.490000F, 0.162537F, 0.000000F },
  { /*EAX_ENVIRONMENT_LIVINGROOM,*/      0.208207F, 0.500000F, 0.096651F, 0.000000F },
  { /*EAX_ENVIRONMENT_STONEROOM,*/       0.500032F, 2.310000F, 0.887974F, 0.000000F },
  { /*EAX_ENVIRONMENT_AUDITORIUM,*/      0.403179F, 4.320000F, 0.503788F, 0.000000F },
  { /*EAX_ENVIRONMENT_CONCERTHALL,*/     0.500032F, 3.920000F, 0.503241F, 0.000000F },
  { /*EAX_ENVIRONMENT_CAVE,*/            0.500032F, 2.910000F, 1.300000F, 0.000000F },
  { /*EAX_ENVIRONMENT_ARENA,*/           0.361407F, 7.240000F, 0.328131F, 0.000000F },
  { /*EAX_ENVIRONMENT_HANGAR,*/          0.500032F, 10.05000F, 0.297354F, 0.000000F },
  { /*EAX_ENVIRONMENT_CARPETEDHALLWAY,*/ 0.153107F, 0.300000F, 0.547392F, 0.000000F },
  { /*EAX_ENVIRONMENT_HALLWAY,*/         0.361407F, 1.490000F, 0.503788F, 0.000000F },
  { /*EAX_ENVIRONMENT_STONECORRIDOR,*/   0.444117F, 2.700000F, 0.635224F, 0.000000F },
  { /*EAX_ENVIRONMENT_ALLEY,*/           0.250032F, 1.490000F, 0.500003F, 0.000000F },
  { /*EAX_ENVIRONMENT_FOREST,*/          0.111044F, 1.490000F, 0.502568F, 0.000000F },
  { /*EAX_ENVIRONMENT_CITY,*/            0.111044F, 1.490000F, 0.496454F, 0.000000F },
  { /*EAX_ENVIRONMENT_MOUNTAINS,*/       0.194087F, 1.490000F, 0.495717F, 0.000000F },
  { /*EAX_ENVIRONMENT_QUARRY,*/          1.000000F, 1.490000F, 0.502175F, 0.000000F },
  { /*EAX_ENVIRONMENT_PLAIN,*/           0.097050F, 1.490000F, 0.171231F, 0.000000F },
  { /*EAX_ENVIRONMENT_PARKINGLOT,*/      0.208207F, 1.650000F, 1.500000F, 0.000000F },
  { /*EAX_ENVIRONMENT_SEWERPIPE,*/       0.652377F, 2.810000F, 0.247328F, 0.000000F },
  { /*EAX_ENVIRONMENT_UNDERWATER,*/      1.000000F, 1.490000F, 0.807378F, 0.000000F },
  { /*EAX_ENVIRONMENT_DRUGGED,*/         0.875991F, 8.390000F, 1.390000F, 0.000000F },
  { /*EAX_ENVIRONMENT_DIZZY,*/           0.139154F, 17.23000F, 0.662518F, 0.000000F },
  { /*EAX_ENVIRONMENT_PSYCHOTIC,*/       0.486405F, 7.560000F, 0.806898F, 0.000000F }
};

#endif

#if 0

//
// EAX 2/3 values, observed on Creative SB Audigy 2 ZS (WDM)
//

static ROOM_PARAMS rooms[EAX_ENVIRONMENT_COUNT]  =
{
  { /*EAX_ENVIRONMENT_GENERIC,*/         0.316228F, 1.490000F, 0.830000F, 0.011000F  },
  { /*EAX_ENVIRONMENT_PADDEDCELL,*/      0.316228F, 0.170000F, 0.100000F, 0.002000F  },
  { /*EAX_ENVIRONMENT_ROOM,*/            0.316228F, 0.400000F, 0.830000F, 0.003000F  },
  { /*EAX_ENVIRONMENT_BATHROOM,*/        0.316228F, 1.490000F, 0.540000F, 0.011000F  },
  { /*EAX_ENVIRONMENT_LIVINGROOM,*/      0.316228F, 0.500000F, 0.100000F, 0.004000F  },
  { /*EAX_ENVIRONMENT_STONEROOM,*/       0.316228F, 2.310000F, 0.640000F, 0.017000F  },
  { /*EAX_ENVIRONMENT_AUDITORIUM,*/      0.316228F, 4.320000F, 0.590000F, 0.030000F  },
  { /*EAX_ENVIRONMENT_CONCERTHALL,*/     0.316228F, 3.920000F, 0.700000F, 0.029000F  },
  { /*EAX_ENVIRONMENT_CAVE,*/            0.316228F, 2.910000F, 1.300000F, 0.022000F  },
  { /*EAX_ENVIRONMENT_ARENA,*/           0.316228F, 7.240000F, 0.330000F, 0.030000F  },
  { /*EAX_ENVIRONMENT_HANGAR,*/          0.316228F, 10.05000F, 0.230000F, 0.030000F  },
  { /*EAX_ENVIRONMENT_CARPETEDHALLWAY,*/ 0.316228F, 0.300000F, 0.100000F, 0.030000F  },
  { /*EAX_ENVIRONMENT_HALLWAY,*/         0.316228F, 1.490000F, 0.590000F, 0.011000F  },
  { /*EAX_ENVIRONMENT_STONECORRIDOR,*/   0.316228F, 2.700000F, 0.790000F, 0.020000F  },
  { /*EAX_ENVIRONMENT_ALLEY,*/           0.316228F, 1.490000F, 0.860000F, 0.011000F  },
  { /*EAX_ENVIRONMENT_FOREST,*/          0.316228F, 1.490000F, 0.540000F, 0.088000F  },
  { /*EAX_ENVIRONMENT_CITY,*/            0.316228F, 1.490000F, 0.670000F, 0.011000F  },
  { /*EAX_ENVIRONMENT_MOUNTAINS,*/       0.316228F, 1.490000F, 0.210000F, 0.100000F  },
  { /*EAX_ENVIRONMENT_QUARRY,*/          0.316228F, 1.490000F, 0.830000F, 0.025000F  },
  { /*EAX_ENVIRONMENT_PLAIN,*/           0.316228F, 1.490000F, 0.500000F, 0.100000F  },
  { /*EAX_ENVIRONMENT_PARKINGLOT,*/      0.316228F, 1.650000F, 1.500000F, 0.012000F  },
  { /*EAX_ENVIRONMENT_SEWERPIPE,*/       0.316228F, 2.810000F, 0.140000F, 0.021000F  },
  { /*EAX_ENVIRONMENT_UNDERWATER,*/      0.316228F, 1.490000F, 0.100000F, 0.011000F  },
  { /*EAX_ENVIRONMENT_DRUGGED,*/         0.316228F, 8.390000F, 1.390000F, 0.030000F  },
  { /*EAX_ENVIRONMENT_DIZZY,*/           0.316228F, 17.23000F, 0.560000F, 0.030000F  },
  { /*EAX_ENVIRONMENT_PSYCHOTIC,*/       0.316228F, 7.560000F, 0.910000F, 0.030000F  }
};

#endif

//############################################################################
//##                                                                        ##
//## Vector math                                                            ##
//##                                                                        ##
//############################################################################

static __inline void RAD_vector_cross_product(MSSVECTOR3D *c,
                                              const MSSVECTOR3D *v1,
                                              const MSSVECTOR3D *v2)
{
  c->x = v1->z * v2->y - v1->y * v2->z;
  c->y = v1->x * v2->z - v1->z * v2->x;
  c->z = v1->y * v2->x - v1->x * v2->y;
}


static __inline void RAD_vector_subtract(MSSVECTOR3D *d,
                                         const MSSVECTOR3D *v1,
                                         const MSSVECTOR3D *v2)
{
  d->x = v1->x - v2->x;
  d->y = v1->y - v2->y;
  d->z = v1->z - v2->z;
}


static __inline void RAD_vector_normalize(MSSVECTOR3D *v)
{
  F32 len = AIL_fsqrt((v->x * v->x) +
                      (v->y * v->y) +
                      (v->z * v->z));
  if (len < 0.0001F)
  {
    v->x = 1.0F;
    v->y = 0.0F;
    v->z = 0.0F;
  }
  else
  {
    v->x /= len;
    v->y /= len;
    v->z /= len;
  }
}

static __inline void RAD_vector_normalize(F32*x,F32*y,F32*z)
{
  F32 len = AIL_fsqrt(((*x) * (*x)) +
                      ((*y) * (*y)) +
                      ((*z) * (*z)));
  if (len > 0.0001F)
  {
    *x /= len;
    *y /= len;
    *z /= len;
  }
}

static __inline F32 RAD_vector_dot_product(const MSSVECTOR3D *v1,
                                           const MSSVECTOR3D *v2)
{
  F32 dot;

  dot  = v1->x * v2->x;
  dot += v1->y * v2->y;
  dot += v1->z * v2->z;

  return( dot );
}

//############################################################################
//#                                                                          #
//# AIL_API_room_type                                                        #
//#                                                                          #
//############################################################################

S32      AILCALL AIL_API_room_type   (HDIGDRIVER dig)
{
   return dig->room_type; 
}

//############################################################################
//#                                                                          #
//# AIL_API_set_room_type                                                    #
//#                                                                          #
//############################################################################

void     AILCALL AIL_API_set_room_type   (HDIGDRIVER dig, //)
                                          S32        EAX_room_type)
{
   if ( !dig ) return;
   
   dig->room_type = EAX_room_type; 

#ifdef IS_XBOX

   AIL_API_set_digital_master_room_type(dig, EAX_room_type);

#else

#ifdef MSS_VFLT_SUPPORTED
   if (dig->voice_filter == NULL)
#endif
      {
      //
      // No output filter -- we're using software reverb (not available on Xbox 1)
      //
      // Output filters will catch the value written to dig->room_type above and
      // set the hardware, HDIGDRIVER, and shadowed reverb parameters accordingly
      //

      AIL_set_digital_master_reverb_levels(dig, 
                                           1.0F,                            // Dry
                                           rooms[ EAX_room_type ].level );  // Wet

      AIL_set_digital_master_reverb       (dig, 
                                           rooms[ EAX_room_type ].time, 
                                           rooms[ EAX_room_type ].predelay, 
                                           rooms[ EAX_room_type ].damping);
      }
#ifdef MSS_VFLT_SUPPORTED
   else
      {
      //
      // Master dry level won't be written by (most) output filters, so we'll set it to 1 here
      // for consistency with the software provider
      //

      dig->master_dry = 1.0F;

      //
      // Force output filter to change its room type immediately, so that its parameters
      // (e.g., effects volume) can be tweaked by the app without being overwritten during
      // the next timer tick
      //
      // Providers that don't support room reverberation should write -1 to dig->room_type here, 
      // in response to an attempt to set a room type
      //

      dig->voice_filter->force_update(dig->voice_filter->driver_state);
      }
#endif
#endif
}

//############################################################################
//#                                                                          #
//# AIL_set_3D_rolloff_factor                                                #
//#                                                                          #
//############################################################################

void     AILCALL AIL_API_set_3D_rolloff_factor (HDIGDRIVER dig, //)
                                                F32        factor )
{
   if ( !dig ) return;
   dig->D3D.rolloff_factor = factor;
}

//############################################################################
//#                                                                          #
//# AIL_API_3D_rolloff_factor                                                #
//#                                                                          #
//############################################################################

F32      AILCALL AIL_API_3D_rolloff_factor (HDIGDRIVER dig)
{
   if ( !dig ) return( 0.0f );
   return dig->D3D.rolloff_factor;
}

//############################################################################
//#                                                                          #
//# AIL_set_3D_doppler_factor                                                #
//#                                                                          #
//############################################################################

void     AILCALL AIL_API_set_3D_doppler_factor (HDIGDRIVER dig, //)
                                                F32        factor )
{
   if ( !dig ) return;
   dig->D3D.doppler_factor = factor;
}

//############################################################################
//#                                                                          #
//# AIL_API_3D_doppler_factor                                                #
//#                                                                          #
//############################################################################

F32      AILCALL AIL_API_3D_doppler_factor (HDIGDRIVER dig)
{
   if ( !dig ) return( 0.0f );
   return dig->D3D.doppler_factor;
}

//############################################################################
//#                                                                          #
//# AIL_set_3D_distance_factor                                               #
//#                                                                          #
//############################################################################

void     AILCALL AIL_API_set_3D_distance_factor (HDIGDRIVER dig, //)
                                                 F32        factor )
{
   if ( !dig ) return;
   dig->D3D.distance_factor = factor;
}

//############################################################################
//#                                                                          #
//# AIL_API_3D_distance_factor                                               #
//#                                                                          #
//############################################################################

F32      AILCALL AIL_API_3D_distance_factor (HDIGDRIVER dig)
{
   if ( !dig ) return( 0.0f );
   return dig->D3D.distance_factor;
}

//############################################################################
//#                                                                          #
//# AIL_API_set_listener_3D_position                                         #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_listener_3D_position         (HDIGDRIVER dig, //)
                                                             F32        X,
                                                             F32        Y,
                                                             F32        Z)
{
   if ( !dig ) return;
   dig->D3D.listen_position.x = X;
   dig->D3D.listen_position.y = Y;
   dig->D3D.listen_position.z = Z;
}

//############################################################################
//#                                                                          #
//# AIL_API_set_listener_3D_velocity                                         #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_listener_3D_velocity         (HDIGDRIVER dig, //)
                                                             F32        dX_per_ms,
                                                             F32        dY_per_ms,
                                                             F32        dZ_per_ms,
                                                             F32        magnitude)
{
   AIL_API_set_listener_3D_velocity_vector(dig,
                                           dX_per_ms * magnitude,
                                           dY_per_ms * magnitude, 
                                           dZ_per_ms * magnitude);
}

//############################################################################
//#                                                                          #
//# AIL_API_set_listener_3D_velocity_vector                                  #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_listener_3D_velocity_vector  (HDIGDRIVER dig, //)
                                                             F32        dX_per_ms,
                                                             F32        dY_per_ms,
                                                             F32        dZ_per_ms)
{
   if ( !dig ) return;
   dig->D3D.listen_velocity.x = dX_per_ms;
   dig->D3D.listen_velocity.y = dY_per_ms;
   dig->D3D.listen_velocity.z = dZ_per_ms;
}

//############################################################################
//#                                                                          #
//# AIL_API_set_listener_3D_orientation                                      #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_listener_3D_orientation      (HDIGDRIVER dig, //)
                                                             F32        X_face,
                                                             F32        Y_face,
                                                             F32        Z_face,
                                                             F32        X_up,
                                                             F32        Y_up,
                                                             F32        Z_up)

{
   if ( !dig ) return;
   RAD_vector_normalize(&X_face, &Y_face, &Z_face);
   RAD_vector_normalize(&X_up,   &Y_up,   &Z_up);

   dig->D3D.listen_face.x = X_face;
   dig->D3D.listen_face.y = Y_face;
   dig->D3D.listen_face.z = Z_face;

   dig->D3D.listen_up.x = X_up;
   dig->D3D.listen_up.y = Y_up;
   dig->D3D.listen_up.z = Z_up;

   RAD_vector_cross_product(&dig->D3D.listen_cross,   
                            &dig->D3D.listen_face, 
                            &dig->D3D.listen_up);

   RAD_vector_normalize(&dig->D3D.listen_cross);

   SS_set_speaker_configuration(&dig->D3D,
                                 NULL,
                                 0,
                                 dig->logical_channels_per_sample);
}

//############################################################################
//#                                                                          #
//# AIL_API_listener_3D_position                                             #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_listener_3D_position    (HDIGDRIVER  dig, //)
                                                    F32 FAR    *X,
                                                    F32 FAR    *Y,
                                                    F32 FAR    *Z)
{
   if ( !dig ) return;
   if (X != NULL) *X = dig->D3D.listen_position.x;
   if (Y != NULL) *Y = dig->D3D.listen_position.y;
   if (Z != NULL) *Z = dig->D3D.listen_position.z;
}

//############################################################################
//#                                                                          #
//# AIL_API_listener_3D_velocity                                             #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_listener_3D_velocity    (HDIGDRIVER  dig, //)
                                                    F32 FAR    *dX_per_ms,
                                                    F32 FAR    *dY_per_ms,
                                                    F32 FAR    *dZ_per_ms)
{
   if ( !dig ) return;
   if (dX_per_ms != NULL) *dX_per_ms = dig->D3D.listen_velocity.x;
   if (dY_per_ms != NULL) *dY_per_ms = dig->D3D.listen_velocity.y;
   if (dZ_per_ms != NULL) *dZ_per_ms = dig->D3D.listen_velocity.z;
}


//############################################################################
//#                                                                          #
//# AIL_API_listener_3D_orientation                                          #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_listener_3D_orientation (HDIGDRIVER  dig, //)
                                                    F32 FAR    *X_face,
                                                    F32 FAR    *Y_face,
                                                    F32 FAR    *Z_face,
                                                    F32 FAR    *X_up,
                                                    F32 FAR    *Y_up,
                                                    F32 FAR    *Z_up)
{
   if ( !dig ) return;
   if (X_face != NULL) *X_face = dig->D3D.listen_face.x;
   if (Y_face != NULL) *Y_face = dig->D3D.listen_face.y;
   if (Z_face != NULL) *Z_face = dig->D3D.listen_face.z;

   if (X_up != NULL) *X_up = dig->D3D.listen_up.x;
   if (Y_up != NULL) *Y_up = dig->D3D.listen_up.y;
   if (Z_up != NULL) *Z_up = dig->D3D.listen_up.z;
}

//############################################################################
//#                                                                          #
//# AIL_API_listener_3D_update_position                                      #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_update_listener_3D_position      (HDIGDRIVER dig, //)
                                                             F32        dt_milliseconds)
{
   F32 X,Y,Z;
   F32 dX_dt,dY_dt,dZ_dt;

   AIL_listener_3D_velocity(dig,&dX_dt,&dY_dt,&dZ_dt);

   if ((AIL_fabs(dX_dt) < MSS_EPSILON) &&
       (AIL_fabs(dY_dt) < MSS_EPSILON) &&
       (AIL_fabs(dZ_dt) < MSS_EPSILON))
      {
      return;
      }

   AIL_listener_3D_position(dig,&X,&Y,&Z);

   X += (dX_dt * dt_milliseconds);
   Y += (dY_dt * dt_milliseconds);
   Z += (dZ_dt * dt_milliseconds);

   AIL_set_listener_3D_position(dig,X,Y,Z);
}

//############################################################################
//#                                                                          #
//# AIL_API_set_sample_3D_position                                           #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_sample_3D_position  (HSAMPLE S, //)
                                                    F32     X,
                                                    F32     Y,
                                                    F32     Z)
{
   if ( !S ) return;
   
   //
   // Enable 3D processing of sample when user specifies its position
   // This state will stay enabled until the next AIL_init_sample() call
   //

   S->is_3D = TRUE;

   S->S3D.position.x = X;
   S->S3D.position.y = Y;
   S->S3D.position.z = Z;
}

//############################################################################
//#                                                                          #
//# AIL_API_set_sample_3D_velocity                                           #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_sample_3D_velocity         (HSAMPLE S, //)
                                                           F32     dX_per_ms,
                                                           F32     dY_per_ms,
                                                           F32     dZ_per_ms,
                                                           F32     magnitude)
{
   AIL_API_set_sample_3D_velocity_vector(S,
                                         dX_per_ms * magnitude,
                                         dY_per_ms * magnitude, 
                                         dZ_per_ms * magnitude);
}

//############################################################################
//#                                                                          #
//# AIL_API_set_sample_3D_velocity_vector                                    #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_sample_3D_velocity_vector  (HSAMPLE S, //)
                                                           F32     dX_per_ms,
                                                           F32     dY_per_ms,
                                                           F32     dZ_per_ms)
{
   if ( !S ) return;
   
   S->S3D.velocity.x = dX_per_ms;
   S->S3D.velocity.y = dY_per_ms;
   S->S3D.velocity.z = dZ_per_ms;
}

//############################################################################
//#                                                                          #
//# AIL_API_set_sample_3D_orientation                                        #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_sample_3D_orientation (HSAMPLE S, //)
                                                      F32     X_face,
                                                      F32     Y_face,
                                                      F32     Z_face,
                                                      F32     X_up,
                                                      F32     Y_up,
                                                      F32     Z_up)

{
   if ( !S ) return;
   
   RAD_vector_normalize(&X_face, &Y_face, &Z_face);
   RAD_vector_normalize(&X_up,   &Y_up,   &Z_up);

   S->S3D.face.x = X_face;
   S->S3D.face.y = Y_face;
   S->S3D.face.z = Z_face;

   S->S3D.up.x = X_up;
   S->S3D.up.y = Y_up;
   S->S3D.up.z = Z_up;
}

//############################################################################
//#                                                                          #
//# AIL_API_sample_3D_position                                               #
//#                                                                          #
//############################################################################

S32        AILCALL AIL_API_sample_3D_position      (HSAMPLE  S, //)
                                                    F32 FAR *X,
                                                    F32 FAR *Y,
                                                    F32 FAR *Z)
{
   if ( !S ) return(0);
   
   if (X != NULL) *X = S->S3D.position.x;
   if (Y != NULL) *Y = S->S3D.position.y;
   if (Z != NULL) *Z = S->S3D.position.z;

   return S->is_3D;
}

//############################################################################
//#                                                                          #
//# AIL_API_sample_3D_velocity                                               #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_sample_3D_velocity      (HSAMPLE  S, //)
                                                    F32 FAR *dX_per_ms,
                                                    F32 FAR *dY_per_ms,
                                                    F32 FAR *dZ_per_ms)
{
   if ( !S ) return;
   
   if (dX_per_ms != NULL) *dX_per_ms = S->S3D.velocity.x;
   if (dY_per_ms != NULL) *dY_per_ms = S->S3D.velocity.y;
   if (dZ_per_ms != NULL) *dZ_per_ms = S->S3D.velocity.z;
}

//############################################################################
//#                                                                          #
//# AIL_API_sample_3D_orientation                                            #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_sample_3D_orientation   (HSAMPLE  S, //)
                                                    F32 FAR *X_face,
                                                    F32 FAR *Y_face,
                                                    F32 FAR *Z_face,
                                                    F32 FAR *X_up,
                                                    F32 FAR *Y_up,
                                                    F32 FAR *Z_up)
{
   if ( !S ) return;
   
   if (X_face != NULL) *X_face = S->S3D.face.x;
   if (Y_face != NULL) *Y_face = S->S3D.face.y;
   if (Z_face != NULL) *Z_face = S->S3D.face.z;

   if (X_up != NULL) *X_up = S->S3D.up.x;
   if (Y_up != NULL) *Y_up = S->S3D.up.y;
   if (Z_up != NULL) *Z_up = S->S3D.up.z;
}

//############################################################################
//#                                                                          #
//# AIL_API_sample_3D_update_position                                        #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_update_sample_3D_position (HSAMPLE S, //)
                                                      F32     dt_milliseconds)
{
   F32 X,Y,Z;
   F32 dX_dt,dY_dt,dZ_dt;

   AIL_sample_3D_velocity(S,&dX_dt,&dY_dt,&dZ_dt);

   if ((AIL_fabs(dX_dt) < MSS_EPSILON) &&
       (AIL_fabs(dY_dt) < MSS_EPSILON) &&
       (AIL_fabs(dZ_dt) < MSS_EPSILON))
      {
      return;
      }

   AIL_sample_3D_position(S,&X,&Y,&Z);

   X += (dX_dt * dt_milliseconds);
   Y += (dY_dt * dt_milliseconds);
   Z += (dZ_dt * dt_milliseconds);

   AIL_set_sample_3D_position(S,X,Y,Z);
}

//############################################################################
//#                                                                          #
//# AIL_API_set_sample_3D_distances                                          #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_sample_3D_distances (HSAMPLE S, //)
                                                    F32     max_dist,
                                                    F32     min_dist,
                                                    S32     auto_3D_wet_atten)
{
   if ( !S ) return;
   
   if (min_dist > max_dist)
      {
      F32 tmp=min_dist;
      min_dist=max_dist;
      max_dist=tmp;
      }

   AIL_lock();

   S->S3D.max_dist      = max_dist;
   S->S3D.min_dist      = min_dist;
   S->S3D.auto_3D_atten = auto_3D_wet_atten;
   S->S3D.dist_changed  = TRUE;

   AIL_unlock();
}

//############################################################################
//#                                                                          #
//# AIL_API_sample_3D_distances                                              #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_sample_3D_distances     (HSAMPLE S, //)
                                                    F32 FAR * max_dist,
                                                    F32 FAR * min_dist,
                                                    S32 FAR * auto_3D_wet_atten)
{
   if ( !S ) return;
   
   if (max_dist != NULL)          *max_dist          = S->S3D.max_dist;
   if (min_dist != NULL)          *min_dist          = S->S3D.min_dist;
   if (auto_3D_wet_atten != NULL) *auto_3D_wet_atten = S->S3D.auto_3D_atten;
}

//############################################################################
//#                                                                          #
//# AIL_API_set_sample_3D_cone                                               #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_sample_3D_cone        (HSAMPLE S, //)
                                                      F32     inner_angle,
                                                      F32     outer_angle,
                                                      F32     outer_volume)
{
   if ( !S ) return;
   
   S->S3D.inner_angle  = DEGS_TO_DIAMS(inner_angle);
   S->S3D.outer_angle  = DEGS_TO_DIAMS(outer_angle);
   S->S3D.outer_volume = outer_volume;

   S->S3D.cone_enabled = (inner_angle < 360.0F) && (outer_volume < 1.0F);
}


//############################################################################
//#                                                                          #
//# AIL_API_sample_3D_cone                                                   #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_sample_3D_cone        (HSAMPLE  S,
                                                  F32 FAR *inner_angle,
                                                  F32 FAR *outer_angle,
                                                  F32 FAR *outer_volume)
{
   if ( !S ) return;
   
   if (inner_angle  != NULL) *inner_angle  = DIAMS_TO_DEGS(S->S3D.inner_angle);
   if (outer_angle  != NULL) *outer_angle  = DIAMS_TO_DEGS(S->S3D.outer_angle);
   if (outer_volume != NULL) *outer_volume = S->S3D.outer_volume;
}

//############################################################################
//#                                                                          #
//# AIL_API_set_sample_obstruction                                           #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_sample_obstruction (HSAMPLE S, //)
                                                   F32     obstruction)
{
   if ( !S ) return;
   
   S->obstruction = obstruction;
}

//############################################################################
//#                                                                          #
//# AIL_API_set_sample_exclusion                                             #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_sample_exclusion (HSAMPLE S, //)
                                                 F32     exclusion)
{
   if ( !S ) return;
   
   S->exclusion = exclusion;

#ifdef IS_XBOX
  // exclusion emulated with dry level
   AIL_set_sample_reverb_levels( S, 1.0F-exclusion, S->wet_level );
#endif
}

//############################################################################
//#                                                                          #
//# AIL_API_set_sample_occlusion                                             #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_set_sample_occlusion   (HSAMPLE S, //)
                                                   F32     occlusion)
{
   if ( !S ) return;
   
   S->occlusion = occlusion;

#ifdef MSS_VFLT_SUPPORTED
   if ((S->driver->voice_filter != NULL) && 
       (S->driver->voice_filter->provider_flags & FPROV_OCCLUSION))
      {
      return;
      }
#endif

#ifndef IS_XBOX
   AIL_set_sample_low_pass_cut_off(S, (1.0F - occlusion) + 0.01F);  
#endif
}

//############################################################################
//#                                                                          #
//# AIL_API_sample_obstruction                                               #
//#                                                                          #
//############################################################################

F32        AILCALL AIL_API_sample_obstruction (HSAMPLE S)
{
   if ( !S ) return( 0.0f );
   
   return S->obstruction;
}

//############################################################################
//#                                                                          #
//# AIL_API_sample_exclusion                                                 #
//#                                                                          #
//############################################################################

F32        AILCALL AIL_API_sample_exclusion (HSAMPLE S)
{
   if ( !S ) return( 0.0f );
   
   return S->exclusion;
}

//############################################################################
//#                                                                          #
//# AIL_API_sample_occlusion                                                 #
//#                                                                          #
//############################################################################

F32        AILCALL AIL_API_sample_occlusion   (HSAMPLE S)
{
   if ( !S ) return( 0.0f );
   
   return S->occlusion;
}
