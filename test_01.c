#include <turglem/lemmatizer.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <turglem/russian/charset_adapters.h>
#include <MAFSA/charset_adapter.h>

#define my_s2l LEM_RUSSIAN_conv_string_to_letters_utf8
#define my_l2s LEM_RUSSIAN_conv_letters_to_string_utf8

void report (const char *s, turglem lem, int *lem_res, size_t i, size_t j)
{
	/*printf("%s.\tparadigm/form_num = %d/%d\n", s, lem_res[i*2], lem_res[i*2 + 1]);*/
	printf
	(
		"%s %u %llx%s\n",
		s,
		turglem_paradigms_get_part_of_speech(lem->paradigms, lem_res[i*2], j),
		turglem_paradigms_get_grammem(lem->paradigms, lem_res[i*2], j),
		lem_res[i*2 + 1] == j ? " *" : ""
	);
}

void do_magic (turglem lem, const char *s)
{
	MAFSA_letter l[32];
	ssize_t ssz_src;
	size_t sz_lem_res;
	size_t i;
	size_t sz;
	int lem_res[32 * 2];
	
	ssz_src = my_s2l(s, l, 32);
	if (ssz_src < 0)
	{
		return;
	}
	
	sz_lem_res = turglem_lemmatize(lem, l, ssz_src, lem_res, 32, RUSSIAN_LETTER_DELIM, 1);
	
	for (i = 0; i < sz_lem_res; i++)
	{
		MAFSA_letter out_letters[32];
		char buf[64];
		size_t j;
		size_t form_cnt = turglem_paradigms_get_form_count(lem->paradigms, lem_res[i*2]);
		/*printf("%lu.\tparadigm/form_num = %d/%d\n", (unsigned long) i, lem_res[i*2], lem_res[i*2 + 1]);
		printf("\tpart-of-speach: %u grammems: %016llX\n",
			turglem_paradigms_get_part_of_speech(lem->paradigms, lem_res[i*2], 0),
			turglem_paradigms_get_grammem(lem->paradigms, lem_res[i*2], 0));*/
		
		/* report(s, lem, lem_res, i, 0); */
		
		for (j = 0; j < form_cnt; j++)
		{
			sz = turglem_build_form(lem, l, ssz_src, out_letters, 32, lem_res[i * 2], lem_res[i * 2 + 1], j);
			sz = my_l2s(out_letters, sz, buf, 64);
			report(buf, lem, lem_res, i, j);
		};
		
		printf("\n");
	}
}

int main (int argc, char **argv)
{
	turglem lem;
	int err_no;
	int err_what;
	
	if (argc != 2)
	{
		printf("usage: %s utf8-russian-word\n", argv[0]);
		return -1;
	}
	
	lem = turglem_load
	(
		"/usr/local/share/turglem/russian/dict_russian.auto",
		"/usr/local/share/turglem/russian/prediction_russian.auto",
		"/usr/local/share/turglem/russian/paradigms_russian.bin",
		&err_no, &err_what
	);
	
	if (0 == lem)
	{
		printf
		(
			"FAILED: err_no/err_what = %d/%d\n (error loading %s: %s: %s)\n",
			err_what, err_no,
			turglem_error_what_string(err_what),
			turglem_error_no_string(err_no),
			strerror(errno)
		);
		return -1;
	}
	
	do_magic(lem, argv[1]);
	
	turglem_close(lem);
	return 0;
}