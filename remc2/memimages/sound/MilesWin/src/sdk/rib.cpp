//############################################################################
//##                                                                        ##
//##  RIB.CPP: RAD Interface Broker API services                            ##
//##                                                                        ##
//##  16-bit protected-mode source compatible with MSC 7.0                  ##
//##  32-bit protected-mode source compatible with MSC 11.0/Watcom 10.6     ##
//##                                                                        ##
//##  Version 1.00 of 20-Apr-98: Initial                                    ##
//##          1.01 of 25-Jan-00: Moved to standalone DLL for Eden           ##
//##                             Don't enumerate NULL providers if no       ##
//##                             specific interface requested               ##
//##          1.02 of 19-Nov-00: Return RIB_NOT_FOUND if requested          ##
//##                             interface entirely unsupported             ##
//##          1.03 of 29-Jun-01: Library provider handles are ref-counted   ##
//##          2.00 of 10-Mar-06: Switched to properties                     ##
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

#if ((defined(__MWERKS__) && !defined(__INTEL__)) || defined(__MRC__) || defined(THINK_C) || defined(powerc) || defined(macintosh) || defined(__powerc) || defined(__APPLE__) || defined(__MACH__)) && !defined(HOLLYWOOD_REV) && !defined(REVOLUTION)
#if !defined(__MACHO__) && !defined(__MACH__)
#ifdef TARGET_API_MAC_CARBON
  #include <carbon.h>
#else
  #include <CodeFragments.h>
  #include <TextUtils.h>
#endif
#endif
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rib.h"

#ifdef IS_MACHO
  #include <mach-o/dyld.h>
  #include <dirent.h>

  #ifdef IS_X86
    #include "loadribdll.h"
  #endif
  
#endif

#ifdef IS_WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define STRICT
#include <windows.h>
#endif

#ifdef BUILD_STANDALONE_RIB

#define AIL_strcpy strcpy
#define AIL_strcmp strcmp
#define AIL_strcat strcat
#define AIL_strlen strlen
#define AIL_stricmp stricmp
#define AIL_sprintf sprintf
#define AIL_mem_alloc_lock malloc
#define AIL_mem_free_lock free
#define AIL_debug_printf debug_printf
#define AIL_set_error
#define AILCALL RIBCALL

C8 MSS_Directory[MAX_PATH] = ".\\";

#define MSS_STRUCT struct

#else

#include "mss.h"
#include "imssapi.h"

#endif

#ifdef IS_WIN16
#include <dos.h>
#endif

#ifdef IS_LINUX
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#endif

#define DIAG_REQUEST 0                    // 1 to print interface requests and results

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

C8 error_text[256];

#define INTERFACE_NAME_HASH_SIZE   9      // Use 9-bit hash table for entry names
#define INTERFACE_BLOCK_SIZE_INIT  400    // Use 400 entires statically (enough for minimal use: mp3 decoder, filters, and system)
#define INTERFACE_BLOCK_SIZE       256    // Allocate 256 new entries at a time
#define INTERFACE_SET_DIM (1 << INTERFACE_NAME_HASH_SIZE)

#define PROVIDER_BLOCK_SIZE_INIT   40     // Use 40 entries statically (enough for minimal use: mp3 decoder, filters, and system)
#define PROVIDER_BLOCK_SIZE        16     // Allocate 16 new entries at a time

MSS_STRUCT INTERFACE_DESCRIPTOR
{
   HPROVIDER          provider;
   C8                 interface_name[128];
   C8                 entry_name    [128];
   RIB_ENTRY_TYPE     type;
   RIB_DATA_SUBTYPE   subtype;
   UINTa              token;
};

//
// Interface list entry
//

MSS_STRUCT ILIST_ENTRY
{
   public:
   ILIST_ENTRY FAR *next;
   ILIST_ENTRY FAR *prev;
   ILIST_ENTRY FAR *next_in_set;
   ILIST_ENTRY FAR *prev_in_set;
   S32              set_key;
   S32              entry_num;

   //
   // User data
   //

   INTERFACE_DESCRIPTOR contents;
};


static void ILE_init(ILIST_ENTRY *ILE, INTERFACE_DESCRIPTOR FAR *src)
      {
#ifdef MSS_VERSION                              // Avoid extern library warning in PS2 version of MSS
      AIL_memcpy(&ILE->contents, src, sizeof(*src)); // WARNING: No operator= semantics!
#else
      ILE->contents = *src;
#endif
      }

static void ILE_cleanup(ILIST_ENTRY *ILE)
      {
      }

static S32 ILE_is_match(ILIST_ENTRY *ILE, INTERFACE_DESCRIPTOR FAR *src)
      {
      if (!src->interface_name[0])
         {
         return (src->type == ILE->contents.type) &&
                (src->provider == ILE->contents.provider) &&
                (!AIL_strcmp(src->entry_name, ILE->contents.entry_name));
         }

      return (src->type == ILE->contents.type) &&
             (src->provider == ILE->contents.provider) &&
             (!AIL_strcmp(src->interface_name, ILE->contents.interface_name)) &&
             (!AIL_strcmp(src->entry_name, ILE->contents.entry_name));
      }

   static U32 ILE_set(INTERFACE_DESCRIPTOR FAR *src)
      {
      //
      // Derive hash key from entry name
      //

#if defined(IS_WIN32API) && defined(IS_X86) && !defined(IS_WIN64)

     U32 sum;
      U8 FAR *entry_name = (U8 FAR *) src->entry_name;

     _asm
        {
        mov ebx,entry_name
        xor eax,eax
        xor edx,edx

        ALIGN 16
      chksum:
        mov dl,BYTE PTR [ebx]
        add eax,edx
        inc ebx
        test edx,edx
        jnz chksum

        mov sum,eax
        }
#else

     U32 sum;

     U8 FAR *ptr = (U8 FAR *) src->entry_name;
     sum     = 0;

     while (*ptr)
        {
       sum += *(ptr++);
        }

#endif

     return sum & (INTERFACE_SET_DIM - 1);
      }


//
// Interface entry list (hashed)
//

MSS_STRUCT InterfaceList
{
public:
   S32              current_size;
   ILIST_ENTRY FAR *used;
   ILIST_ENTRY FAR *avail;
   ILIST_ENTRY FAR *table[INTERFACE_SET_DIM];
   ILIST_ENTRY FAR *array;
   ILIST_ENTRY static_array[INTERFACE_BLOCK_SIZE_INIT];
};

static void IL_construct(InterfaceList FAR *IL)
      {
      IL->current_size = INTERFACE_BLOCK_SIZE_INIT;

      IL->array = IL->static_array;

      S32 i;
      for (i=0; i < IL->current_size; i++)
         {
         IL->array[i].prev = &IL->array[i-1];
         IL->array[i].next = &IL->array[i+1];
         IL->array[i].entry_num = -1;
         }

      IL->array[0].prev = NULL;
      IL->array[IL->current_size-1].next = NULL;

      IL->used = NULL;
      IL->avail = &IL->array[0];

      for (i=0; i < INTERFACE_SET_DIM; i++) IL->table[i] = NULL;
      }

static void IL_dealloc(InterfaceList FAR *IL, ILIST_ENTRY FAR *entry)
      {
      ILE_cleanup(entry);

      if (entry->next_in_set) entry->next_in_set->prev_in_set = entry->prev_in_set;
      if (entry->prev_in_set) entry->prev_in_set->next_in_set = entry->next_in_set;

      entry->entry_num = -1;

      if (entry->next) entry->next->prev = entry->prev;
      if (entry->prev) entry->prev->next = entry->next;

      if (IL->used == entry) IL->used = entry->next;

      if (IL->table[entry->set_key] == entry)
         IL->table[entry->set_key] = entry->next_in_set;

      entry->next = IL->avail;
      entry->prev = NULL;

      if (IL->avail) IL->avail->prev = entry;
      IL->avail = entry;
      }
/*
static void IL_dealloc(InterfaceList FAR *IL, S32 entry_num)
      {
      if ((entry_num != -1) && (IL->array[entry_num].entry_num != -1))
         IL_dealloc(IL, &IL->array[entry_num]);
      }
*/
static void IL_destroy(InterfaceList FAR *IL)
      {
      if (IL->array)
         {
       while (IL->used) IL_dealloc(IL, IL->used);

       IL->current_size = 0;
       if ( IL->array != IL->static_array )
         AIL_mem_free_lock(IL->array);
       IL->array = NULL;
         }
      }

static ILIST_ENTRY FAR *IL_verify_interface(InterfaceList FAR *IL, HPROVIDER provider, C8 const *name)
      {
      S32 i;

      for (i=0; i < IL->current_size; i++)
         {
         if (IL->array[i].entry_num != -1)
            {
            if (provider == IL->array[i].contents.provider)
               {
               if (!AIL_strcmp(name, IL->array[i].contents.interface_name))
                  {
                  return &IL->array[i];
                  }
               }
            }
         }

      return NULL;
      }

static ILIST_ENTRY FAR *IL_verify_provider(InterfaceList FAR *IL, HPROVIDER provider)
      {
      S32 i;

      for (i=0; i < IL->current_size; i++)
         {
         if (IL->array[i].entry_num != -1)
            {
            if (provider == IL->array[i].contents.provider)
               {
               return &IL->array[i];
               }
            }
         }

      return NULL;
      }

static ILIST_ENTRY FAR *IL_lookup(InterfaceList FAR *IL, INTERFACE_DESCRIPTOR FAR *seed)
      {
      ILIST_ENTRY FAR *found = IL->table[ILE_set(seed)];

      while (found)
         {
         if (ILE_is_match(found, seed))
            break;

         found = found->next_in_set;
         }

      return found;
      }

static ILIST_ENTRY FAR *IL_alloc(InterfaceList FAR *IL, INTERFACE_DESCRIPTOR FAR *seed)
      {
      U32 key = ILE_set(seed);
      ILIST_ENTRY FAR *entry;

      if (IL->avail == NULL)
         {
         S32 prev_size = IL->current_size;
         S32 new_size = prev_size + INTERFACE_BLOCK_SIZE;

         ILIST_ENTRY FAR *old_array = IL->array;
         ILIST_ENTRY FAR *new_array = (ILIST_ENTRY FAR *)
            AIL_mem_alloc_lock(sizeof(ILIST_ENTRY) * (size_t) new_size);
         if (new_array == NULL) return NULL;

         UINTa adjust = ((UINTa) new_array) - ((UINTa) old_array);

         S32 i;
         for (i=0; i < prev_size; i++)
            {
#ifdef MSS_VERSION                                                          // Avoid extern library warning in PS2 version of MSS
            AIL_memcpy(&new_array[i], &old_array[i], sizeof(old_array[i])); // WARNING: No operator= semantics!
#else
            new_array[i] = old_array[i];
#endif

            if (new_array[i].next_in_set) new_array[i].next_in_set = (ILIST_ENTRY FAR *) (((UINTa) new_array[i].next_in_set) + adjust);
            if (new_array[i].prev_in_set) new_array[i].prev_in_set = (ILIST_ENTRY FAR *) (((UINTa) new_array[i].prev_in_set) + adjust);

            if (new_array[i].next) new_array[i].next = (ILIST_ENTRY FAR *) (((UINTa) new_array[i].next) + adjust);
            if (new_array[i].prev) new_array[i].prev = (ILIST_ENTRY FAR *) (((UINTa) new_array[i].prev) + adjust);
            }

         for (i=0; i < INTERFACE_SET_DIM; i++)
            if (IL->table[i]) IL->table[i] = (ILIST_ENTRY FAR *) (((UINTa) IL->table[i]) + adjust);

         for (i=prev_size; i < new_size; i++)
            {
            new_array[i].entry_num = -1;
            new_array[i].prev = &new_array[i-1];
            new_array[i].next = &new_array[i+1];
            }

         new_array[prev_size].prev = NULL;
         new_array[new_size-1].next = NULL;
         IL->used = (ILIST_ENTRY FAR *) (((UINTa) IL->used) + adjust);
         IL->avail = &new_array[prev_size];
         IL->current_size = new_size;

         if ( IL->array != IL->static_array )
           AIL_mem_free_lock(IL->array);
         IL->array = new_array;
         }

      entry = IL->avail;
      IL->avail = entry->next;
      entry->next = IL->used;
      entry->prev = NULL;

      if (IL->used) IL->used->prev = entry;
      IL->used = entry;

      entry->set_key = key;
      entry->next_in_set = IL->table[key];
      entry->prev_in_set = NULL;

      if (IL->table[key])
         IL->table[key]->prev_in_set = entry;

      IL->table[key] = entry;                           // TODO: 64-bit nastiness below
      entry->entry_num = (S32) ((((UINTa) entry) - ((UINTa) IL->array)) / sizeof(ILIST_ENTRY));

      ILE_init(entry, seed);

      return entry;
      }



//
// Provider list entry
//

MSS_STRUCT PVLIST_ENTRY
{
   PVLIST_ENTRY FAR *next;
   PVLIST_ENTRY FAR *prev;
   S32               entry_num;

   //
   // User data
   //

   SINTa module;            // HMODULE if DLL, not used if static
   RIB_MAIN_FUNC addr;      // address of startup 
   char filename[512];      // Module name for which the handle was allocated
   S32 refcnt;              // # of times associated module name has been loaded
   SINTa user_data  [8];    // Miscellaneous user data
   SINTa system_data[8];    // Miscellaneous system data
};

//
// Provider list (non-hashed)
//

MSS_STRUCT ProviderList
{
public:
   S32 current_size;
   PVLIST_ENTRY FAR *used;
   PVLIST_ENTRY FAR *avail;
   PVLIST_ENTRY FAR *array;
   PVLIST_ENTRY static_array[PROVIDER_BLOCK_SIZE_INIT];
};

static void PL_construct(ProviderList FAR *PL)
      {
      PL->current_size = PROVIDER_BLOCK_SIZE_INIT;

      PL->array = PL->static_array;

      for (S32 i=0; i < PL->current_size; i++)
         {
         PL->array[i].prev = &PL->array[i-1];
         PL->array[i].next = &PL->array[i+1];
         PL->array[i].entry_num = -1;
         }

      PL->array[0].prev = NULL;
      PL->array[PL->current_size-1].next = NULL;

      PL->used = NULL;
      PL->avail = &PL->array[0];
      }

static void PL_dealloc(ProviderList FAR *PL, PVLIST_ENTRY FAR *entry)
      {
      entry->entry_num = -1;

      if (entry->next) entry->next->prev = entry->prev;
      if (entry->prev) entry->prev->next = entry->next;

      if (PL->used == entry) PL->used = entry->next;

      entry->next = PL->avail;
      entry->prev = NULL;

      if (PL->avail) PL->avail->prev = entry;
      PL->avail = entry;
      }

static void PL_dealloc(ProviderList FAR *PL, S32 entry_num)
      {
      if ((entry_num != -1) && (PL->array[entry_num].entry_num != -1))
         PL_dealloc(PL, &PL->array[entry_num]);
      }

static void PL_destroy(ProviderList FAR *PL)
      {
      if (PL->array)
         {
       while (PL->used) PL_dealloc(PL, PL->used);

       PL->current_size = 0;
       if ( PL->array != PL->static_array )
         AIL_mem_free_lock(PL->array);
       PL->array = NULL;
         }
      }

static PVLIST_ENTRY FAR *PL_alloc(ProviderList FAR *PL)
      {
      PVLIST_ENTRY FAR *entry;

      if (PL->avail == NULL)
         {
         S32 prev_size = PL->current_size;
         S32 new_size = prev_size + PROVIDER_BLOCK_SIZE;

         PVLIST_ENTRY FAR *old_array = PL->array;
         PVLIST_ENTRY FAR *new_array = (PVLIST_ENTRY FAR *)
            AIL_mem_alloc_lock(sizeof(PVLIST_ENTRY) * (size_t) new_size);
         if (new_array == NULL) return NULL;

         UINTa adjust = ((UINTa) new_array) - ((UINTa) old_array);

         S32 i;
         for (i=0; i < prev_size; i++)
            {
#ifdef MSS_VERSION                                                          // Avoid extern library warning in PS2 version of MSS
            AIL_memcpy(&new_array[i], &old_array[i], sizeof(old_array[i])); // WARNING: No operator= semantics!
#else
            new_array[i] = old_array[i];
#endif

            if (new_array[i].next) new_array[i].next = (PVLIST_ENTRY FAR *) (((UINTa) new_array[i].next) + adjust);
            if (new_array[i].prev) new_array[i].prev = (PVLIST_ENTRY FAR *) (((UINTa) new_array[i].prev) + adjust);
            }

         for (i=prev_size; i < new_size; i++)
            {
            new_array[i].entry_num = -1;
            new_array[i].prev = &new_array[i-1];
            new_array[i].next = &new_array[i+1];
            }

         new_array[prev_size].prev = NULL;
         new_array[new_size-1].next = NULL;
         PL->used = (PVLIST_ENTRY FAR *) (((UINTa) PL->used) + adjust);
         PL->avail = &new_array[prev_size];
         PL->current_size = new_size;

         if ( PL->array != PL->static_array )
           AIL_mem_free_lock(PL->array);
         PL->array = new_array;
         }

      entry = PL->avail;
      PL->avail = entry->next;
      entry->next = PL->used;
      entry->prev = NULL;

      if (PL->used) PL->used->prev = entry;
      PL->used = entry;
                                                        // TODO: 64-bit scariness below
      entry->entry_num = (S32) ((((UINTa) entry) - ((UINTa) PL->array)) / sizeof(PVLIST_ENTRY));

      return entry;
      }

//############################################################################
//#                                                                          #
//# Globals                                                                  #
//#                                                                          #
//# These don't need to be locked for DOS or Win16, since RIB functions      #
//# are not supported in callbacks                                           #
//#                                                                          #
//############################################################################

InterfaceList interface_list;
ProviderList  provider_list;

S32 list_initialized = 0;


//############################################################################
//#                                                                          #
//# Load a RIB-compatible library                                            #
//#                                                                          #
//############################################################################

RIBDEF HPROVIDER RIBEXPORT RIB_load_static_provider_library (RIB_MAIN_FUNC main, char FAR* description)
{
   //
   // See if we've already allocated a provider handle for this
   // module
   //

   HPROVIDER provider = 0;
   U32       index = 0;

   while ((list_initialized) &&
            (index < (U32) provider_list.current_size))
   {
     PVLIST_ENTRY *p = &provider_list.array[index];

     if ((index != 0) && (p->entry_num != -1))
     {
       if (p->addr == main)
       {
         provider = index;
         break;
       }
     }

     index++;
   }

   //
   // If not. allocate provider entry
   //

   if (provider == 0)
   {
     provider = RIB_alloc_provider_handle(0);

     provider_list.array[provider].addr = main;
   }

   //
   // Bump provider's reference count and return it
   //

   ++provider_list.array[ provider ].refcnt;

   if ( provider_list.array[ provider ].refcnt == 1 )
   {
     if ( provider_list.array[provider].addr( provider, 1 ) )
     {
       provider_list.array[ provider ].module = 0;
       AIL_strcpy(provider_list.array[ provider ].filename, description); 
     }
     else
     {
       RIB_free_provider_handle( provider );
       return( 0 );
     }
   }
  return provider;
}

//############################################################################
//#                                                                          #
//# Allocate an HPROVIDER descriptor                                         #
//#                                                                          #
//############################################################################

RIBDEF HPROVIDER RIBEXPORT RIB_alloc_provider_handle (long module)
{
   //
   // Initialize interface list, if not already done
   //

   if (!list_initialized++)
      {
      IL_construct(&interface_list);
      PL_construct(&provider_list);

      //
      // Mark entry 0 as permanently in use
      // so that NULL HPROVIDERs can be treated
      // as errors
      //

      PL_alloc(&provider_list);
      }                          

   //
   // Allocate provider entry with zero references
   //

   PVLIST_ENTRY FAR *PROVIDER = PL_alloc(&provider_list);

   PROVIDER->module = module;
   
   provider_list.array[ PROVIDER->entry_num ].addr = 0;
   provider_list.array[ PROVIDER->entry_num ].refcnt = 0;
   provider_list.array[ PROVIDER->entry_num ].filename[0] = 0;

   return PROVIDER->entry_num;
}

//############################################################################
//#                                                                          #
//# Free an HPROVIDER descriptor                                             #
//#                                                                          #
//############################################################################

RIBDEF void RIBEXPORT RIB_free_provider_handle (HPROVIDER provider)
{
   PL_dealloc(&provider_list, provider);

   //
   // Destroy interface list when last provider freed
   //

   if (list_initialized)
      {
      if (!--list_initialized)
         {
         IL_destroy(&interface_list);
         PL_destroy(&provider_list);
         }
      }
}

//############################################################################
//#                                                                          #
//# Load a RIB-compatible library                                            #
//#                                                                          #
//############################################################################

RIBDEF HPROVIDER RIBEXPORT RIB_load_provider_library (C8 const FAR *filename)
{
   //
   // See if we've already allocated a provider handle for this
   // module
   //

   HPROVIDER provider = 0;
   U32       index = 0;

   while ((list_initialized) &&
            (index < (U32) provider_list.current_size))
   {
     PVLIST_ENTRY *p = &provider_list.array[index];

     if ((index != 0) && (p->entry_num != -1))
     {
       if (!AIL_stricmp(filename,p->filename))
       {
         provider = index;
         break;
       }
     }

     index++;
   }

   //
   // If not. allocate provider entry
   //

   if (provider == 0)
   {
     provider = RIB_alloc_provider_handle(0);

     AIL_strcpy(provider_list.array[provider].filename,
            filename);
   }

   //
   // Bump provider's reference count and return it
   //

   ++provider_list.array[ provider ].refcnt;

   if ( provider_list.array[ provider ].refcnt == 1 )
   {

#ifdef LOADRIBDLLH
    void * module = load_rib_dll(filename);

    if ( module == 0 )
    {
      RIB_free_provider_handle(provider);
      return 0;
    }

    RIB_MAIN_FUNC init;

    init = get_rib_main( module );

    if ( init == 0 )
    {
     do_err: 
      unload_rib_dll( module );
      RIB_free_provider_handle( provider );
      return 0;
    }

    if ( init( provider, 1 ) )
    {
      provider_list.array[ provider ].module = (SINTa)module;
      provider_list.array[ provider ].addr = init; 
    }
    else
    {
      goto do_err;
    }
      
#else


#ifdef IS_WINDOWS

    //
    // Load specified library
    //

    UINT err=SetErrorMode(0x8000);
    HMODULE module = LoadLibrary(filename);
    SetErrorMode(err);

    if (module == NULL)
    {
      RIB_free_provider_handle(provider);
      return 0;
    }

    RIB_MAIN_FUNC init;

#ifdef IS_WIN64
    init = (RIB_MAIN_FUNC) GetProcAddress( module, "RIB_Main" );
#else
    init = (RIB_MAIN_FUNC) GetProcAddress( module, "_RIB_Main@8" );
#endif
    if ( init == 0 )
    {
     do_err: 
      FreeLibrary( module );
      RIB_free_provider_handle( provider );
      return 0;
    }

    if ( init( provider, 1 ) )
    {
      provider_list.array[ provider ].module = (SINTa)module;
      provider_list.array[ provider ].addr = init; 
    }
    else
    {
      goto do_err;
    }
#else

#ifdef IS_MACHO

    //
    // Load specified library
    //
    
    NSObjectFileImage fi;
    NSModule module = 0;
    
    if ( NSCreateObjectFileImageFromFile(filename, &fi ) == NSObjectFileImageSuccess )
    {
      module = NSLinkModule( fi, filename, NSLINKMODULE_OPTION_RETURN_ON_ERROR | NSLINKMODULE_OPTION_PRIVATE |NSLINKMODULE_OPTION_BINDNOW );
      NSDestroyObjectFileImage( fi );
    }

    if (module == 0)
    {
      RIB_free_provider_handle(provider);
      return 0;
    }

    RIB_MAIN_FUNC init;

    NSSymbol sym = NSLookupSymbolInModule(module, "_RIB_Main");
    if ( sym == 0 )
      goto do_err;
      
     init = (RIB_MAIN_FUNC) NSAddressOfSymbol( sym );

    if ( init == 0 )
    {
     do_err: 
      NSUnLinkModule( module, 0 );
      RIB_free_provider_handle( provider );
      return 0;
    }

    if ( init( provider, 1 ) )
    {
      provider_list.array[ provider ].module = (SINTa)module;
      provider_list.array[ provider ].addr = init; 
    }
    else
    {
      goto do_err;
    }

#else

#ifdef IS_MAC

    CFragConnectionID module;
    CFragSymbolClass theClass;
    Ptr mainaddr;
    FSSpec theSpec;
    unsigned char fn[256];
    fn[0]=(char)AIL_strlen(filename);
    AIL_memcpy(fn+1,filename,fn[0]);
    OSErr err = ::FSMakeFSSpec( 0, 0, (U8 FAR*)fn, &theSpec );
    Str63 theOptName = "\p";
    Str63 theErrName = "\p";

    if( err )
    {
      RIB_free_provider_handle( provider );
      return 0;
    }

    err = ::GetDiskFragment( &theSpec, 0/*with begining*/, 0 /*whole data fork*/,
            theOptName, kReferenceCFrag, &module,
            &mainaddr, theErrName );

    if( err )
    {
      RIB_free_provider_handle( provider );
      return 0;
    }

    RIB_MAIN_FUNC init;

    err = ::FindSymbol( module, "\pRIB_Main", (Ptr*)&init, &theClass );
    if ( err )
    {
     do_err: 
      ::CloseConnection(&module);
      RIB_free_provider_handle( provider );
      return( 0 );
    }

    if ( init( provider, 1 ) )
    {
      provider_list.array[ provider ].module = (long)module;
      provider_list.array[ provider ].addr = init; 
    }
    else
    {
     goto do_err;
    }
#else

#ifdef IS_LINUX

    //
    // Load specified library
    //

    void *module = dlopen( filename, RTLD_NOW );
    if ( module == NULL )
    {
      RIB_free_provider_handle( provider );
      return 0;
    }

    RIB_MAIN_FUNC initfunc;
    initfunc = (RIB_MAIN_FUNC) dlsym( module, "RIB_Main" );
    if ( initfunc == NULL )
    {
     do_err:
      dlclose( module );
      RIB_free_provider_handle( provider );
      return 0;
    }
    if ( initfunc( provider, 1 ) )
    { 
      provider_list.array[ provider ].module = (long)(void FAR*)module;
      provider_list.array[ provider ].addr = initfunc; 
    }
    else
    {
      goto do_err;
    }
           
#else

   filename = filename;

   RIB_free_provider_handle( provider );
   return 0;

#endif

#endif

#endif

#endif

#endif
  }
  return provider;
}

//############################################################################
//#                                                                          #
//# Free a RIB-compatible library                                            #
//#                                                                          #
//############################################################################

RIBDEF void RIBEXPORT RIB_free_provider_library (HPROVIDER provider)
{
  if (provider == (HPROVIDER) NULL)
  {
    //
    // Free all providers
    //

    U32 index = 0;

    while ((list_initialized) &&
           (index < (U32) provider_list.current_size))
    {
      PVLIST_ENTRY *p = &provider_list.array[index];

      if ((index != 0) && (p->entry_num != -1))
      {
        RIB_free_provider_library((HPROVIDER) index);
      }

      index++;
    }
  }
  else
  {
    --provider_list.array[provider].refcnt;

    if (provider_list.array[provider].refcnt <= 0)
    {
      //
      // Free specified provider
      //

      if (provider_list.array[provider].module != 0)
      {
        if (provider_list.array[provider].addr != 0)
        {
          provider_list.array[provider].addr(provider,0);
        }

        #ifdef IS_WINDOWS
          FreeLibrary((HMODULE) (provider_list.array[provider].module));
        #else
          #ifdef IS_MACHO
            NSUnLinkModule( (NSModule) provider_list.array[provider].module, 0 );
          #else
            #ifdef IS_MAC
              ::CloseConnection((CFragConnectionID *)&(provider_list.array[provider].module));
            #else
              #ifdef IS_LINUX
                dlclose(provider_list.array[provider].module);
              #endif
            #endif  
          #endif
        #endif
      }
      else
      {
        // static lib
        if (provider_list.array[provider].addr != 0)
        {
          provider_list.array[provider].addr(provider,0);
        }
      }

      RIB_free_provider_handle(provider);
    }
  }
}

//############################################################################
//#                                                                          #
//# Handle data or code interface registration                               #
//#                                                                          #
//############################################################################

RIBDEF RIBRESULT RIBEXPORT RIB_register_interface (HPROVIDER                      provider, //)
                                                   C8 const                  FAR *interface_name,
                                                   S32                            entry_count,
                                                   RIB_INTERFACE_ENTRY const FAR *list)
{
   if ( IL_verify_provider( &interface_list, 
                           provider ) == NULL )
   {
     S32 ver = 0;
     S32 ver_major = 0;
     S32 ver_minor = 0;

     // ok this is the first register for this provider
     //   make sure that the first entry is for a property function

     if ( list[ 0 ].type != RIB_FUNCTION )
       goto err;
     
     if ( AIL_strcmp( list[ 0 ].entry_name, "PROVIDER_property" ) )
       goto err;

     if (((PROVIDER_PROPERTY)list[ 0 ].token)(PROVIDER_VERSION,&ver,0,0) == 0 )
       goto err;
     
     ver_major = (ver >> 8) & 0xff;
     ver_minor = (ver >> 4) & 0x0f;

     if ((ver_major < 1) || ((ver_major == 1) && (ver_minor < 1)))  // we load version 1.10 or later
     {
       err:
       AIL_set_error("Attempt to load obsolete (pre-V1.10) RIB");
       return RIB_NOT_FOUND;
     }
   }

   //
   // Register all entries in list
   //

   INTERFACE_DESCRIPTOR temp;

   temp.provider = provider;
   AIL_strcpy(temp.interface_name, interface_name);

   for (S32 i=0; i < entry_count; i++)
      {
      AIL_strcpy(temp.entry_name, list[i].entry_name);

      temp.type    = list[i].type;
      temp.subtype = list[i].subtype;
      temp.token   = list[i].token;

      IL_alloc(&interface_list, &temp);
      }

   return RIB_NOERR;
}

//############################################################################
//#                                                                          #
//# Handle requests for data or code interfaces                              #
//#                                                                          #
//############################################################################

RIBDEF RIBRESULT RIBEXPORT RIB_request_interface  (HPROVIDER                provider, //)
                                                   C8 const FAR            *interface_name,
                                                   S32                      entry_count,
                                                   RIB_INTERFACE_ENTRY FAR *list)
{
   //
   // Build descriptor for specified request
   //

   INTERFACE_DESCRIPTOR temp;

   temp.provider = provider;
   AIL_strcpy(temp.interface_name, interface_name);

   //
   // See if provider supports any entries for specified interface
   //

   if (IL_verify_interface(&interface_list, 
                            provider,
                            interface_name) == NULL)
      {
      return RIB_NOT_FOUND;
      }

   //
   // Resolve all entries in list
   //

   S32 misses = 0;

   for (S32 i=0; i < entry_count; i++)
      {
#if DIAG_REQUEST
      AIL_debug_printf("RIB: Looking for %s... ",list[i].entry_name);
#endif

      AIL_strcpy(temp.entry_name, list[i].entry_name);
      temp.type = list[i].type;

      ILIST_ENTRY FAR *entry = IL_lookup(&interface_list, &temp);

      if (entry == NULL)
         {
#if DIAG_REQUEST
         AIL_debug_printf("Failed!\n");
#endif
         if (list[i].type == RIB_FUNCTION)
            {
            //
            // Failed function lookups return NULL pointer
            //

            *(UINTa FAR *) (list[i].token) = 0;
            }
         else
            {
            //
            // Failed pref/attrib token lookups return -1
            //

            *(SINTa FAR *) (list[i].token) = (SINTa) -1;
            }

         ++misses;
         }
      else
         {
#if DIAG_REQUEST
         AIL_debug_printf("Found %d (%X)\n",entry->contents.token,
            entry->contents.token);
#endif
         *(UINTa FAR *) (list[i].token) = entry->contents.token;
         }
      }

   //
   // Return RIB_NOT_ALL_AVAILABLE if one or more entries could not be
   // resolved
   //
   // These entries are set to NULL (0)
   //

   if (!misses)
      {
      return RIB_NOERR;
      }
   else
      {
      AIL_strcpy(error_text,"One or more requested entries not found");
      return RIB_NOT_ALL_AVAILABLE;
      }
}

//############################################################################
//#                                                                          #
//# Handle request for single data or code interface entry                   #
//#                                                                          #
//############################################################################

static ILIST_ENTRY FAR * last_entry;

RIBDEF RIBRESULT RIBEXPORT RIB_request_interface_entry  (HPROVIDER       provider, //)
                                                         C8 const FAR   *interface_name,
                                                         RIB_ENTRY_TYPE  entry_type,
                                                         C8 const FAR   *entry_name,
                                                         UINTa FAR      *token)
{
   //
   // Build descriptor for specified request
   //

   INTERFACE_DESCRIPTOR temp;
   
   temp.provider = provider;
   temp.type     = entry_type;

   AIL_strcpy(temp.interface_name, interface_name);
   AIL_strcpy(temp.entry_name,     entry_name);

#if DIAG_REQUEST
      AIL_debug_printf("RIB: Looking for entry %s... ",entry_name);
#endif

   //
   // Resolve requested entry
   //

   ILIST_ENTRY FAR *entry = IL_lookup(&interface_list, &temp);

   if (entry == NULL)
      {
      *token = 0;
#if DIAG_REQUEST
         AIL_debug_printf("Failed!\n");
#endif
      return RIB_NOT_FOUND;
      }
   else
      {
#if DIAG_REQUEST
         AIL_debug_printf("Found %d (%X)\n",entry->contents.token,
            entry->contents.token);
#endif

      *token = entry->contents.token;
      last_entry = entry;
      return RIB_NOERR;
      }
}

//############################################################################
//#                                                                          #
//# Unregister interface entries                                             #
//#                                                                          #
//############################################################################

RIBDEF RIBRESULT RIBEXPORT RIB_unregister_interface  (HPROVIDER                      provider, //)
                                                      C8 const FAR                  *interface_name,
                                                      S32                            entry_count,
                                                      RIB_INTERFACE_ENTRY const FAR *list)
{
   //
   // Resolve all entries in list
   //

   S32 misses = 0;

   if ((list != NULL) && (interface_name != NULL))
      {
      //
      // Build descriptor for specified interface
      //

      INTERFACE_DESCRIPTOR temp;
   
      temp.provider = provider;
      AIL_strcpy(temp.interface_name, interface_name);

      //
      // Remove only listed entries
      //

      for (S32 i=0; i < entry_count; i++)
         {
         AIL_strcpy(temp.entry_name, list[i].entry_name);
         temp.type = list[i].type;

         ILIST_ENTRY FAR *entry = IL_lookup(&interface_list, &temp);

         if (entry == NULL)
            {
            ++misses;
            }
         else
            {
            IL_dealloc(&interface_list, entry);
            }
         }
      }
   else
      {
      //
      // No valid list specified -- remove all entries that match provider
      // and interface name
      //

      for (S32 set=0; set < INTERFACE_SET_DIM; set++)
         {
         ILIST_ENTRY FAR *found = interface_list.table[set];

         while (found)
            {
            ILIST_ENTRY FAR *next = found->next_in_set;

            if ((found->entry_num != -1) && (found->contents.provider == provider))
               {
               if ((interface_name == NULL) || 
                   (!AIL_strcmp(interface_name, found->contents.interface_name)))
                   {
                   IL_dealloc(&interface_list, found);
                   }
               }

            found = next;
            }
         }
      }

   //
   // Return RIB_NOT_ALL_AVAILABLE if one or more entries were not found
   //

   if (!misses)
      {
      return RIB_NOERR;
      }
   else
      {
      AIL_strcpy(error_text,"One or more entries not found");
      return RIB_NOT_ALL_AVAILABLE;
      }
}

//############################################################################
//#                                                                          #
//# Enumerate interface entries                                              #
//#                                                                          #
//############################################################################

RIBDEF S32 RIBEXPORT RIB_enumerate_interface(HPROVIDER                provider, //)
                                             C8 const FAR            *interface_name,
                                             RIB_ENTRY_TYPE           type,
                                             HINTENUM  FAR           *next,
                                             RIB_INTERFACE_ENTRY FAR *dest)
{
   S32              set;
   ILIST_ENTRY FAR *entry;

   if (*next == HINTENUM_FIRST)
      {
      //
      // Begin search at start of hash space
      //

      set = 0;
      entry = interface_list.table[set];
      }
   else
      {
      //
      // Begin search at next non-empty entry 
      //

      set = (*next) & (INTERFACE_SET_DIM - 1);
      entry = &interface_list.array[(*next) >> INTERFACE_NAME_HASH_SIZE];

      entry = entry->next_in_set;

      while ((entry == NULL) && (set < INTERFACE_SET_DIM))
         {
         entry = interface_list.table[++set];
         }
      }

   //
   // Find next qualified entry
   //

   while (set < INTERFACE_SET_DIM)
      {
      while (entry)
         {
         if (((!provider) || (entry->contents.provider == provider)) &&
             (entry->entry_num != -1) &&
             (entry->contents.type == type) &&
             (!AIL_strcmp(entry->contents.interface_name,
                          interface_name)))
            {
            //
            // Qualified entry found -- pack hash set and index # 
            // into U32 and return entry information
            //

            *next = (entry->entry_num << INTERFACE_NAME_HASH_SIZE) | set;

            dest->type       = entry->contents.type;
            dest->entry_name = entry->contents.entry_name;
            dest->token      = entry->contents.token;
            dest->subtype    = entry->contents.subtype;

            return 1;
            }

         entry = entry->next_in_set;
         }

      entry = interface_list.table[++set];
      }

   //
   // No qualified entries found -- reset pointer to start of list and
   // return 0
   //

   *next = HINTENUM_FIRST;
   return 0;
}

//############################################################################
//#                                                                          #
//# Enumerate available providers of specified interface, or of all          #
//# interfaces if interface_name = NULL                                      #
//#                                                                          #
//############################################################################

RIBDEF S32 RIBEXPORT RIB_enumerate_providers     (C8 FAR        *interface_name, //)
                                                  HPROENUM FAR  *next,
                                                  HPROVIDER FAR *dest)
{
   HPROVIDER index;

   HPROENUM dummy = HPROENUM_FIRST;

   if (next == NULL)
      {
      next = &dummy;
      }

   if (*next == HPROENUM_FIRST)
      {
      //
      // Skip dummy first entry
      //

      index = 1;
      }
   else
      {
      index = (*next) + 1;
      }

   //
   // Find next qualified entry
   //

   while (index < (U32) provider_list.current_size)
      {
      S32 qual = 0;

      if (interface_name == NULL)
         {
         S32              set = 0;
         ILIST_ENTRY FAR *entry = interface_list.table[set];

         while ((set < INTERFACE_SET_DIM) && (!qual))
            {
            while (entry)
               {
               if ((entry->entry_num != -1) && (entry->contents.provider == index))
                  {
                  qual = 1;
                  break;
                  }

               entry = entry->next_in_set;
               }

            entry = interface_list.table[++set];
            }
         }
      else
         {
         S32              set = 0;
         ILIST_ENTRY FAR *entry = interface_list.table[set];

         while ((set < INTERFACE_SET_DIM) && (!qual))
            {
            while (entry)
               {
               if ((entry->entry_num != -1) &&
                   (entry->contents.provider == index) &&
                   (!AIL_strcmp(entry->contents.interface_name,
                                interface_name)))
                  {
                  qual = 1;
                  break;
                  }

               entry = entry->next_in_set;
               }

            entry = interface_list.table[++set];
            }
         }

      //
      // Return success if match
      //

      if (qual)
         {
         *next = index;
         *dest = index;
         return 1;
         }

      //
      // Otherwise, keep looking...
      //

      ++index;
      }

   //
   // No qualified entries found -- reset pointer to start of list and
   // return 0
   //

   *next = HPROENUM_FIRST;
   *dest = 0;
   return 0;
}

//############################################################################
//#                                                                          #
//# Return pointer to static string containing value formatted according to  #
//# its type                                                                 #
//#                                                                          #
//############################################################################

RIBDEF C8 FAR * RIBEXPORT RIB_type_string (void const FAR*  data, //)
                                           RIB_DATA_SUBTYPE subtype)
{
   static C8 string[256];

   switch (subtype & ~RIB_READONLY )
      {
      case RIB_HEX:     
        AIL_sprintf(string,"0x%X",*(S32 FAR *)data);
        break;
        
      case RIB_FLOAT:  
#ifdef IS_PS2
         AIL_sprintf(string,"%d",(S32) *(F32 FAR *)data);
#else
         AIL_sprintf(string,"%3.1f",*(F32 FAR *)data);
#endif
        break;

      case RIB_PERCENT:  
         {
#ifdef IS_PS2
         AIL_sprintf(string,"%d%%",(S32) *(F32 FAR *)data);
#else
         AIL_sprintf(string,"%3.1f%%",*(F32 FAR *)data);
#endif
         }
         break;

      case RIB_BOOL:    
        AIL_strcpy(string, ( *(S32 FAR *)data ) ? "True" : "False"); 
        break;
        
      case RIB_STRING:  
        AIL_strcpy(string,*(char FAR * FAR*)data); 
        break;

      default:   
        AIL_sprintf(string,"%d",*(S32 FAR *)data);
        break;
      }

   if ( subtype & RIB_READONLY )
     AIL_strcat( string + AIL_strlen( string ), " (Read only)" );
   
   return string;
}

// match a file suffix

S32 AILCALL match_suffix(char const FAR* suffix,char const FAR* file_suffix)
{
  //
  // Scan Windows-compatible filespec string to see if specified
  // suffix is supported
  //

  S32 tl = (S32) strlen(file_suffix);

  S32 i;
  for (i=tl-1; i > 0; i--)
     {
     if (file_suffix[i] == '.')
        {
        break;
        }
     }

  C8 const FAR *search_suffix = &file_suffix[i];
  tl                    = (S32) strlen(search_suffix);

  for(;;)
     {
     S32 sl = (S32) AIL_strlen(suffix);

     suffix = &suffix[sl+1];

     sl = (S32) AIL_strlen(suffix);

     if (sl == 0)
        {
        break;
        }

     if (sl >= tl)
        {
        //
        // Suffix length is ("*.xxx") longer than requested search
        // suffix (".xxx" or "xxx")
        //

        if (!AIL_stricmp(&suffix[sl-tl],
                          search_suffix))
           {
           return 1;
           }
        }

     suffix = &suffix[sl+1];

     if (!*suffix)
        {
        break;
        }
     }
  return(0);
}


//############################################################################
//#                                                                          #
//# Find provider of specified interface which is capable of supporting the  #
//# specified file type                                                      #
//#                                                                          #
//############################################################################

RIBDEF HPROVIDER RIBEXPORT RIB_find_file_provider (C8 const FAR *interface_name, //)
                                                   C8 const FAR *property_name,
                                                   C8 const FAR *file_suffix)
{
   S32 set = 0;
   ILIST_ENTRY FAR *entry = interface_list.table[set];

   while (set < INTERFACE_SET_DIM)
      {
      while (entry)
         {
         //
         // Find specified interface's PROVIDER_property() function
         // entry
         //

         if ((entry->entry_num != -1)                                      &&
             (entry->contents.type ==                     RIB_FUNCTION)    &&
             (!AIL_strcmp(entry->contents.interface_name, interface_name)) &&
             (!AIL_strcmp(entry->contents.entry_name,    "PROVIDER_property")))
            {
            //
            // Get address of property handler
            //

            PROVIDER_PROPERTY PROVIDER_property =
               (PROVIDER_PROPERTY) entry->contents.token;

            //
            // Get token for specified filespec property
            //

            HPROPERTY token;

            if (RIB_request_interface_entry  (entry->contents.provider,
                                              entry->contents.interface_name,
                                              RIB_PROPERTY,
                                              property_name,
                                             &token))
               {
               //
               // Skip if specified property not exported by provider
               //

               entry = entry->next_in_set;
               continue;
               }

            //
            // Get filespec string value
            //

            C8 FAR *suffix=""; PROVIDER_property(token, &suffix, 0, 0);

            if (match_suffix(suffix,file_suffix))
              return entry->contents.provider;
            
            }

         //
         // Move to next entry in hash set
         //

         entry = entry->next_in_set;
         }

      //
      // Move to next hash set
      //

      entry = interface_list.table[++set];
      }

   return 0;
}


//############################################################################
//#                                                                          #
//# Find provider of specified interface which is capable of supporting the  #
//# specified file type for in and out                                       #
//#                                                                          #
//############################################################################

RIBDEF HPROVIDER RIBEXPORT RIB_find_files_provider (C8 const FAR *interface_name, //)
                                                    C8 const FAR *property_name_1,
                                                    C8 const FAR *file_suffix_1,
                                                    C8 const FAR *property_name_2,
                                                    C8 const FAR *file_suffix_2)
{
   S32 set = 0;
   ILIST_ENTRY FAR *entry = interface_list.table[set];

   while (set < INTERFACE_SET_DIM)
      {
      while (entry)
         {
         //
         // Find specified interface's PROVIDER_property() function
         // entry
         //

         if ((entry->entry_num != -1)                                      &&
             (entry->contents.type ==                     RIB_FUNCTION)    &&
             (!AIL_strcmp(entry->contents.interface_name, interface_name)) &&
             (!AIL_strcmp(entry->contents.entry_name,    "PROVIDER_property")))
            {
            //
            // Get address of property-query handler
            //

            PROVIDER_PROPERTY PROVIDER_property =
               (PROVIDER_PROPERTY) entry->contents.token;

            //
            // Get token for specified filespec property
            //

            HPROPERTY token_1,token_2;

            if (RIB_request_interface_entry  (entry->contents.provider,
                                              entry->contents.interface_name,
                                              RIB_PROPERTY,
                                              property_name_1,
                                             &token_1))
               {
               //
               // Skip if specified property not exported by provider
               //

               entry = entry->next_in_set;
               continue;
               }

            if (RIB_request_interface_entry  (entry->contents.provider,
                                              entry->contents.interface_name,
                                              RIB_PROPERTY,
                                              property_name_2,
                                             &token_2))
               {
               //
               // Skip if specified property not exported by provider
               //

               entry = entry->next_in_set;
               continue;
               }

            //
            // Get filespec string value
            //

            C8 FAR *suffix_1=""; PROVIDER_property(token_1, &suffix_1, 0, 0);
            C8 FAR *suffix_2=""; PROVIDER_property(token_2, &suffix_2, 0, 0);

            if ((match_suffix(suffix_1,file_suffix_1)) &&
                (match_suffix(suffix_2,file_suffix_2)))
              return entry->contents.provider;

            }

         //
         // Move to next entry in hash set
         //

         entry = entry->next_in_set;
         }

      //
      // Move to next hash set
      //

      entry = interface_list.table[++set];
      }

   return 0;
}


//############################################################################
//#                                                                          #
//# Find provider of specified interface which is capable of supporting the  #
//# specified file wave format tag and the output file type                  #
//#                                                                          #
//############################################################################

RIBDEF HPROVIDER RIBEXPORT RIB_find_file_dec_provider (C8 const FAR *interface_name, //)
                                                       C8 const FAR *property_name_1,
                                                       U32 decimal_property_value_1,
                                                       C8 const FAR *property_name_2,
                                                       C8 const FAR *file_suffix_2)
{
   S32 set = 0;
   ILIST_ENTRY FAR *entry = interface_list.table[set];

   while (set < INTERFACE_SET_DIM)
      {
      while (entry)
         {
         //
         // Find specified interface's PROVIDER_property() function
         // entry
         //

         if ((entry->entry_num != -1)                                      &&
             (entry->contents.type ==                     RIB_FUNCTION)    &&
             (!AIL_strcmp(entry->contents.interface_name, interface_name)) &&
             (!AIL_strcmp(entry->contents.entry_name,    "PROVIDER_property")))
            {
            //
            // Get address of property-query handler
            //

            PROVIDER_PROPERTY PROVIDER_property =
               (PROVIDER_PROPERTY) entry->contents.token;

            //
            // Get token for specified filespec attribute
            //

            HPROPERTY token_1,token_2;

            if (RIB_request_interface_entry  (entry->contents.provider,
                                              entry->contents.interface_name,
                                              RIB_PROPERTY,
                                              property_name_1,
                                             &token_1))
               {
               //
               // Skip if specified property not exported by provider
               //

               entry = entry->next_in_set;
               continue;
               }

            if (RIB_request_interface_entry  (entry->contents.provider,
                                              entry->contents.interface_name,
                                              RIB_PROPERTY,
                                              property_name_2,
                                             &token_2))
               {
               //
               // Skip if specified property not exported by provider
               //

               entry = entry->next_in_set;
               continue;
               }

            //
            // Get filespec string value
            //

            U32 value1=(U32)~0;  PROVIDER_property(token_1,&value1,0,0);
            C8 FAR *suffix_2=""; PROVIDER_property(token_2,&suffix_2,0,0);

            if ((decimal_property_value_1==value1) &&
                (match_suffix(suffix_2,file_suffix_2)))
               {
               return entry->contents.provider;
               }
            }

         //
         // Move to next entry in hash set
         //

         entry = entry->next_in_set;
         }

      //
      // Move to next hash set
      //

      entry = interface_list.table[++set];
      }

   return 0;
}


//############################################################################
//#                                                                          #
//# Find provider of specified interface which has the matching property     #
//#                                                                          #
//############################################################################

RIBDEF HPROVIDER RIBEXPORT RIB_find_provider (C8 const FAR *interface_name, //)
                                              C8 const FAR *property_name,
                                              void const FAR *property_value)
{
   S32 set = 0;
   ILIST_ENTRY FAR *entry = interface_list.table[set];

   while (set < INTERFACE_SET_DIM)
      {
      while (entry)
         {
         //
         // Find specified interface's PROVIDER_property() function
         // entry
         //

         if ((entry->entry_num != -1)                                      &&
             (entry->contents.type ==                     RIB_FUNCTION)    &&
             (!AIL_strcmp(entry->contents.interface_name, interface_name)) &&
             (!AIL_strcmp(entry->contents.entry_name,    "PROVIDER_property")))
            {
            //
            // Get address of property-query handler
            //

            PROVIDER_PROPERTY PROVIDER_property =
               (PROVIDER_PROPERTY) entry->contents.token;

            //
            // Get token for specified property
            //

            HPROPERTY token;

            if (RIB_request_interface_entry  (entry->contents.provider,
                                              entry->contents.interface_name,
                                              RIB_PROPERTY,
                                              property_name,
                                             &token))
               {
               //
               // Skip if specified property not exported by provider
               //

               entry = entry->next_in_set;
               continue;
               }

            //
            // Get the value
            //

            UINTa value=0;  PROVIDER_property(token,&value,0,0);

            //
            //  do we match?
            //

            F32 diff;
            
            switch( last_entry->contents.subtype & ~RIB_READONLY )
            {
              case RIB_DEC:
              case RIB_HEX:
                if ( value && ( ((U32 FAR*)value)[0] == ((U32 FAR*)property_value)[0] ) )
                  return entry->contents.provider;
                break;

              case RIB_FLOAT:
              case RIB_PERCENT:
                diff = ((F32 FAR*)value)[0] - ((U32 FAR*)property_value)[0];
                if ( value && ( diff < 0.0001f) && ( diff > -0.0001f ) )
                  return entry->contents.provider;
                break;

              case RIB_BOOL:
                if ( value )
                {
                  U32 b1, b2;
                  b1 = (((U32 FAR*)value)[0])?1:0;
                  b2 = (((U32 FAR*)property_value)[0])?1:0;
                  if ( ( b1 ^ b2 ) == 0 )
                    return entry->contents.provider;
                }
                break;

              case RIB_STRING:
                if (value && ( AIL_stricmp((char FAR*)value,(char FAR*)property_value)==0) )
                  return entry->contents.provider;
                break;
            }
         }

         //
         // Move to next entry in hash set
         //

         entry = entry->next_in_set;
         }

      //
      // Move to next hash set
      //

      entry = interface_list.table[++set];
      }

   return 0;
}

//############################################################################
//#                                                                          #
//# Load all RIB provider libraries from application-relative directory      #
//#                                                                          #
//############################################################################

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX)

#if defined( IS_MACHO ) || defined(IS_LINUX)

static S32 load_application_providers(char const FAR* dir, char const FAR *in_filespec)
{
   S32 n = 0;

   //
   // Get application path
   //

   C8 name_buffer[MAX_PATH+4];

   //
   // Preload all available ASI providers from application path
   //

   DIR *dirp;
   struct dirent *entry;

   dirp = opendir(dir);

   if ( dirp != NULL )
      {
      size_t filespec_len;
      if ( *in_filespec == '*' )
         ++in_filespec;
      filespec_len = strlen(in_filespec);
      

      while ((entry = readdir(dirp)) != NULL )
         {
         size_t len = strlen(entry->d_name);
         if (len <= filespec_len ||
             strcmp(&entry->d_name[len-filespec_len], in_filespec))
            continue;

         AIL_strcpy(name_buffer, dir);
         AIL_strcat(name_buffer, "/" );
         AIL_strcat(name_buffer, entry->d_name);
   
         HPROVIDER index = RIB_load_provider_library(name_buffer);

         if (index)
            {
            ++n;
#if 0
            AIL_debug_printf("RIB: Loaded provider %s = %X\n",name_buffer,index);
#endif
            }
         }
   
      closedir(dirp);
      }

   return n;
}
#elif defined( IS_MAC )

static S32 load_application_providers(char const FAR* dir, char const FAR *in_filespec)
{
////////////////////////////////////////////////////////////
  S32 n = 0; 
  CInfoPBRec    filespec;                          /* local pb */
  HFileInfo     *fpb = (HFileInfo *)&filespec;     /* to pointers */
  DirInfo       *dpb = (DirInfo *) &filespec;
  short  idx;
  unsigned char name_buffer[256];
  FSSpec fsSpec;
  
  name_buffer[0]=(char)AIL_strlen(dir);
  AIL_memcpy(name_buffer+1,dir,name_buffer[0]);

  OSErr err = ::FSMakeFSSpec(0, 0, (unsigned char *)name_buffer, &fsSpec);
  
  dpb->ioNamePtr = (U8 FAR*)name_buffer;
  fpb->ioFDirIndex = 0;
  PBGetCatInfo(&filespec, FALSE);

  if(dpb->ioResult)
    return 0;
  
  fpb->ioFDirIndex = -1;
  long constDirID = dpb->ioDrDirID;

  for( idx=1; TRUE; idx++)
  {
    /* indexing loop */
    fpb->ioVRefNum = fsSpec.vRefNum;
    fpb->ioNamePtr = (U8 FAR*)name_buffer;             /* buffer to receive name */
    fpb->ioDirID = constDirID;         /* must set on each loop */
    fpb->ioFDirIndex = idx;

    PBGetCatInfo( &filespec, FALSE );
    if (dpb->ioResult)
      return n;     /* exit when no more entries */

    if (!(fpb->ioFlAttrib & 16))
    {
      //Need to check for *.XXX                 ;

      if(!AIL_strnicmp((char *)&name_buffer[fpb->ioNamePtr[0] + 1 - 4], &in_filespec[1], 4))
      {
        char temp[256];
        int i=AIL_strlen(dir);
        AIL_memcpy(temp, dir,i);
        AIL_memcpy(temp+i,fpb->ioNamePtr+1,fpb->ioNamePtr[0]);
        temp[i+fpb->ioNamePtr[0]]=0;

        HPROVIDER index = RIB_load_provider_library(temp);
        if (index)
          ++n;
      }
    }
  }
}

#else

static S32 load_application_providers(char const FAR* dir, char const FAR *in_filespec)
{
   S32 n = 0;
   //
   // Get application path
   //

   C8 name_buffer[MAX_PATH+4];

   AIL_strcpy(name_buffer,dir);
   AIL_strcat(name_buffer,in_filespec);

#ifdef IS_WIN32

   //
   // Preload all available ASI providers from application path
   //

   HANDLE          search_handle;
   WIN32_FIND_DATA found;

   search_handle = FindFirstFile(name_buffer, &found);

   if ((search_handle != NULL) && (search_handle != INVALID_HANDLE_VALUE))
      {
      do
         {
         AIL_strcpy(name_buffer, dir);
         AIL_strcat(name_buffer, found.cFileName);

         HPROVIDER index = RIB_load_provider_library(name_buffer);

         if (index)
            {
            ++n;
#if 0
            AIL_debug_printf("RIB: Loaded provider %s = %X\n",name_buffer,index);
#endif
            }
         }
      while (FindNextFile(search_handle, &found));

      FindClose(search_handle);
      }

   return n;
#elif defined( IS_WIN16 )

   struct _find_t found;

   U32 err = _dos_findfirst(name_buffer, _A_NORMAL, &found );

   while (!err)
      {

      AIL_strcpy(name_buffer,dir);
      AIL_strcat(name_buffer,found.name);

      HPROVIDER index = RIB_load_provider_library(name_buffer);

      if (index)
         {
         ++n;
#if 0
         AIL_debug_printf("RIB: Loaded provider %s = %X\n",found.name,index);
#endif
         }

      err = _dos_findnext(&found);
      }

   return n;

#endif
}
#endif
#endif

//############################################################################
//#                                                                          #
//# Load all RIB provider libraries from application-relative directory      #
//#                                                                          #
//############################################################################

RIBDEF S32 RIBEXPORT RIB_load_application_providers(char const FAR *filespec)
{
#if !(defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX))

   filespec=filespec;
   return(0);

#else

   U32 n;
   char dir[512];
   
   AIL_strcpy(dir,MSS_Directory);
#ifdef IS_X86
#if defined( IS_LINUX ) || defined(IS_MAC)
   AIL_strcat(dir,"/");
#else
   AIL_strcat(dir,"\\");
#endif
#endif

   n=load_application_providers(dir,filespec);

#ifndef BUILD_STANDALONE_RIB

   if (*AIL_redist_directory)
   {
     if (AIL_redist_directory[0]=='*')
       n+=load_application_providers(AIL_redist_directory + 1,filespec);
     else      
#ifdef IS_X86
     if ((AIL_redist_directory[0]=='/') || (AIL_redist_directory[0]=='\\') || (AIL_redist_directory[1]==':'))
#elif defined(IS_MACHO)
     if (AIL_redist_directory[0] == '/')
#else
     if (AIL_redist_directory[0] != ':')
#endif
       n+=load_application_providers(AIL_redist_directory,filespec);
     else
     {
       // relative path
       AIL_strcpy(dir,MSS_Directory);
#ifdef IS_X86
#if defined( IS_LINUX) || defined(IS_MAC)
   AIL_strcat(dir,"/");
#else
   AIL_strcat(dir,"\\");
#endif
       AIL_strcat(dir,AIL_redist_directory);
#elif defined(IS_MACHO)
       AIL_strcat(dir,"/");
       AIL_strcat(dir,AIL_redist_directory);
#else
       AIL_strcat(dir,AIL_redist_directory+1);
#endif       

       n+=load_application_providers(dir,filespec);
     }
   }

#endif

   return((S32)n);
#endif
}


//############################################################################
//##                                                                        ##
//## Set provider user data value at specified index                        ##
//##                                                                        ##
//## Any desired value may be stored at one of eight user data words        ##
//## associated with a given HPROVIDER                                      ##
//##                                                                        ##
//## Callback functions may access the user data array at interrupt time    ##
//##                                                                        ##
//############################################################################

RIBDEF void RIBEXPORT RIB_set_provider_user_data(HPROVIDER provider, U32 index, SINTa value)
{
   if (!provider)
      {
      return;
      }

   provider_list.array[provider].user_data[index] = value;
}

//############################################################################
//##                                                                        ##
//## Get provider user data value at specified index                        ##
//##                                                                        ##
//## Any desired value may be stored at one of eight user data words        ##
//## associated with a given HPROVIDER                                      ##
//##                                                                        ##
//## Callback functions may access the user data array at interrupt time    ##
//##                                                                        ##
//############################################################################

RIBDEF SINTa RIBEXPORT RIB_provider_user_data(HPROVIDER provider, U32 index)
{
   if (!provider)
      {
      return 0;
      }

   return provider_list.array[provider].user_data[index];
}


//############################################################################
//##                                                                        ##
//## Set provider system data value at specified index                      ##
//##                                                                        ##
//## Any desired value may be stored at one of eight system data            ##
//## words associated with a given HPROVIDER                                ##
//##                                                                        ##
//## Callback functions may access the system data array at interrupt time  ##
//##                                                                        ##
//############################################################################

RIBDEF void RIBEXPORT RIB_set_provider_system_data(HPROVIDER provider, U32 index, SINTa value)
{
   if (!provider)
      {
      return;
      }

   provider_list.array[provider].system_data[index] = value;
}

//############################################################################
//##                                                                        ##
//## Get provider system data value at specified index                      ##
//##                                                                        ##
//## Any desired value may be stored at one of eight system data            ##
//## words associated with a given HPROVIDER                                ##
//##                                                                        ##
//## Callback functions may access the system data array at interrupt time  ##
//##                                                                        ##
//############################################################################

RIBDEF SINTa RIBEXPORT RIB_provider_system_data(HPROVIDER provider, U32 index)
{
   if (!provider)
      {
      return 0;
      }

   return provider_list.array[provider].system_data[index];
}

//############################################################################
//#                                                                          #
//# Return current error status                                              #
//#                                                                          #
//############################################################################

RIBDEF C8 FAR * RIBEXPORT RIB_error (void)
{
   return error_text;
}

#if defined(IS_WIN32) || defined(IS_WIN64)

  #define DLL_PROCESS_ATTACH   1

  typedef struct HINSTANCE__ *HINSTANCE;
  typedef HINSTANCE HMODULE;      

  __declspec(dllimport)
  BOOL
  __stdcall
  DisableThreadLibraryCalls(
       HMODULE hLibModule
      );

extern "C" RIBDEF BOOL __stdcall MSSDisableThreadLibraryCalls( HMODULE hLibModule )
{
  DisableThreadLibraryCalls( hLibModule );
  return( 1 );
}

#endif
