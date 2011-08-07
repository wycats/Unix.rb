#include "c_unix_sysconf.h"

static VALUE rb_sysconf(VALUE self, VALUE symbolic_name) {
  VALUE rb_mUnix = rb_const_get(rb_cObject, rb_intern("Unix"));
  VALUE unix_macros = rb_const_get(rb_mUnix, rb_intern("SYSCONF_NAMES"));
  VALUE unix_errnos = rb_const_get(rb_cObject, rb_intern("UNIX_ERRNOS"));
  VALUE int_name;

	long value;
  int name;

	errno = 0;

	if (TYPE(symbolic_name) == T_SYMBOL) {
    int_name = rb_hash_aref(unix_macros, symbolic_name);
    if (int_name == Qnil) {
      rb_raise(rb_const_get(rb_mErrno, rb_intern("EINVAL")), "Invalid sysconf name: %s", rb_id2name(SYM2ID(symbolic_name)));
    }
	} else if (TYPE(symbolic_name) == T_FIXNUM) {
    int_name = symbolic_name;
  } else {
    rb_raise(rb_eArgError, "Invalid argument: %s. Expected Symbol or Integer, got %s",
        RSTRING_PTR(rb_inspect(symbolic_name)),
        rb_obj_classname(symbolic_name));
  }

  name = NUM2INT(int_name);

	if ((value = sysconf(name)) == -1) {
    if (errno == 0) {
      return INT2NUM(value);
    } else {
      VALUE error_symbol = rb_hash_aref(unix_errnos, INT2NUM(errno));
      rb_sys_fail("sysconf");
    }
  } else {
    return INT2NUM(value);
  }
}

void init_c_unix_sysconf(VALUE unix_module) {
  init_c_sysconf_macros(unix_module);
  rb_define_singleton_method(unix_module, "sysconf", rb_sysconf, 1);
}
