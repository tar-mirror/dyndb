#ifndef BAILOUT_H
#define BAILOUT_H

#ifdef __GNUC__
#if __GNUC__ > 2 || (__GNUC__ ==2 && __GNUC_MINOR__ >=5)
#define BAILOUT_NORETURN __attribute__((noreturn))
#endif
#endif
#ifndef BAILOUT_NORETURN
#define BAILOUT_NORETURN
#endif

extern const char *flag_bailout_log_name;
extern int flag_bailout_log_pid;
extern int flag_bailout_fatal_begin;

void warning(int erno, const char *s1, const char *s2, const char *s3,
        const char *s4);
void bailout(int erno, const char *s1, const char *s2, const char *s3,
        const char *s4) BAILOUT_NORETURN;
void oom(void) BAILOUT_NORETURN;
void xbailout(int ecode, int erno, const char *s1, const char *s2, 
		const char *s3, const char *s4) BAILOUT_NORETURN;
void bailout_progname(const char *keep_this_string_in_place);

#undef BAILOUT_NORETURN
#endif
