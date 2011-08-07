#include "c_unix_sysconf.h"

static VALUE rb_sysconf(VALUE self, VALUE symbolic_name) {
  VALUE int_name;
	long value;
  int name;
  VALUE unix_macros = rb_const_get(rb_cObject, rb_intern("UNIX_MACROS"));
  VALUE unix_errnos = rb_const_get(rb_cObject, rb_intern("UNIX_ERRNOS"));
	errno = 0;

	if (TYPE(symbolic_name) == T_SYMBOL) {
    int_name = rb_hash_aref(unix_macros, symbolic_name);
	} else {
    int_name = symbolic_name;
  }

  name = NUM2INT(int_name);

	if ((value = sysconf(name)) == -1) {
    if (errno == 0) {
      return INT2NUM(value);
    } else {
      return rb_hash_aref(unix_errnos, INT2NUM(errno));
    }
  } else {
    return INT2NUM(value);
  }
}

void init_c_unix_sysconf(VALUE unix_module) {
  rb_define_singleton_method(unix_module, "sysconf", rb_sysconf, 1);
}
