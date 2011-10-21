/* Copyright (c) 2011 Artem Shinkarov <artyom.shinkaroff@gmail.com>
                      Pavel Zaichenkov <zaichenkov@gmail.com>

   Permission to use, copy, modify, and distribute this software for any
   purpose with or without fee is hereby granted, provided that the above
   copyright notice and this permission notice appear in all copies.
  
   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.  */

#include "types.h"
#include "tree.h"

struct tree_type_node * type_table;

/* Data structures related to types initialization.  */
void
types_init ()
{
  type_table = NULL;
  types_add_type (B_TYPE, 1, NULL, NULL);
  types_add_type (N_TYPE, sizeof (unsigned) * 8, NULL, NULL);
  types_add_type (R_TYPE, sizeof (double) * 8, NULL, NULL);
  types_add_type (Z_TYPE, sizeof (int) * 8, NULL, NULL); 
}

/* Generate data for hash.  */
UT_array* 
gen_hash_data (enum tree_code code, size_t size, tree dim, tree shape)
{
  UT_array *hash_data;
  utarray_new (hash_data, &ut_int_icd);
  utarray_push_back (hash_data, &code);
  utarray_push_back (hash_data, &size);
  tree_get_hash_data (dim, hash_data);
  tree_get_hash_data (shape, hash_data);
  return hash_data;
}

/* Add a new type to hash table.  */
struct tree_type_node *
types_add_type (enum tree_code code, size_t size, tree dim, tree shape)
{
  tree el = NULL;
  UT_array* hash_data = gen_hash_data (code, size, dim, shape);

  assert (TREE_CODE_CLASS (code) == tcl_type, "code class has to be a type");
  el = make_type (code);
  TYPE_SIZE (el) = size;
  TYPE_DIM (el) = dim;
  TYPE_SHAPE (el) = shape;

  HASH_ADD_KEYPTR (hh, type_table, utarray_eltptr (hash_data, 0), 
    utarray_len (hash_data) * sizeof (int), &TYPE_HASH (el));
  utarray_free (hash_data);
  return ((struct tree_type_node *) el);
}

struct tree_type_node * 
types_find_in_table (enum tree_code code, size_t size, tree dim, tree shape)
{
  tree el = NULL;
  struct tree_type_node* ret = NULL;
  UT_array* hash_data = gen_hash_data (code, size, dim, shape);

  assert (TREE_CODE_CLASS (code) == tcl_type, "code class has to be a type");
  el = make_type (code);
  TYPE_SIZE (el) = size;
  TYPE_DIM (el) = dim;
  TYPE_SHAPE (el) = shape;
  
  HASH_FIND (hh, type_table, utarray_eltptr (hash_data, 0), 
    utarray_len (hash_data) * sizeof (int), ret);
  utarray_free (hash_data);
  return ret;
}

/* Try to find a corresponded type, if not found -- add a new one.
   FIXME: Now works properly only when dim is NULL and shape is NULL. Need to
   compare trees.  */
tree
types_assign_type (enum tree_code code, size_t size, tree dim, tree shape)
{
  struct tree_type_node * found = 
      types_find_in_table (code, size, dim, shape);
  if (found == NULL)
    return ((tree) types_add_type (code, size, dim, shape));
  else
    return ((tree) types_find_in_table (code, size, dim, shape));
}

/* We call free function for trees, as tree is a tree node too.  */
void
types_free_type (struct tree_type_node * type)
{
  free_tree ((tree) type);
}

void types_finalize ()
{
  struct tree_type_node *current, *tmp;
  HASH_ITER (hh, type_table, current, tmp)
    {
      HASH_DEL (type_table, current);
      free_tree_type ((tree) current);
    }
  type_table = NULL;
}

