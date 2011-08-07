#include "c_unix_sysconf.h"

/*
 * call-seq:
 *    Unix.sysconf(option)     -> Fixnum
 *
 * This method allows you to get the value of a POSIX configuration option 
 *
 * +option+ can be a Symbol or a Fixnum. If option is a Symbol, it must be
 * one of the options defined in SUS3 without the leading +_SC_+.
 *
 * Example:
 *
 *    Unix.sysconf(:PROCESSORS_CONF) -> 2
 *
 * You can get a list of available symbols by inspecting the keys in
 * +Unix::SYSCONF_NAMES+  That constant is populated at compile time
 * from the SUS3 values.
 *
 * If you know the integer value of a particular option for your system
 * that is not defined by POSIX.1, you can pass it in directly. If your
 * system does not recognize the number, +Unix.sysconf+ will raise
 * a +Errno::EINVAL+.
 *
 * If the call results in another value of errno being set, the
 * corresponding +Errno+ will be raised. If you pass in a Symbol
 * not found in +Unix::SYSCONF_NAMES+, an +Errno::EINVAL+ will be
 * raised.
 */
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
      rb_raise(rb_const_get(rb_mErrno, rb_intern("EINVAL")),
          "Invalid sysconf name: %s",
          rb_id2name(SYM2ID(symbolic_name)));
    }
	} else if (TYPE(symbolic_name) == T_FIXNUM) {
    int_name = symbolic_name;
  } else {
    rb_raise(rb_eArgError, "Invalid argument: %s. Expected Symbol or Fixnum, got %s",
        RSTRING_PTR(rb_inspect(symbolic_name)),
        rb_obj_classname(symbolic_name));
  }

  name = NUM2INT(int_name);

	// if errno is zero, a sysconf value of -1 might not be an error. For example,
	// a -1 return for _SC_ATEXIT_MAX means "infinite"
	if ((value = sysconf(name)) == -1 && errno != 0) {
    VALUE error_symbol = rb_hash_aref(unix_errnos, INT2NUM(errno));
    rb_sys_fail("sysconf");
  } else {
    return INT2NUM(value);
  }
}

void init_c_unix_sysconf(VALUE unix_module) {
  init_c_sysconf_macros(unix_module);
  rb_define_singleton_method(unix_module, "sysconf", rb_sysconf, 1);
}
