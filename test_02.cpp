#include <turglem/lemmatizer.hpp>
#include <turglem/russian/charset_adapters.hpp>

#include <stdio.h>

void do_cpp_magic(const tl::lemmatizer &lem, const char *s)
{
	tl::lem_result lr;
	size_t sz_lem = lem.lemmatize<russian_utf8_adapter>(s, lr);

	if (sz_lem)
	{
		printf("Total lemmatize results: %u\n", sz_lem);
		for (size_t i = 0; i < sz_lem; i++)
		{
			printf("%u: paradigm/form = %d/%d\n", i,
			    lem.get_paradigm(lr, i),
			    lem.get_src_form(lr, i)
			);
			std::string nform = lem.get_text<russian_utf8_adapter>(lr, i, 0);
			printf("\tnormal form: '%s'\n", nform.c_str());
		}
	}
	else
	{
		printf("Empty result! Empty string? If not, is it in in Russian?\n");
	}
}

int main(int argc, char **argv)
{
	tl::lemmatizer lem;

	try
	{
		lem.load_lemmatizer("/usr/local/share/turglem/russian/dict_russian.auto",
		    "/usr/local/share/turglem/russian/paradigms_russian.bin",
		    "/usr/local/share/turglem/russian/prediction_russian.auto"
		);

		do_cpp_magic(lem, "аццкая");
		do_cpp_magic(lem, "находящиеся");
	}
	catch (const std::exception &e)
	{
		printf("EXCEPTION: %s\n", e.what());
	}

	return 0;
}

