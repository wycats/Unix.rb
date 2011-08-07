#include "c_sysconf_macros.h"

static struct macro_names {
	const char *name;
	intptr_t value;
};

struct macro_names sysconf_names[] = {
#include "gen/sysconf.inc"
	{ NULL, 0 }
};

struct macro_names pathconf_names[] = {
#include "gen/pathconf.inc"
	{ NULL, 0 }
};

static void rb_setup_macros(VALUE module, struct macro_names names[], const char *name_hash, const char *value_hash) {
	VALUE symbol_hash = rb_hash_new();
	VALUE int_hash = rb_hash_new();
	VALUE name;
	VALUE value;

	int index;
	intptr_t raw_value;

	for (index = 0; names[index].name != NULL; index++) {
		name = ID2SYM(rb_intern((names[index].name)));
		raw_value = names[index].value;

		if (raw_value == 0) {
			value = ID2SYM(rb_intern("undefined"));
		} else {
			value = INT2NUM(raw_value);
		}

		rb_hash_aset(symbol_hash, name, value);
		rb_hash_aset(int_hash, value, name);
	}

	rb_define_const(module, name_hash, symbol_hash);
	rb_define_const(module, value_hash, int_hash);
}

void init_c_sysconf_macros(VALUE unix_module) {
	rb_setup_macros(unix_module, sysconf_names, "SYSCONF_NAMES", "SYSCONF_VALUES");
	rb_setup_macros(unix_module, pathconf_names, "PATHCONF_NAMES", "PATHCONF_VALUES");
}
