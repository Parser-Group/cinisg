#ifndef INISG_H
#define INISG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>


#ifndef NO_STD_TYPES

#include <stdint.h>


#ifndef _MAX_TYPES_DEFINED
#define _MAX_TYPES_DEFINED

typedef intmax_t smax;
typedef size_t umax;
#define PRIMAX_T "d"

#endif /* _MAX_TYPES_DEFINED */

typedef umax ptr_t;

#endif /* NO_STD_TYPES */

#if !defined(INISG_COSTUME_MACROS_DEFINED) && !defined(INISG_MACROS_DEFINED)
/*
 * UTILITY_DEFINES
 * -----------------
 * 
 * INISG_FUNC(name)
 * INISG_PUBLIC_UTIL(name)
 * INISG_PRIVATE_UTIL(name)
 * INISG_MEM(name)
 * INISG_GCC(name)			// gcc funcs for msvc
 * 
 */

#define INISG_FUNC(name) inisg_##name
#define INISG_PUBLIC_UTIL(name) INISG_FUNC(util_##name)
#define INISG_PRIVATE_UTIL(name) __inisg_util_##name
#define INISG_MEM(name) inisgm_x##name
#define INISG_DMEM(name) inisgm_x##name
#define INISG_GCC(name) inigcc_##name

/*
* DEBUG
*/
#if defined(INISG_DEBUG) || defined(_INISG_DEBUG)
#	ifndef INISG_DEBUG
#		define INISG_DEBUG
#	endif
#	define DEBUG INISG_DEBUG
#endif /* defined(DEBUG) || defined(_DEBUG) */

/* Error handling */
#ifdef INISG_DEBUG
#	define _ferror_inisg(str) INISG_FUNC(fatal)(str, __FILE__, __LINE__)
#	define ifdebug(code) code
#else 
#	define _ferror_inisg(str) INISG_GCC(fatal)(str)
#	define ifdebug(code) 
#endif /* DEBUG */

/*
* Debug Logging
* Use Debug Modes
*/

#ifdef INISG_DEBUG
#	if DEBUG == 2
#		undef DEBUG
#		define DEBUG 1
#	endif
#	if DEBUG == 1
	/*
	* Allocation
	*/
#		define INISG_DEBUG_L1
#		define ifdebug1(code) code

#		undef INISG_DMEM
#		undef INISG_MEM
#		define INISG_DMEM(name) debug_inisgm_x##name
#		define INISG_MEM(name) INISG_DB_##name

#		define INISG_DB_malloc(size) INISG_DMEM(malloc)(size, __FILE__, __LINE__)
#		define INISG_DB_realloc(ptr, size) INISG_DMEM(realloc)(ptr, size, __FILE__, __LINE__)
#		undef DEBUG
#		define DEBUG 0
#	else
#		define ifdebug1(code)
#	endif /* Debug level 1 */
#endif


#define INISG_MACROS_DEFINED
#endif /* !defined(INISG_COSTUME_MACROS_DEFINED) && !defined(INISG_MACROS_DEFINED) */

typedef struct INI_ENTRY {
	char* key;
	char* value;
} iniEntry_t;

typedef struct INI_SECTION {
	char* name;
	umax numEntries;
	iniEntry_t *entries;
} iniSection_t;

typedef struct INI_DATA {
	umax numSections;
	iniSection_t* sections;
} ini_t;



ini_t*			INISG_FUNC( parse 	) 		(const char*); 								// arg0 is a string that contains the ini in text format
char* 	INISG_FUNC( print 	)		(const ini_t* const); 									// return the ini in text format

iniEntry_t* 	INISG_FUNC( GetEntry 	) 	(const iniSection_t* const, const char*);				// returns the entry in the section arg0 
																						// with the same content in key as in arg1
																						// if entry doesn't exist, return NULL

iniSection_t* 	INISG_FUNC( GetSection 	) 	(const ini_t* const, const char*);						// returns the section in the ini structure arg0
																						// with the same content in section.name as in arg1
																						// if section does not exist, return NULL

iniEntry_t* 	INISG_FUNC( AddEntry 	)	(iniSection_t*, const char*, const char*);	// returns the Entry created. 
																						// Takes the section, the key and the value.
																						// if a entry with that key exists it returns it.

iniSection_t* 	INISG_FUNC( AddSection 	) 	(ini_t*, const char*);						// returns the Section created. 
																						// Takes the ini structure and the section-name.
																						// if a section with that name exists it return it.

char			INISG_FUNC( ClearEntry )	(iniEntry_t*);								// frees everthing inside the entry 
																						// pointer stays valid, even though the entry is empty

char			INISG_FUNC( ClearSection )	(iniSection_t*);							// frees and deletes everything inside the section
																						// pointer stays valide, even though the section is empty

void 			INISG_FUNC( RemoveEntry	) 	(iniSection_t*, const char*);				// removes the entry in a section, 
																						// that corresponds to the second argument given

void 			INISG_FUNC( RemoveSection ) (ini_t*, const char*);						// removes the section in the ini structure,
																						// that correspondes to the section-name given in the second argument

void 			INISG_FUNC( RemoveIni 	)	(ini_t*);									// frees the complete ini structure with all it sub-structures

void			INISG_FUNC( ChangeValue )	(iniEntry_t*, const char*);					// changes the value of the entry given in arg0
																						// to the value in arg1 that has the size in arg2

/*
 * The way it works
 *
 * get	-> linear search
 *
 * 
 * add	-> does it exist? yes? get it, return it. 
 *		-> no? create it. return it.
 *
*/

#if !defined(INISG_SMEM_DEFINED)

#include <stdlib.h>

void* INISG_DMEM( malloc	)	(umax size 
#ifdef INISG_DEBUG_L1
	, const char* file, umax line
#endif
	);

void* INISG_DMEM( realloc	)	(void *ptr, umax size 
#ifdef INISG_DEBUG_L1
	, const char* file, umax line
#endif
	);

#endif /* !defined(INISG_SMEM_DEFINED) */

/*
* Compiler specific defines
*/

#if !defined(INISG_GCC_DEFINED)
void INISG_GCC(fatal) (const char*);
char* INISG_GCC(strdup) (const char*);
char* INISG_GCC(strtok) (char* restrict, const char* restrict);
#endif /* !defined(INISG_GCC_DEFINED) */


#ifdef INISG_IMPLEMENTATION

void INISG_GCC(fatal) (const char* c) {
	fprintf(stderr, "\n%s\n", c);
	abort();
}

void INISG_FUNC(fatal) (const char* str, const char* file, umax line) {
	fprintf(stderr, "Fatal Error:\n\t%s\n\nIn file:\t%s\nOn line:\t%" PRIMAX_T "\n\n", str, file, line);
	abort();
}

char* INISG_GCC(strdup) (const char* str) {
	char* newstr = NULL;
#ifdef _WIN32 
	newstr = _strdup(str);
#else
	newstr = strdup(str);
#endif
	return newstr;
}

char* INISG_GCC(strtok) (char* restrict str, const char* restrict delim) {
#ifdef _WIN32
	static char* next_token;
	if(str) next_token = NULL;
	return strtok_s(str, delim, &next_token);
#else
	return strtok(str, delim);
#endif /*_MSC_VER*/
}

#if !defined(INISG_SMEM_DEFINED)

/* malloc */
void * INISG_DMEM( malloc ) (umax size 
#ifdef INISG_DEBUG_L1
	, const char* file, umax line
#endif
	)
{
	ifdebug1(printf("malloc:   %" PRIMAX_T "\tLine: %" PRIMAX_T "\n", size, line));
	void *value = malloc (size);
	if (!value) {
		ifdebug1(fprintf(stderr, "mallocation failure\nFile:\t%s\t\tLine:\t%" PRIMAX_T "\n", file, line));
		_ferror_inisg("Virtual memory exhausted");
	}
	return value; 
}

void* INISG_DMEM( realloc ) (void *ptr, umax size 
#ifdef INISG_DEBUG_L1
	, const char* file, umax line
#endif
	)
{
	ifdebug1(printf("realloc:  %" PRIMAX_T "\tLine: %" PRIMAX_T "\n", size, line));
	void* value = realloc(ptr, size);
	if (!value) {
		ifdebug1(fprintf(stderr, "reallocation failure.\nFile:\t%s\t\tLine:\t%" PRIMAX_T "\n", file, line));
		_ferror_inisg("Virtual memory exhausted");
	}
	return value;
}

#endif // !defined(INISG_SMEM_DEFINED)

#if !defined(INISG_COSTUME_UTILS_DEFINED) && !defined(INISG_UTILS_DEFINED)

/* ----------------------
 * 	Utilities
 * ----------------------
 * strtrm:	trims the given string and returns it.
 * 		creates a new string to return 
 *		the original is not touched.
*/

char * INISG_PRIVATE_UTIL( ltrim_inp ) (char *s)
{
	while(isspace(*s)) s++;
	return s;
}

char* INISG_PRIVATE_UTIL( rtrim_inp ) (char *s)
{
	char* back = s + strlen(s);
	while(isspace(*--back));
	*(back+1) = '\0';
	return s;
}

char* INISG_PRIVATE_UTIL( strtrim_inp )(char *s)
{
	return INISG_PRIVATE_UTIL( rtrim_inp )(INISG_PRIVATE_UTIL( ltrim_inp )(s));
}

char* INISG_PUBLIC_UTIL( strtrm )( const char *s ) {
	if(!s) {
		return NULL;
	}
	char* str = INISG_GCC(strdup)(s);
	char* tmp = INISG_PRIVATE_UTIL(strtrim_inp)(str);
	if(!tmp) {
		return NULL;
	}
	char* res = INISG_GCC(strdup)(tmp);
	res[strlen(tmp)] = '\0';
	free(str);
	tmp = NULL;
	return res;
}

#define INISG_COMMENT_INDICATOR '#'

void INISG_PRIVATE_UTIL( removeCommentFromLine ) (char** line) {
	char* seperator = strchr(*line, INISG_COMMENT_INDICATOR);
	if(!seperator) {return;}
	ptr_t len = (ptr_t)(seperator - *line);
	char* newstr = INISG_MEM(malloc)(len + 1);
	memcpy(newstr, *line, len * sizeof(char));
	newstr[len] = 0;
	free(line);
	*line = newstr;
}

char*  INISG_PUBLIC_UTIL(strcat)(const char* str1, const char* str2) {
	if(!str1 || !str2) {
		fprintf(stderr, "inisg string-cat error: \n\tstring pointer is NULL;\n");
		return NULL;
	}
	char* new_str = INISG_MEM(malloc)(sizeof(char) * (strlen(str1) + strlen(str2) + 1));
	new_str[0] = '\0';
	strcat(new_str, str1);
	strcat(new_str, str2);
	return new_str;
}

/* returns a pointer to the first argument */
void INISG_PUBLIC_UTIL(strapp)(char** str, const char* str2) {
	if(!str || !(*str)) {
		fprintf(stderr, "inisg string-append error: \n\tdest pointer is NULL;\n");
		return;
	}
	if(!str2) {
		return;
	}
	umax len = strlen(*str);
	umax slen = len + strlen(str2);
	if(slen < 1) return;
	*str = INISG_MEM(realloc)(*str, sizeof(char) * (slen + 1));
	memset(&(*str)[len], 0, slen - len);
	strcpy(&((*str)[len]), str2);
	(*str)[slen] = 0;
}

#define INISG_UTILS_DEFINED
#endif /* !defined(INISG_COSTUME_UTILS) && !defined(INISG_UTILS_DEFINED) */

/* parse */

#define INISG_SECNAME_MAX_LEN 64

ini_t* INISG_FUNC( parse ) (const char* iniContentString) {
	char* inistr = INISG_GCC(strdup)(iniContentString);
	char* token = INISG_GCC(strtok)(inistr, "\n\0");

	/* resulting ini */
	ini_t* inires = INISG_MEM(malloc)(sizeof(ini_t));
	inires->sections = NULL;
	inires->numSections = 0;
	/* active section */
	iniSection_t* acsec = NULL;

	while(token) {
		char* tline = INISG_GCC(strdup)(token);
		INISG_PRIVATE_UTIL(removeCommentFromLine)(&tline); 	/* Comments will not be saved and therefore,
									 * will vanish when being unparsed */
		char* line = INISG_PUBLIC_UTIL(strtrm)(tline);
		free(tline);

		if(!strlen(line)) { /* skip if the line is empty */
			goto skip;
		}

		/* section */
		/* Syntax: [{name}] */
		if(*line == '[') {
			char* tmp = line;

			char* secname = INISG_MEM(malloc)(sizeof(char*) * (INISG_SECNAME_MAX_LEN+1));
			char* strtmp = secname;
			/* go from '[' to ']' in within the string lays. syntax: "[...]" */
			while(*tmp++ != ']' && (tmp - line) < (INISG_SECNAME_MAX_LEN + 1)) {
				*strtmp++ = *tmp;
			}
			*(strtmp-1) = '\0';
			
			char* name = INISG_GCC(strdup)(secname);
			free(secname);
			char* trmname = INISG_PUBLIC_UTIL(strtrm)(name);
			free(name);

			if(!strlen(trmname)) {
				free(trmname);
				trmname = INISG_GCC(strdup)("empty");
			}

			acsec = INISG_FUNC(AddSection)(inires, name);
			free(trmname);
			goto skip;
		}

		/* entry */
		/* Syntax: {key} = {value} */
		char* equal = strchr(line, '=');
		if(equal) {
			ptr_t index = (ptr_t)(equal - line);
			if(index < 1) { goto skip; }
			char* value = INISG_GCC(strdup)(equal + 1);
			*equal = '\0';
			char* key = INISG_GCC(strdup)(line);
			*equal = '=';

			char* k = INISG_PUBLIC_UTIL(strtrm)(key);
			char* v = INISG_PUBLIC_UTIL(strtrm)(value);
			free(key);
			free(value);
			if(!strlen(k)) {
				fprintf(stderr, "inisg: key is empty!\n");
				goto entryskip;
			}

			if(!acsec) {
				acsec = INISG_FUNC(AddSection(inires, "empty"));
			}
			if(!strlen(v)) {
				INISG_FUNC(AddEntry)(acsec, k, NULL);
				goto entryskip;
			}
			INISG_FUNC(AddEntry)(acsec, k, v);

		entryskip:
			free(k);
			free(v);
			goto skip;
		}
	

	skip:
		free(line);
		token = INISG_GCC(strtok)(NULL, "\n\0");
	}
	
	free(inistr);
	ifdebug1(printf("finished parsing\n"));
	return inires;
end:
	free(inistr);
	INISG_FUNC(RemoveIni)(inires);
	return NULL;
}

/* print(unparse) */


char* INISG_FUNC( print ) (const ini_t* const ini) {
	char* inistr = INISG_MEM(malloc)(sizeof(char));
	if(!ini->numSections && !ini->sections) return NULL;
	*inistr = 0;
	iniSection_t* acsec = INISG_FUNC(GetSection)(ini, "empty");
	iniEntry_t* acetr = NULL;
	if(acsec) {
		if(!acsec->numEntries || !acsec->entries) goto skipempty;
		for(umax i = 0; i < acsec->numEntries; i++) {
			acetr = &acsec->entries[i];
			char* entry = INISG_MEM(malloc)(sizeof(char));
			*entry = 0;
			INISG_PUBLIC_UTIL(strapp)(&entry, acetr->key);
			INISG_PUBLIC_UTIL(strapp)(&entry, " = ");
			INISG_PUBLIC_UTIL(strapp)(&entry, acetr->value);
			
			INISG_PUBLIC_UTIL(strapp)(&inistr, entry);
			INISG_PUBLIC_UTIL(strapp)(&inistr, "\n");
			free(entry);
		}
	}
skipempty:
	for(umax as = 0; as < ini->numSections; as++) {
		acsec = &ini->sections[as];
		if(!acsec->name || (!strcmp(acsec->name, "empty"))) {
			goto skip;
		}
		char* section = INISG_MEM(malloc)(sizeof(char));
		*section = 0;

		INISG_PUBLIC_UTIL(strapp)(&section, "[ ");
		INISG_PUBLIC_UTIL(strapp)(&section, acsec->name);
		INISG_PUBLIC_UTIL(strapp)(&section, " ]\n");
		
		if(!acsec->numEntries || !acsec->entries) goto secend;

		for(umax ae = 0; ae < acsec->numEntries; ae++) {
			acetr = &acsec->entries[ae];
			char* entry = INISG_MEM(malloc)(sizeof(char));
			*entry = 0;

			INISG_PUBLIC_UTIL(strapp)(&entry, acetr->key);
			INISG_PUBLIC_UTIL(strapp)(&entry, " = ");
			INISG_PUBLIC_UTIL(strapp)(&entry, acetr->value);

			INISG_PUBLIC_UTIL(strapp)(&section, entry);
			INISG_PUBLIC_UTIL(strapp)(&section, "\n");
			free(entry);
		}
	secend:
		INISG_PUBLIC_UTIL(strapp)(&inistr, section);
		free(section);
	skip:
		INISG_PUBLIC_UTIL(strapp)(&inistr, "\n");
	}
	return inistr;
}

// GetEntry
iniEntry_t* INISG_FUNC( GetEntry ) (const iniSection_t* const sec, const char* key) {
	for(umax i = 0; i < sec->numEntries; i++) {
		if (!strcmp(sec->entries[i].key, key)) {
			return &sec->entries[i];
		}
	}
	return NULL;
}

// GetSection
iniSection_t* INISG_FUNC( GetSection ) (const ini_t* const ini, const char* name) {
	if(!ini) return NULL;
	for(umax i = 0; i < ini->numSections; i++) {
		if(!strcmp(ini->sections[i].name, name)) {
			return &ini->sections[i];
		}
	}
	return NULL;
}
// AddEntry
iniEntry_t* INISG_FUNC( AddEntry ) (iniSection_t* sec, const char* key, const char* value) {
	iniEntry_t* e = INISG_FUNC(GetEntry)(sec, key);
	if(e && sec) return e;
	if(!sec->entries) {
		sec->entries = INISG_MEM(malloc)(0);
	}

	sec->entries = INISG_MEM(realloc)
				(sec->entries ,sizeof(iniEntry_t) * (sec->numEntries+1));
	e = &sec->entries[sec->numEntries++];
	e->key = INISG_GCC(strdup)(key);
	e->value = INISG_PUBLIC_UTIL(strtrm)(INISG_GCC(strdup)(value));
	return e;
}

// AddSection
iniSection_t* INISG_FUNC( AddSection ) (ini_t* ini, const char* name) {
	iniSection_t* sec = INISG_FUNC(GetSection)(ini, name);
	if(sec && ini) return sec;
	if(!ini->sections) {
		ini->sections = INISG_MEM(malloc)(0);
	}
	ini->sections = INISG_MEM(realloc)
				(ini->sections, sizeof(iniSection_t) * (ini->numSections+1));
	sec = &ini->sections[ini->numSections++];
	sec->name = INISG_GCC(strdup)(name);
	sec->entries = NULL;
	sec->numEntries = 0;
	return sec;
}

// ClearEntry
char INISG_FUNC( ClearEntry ) (iniEntry_t* e) {
	if(!e) return 1;
	if(e->key) {
		free(e->key);
	}
	if(e->value) {
		free(e->value);
	}
	return 0;
}

// ClearSection
char INISG_FUNC( ClearSection ) (iniSection_t* sec) {
	if(!sec) return 1;
	if(sec->name) {
		free(sec->name);
	}
	if(sec->entries) {
		for(umax i = 0; i < sec->numEntries; i++) {
			iniEntry_t* e = &sec->entries[i];
			char r = INISG_FUNC(ClearEntry)(e);
		}
	}
	free(sec->entries);
	sec->numEntries = 0;
	return 0;
}

// RemoveEntry
void INISG_FUNC( RemoveEntry ) (iniSection_t* sec, const char* key) {
	iniEntry_t* entry = INISG_FUNC(GetEntry)(sec, key);
	if(!entry) {
		return;	
	}
	char r = INISG_FUNC(ClearEntry)(entry);
	if(r) {
		printf("error in clearing entry\n");
		return;
	}

	ptr_t index = (ptr_t)(entry - sec->entries);
	
	umax nbefore = index;
	umax nafter  = sec->numEntries - (index+1);
	iniEntry_t* entriesAfter;
	if(nafter) {
		entriesAfter = (iniEntry_t*)INISG_MEM(malloc)
						(sizeof(iniEntry_t) * nafter);
		memcpy(entriesAfter, 
			entry + 1, 
			sizeof(iniEntry_t) * nafter);
	}
	sec->entries = (iniEntry_t*)INISG_MEM(realloc)
				(sec->entries, 
				 sizeof(iniEntry_t) * (sec->numEntries-1) );
	if(nafter) {
		memcpy(
			sec->entries +  index, 
			entriesAfter,
			sizeof(iniEntry_t) * nafter);
		free(entriesAfter);
	}
	sec->numEntries -= 1;
}

// RemoveSection
void INISG_FUNC( RemoveSection ) (ini_t* ini, const char* name) {
	iniSection_t* sec = INISG_FUNC(GetSection)(ini, name);
	if(!sec) {
		return;
	}
	char r = INISG_FUNC(ClearSection)(sec);
	if(r) {
		printf("error in clearing section\n");
		return;
	}

	ptr_t index = (ptr_t)(sec - ini->sections);

	umax nbefore = index;
	umax nafter  = ini->numSections - (index+1);


	iniSection_t* secsAfter;

	if(nafter) {
		secsAfter = (iniSection_t*)INISG_MEM(malloc)
						(sizeof(iniSection_t) * nafter);
		memcpy(secsAfter,
			sec + 1,
			sizeof(iniSection_t) * nafter);
	}
	ini->sections = (iniSection_t*)INISG_MEM(realloc)
				(ini->sections,
				 sizeof(iniSection_t) * (ini->numSections - 1) );
	if(nafter) {
		memcpy(
			ini->sections + index,
			secsAfter,
			sizeof(iniSection_t) * nafter);
		free(secsAfter);
	}
	ini->numSections -= 1;
}


// RemoveIni
void INISG_FUNC( RemoveIni ) (ini_t* ini) {
	for(umax i = 0; i < ini->numSections; i++) {
		char r = INISG_FUNC(ClearSection)(&ini->sections[i]);
		if(r) {
			printf("error in clearing section");
		}
	}
	free(ini->sections);
	free(ini);
}

// ChangeValue

void INISG_FUNC( ChangeValue ) (iniEntry_t* e, const char* str) {
	if(e->value) {
		free(e->value);
	}
	e->value = INISG_PUBLIC_UTIL(strtrm)(str);
}

#endif // INISG_IMPLEMENTATION

#undef _ferror_inisg
#undef ifdebug

#define inisg_ForEachSection(element, ini) for(element = (ini->numSections > 0 && ini->sections) ? ini->sections : NULL; (element != NULL && ((element - ini->sections) < ini->numSections)); element++)

#define inisg_ForEachEntry(element, sec) for(element = (sec->numEntries > 0 && sec->entries) ? sec->entries : NULL; (element != NULL && ((element - sec->entries) < sec->numEntries)); element++)


#endif // INISG_H
