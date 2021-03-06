#include "expand.h"
#include "global.h"
#include "parser.h"
#include "config.h"

void usage (const char *);

void
usage (const char *  prog)
{
  fprintf (stderr, "%s:%s (revision: %s) <input-file>\n",
	   prog, VERSION, COMMIT_DATE);
  return;
}

int
main (int argc, char *argv[])
{
  int ret = 0;
  struct lexer *lex = (struct lexer *) malloc (sizeof (struct lexer));
  struct parser *parser = (struct parser *) malloc (sizeof (struct parser));

  init_global ();
  init_global_tree ();
  
  if (argc <= 1)
    {
      fprintf (stderr, "%s:%s filename argument required\n", argv[0], VERSION);
      usage (argv[0]);
      ret = -1;
      goto cleanup;
    }

  if (!lexer_init (lex, argv[1]))
    {
      fprintf (stderr, "%s:%s cannot create a lexer for file %s\n", 
	       argv[1], argv[0], VERSION);
      ret = -2;
      goto cleanup;
    }

  parser_init (parser, lex);
  parse (parser);
cleanup:
  parser_finalize (parser);
  finalize_global_tree ();
  finalize_global ();

  /* That should be called at the very end.  */
  free_atomic_trees ();

  if (parser)
    free (parser);
  if (lex)
    free (lex);

  return ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
