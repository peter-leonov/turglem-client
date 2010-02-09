#include <turglem/lemmatizer.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <turglem/russian/charset_adapters.h>
#include <MAFSA/charset_adapter.h>

MAFSA_conv_string_to_letters my_s2l = 0;
MAFSA_conv_letters_to_string my_l2s = 0;

void do_magic(turglem lem, const char *s, int all_forms)
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
		printf("can't convert word <%s> to letters! Is it russian word in UTF-8?\n", s);
		return;
	}

	sz_lem_res = turglem_lemmatize(lem, l, ssz_src, lem_res, 32, RUSSIAN_LETTER_DELIM, 1);
	printf("lemmatized word <%s>: %u total variants.\n", s, sz_lem_res);

	for (i = 0; i < sz_lem_res; i++)
	{
		MAFSA_letter out_letters[32];
		char buf[64];
		size_t j;
		size_t form_cnt = turglem_paradigms_get_form_count(lem->paradigms, lem_res[i*2]);
		printf("%u.\tparadigm/form_num = %d/%d\n", i, lem_res[i*2], lem_res[i*2 + 1]);
		printf("\tparf-of_speach: %u grammems: %016llX\n",
		  turglem_paradigms_get_part_of_speech(lem->paradigms, lem_res[i*2], 0),
		  turglem_paradigms_get_grammem(lem->paradigms, lem_res[i*2], 0));
		for (j = 0; j < form_cnt; j++)
		{
			if (!all_forms && j) break;
			sz = turglem_build_form(lem, l, ssz_src, out_letters, 32, lem_res[i * 2], lem_res[i * 2 + 1], j);
			sz = my_l2s(out_letters, sz, buf, 64);
			printf("\t(%02u) %s%s\n", j, buf, lem_res[i*2 + 1] == j ? " (*)" : "");
		};
	}
}

void show_demo(turglem lem, int all_forms)
{
	printf("lemmatizer loaded, going to show examples :)\n");

	printf("general lemmatization:\n");
	do_magic(lem, "ПОДУШКАМИ", all_forms);
	do_magic(lem, "ОКНА", all_forms);
	do_magic(lem, "ПЕРЕД", all_forms);

	printf("prediction (by throwing first n letter)\n");
	do_magic(lem, "СУПЕРРРРВКУСНЕНЬКИХ", all_forms);

	printf("prediction (by maximal siffix)\n");
	do_magic(lem, "АФФТАРСКОГО", all_forms);

	printf("free memory used by lemmatizer\n");
}

void show_stdin_demo(turglem lem)
{
	char buf[1024];
	char *nl_rl;

	printf("write words in Russian, one word per line :)\n");
	while (!feof(stdin))
	{
		if (fgets(buf, 1024, stdin) == 0) continue;

		nl_rl = strchr(buf, '\n');
		if (nl_rl) nl_rl[0] = 0;
		nl_rl = strchr(buf, '\r');
		if (nl_rl) nl_rl[0] = 0;

		do_magic(lem, buf, 1);
	}
}

int main(int argc, char **argv)
{
	turglem lem;
	int err_no;
	int err_what;

	if (argc != 2)
	{
		printf("usage: %s <DEMO | DEMO_FULL | STDIN_UTF8 | STDIN_KOI8 | STDIN_CP1251>\n", argv[0]);
		return -1;
	}

	printf("test_01: we try to load russian dictionaries from /usr/local/share/turglem/russian\n");
	lem = turglem_load("/usr/local/share/turglem/russian/dict_russian.auto",
			   "/usr/local/share/turglem/russian/prediction_russian.auto",
			   "/usr/local/share/turglem/russian/paradigms_russian.bin",
			   &err_no, &err_what);
	if (0 == lem)
	{
		printf("FAILED: err_no/err_what = %d/%d\n (error loading %s: %s: %s)\n",
			err_what, err_no,
			turglem_error_what_string(err_what),
			turglem_error_no_string(err_no),
			strerror(errno));
		return -1;
	}

	my_s2l = LEM_RUSSIAN_conv_string_to_letters_utf8;
	my_l2s = LEM_RUSSIAN_conv_letters_to_string_utf8;
	if (strcmp(argv[1], "DEMO") == 0)
	{
		show_demo(lem, 0);
	}
	else
	if (strcmp(argv[1], "DEMO_FULL") == 0)
	{
		show_demo(lem, 1);
	}
	else
	if (strcmp(argv[1], "STDIN_UTF8") == 0)
	{
		show_stdin_demo(lem);
	}
	else
	if (strcmp(argv[1], "STDIN_CP1251") == 0)
	{
		my_s2l = LEM_RUSSIAN_conv_string_to_letters_cp1251;
		my_l2s = LEM_RUSSIAN_conv_letters_to_string_cp1251;
		show_stdin_demo(lem);
	}
	else
	if (strcmp(argv[1], "STDIN_KOI8") == 0)
	{
		my_s2l = LEM_RUSSIAN_conv_string_to_letters_koi8r;
		my_l2s = LEM_RUSSIAN_conv_letters_to_string_koi8r;
		show_stdin_demo(lem);
	}

	turglem_close(lem);
	return 0;
}

