#include "../fe.h"

char fe_randbyte_keyed(char const *key, uint32_t x, BitSequence *hashval) {
    char *template, *data;
    HashReturn ret;
    static uint32_t last_x, required_x;
    size_t datalen, templatelen;
    unsigned ntemplates, i;

    /* Need to recalculate the hash if:
       - there is no previous
       - current hash is more than HASH_BYTE_SIZE off
    */
    required_x = (x / HASH_BYTE_SIZE) * HASH_BYTE_SIZE;
    if (! *hashval || last_x != required_x) {
	
	/* Get basic template for hash input: offset with user key,
	 * multiplied until we get at least HASH_BYTE_SIZE chars
	 * as input
	 */
	last_x = required_x;
	fe_xasprintf(&template, "%u%s", last_x, key);

	/* Pre 1.07 code, optimized in 1.07 for speed:
	data = fe_xstrdup(template);
	while (strlen(data) < HASH_BYTE_SIZE)
	   data = fe_xstrcat(data, template);
	if ( (ret = fe_Hash(HASH_BIT_SIZE, (BitSequence const *) data,
			 strlen(data), hashval)) != SUCCESS )
	    fe_error("Hash algorithm indicates error %d\n", (int)ret);
	*/
	templatelen = strlen(template);
	ntemplates = HASH_BYTE_SIZE / templatelen;
	if (HASH_BYTE_SIZE % templatelen)
	    ntemplates++;
	data = fe_xmalloc(templatelen * ntemplates);
	for (datalen = 0, i = 0; i < ntemplates; i++, datalen += templatelen)
	    memcpy(data + datalen, template, templatelen);

	/* Convert multiplied template to hashbits, then discard input data */
	if ( (ret = fe_Hash(HASH_BIT_SIZE, (BitSequence const *) data,
			 datalen, hashval)) != SUCCESS )
	    fe_error("Hash algorithm indicates error %d\n", (int)ret);

	/*
	{
	    int i;
	    fprintf(stderr, "%s->", template);
	    for (i = 0; i < HASH_BYTE_SIZE; i++)
		fprintf(stderr, "%2.2x", hashval[i]);
	    fprintf(stderr, "\n");
	}
	*/
	
	free(template);
	free(data);
    }

    /* Next pseudorandom value is in the hashed bits */
    return (hashval[x % HASH_BYTE_SIZE]) & 0xff;
}
