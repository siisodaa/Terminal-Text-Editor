/* COMP 211 Homework 9:  Using linked lists to implement an editor buffer
 *
 * N. Danner
 */

/*  A struct buffer value b represents a character sequence with position.  We
 *  write <c_0,...,c_{i-1}, |c_i,...,c_{n-1}> to represent such a sequence with
 *  n characters, where the insertion mark is between c_{i-1} and c_i.
 */
struct buffer ;

/* This line just makes 'buffer' an abbreviation for 'struct buffer'.
 */
typedef struct buffer buffer ;

/* empty = <>.
 */
buffer* buf_empty() ;

/* insert(buf, c) inserts c to the left of the insertion point of buf.  After
 * the insertion, the insert mark is to the right of c.
 *
 * Pre-condition:   buf = <c_0,...,c_{i-1}, |c_i,...,c_{n-1}>.
 * Post-condition:  buf = <c_0,...,c_{i-1}, c, |c_i,...,c_{n-1}>.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME.
 */
void buf_insert(buffer *, char) ;

/* delete_left(buf) deletes the character to the left of the insert mark.  If
 * there is no character to the left of the insert mark, this function has no
 * effect.
 *
 * Pre-condition:  buf = <c_0,...,c_{i-1}, |c_i,...,c_{n-1}>.
 * Post-condition: buf = <c_0,...,c_{i-2}, |c_i,...,c_{n-1}>.
 *   If i=0, this function has no effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME.
 */
void buf_delete_left(buffer *) ;

/* delete_right(buf) deletes the character to the right of the insert mark.  If
 * there is no character to the right of the insert mark, this function has no
 * effect.
 *
 * Pre-condition:  buf = <c_0,...,c_{i-1}, |c_i,...,c_{n-1}>.
 * Post-condition: buf = <c_0,...,c_{i-1}, |c_{i+1},...,c_{n-1}>.
 *   If i=n, this function has no effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME.
 */
void buf_delete_right(buffer *) ;

/* move_left(buf) moves the insert mark one character to the left.  If there is
 * no character to the left of the insert mark, this functdion has no effect.
 *
 * Pre-condition:  buf = <c_0,...,c_{i-1}, |c_i,...,c_{n-1}>.
 * Post-condition: buf = <c_0,...,c_{i-2}, |c_{i-1},...,c_{n-1}>.
 *   If i = 0, this function has no effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME.
 */
void buf_move_left(buffer *) ;

/* move_right(buf) moves the insert mark one character to the right.  If there
 * is no character to the right of the insert mark, this functdion has no
 * effect.
 *
 * Pre-condition:  buf = <c_0,...,c_{i-1}, |c_i,...,c_{n-1}>.
 * Post-condition: buf = <c_0,...,c_i, |c_{i+1},...,c_{n-1}>.
 *   If i = n, this function has no effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME.
 */
void buf_move_right(buffer *) ;

/* set(buf, i) sets the insert mark so that it is to the left of the i-th
 * character.  Characters are 0-indexed, so set(buf, 0) sets the insert mark to
 * the beginning of the buffer.  i may be equal to the length of the buffer; in
 * this case, the insert mark is set to the right of the last character.
 *
 * Pre-condition:  buf = <c_0,...,c_{n-1}>, 0 <= i <= n.
 * Post-condition: buf = <c_0,...,c_{i-1}, |c_i,...,c_{n-1}>.
 *
 * THIS FUNCTION MUST BE LINEAR TIME.
 *
 */
void buf_set(buffer *, int) ;

/* buf_get_line(buf, s).
 *
 * Pre-condition:  buf = <c_0,...,c_{n-1}>, length(s) ≥ n.
 *
 * Post-condition:  s[i] = c_i for 0 ≤ i < n.
 *
 * Ex:  if buf represents "abcd|efg", then s must have size at least 7 and
 * after calling buf_get_line(buf, s), s will start with {'a', 'b', 'c', 'd',
 * 'e', 'f', 'g', ...}, with the characters after 'g' unspecified.
 *
 * THIS FUNCTION MUST BE LINEAR TIME.
 */
void buf_get_contents(buffer*, char []) ;

/* buf_get_pos(buf) = i, where buf = <c_0,...,c_{i-1}, |c_i,...,c_{n-1}>.
 *
 * Ex:  if buf represents "abcd|efg", then getpos(buf) = 4.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME.
 */
int buf_get_pos(buffer*) ;

/* int buf_size(buf) = n, where buf = <c_0,...,c_{n-1>>.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME.
 */
int buf_size(buffer*) ;

/* buf_free(buf):  frees all resources associated to buf, including buf itself.
 *
 * THIS FUNCTION MUST BE LINEAR TIME.
 */
void buf_free(buffer*) ;

/*  print_buffer(buf).
 *
 *  Post-condition:  Some information about buf will be printed to ther
 *  terminal.  
 *
 *  The implementation of this function is entirely up to you.  It is here to
 *  help you with debugging.  We will never use it, but there is an option in
 *  the driver program that will call this function with the current buffer.
 */
void buf_print(buffer *) ;
