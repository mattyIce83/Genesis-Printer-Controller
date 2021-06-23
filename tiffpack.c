/*------------------------------------------------------------*/
/*- Tiff Pack                                                -*/
/*------------------------------------------------------------*/
unsigned char *TiffPack (	unsigned char 	*rawData, 
							unsigned char 	*compressedData,
							unsigned int 	*length) {

	// Just return users pointer

	signed char *op, *lastliteral, *ep, *rd;
	int n, b, cc;
	enum { BASE, LITERAL, RUN, LITERAL_RUN } state;

	op = (signed char *) compressedData;
#ifdef END_CHECK
	ep = (char *)(combuf + CBUFSIZ);
#else
	ep = 0;// gets rid of warning
#endif
	cc = *length;
	state = BASE;
	lastliteral = 0;
	rd = (signed char *)rawData;
	while (cc > 0) {
		/*
		 * Find the longest string of identical bytes.
		 */
		b = *rd++, cc--, n = 1;
		for (; cc > 0 && b == *rd; cc--, rd++)
			n++;
	again:
#ifdef END_CHECK
		if (op + 2 >= ep) {		/* insure space for new data */
			printf ("Compression buffer overflow: cc = %d\n", cc);
			return;
		}
#endif
		switch (state) {
		case BASE:		/* initial state, set run/literal */
			if (n > 1) {
				state = RUN;
				if (n > 128) {
					*op++ = -127;
					*op++ = b;
					n -= 128;
					goto again;
				}
				*op++ = -(n-1);
				*op++ = b;
			} else {
				lastliteral = op;
				*op++ = 0;
				*op++ = b;
				state = LITERAL;
			}
			break;
		case LITERAL:		/* last object was literal string */
			if (n > 1) {
				state = LITERAL_RUN;
				if (n > 128) {
					*op++ = -127;
					*op++ = b;
					n -= 128;
					goto again;
				}
				*op++ = -(n-1);		/* encode run */
				*op++ = b;
			} else {			/* extend literal */
				if (++(*lastliteral) == 127)
					state = BASE;
				*op++ = b;
			}
			break;
		case RUN:		/* last object was run */
			if (n > 1) {
				if (n > 128) {
					*op++ = -127;
					*op++ = b;
					n -= 128;
					goto again;
				}
				*op++ = -(n-1);
				*op++ = b;
			} else {
				lastliteral = op;
				*op++ = 0;
				*op++ = b;
				state = LITERAL;
			}
			break;
		case LITERAL_RUN:	/* literal followed by a run */
			/*
			 * Check to see if previous run should
			 * be converted to a literal, in which
			 * case we convert literal-run-literal
			 * to a single literal.
			 */
			if (n == 1 && *(op - 2) == (char)-1 &&
			    *lastliteral < 126) {
				state = (((*lastliteral) += 2) == 127 ?
				    BASE : LITERAL);
				*(op -2) = *(op - 1);	/* replicate */
			} else
				state = RUN;
			goto again;
		}
	}
	*length =  (unsigned long)op - (unsigned long)compressedData;

	return (compressedData);
}

/*------------------------------------------------------------*/
/*- Tiff UnPack                                              -*/
/*------------------------------------------------------------*/
void TiffUnPack (unsigned char 	*compressedData, 
				 unsigned char 	*rawData,
				 int	*length) {

signed char *op, *ep, *rd;
int n, b, tu = 0;

	ep = (signed char *)compressedData;
	op = (signed char *)rawData;
	rd = ep + *length; // end of buffer

	while(1) {
		if(*ep < 0) {
			*ep ^= 0x80;
			n = 129 - *ep++;
			for (b = 0; b < n; b++) {
				*op++ = *ep;
			}
			ep++;
		}
		else {
			n = *ep++ + 1;
			for (b = 0; b < n; b++) {
				*op++ = *ep++;
			}
		}
		tu += n;
		if(ep >= rd)
			break;
	}
	*length = tu;
}
