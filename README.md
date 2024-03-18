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

// easy print helper
void printini( FILE* stream, ini_t* ini) {
	char* inistr = inisg_print(ini);
	fprintf(stream,"\n%s\n", inistr);
	fflush(stream);
	free(inistr);
}


int main(int agrc, char** argv) {
	// still manual ini creation
	ini_t* ini = malloc(sizeof(ini_t));
	// if you call inisg_parse there is no need to have an allocated pointer.
	// inisg will malloc it for you.
	free(ini);

	
	ini = inisg_parse("test = 3\nHelp =Hello");

	// with such a pointer its easier to switch between sections without creating new variables
	iniSection_t* section = NULL;

	printf("\n");
	// for each takes the subStructure and the parentStructure and allows you to loop over them, with the subStructure beeing the active pointer
	inisg_ForEachSection(section, ini) {
		iniEntry_t* entry = NULL;
		printf( "\"%s\"\n", section->name);
		inisg_ForEachEntry(entry, section) {
			// everything is stored as a c-string.
			printf("\t%s -> %s\n", entry->key, entry->value);
		}
	}
	
	printini(stdout, ini);

	printf("\n");

	section = inisg_AddSection(ini, "Construction");
	// every thing is stored as strings. you can format them youself.
	inisg_AddEntry(section, "thread_count", "8");

	// cleans up the whole section if it exists
	inisg_RemoveSection(ini, "empty");

	printini(stdout, ini);

	// cleans up the ini, no memory left.
	inisg_RemoveIni(ini);

	return 0;
}

```
