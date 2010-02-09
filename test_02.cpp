#include <turglem/lemmatizer.hpp>
#include <turglem/russian/charset_adapters.hpp>

#include <stdio.h>

void do_cpp_magic(const tl::lemmatizer &lem, const char *s)
{
	tl::lem_result lr;
	size_t sz_lem = lem.lemmatize<russian_utf8_adapter>(s, lr);
	
	if (sz_lem)
	{
		
		for (size_t i = 0; i < sz_lem; i++)
		{
			printf("%d/%d:", lem.get_paradigm(lr, i), lem.get_src_form(lr, i));
			printf(" %s", lem.get_text<russian_utf8_adapter>(lr, i, 0).c_str());
			printf(" %s", lem.get_text<russian_utf8_adapter>(lr, i, 1).c_str());
			printf(" %s", lem.get_text<russian_utf8_adapter>(lr, i, 2).c_str());
			printf(" %s", lem.get_text<russian_utf8_adapter>(lr, i, 3).c_str());
			printf(" %s", lem.get_text<russian_utf8_adapter>(lr, i, 4).c_str());
			printf(" %s", lem.get_text<russian_utf8_adapter>(lr, i, 5).c_str());
			printf("\n");
		}
	}
}

int main(int argc, char **argv)
{
	tl::lemmatizer lem;
	
	if (argc != 2)
	{
		printf("usage: %s водка\n", argv[0]);
		return 1;
	}
	
	try
	{
		lem.load_lemmatizer("/usr/local/share/turglem/russian/dict_russian.auto",
		    "/usr/local/share/turglem/russian/paradigms_russian.bin",
		    "/usr/local/share/turglem/russian/prediction_russian.auto"
		);
		
		do_cpp_magic(lem, argv[1]);
	}
	catch (const std::exception &e)
	{
		printf("EXCEPTION: %s\n", e.what());
		return 1;
	}
	
	return 0;
}

