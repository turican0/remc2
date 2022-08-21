/*
** Based upon hash.c from sqlite which author disclaims copyright to this source code. In place of
** a legal notice, here is a blessing:
**
** May you do good and not evil.
** May you find forgiveness for yourself and forgive others.
** May you share freely, never taking more than you give.
*/
#ifndef	__SJHASH_H__
#define	__SJHASH_H__


#ifdef __cplusplus
extern "C"
{
#endif



/* Forward declarations	of structures.
 */
typedef	struct sjhash		sjhash;
typedef	struct sjhashElem	sjhashElem;



/* A complete hash table is	an instance	of the following structure.
 * The internals of	this structure are intended	to be opaque --	client
 * code	should not attempt to access or	modify the fields of this structure
 * directly.  Change this structure	only by	using the routines below.
 * However,	many of	the	"procedures" and "functions" for modifying and
 * accessing this structure	are	really macros, so we can't really make
 * this	structure opaque.
 */
struct sjhash
{
	char			keyClass;		/* SJHASH_INT, _POINTER, _STRING, _BINARY */
	char			copyKey;		/* True	if copy	of key made	on insert */
	int				count;			/* Number of entries in	this table */
	sjhashElem*		first;			/* The first element of	the	array */
	int				htsize;			/* Number of buckets in	the	hash table */
	struct _ht
	{								/* the hash	table */
		int			count;			/* Number of entries with this hash	*/
		sjhashElem*	chain;			/* Pointer to first	entry with this	hash */
	} *ht;
};



/* Each	element	in the hash	table is an	instance of	the	following 
 * structure.  All elements	are	stored on a	single doubly-linked list.
 *
 * Again, this structure is	intended to	be opaque, but it can't	really
 * be opaque because it	is used	by macros.
 */
struct sjhashElem 
{
  sjhashElem	*next, *prev;	/* Next	and	previous elements in the table */
  void*			data;			/* Data	associated with	this element */
  void*			pKey;			/* Key associated with this	element	*/
  int			nKey;			/* Key associated with this	element	*/
};



/*
 * There are 4 different modes of operation	for	a hash table:
 *
 *	 SJHASH_INT			nKey is	used as	the	key	and	pKey is	ignored.
 *
 *	 SJHASH_POINTER		pKey is	used as	the	key	and	nKey is	ignored.
 *
 *	 SJHASH_STRING		pKey points	to a string	that is	nKey bytes long
 *						(including the null-terminator,	if any).  Case
 *						is ignored in comparisons.
 *
 *	 SJHASH_BINARY		pKey points	to binary data nKey	bytes long.	
 *						memcmp() is	used to	compare	keys.
 *
 * A copy of the key is	made for SJHASH_STRING and SJHASH_BINARY
 * if the copyKey parameter	to SjHashInit is 1.	 
 */
#define	SJHASH_INT		 1
#define	SJHASH_POINTER	 2
#define	SJHASH_STRING	 3
#define	SJHASH_BINARY	 4



/*
 * Access routines.	 To	delete an element, insert a	NULL pointer.
 */
void	sjhashInit		(sjhash*, int keytype, int copyKey);
void*	sjhashInsert	(sjhash*, const	void *pKey,	int	nKey, void *pData);
void*	sjhashFind		(const sjhash*,	const void *pKey, int nKey);
void	sjhashClear		(sjhash*);



/*
 * Macros for looping over all elements	of a hash table.  The idiom	is
 * like	this:
 *
 *	 SjHash	h;
 *	 SjHashElem	*p;
 *	 ...
 *	 for(p=sjhashFirst(&h);	p; p=sjhashNext(p)){
 *	   SomeStructure *pData	= sjhashData(p);
 *	   // do something with	pData
 *	 }
 */
#define	sjhashFirst(H)		((H)->first)
#define	sjhashNext(E)		((E)->next)
#define	sjhashData(E)		((E)->data)
#define	sjhashKey(E)		((E)->pKey)
#define	sjhashKeysize(E)	((E)->nKey)



/*
 * Number of entries in	a hash table
 */
#define	sjhashCount(H)	((H)->count)



#ifdef __cplusplus
};
#endif


#endif /* __SJHASH_H__ */
