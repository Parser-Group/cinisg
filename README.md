# cinisg
This is a C-Addaptation of the library created for the inisg-project in C++ by [N0ffie](https://github.com/Knoficooki/inisg)

## How to use
cinisg is a single header library,\
to use it you have to include the implementation in a C-Source file like this:

```cpp
#define INISG_IMPLEMENTATION
#include <inisg.h>
``` 

## Differece to inisg
This is just to load the ini-file and unparse it.
The actuall commandline functionlity is not provided and not the idea of this project

# Future
I'm looking forward to learn about the usage of C in C++ to handle errors, if somebody has a tip for me, please create a issue.


# Example

```cpp
#include <stdio.h>

#define INISG_IMPLEMENTATION
#include <inisg.h>

void printini( FILE* stream, ini_t* ini) {
	char* inistr = inisg_print(ini);
	fprintf(stream,"\n%s\n", inistr);
	fflush(stream);
	free(inistr);
}


int main(int agrc, char** argv) {
	ini_t* ini = malloc(sizeof(ini_t));

	free(ini);
	ini = inisg_parse("test = 3\nHelp =Hello");

	iniSection_t* section = NULL;

	printf("\n");
	inisg_ForEachSection(section, ini) {
		iniEntry_t* entry = NULL;
		printf( "\"%s\"\n", section->name);
		inisg_ForEachEntry(entry, section) {
			printf("\t%s -> %s\n", entry->key, entry->value);
		}
	}
	
	printini(stdout, ini);

	printf("\n");

	section = inisg_AddSection(ini, "Construction");

	inisg_AddEntry(section, "thread_count", "8");

	inisg_RemoveSection(ini, "empty");

	printini(stdout, ini);

	inisg_RemoveIni(ini);

	return 0;
}

```
