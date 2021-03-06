#ifndef __PARSER_H__
#define __PARSER_H__

#include "expand.h"

struct parser
{
  struct lexer *lex;

  /* Buffer and lengths associated with buffer.
     Buffer holds up-to BUF_SIZE tokens, which means
     that it is possible to look BUF_SIZE tokens
     forward.  */
  struct token **token_buffer;
  size_t buf_size;
  size_t buf_start, buf_end, unget_idx;
  bool buf_empty;

  /* Count of opened parens, square brackets and
     figure brackets. Used when we skip the tokens
     skip is finished when all the three counters
     are zeroes.  */
  int paren_count;
  int square_count;
  int brace_count;
  /* If we are inside \match, we allowed to use \expr macros there,
     otherwise we are to ban it  */
  bool match_expr_allowed;
};


__BEGIN_DECLS

#define TOKEN_CLASS(a, b) \
static inline bool \
token_is_ ## a (struct token *  tok, enum token_kind tkind) \
{ \
  return token_class (tok) == tok_ ## a && token_value (tok) == tkind; \
}
#include "token_class.def"
#undef TOKEN_CLASS

int parse (struct parser *);
bool parser_init (struct parser *, struct lexer *);
bool parser_finalize (struct parser *);
/* FIXME These functions should be static at some point.  */
struct token *parser_get_token (struct parser *);
void parser_unget (struct parser *);

struct token *parser_get_until_one_of_val (struct parser *, int, ...);
struct token *parser_get_until_tval (struct parser *, enum token_kind);
struct token *parser_get_until_tclass (struct parser *, enum token_class);
struct token *parser_forward_tval (struct parser *, enum token_kind);
struct token *parser_forward_tclass (struct parser *, enum token_class);
struct token *parser_token_alternative_tval (struct parser *, enum token_kind,
					    enum token_kind);
struct token *parser_token_alternative_tclass (struct parser *,
					      enum token_class,
					      enum token_class);
bool parser_expect_tval (struct parser *, enum token_kind);
bool parser_expect_tclass (struct parser *, enum token_class);
tree handle_type (struct parser *);
tree handle_ext_type (struct parser *);
tree handle_list (struct parser *, tree (*)(struct parser *),
		  enum token_kind);
tree handle_ext_type_or_ext_type_list (struct parser *);
tree handle_sexpr_or_sexpr_list (struct parser *);
tree handle_id (struct parser *);
tree handle_indexes (struct parser *, tree);
tree handle_idx (struct parser *);
tree handle_idx_or_idx_list (struct parser *);
tree handle_lower (struct parser *);
tree handle_upper (struct parser *);
tree handle_function (struct parser *);
tree handle_function_call (struct parser *);
tree handle_linear (struct parser *);
tree handle_divide (struct parser *);
tree handle_sexpr (struct parser *);
tree handle_sexpr_op (struct parser *);
tree handle_condition (struct parser *);
tree handle_filter (struct parser *);
tree handle_matrix (struct parser *);
tree handle_vector (struct parser *);
tree handle_genarray (struct parser *);
tree handle_expr (struct parser *);
tree handle_return (struct parser *);
tree handle_assign (struct parser *, tree);
tree handle_declare (struct parser *, tree);
tree handle_instr (struct parser *);
tree handle_with_loop (struct parser *, tree);
tree handle_with_loop_cases (struct parser *);
tree handle_numx (struct parser *);
tree handle_idx_numx (struct parser *);
bool handle_match (struct parser *);


#define PARSER_MATCH_EXPR_ALLOWED(parser) ((parser)->match_expr_allowed)
__END_DECLS

#endif /* __PARSER_H__  */
