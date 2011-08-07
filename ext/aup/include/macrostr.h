#ifndef _MACROSTR_H_
#define _MACROSTR_H_

const char *get_macrostr(const char *cat, int code, const char **desc);
int aup_macrodb_len();
const char *aup_macrodb_cat(int index);
long aup_macrodb_code(int index);
const char *aup_macrodb_macro(int index);
const char *aup_macrodb_desc(int index);

#endif /* _MACROSTR_H_ */

