/*Federico Madden
COEN 12 MWF 10:30AM
Prof. Liu
PROJECT 3 */

#include "set.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define EMPTY 0
#define FILLED 1
#define DELETED 2

struct set
{
	int count;
	int length;
	char **elts;
	int *flags;
};

static int search(SET *sp, char *elt, bool *found);
unsigned strhash(char *s);

SET *createSet(int maxElts)
{
	SET *sp;
	sp = malloc(sizeof(SET*));
	assert(sp != NULL);

	sp->length = maxElts;
	sp->count = 0;
	
	sp->elts = malloc(sizeof(char*)*maxElts);
	assert(sp->elts != NULL);

	sp->flags = malloc(sizeof(int)*maxElts);
	assert(sp->flags != NULL);

	int i;
	for(i = 0; i < sp->length; i++)
		sp->flags[i] = EMPTY;

	return sp;
	
}

void addElement(SET *sp, char *elt)
{
	bool found;
	int idx = search(sp, elt, &found);

	if((sp->count < sp->length) && (!found))//check if array capacity has been reached and if elt already exists in the set
	{
		sp->elts[idx] = strdup(elt);		
		sp->count++;
		sp->flags[idx] = FILLED;
	}
}

void removeElement(SET *sp, char *elt)
{
	bool found;
	int idx = search(sp, elt, &found);
	
	if(found)
	{
		free(sp->elts[idx]);
		sp->flags[idx] = DELETED;
		sp->count -= 1;
	}
}

void destroySet(SET *sp)
{
	assert(sp != NULL);
	for(int i = 0; i < sp->length; i++)
	{
		if(sp->flags[i] == FILLED)
			free(sp->elts[i]);
	}
	free(sp->elts);
	free(sp->flags);
	free(sp);
}

static int search(SET *sp, char *elt, bool *found)//sequentially searches through set sp for elt. Returns index of elt if found and -1 if not found
{
	
	*found = false;
	int start = strhash(elt) % sp->length;
	int idx;

	bool foundDel = false;
	int idxDel;
	for(int i = 0; i < sp->length; i++)
	{
		idx = (start + i) % sp->length;
		if(sp->flags[idx] == FILLED)
		{
			if(strcmp(elt, sp->elts[idx]) == 0)
			{
				*found = true;
				return idx;
			}
		}
		else if(sp->flags[idx] == DELETED && !foundDel)
		{
			foundDel = true;
			idxDel = idx;
		}
		else if(sp->flags[idx] == EMPTY)
		{
			if(foundDel)
				return idxDel;
			else 
				return idx;
		}
	}
	return -1;
}



int numElements(SET *sp)
{
	assert(sp != NULL);
	return(sp->count);
}

char *findElement(SET *sp, char *elt)
{
	bool found;
	int index = search(sp, elt, &found);
	
	if(found)
	{
		return(sp->elts[index]);
	}
	else
	{
		return NULL;
	}
}

char **getElements(SET *sp)
{
	char **sq;
	sq = malloc(sizeof(char*)*(sp->count));
	assert(sq != NULL);
	
	int j = 0;
	for(int i = 0; i < (sp->length); i++)
	{
		if(sp->flags[i] == FILLED)
			sq[j] = strdup(sp->elts[i]);
			j++;
	}
	return sq;
	
}

unsigned strhash(char *s)
{
	unsigned hash = 0;

	while (*s != '\0')
		hash = 31*hash +*s ++;

	return hash;
}
