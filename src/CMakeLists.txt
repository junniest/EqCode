add_definitions (${CFLAGS})
# EQ library files
set (eqlib_src
global.c  lex.c  
parser.c  print.c  
tree.c matcher.c)
add_library (eqlib ${eqlib_src})

