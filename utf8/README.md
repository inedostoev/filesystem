# utf8 converter and decoder

# Convert
```C
/* to_utf8: convert to utf-8
 * @param n_blocks - ptr to number of 8 bit blocks. Calc in func.
 * @param x - input number
 * @return encoded number
 */
uint8_t *to_utf8(uint8_t *n_blocks, const uint32_t *x);
```

# Decode
```C
/* from_utf8: from utf8 to unicode
 * @param x - input utf8 code
 * @return decoded number
 */
uint32_t from_utf8(const uint8_t *x);
```
