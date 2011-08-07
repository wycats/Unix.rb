#include <c_unix.h>

void Init_c_unix() {
	VALUE rb_mUnix = rb_define_module("Unix");
	VALUE xopen_version_name;
	VALUE xopen_spec_name;
	VALUE posix_version_name;
	VALUE posix_spec_name;

	/**
	 * These are the values we asked for
	 */

	#ifdef _POSIX_SOURCE
		VALUE source = Qtrue;
		VALUE c_source = LONG2FIX((long)_POSIX_C_SOURCE);
	#else
		VALUE source = Qfalse;
		VALUE c_source = Qnil;
	#endif

	rb_define_const(rb_mUnix, "POSIX_SOURCE", source);
	rb_define_const(rb_mUnix, "POSIX_C_SOURCE", c_source);

	#ifdef _XOPEN_SOURCE
		#if _XOPEN_SOURCE +0 == 0
			VALUE xopen_source = Qtrue;
		#else
			VALUE xopen_source = INT2NUM(_XOPEN_SOURCE);
		#endif
	#else
		VALUE xopen_source = Qnil;
	#endif

	rb_define_const(rb_mUnix, "XOPEN_SOURCE", xopen_source);

	#ifdef _XOPEN_SOURCE_EXTENDED
		VALUE xopen_source_extended = Qtrue;
	#else
		VALUE xopen_source_extended = Qfalse;
	#endif

	rb_define_const(rb_mUnix, "XOPEN_SOURCE_EXTENDED", xopen_source_extended);

	/**
	 * Introspection values
	 */

	#ifdef _POSIX_VERSION
		VALUE posix_version = INT2NUM(_POSIX_VERSION);

		if (_POSIX_VERSION == 200112) {
			posix_version_name = rb_str_new2("POSIX.1-2001");
			posix_spec_name = rb_str_new2("IEEE 1003.1-2001");
		} else if (_POSIX_VERSION == 199506) {
			posix_version_name = rb_str_new2("POSIX.1-1996");
			posix_spec_name = rb_str_new2("IEEE 1003.1-1996");
		} else if (_POSIX_VERSION == 199309) {
			posix_version_name = rb_str_new2("POSIX.1b-1993");
			posix_spec_name = rb_str_new2("IEEE 1003.1b-1993");
		} else if (_POSIX_VERSION == 199009) {
			posix_version_name = rb_str_new2("POSIX.1-1990");
			posix_spec_name = rb_str_new2("ISO/IEC 9945-1:1990");
		} else if (_POSIX_VERSION == 198808) {
			posix_version_name = rb_str_new2("POSIX.1-1988");
			posix_spec_name = Qnil;
		}
	#else
		#ifdef _POSIX2_C_VERSION
			VALUE posix_version = INT2NUM(_POSIX2_C_VERSION);
			posix_version_name = rb_str_new2("POSIX.2");
			posix_spec_name = rb_str_new2("ISO/IEC 9945-2:1993");
		#else
			VALUE posix_version = Qfalse;
			posix_version_name = Qnil;
		#endif
	#endif

	rb_define_const(rb_mUnix, "POSIX_VERSION", posix_version);
	rb_define_const(rb_mUnix, "POSIX_VERSION_NAME", posix_version_name);
	rb_define_const(rb_mUnix, "POSIX_SPEC_NAME", posix_spec_name);

	#ifdef _XOPEN_UNIX
		VALUE xopen_unix = Qtrue;
	#else
		VALUE xopen_unix = Qfalse;
	#endif

	#ifdef _XOPEN_VERSION
		VALUE xopen_version = INT2NUM(_XOPEN_VERSION);

		if (_XOPEN_VERSION == 3) {
			xopen_version_name = rb_str_new2("XPG3");
		} else if (_XOPEN_VERSION == 4) {
			#ifdef _XOPEN_UNIX
				xopen_version_name = rb_str_new2("SUS");
			#else
				xopen_version_name = rb_str_new2("XPG4");
			#endif
		} else if (_XOPEN_VERSION == 500) {
			xopen_version_name = rb_str_new2("SUS2");
		} else if (_XOPEN_VERSION == 600) {
			xopen_version_name = rb_str_new2("SUS3");
		} else {
			xopen_version_name = Qnil;
		}
	#else
		VALUE xopen_version = Qnil;
		xopen_version_name = Qnil;
	#endif

	rb_define_const(rb_mUnix, "XOPEN_UNIX", xopen_unix);
	rb_define_const(rb_mUnix, "XOPEN_VERSION", xopen_version);
	rb_define_const(rb_mUnix, "XOPEN_VERSION_NAME", xopen_version_name);

	init_c_unix_date(rb_mUnix);
	init_c_unix_sysconf(rb_mUnix);
}
