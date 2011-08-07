#include "c_sysconf_macros.h"

static struct {
	const char *name;
	intptr_t value;
} sysconf_names[] = {
#include "gen/sysconf.inc"
	{ NULL, 0 }
};

void init_c_sysconf_macros(VALUE unix_module) {
	VALUE symbol_hash = rb_hash_new();
	VALUE int_hash = rb_hash_new();
	VALUE name;
	VALUE value;

	int index;
	intptr_t raw_value;

	for (index = 0; sysconf_names[index].name != NULL; index++) {
		name = ID2SYM(rb_intern((sysconf_names[index].name)));
		raw_value = sysconf_names[index].value;

		if (raw_value == 0) {
			value = ID2SYM(rb_intern("undefined"));
		} else {
			value = INT2NUM(raw_value);
		}

		rb_hash_aset(symbol_hash, name, value);
		rb_hash_aset(int_hash, value, name);
	}

	rb_define_const(unix_module, "SYSCONF_NAMES", symbol_hash);
	rb_define_const(unix_module, "SYSCONF_VALUES", int_hash);
}
