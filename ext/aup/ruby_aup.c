#include <ruby.h>
#include "macrostr.h"

void Init_ruby_aup() {
  int macro_len = aup_macrodb_len();
  int index = 0;
  const char *type;
  VALUE macro;
  VALUE code;

  VALUE sym_hash = rb_hash_new();
  VALUE errno_hash = rb_hash_new();

  for (index = 0; index < macro_len; index++) {
    macro = ID2SYM(rb_intern(aup_macrodb_macro(index)));
    code = INT2NUM(aup_macrodb_code(index));
    type = aup_macrodb_cat(index);

    rb_hash_aset(sym_hash, macro, code);

    if (type == "errno") {
      rb_hash_aset(errno_hash, code, macro);
    }
  }

  rb_define_const(rb_cObject, "UNIX_MACROS", sym_hash);
  rb_define_const(rb_cObject, "UNIX_ERRNOS", errno_hash);
}
