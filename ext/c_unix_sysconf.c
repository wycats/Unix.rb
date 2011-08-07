#include "c_unix_sysconf.h"

/**
 * +sysconf+, +pathconf+ and +fpathconf+ have a fair bit of shared setup
 * and argument checking. This method performs the setup in common.
 *
 * This method takes a char * for the name of the constant containing
 * the Hash of symbolic names to values, and the name passed into the
 * user-facing method.
 *
 * That name can be a Symbol or a Fixnum. If it's a Symbol, this method
 * will ensure that it's in the name Hash. If it's not, it will raise
 * an Errno::EINVAL, the same error that would be raised if an invalid
 * integer was passed to the syscall.
 */
static VALUE rb_unix_prepare_conf(const char *conf_name, VALUE symbolic_name) {
  VALUE rb_mUnix = rb_const_get(rb_cObject, rb_intern("Unix"));
  VALUE unix_macros = rb_const_get(rb_mUnix, rb_intern(conf_name));
	VALUE int_name;

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
    rb_raise(rb_eTypeError, "Invalid name: %s. Expected Symbol or Fixnum, got %s",
        RSTRING_PTR(rb_inspect(symbolic_name)),
        rb_obj_classname(symbolic_name));
  }

	errno = 0;
	return int_name;
}

/**
 * +sysconf+, +pathconf+ and +fpathconf+ similarly have shared return
 * semantics. This method will check to make sure the return value is
 * not an error. If it's an error, it will raise the appropriate
 * Errno error.
 *
 * Otherwise, it will return the value returned by the underlying
 * syscall.
 */
static VALUE rb_unix_pathconf_return(long value, char *mesg) {
	// if errno is zero, a sysconf value of -1 might not be an error. For example,
	// a -1 return for _SC_ATEXIT_MAX means "infinite"
	if (value == -1 && errno != 0) {
		rb_sys_fail(mesg);
		return Qnil;
	} else {
		return INT2NUM(value);
	}
}

/**
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
	VALUE name = rb_unix_prepare_conf("SYSCONF_NAMES", symbolic_name);

	long value = sysconf(NUM2INT(name));
	return rb_unix_pathconf_return(value, (char *)"sysconf");
}

/**
 * call-seq:
 * 		Unix.pathconf(path, options)    -> Fixnum
 *
 * This method alows you to get the value of a POSIX path configuration
 * option for a given path.
 *
 * The +option+ parameter is the same as the parameter passed to
 * +Unix.sysconf+, except that its acceptable values are defined by
 * the options defined in SUS3 with a leading +_PC_+ and listed in
 * +Unix::PATHCONF_NAMES+.
 */
static VALUE rb_pathconf(VALUE self, VALUE passed_path, VALUE symbolic_name) {
	VALUE name = rb_unix_prepare_conf("PATHCONF_NAMES", symbolic_name);

	// If the passed value cannot be coerced into a String with to_str,
	// this will raise an exception.
	VALUE path = rb_convert_type(passed_path, T_STRING, "String", "to_str");

	long value = pathconf(RSTRING_PTR(path), NUM2INT(name));
	return rb_unix_pathconf_return(value, RSTRING_PTR(path));
}

/**
 * call-seq
 * 		Unix.fpathconf(file, options)    -> Fixnum
 *
 * This method allows you to get the value of a POSIX path configuration
 * option for an open file descriptor represented by a File object.
 *
 * According to SUS3, fpathconf's behavior for non-files is undefined,
 * so this method is limited to File objects.
 *
 * Otherwise, it behaves identically to +Unix.pathconf+.
 */
static VALUE rb_fpathconf(VALUE self, VALUE passed_io, VALUE symbolic_name) {
	VALUE name = rb_unix_prepare_conf("PATHCONF_NAMES", symbolic_name);

	// Convert the passed in IO via to_io.
	VALUE io = rb_convert_type(passed_io, T_FILE, "IO", "to_io");

	// This may return an IO, rather than a File. Since pathconf expects
	// a file descriptor from a file, let's be explicit here.
	if (!rb_obj_is_kind_of(io, rb_cFile)) {
    rb_raise(rb_eTypeError, "Invalid file: %s. Expected File, got %s",
        RSTRING_PTR(rb_inspect(io)),
        rb_obj_classname(io));
	}

	// The IO interface requires us to make Ruby method calls to get at
	// the file descriptors and path from the object.
	int fd = NUM2INT(rb_funcall(io, rb_intern("fileno"), 0));
	char *path = RSTRING_PTR(rb_funcall(io, rb_intern("to_path"), 0));

	long value = fpathconf(fd, NUM2INT(name));
	return rb_unix_pathconf_return(value, path);
}

void init_c_unix_sysconf(VALUE unix_module) {
	init_c_sysconf_macros(unix_module);
	rb_define_singleton_method(unix_module, "sysconf", rb_sysconf, 1);
	rb_define_singleton_method(unix_module, "pathconf", rb_pathconf, 2);
	rb_define_singleton_method(unix_module, "fpathconf", rb_fpathconf, 2);
}
