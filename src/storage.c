#include <X11/Xlib.h>
#include <stdint.h>
#include <stdlib.h>
#include "storage.h"

#ifdef STORAGE_GHASH
#include <glib.h>
#endif

#ifdef STORAGE_GHASH
GHashTable *gStorage = NULL;
#define GINT_TO_POINTER(i) ((gpointer) (glong) (i))
#define GUINT_TO_POINTER(u) ((gpointer) (gulong) (u))
#endif

void debug();

void _kbdd_storage_init() {
    if ( gStorage!=NULL ) return; 
#ifdef STORAGE_GHASH
    gStorage = g_hash_table_new(g_direct_hash, NULL);
//    debug();
#endif
}

void _kbdd_storage_free() {
    if ( gStorage != NULL ) {
#ifdef STORAGE_GHASH
        g_hash_table_destroy(gStorage);
#endif
    }
}

void _kbdd_storage_put(Window win, GROUP_TYPE group)
{
    if ( gStorage != NULL ) {
#ifdef STORAGE_GHASH
        gpointer key = GUINT_TO_POINTER(win);
        gpointer val = GUINT_TO_POINTER(group);
//        printf("%u %u => %u %u\n", win, group,key, val);
        g_hash_table_replace(gStorage, key, val);
//        debug();
    }
#endif
}

GROUP_TYPE _kbdd_storage_get(Window win)
{
    GROUP_TYPE group;
    if (gStorage != NULL) 
    {
#ifdef STORAGE_GHASH
//        debug();
        gpointer key = NULL;
        gpointer value = NULL;
        gpointer pWindow = GUINT_TO_POINTER(win);
//        printf("%u -> %u %u %u\n",win, pWindow);
        if ( g_hash_table_lookup_extended(gStorage, pWindow, &key, &value) )
        {
            group = (GROUP_TYPE)value;
//            printf(":%u %u\n",value,group);
        }
        else 
        {
            return 0;
        }
#endif
    }
    return group;
}

void _kbdd_storage_remove(Window win)
{
    if (gStorage != NULL) {
#ifdef STORAGE_GHASH
        gpointer key = GUINT_TO_POINTER(win);
        g_hash_table_remove(gStorage, key);
#endif
    }
}

void debug() {
    GHashTableIter iter;
    g_hash_table_iter_init (&iter, gStorage);
    int size=g_hash_table_size(gStorage);
    printf("=================\nFirst size: %d\n",size);

    uint32_t *val;
    uint32_t *key_;
    while (g_hash_table_iter_next (&iter, (gpointer) &key_, (gpointer) &val))
    {
        printf("key %u ---> %u\n",(uint32_t)key_,(uint32_t)val);
    }
    printf("=================\n");
}

