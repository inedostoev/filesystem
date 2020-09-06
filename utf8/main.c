#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

/* to_utf8: convert to utf-8
 * @param n_blocks - ptr to number of 8 bit blocks. Calc in func.
 * @param x - input number
 * @return encoded number
 */
uint8_t *to_utf8(uint8_t *n_blocks, const uint32_t *x);

/* from_utf8: from utf8 to  unicode
 * @param x - input utf8 code
 * @return decoded number
 */
uint32_t from_utf8(const uint8_t *x);


uint8_t *to_utf8(uint8_t *n_blocks, const uint32_t *x) {
	if(*x <= 0x7F) *n_blocks = 1;
	else if(*x <= 0x7FF) *n_blocks = 2;
	else if(*x <= 0xFFFF) *n_blocks = 3;
	else if(*x <= 0x1FFFFF) *n_blocks = 4;
	else {
		printf("Error: in func to_utf8: *x > 0x1FFFFF\n");
		exit(1);
	}
	
	uint8_t *res = (uint8_t*)malloc(sizeof(uint8_t) * (*n_blocks));
	
	uint8_t a = (uint8_t)((*x) & 0x3F);
	uint8_t b = (uint8_t)(((*x) >> 6) & 0x3F);
	uint8_t c = (uint8_t)(((*x) >> 12) & 0x3F);
	uint8_t d = (uint8_t)((*x) >> 18);
	switch(*n_blocks) {
		case 1:
			res[0] = *x;
			break;
		case 2:
			res[0] = b | 0xC0;
			res[1] = a | 0x80;
			break;
		case 3:
			res[0] = c | 0xE0;
			res[1] = b | 0x80;
			res[2] = a | 0x80;
			break;
		case 4:
			res[0] = d | 0xF0;
			res[1] = c | 0x80;
			res[2] = b | 0x80;
			res[3] = a | 0x80;
			break;
		default: printf("Error: in func to_utf8: wrong n_blocks\n");
			 exit(1);
			 break;
	}
	return res;
}

uint32_t from_utf8(const uint8_t *x) {
	if(!x) {
		printf("Error: in func from_utf8: x == NULL\n");
		exit(1);
	}
	uint8_t n_blocks = 0;
	uint8_t b1 = 0;
	
	if((x[0] >> 3) == 0x1E) {
		n_blocks = 4;
		b1 = x[0] & 0x7;
	}
	else if((x[0] >> 4) == 0xE) {
		n_blocks = 3;
		b1 = x[0] & 0xF;
	}
	else if((x[0] >> 5) == 0x6) {
		n_blocks = 2;
		b1 = x[0] & 0x1F;
	}
	else if((x[0] >> 7) == 0) {
		n_blocks = 1;
		b1 = x[0];
	}
	else {
		printf("Error: in from_utf8: wrong format\n");
		exit(1);
	}

	uint8_t i = 0;
	uint32_t res = (uint32_t)b1;
	for(i = 1; i < n_blocks; i++) {
		res = res * 0x40 + (x[i] & 0x3F);
	}
	return res;
}

int encode(uint32_t x) {
	uint8_t n = 0;	
	uint8_t *res = to_utf8(&n, &x);

	
	printf("to_utf8\nInput: %u\nOutput: ", x);
	int i = 0;
	for(i = 0; i < n; i++) 
		printf("%x ", res[i]); //%hhu
	printf("\n");

	uint32_t res2 = from_utf8(res);
	printf("\nfrom_utf8\nInput: ");
	for(i = 0; i < n; i++) 
		printf("%x ", res[i]); //%hhu
	printf("\n");
	printf("Output: %u\n", res2);
	
	return 0;
}

int help() {
	printf("Options:\n[-e] encode number\nEx.: ./encoder -e 123456\n");
	return 0;
}

int main(int argc, char *argv[]) {
	
	char *opts = "e";
	int opt;
	if(argc == 1) {help(); exit(1);}
	while ((opt = getopt(argc, argv, opts)) != -1) {
		switch(opt) {
			case 'e':
				encode(atoi(argv[optind]));
				break;
			default: 
				help();
				break;
		}
	}

	return 0;
}
