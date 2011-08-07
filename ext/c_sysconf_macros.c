#include "c_sysconf_macros.h"

static struct {
	const char *name;
	intptr_t value;
} sysconf_names[] = {
	{ "ARG_MAX",  					(intptr_t)_SC_ARG_MAX },
	{ "CHILD_MAX", 					(intptr_t)_SC_CHILD_MAX },
	{ "CLK_TCK",  					(intptr_t)_SC_CLK_TCK },
	{ "IOV_MAX",     				(intptr_t)_SC_IOV_MAX },
	{ "NGROUPS_MAX", 				(intptr_t)_SC_NGROUPS_MAX },
	{ "NPROCESSORS_CONF", 	(intptr_t)_SC_NPROCESSORS_CONF },
	{ "NPROCESSORS_ONLN", 	(intptr_t)_SC_NPROCESSORS_ONLN },
	{ "OPEN_MAX", 					(intptr_t)_SC_OPEN_MAX },
	{ "PAGESIZE", 					(intptr_t)_SC_PAGESIZE },
	{ "STREAM_MAX", 				(intptr_t)_SC_STREAM_MAX },
	{ "TZNAME_MAX", 				(intptr_t)_SC_TZNAME_MAX },
	{ "JOB_CONTROL", 				(intptr_t)_SC_JOB_CONTROL },
	{ "SAVED_IDS", 					(intptr_t)_SC_SAVED_IDS },
	{ "VERSION", 						(intptr_t)_SC_VERSION },
	{ "BC_BASE_MAX", 				(intptr_t)_SC_BC_BASE_MAX },
	{ "BC_DIM_MAX", 				(intptr_t)_SC_BC_DIM_MAX },
	{ "BC_SCALE_MAX",   		(intptr_t)_SC_BC_SCALE_MAX },
  { "BC_STRING_MAX", 			(intptr_t)_SC_BC_STRING_MAX },
	{ "COLL_WEIGHTS_MAX", 	(intptr_t)_SC_COLL_WEIGHTS_MAX },
	{ "EXPR_NEST_MAX",  		(intptr_t)_SC_EXPR_NEST_MAX },
	{ "LINE_MAX", 					(intptr_t)_SC_LINE_MAX },
	{ "RE_DUP_MAX", 				(intptr_t)_SC_RE_DUP_MAX },
	{ "2_VERSION", 					(intptr_t)_SC_2_VERSION },
	{ "2_C_BIND", 					(intptr_t)_SC_2_C_BIND },
	{ "2_C_DEV", 						(intptr_t)_SC_2_C_DEV },
	{ "2_CHAR_TERM",   			(intptr_t)_SC_2_CHAR_TERM },
	{ "2_FORT_DEV", 				(intptr_t)_SC_2_FORT_DEV },
	{ "2_FORT_RUN", 				(intptr_t)_SC_2_FORT_RUN },
	{ "2_LOCALEDEF", 				(intptr_t)_SC_2_LOCALEDEF },
	{ "2_SW_DEV", 					(intptr_t)_SC_2_SW_DEV },
	{ "2_UPE", 							(intptr_t)_SC_2_UPE },
#ifdef _SC_PHYS_PAGES
	{ "PHYS_PAGES", 				_SC_PHYS_PAGES },
#endif
	{ NULL, 0 }
};

void init_c_sysconf_macros(VALUE unix_module) {
	VALUE symbol_hash = rb_hash_new();
	VALUE int_hash = rb_hash_new();
	VALUE name;
	VALUE value;

	int index;

	for (index = 0; sysconf_names[index].name != NULL; index++) {
		name = ID2SYM(rb_intern((sysconf_names[index].name)));
		value = INT2NUM(sysconf_names[index].value);

		rb_hash_aset(symbol_hash, name, value);
		rb_hash_aset(int_hash, value, name);
	}

	rb_define_const(unix_module, "SYSCONF_NAMES", symbol_hash);
	rb_define_const(unix_module, "SYSCONF_VALUES", int_hash);
}
