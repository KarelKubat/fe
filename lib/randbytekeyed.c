#include "../fe.h"

char randbyte_keyed(char const *key, uint32_t x, BitSequence *hashval) {
    char *template, *data;
    HashReturn ret;
    static uint32_t last_x, required_x;

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
	xasprintf(&template, "%u%s", last_x, key);
	data = xstrdup(template);
	while (strlen(data) < HASH_BYTE_SIZE)
	    data = xstrcat(data, template);

	/* Convert multiplied template to hashbits, then discard input data */
	if ( (ret = Hash(HASH_BIT_SIZE, (BitSequence const *) data,
			 strlen(data), hashval)) != SUCCESS )
	    error("Hash algorithm indicates error %d\n", (int)ret);

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
