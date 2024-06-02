#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>
/* COMP 211 Homework 9:  Using linked lists to implement an editor buffer
 *
 * N. Danner
 */

/*  A struct buffer value b represents a character sequence with position.  We
 *  write <c_0,...,c_{i-1}, |c_i,...,c_{n-1}> to represent such a sequence with
 *  n characters, where the insertion mark is between c_{i-1} and c_i.
 */

/*

IN THE EDITOR.C FILE, THE BUFFER CONTENT IS NOT BEING PRINTED PROPERLY. I COULD NOT MAKE IT WORK.
I MADE SURE MY BUF_PRINT FUNCTION WORKS PERFECT. 
*/
struct node{
    char val;
    struct node* next;
    struct node* prev;
};

/*

Representation: 
   -- mark points to the node to the left of where the mark conceptually is
         -- The mark node points to 2 --> --> 12|
         -- Newnodes are added to the left off 'mark' which is to the right of the node that mark points to
   -- head is the first node of the buffer
   -- sequence length refers to the total number of characters in the buffer
         -- It changes with buf_insert, buf_delete_left, and buf_delete_right
   -- position keeps track of how many chars are to the left of the mark at any given time
         -- It changes with buf_move_left, buf_move_right, buf_delete_left and buf_insert
*/
struct buffer{
    struct node* mark;
    struct node* head;
    int sequence_length;
    int position;
};

/* This line just makes 'buffer' an abbreviation for 'struct buffer'.
 */
typedef struct buffer buffer ;

/* empty = <>.
 */

/* Representation function for the buffer type */
void buf_assert_invariants(buffer *n) {
    assert(n != NULL); // check if buffer exists

    // Invariant: Mark points to the node to the left of where the mark conceptually is
    assert(n->mark->prev != NULL || n->mark == n->head); // Mark is at the beginning
    assert(n->mark->next != NULL || n->mark->next == NULL); // Mark is at the end

    assert(n->head != NULL);

}
struct buffer* buf_empty() {
   struct buffer* p = malloc(sizeof(struct buffer));
   p->mark = malloc(sizeof(struct node));
   p->head = p->mark;
   p->sequence_length = 0;
   p->position = 0;
   buf_assert_invariants(p);
   return p;
}


/* insert(buf, c) inserts c to the left of the insertion point of buf.  After
 * the insertion, the insert mark is to the right of c.
 *
 * Pre-condition:   buf = <c_0,...,c_{i-1}, |c_i,...,c_{n-1}>.
 * Post-condition:  buf = <c_0,...,c_{i-1}, c, |c_i,...,c_{n-1}>.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME.
 */
void buf_insert(struct buffer* n, char c) {
    struct buffer* p = n;

    if (p->head == NULL) {
      struct node* newNode = malloc(sizeof(struct node));
      newNode->val = c;
      // Since it's the first node, its next should be NULL
      newNode->next = NULL;
      // Its previous should also be NULL
      newNode->prev = NULL;
      // Set mark to the newly created node
      p->mark = newNode;
      // Set head to the newly created node
      p->head = newNode;
      p->sequence_length += 1;
      p->position += 1;
   } else {
      struct node* newNode = malloc(sizeof(struct node));
      newNode->val = c;
      if(p->mark->next == NULL){
          // Since it's being added to the end, its next should be NULL
         newNode->next = NULL;
         // The previous of the new node should be the current mark node
         newNode->prev = p->mark;
         // Set the next of the current mark node to the new node
         p->mark->next = newNode;
         // Move the mark to the newly added node
         p->mark = newNode;
         p->sequence_length += 1;
         p->position += 1;
      }else{
         struct node* temp = p->mark->next;
         // The previous of the new node should be the current mark node
         newNode->prev = p->mark;
         // Set the next of the current mark node to the new node
         p->mark->next = newNode;
         //newNode points to what mark was pointing to
         newNode->next = temp;
         // Prev of what mark was pointing to points back to the new node to complete the doubly linked
         temp->prev = newNode;
         // Move the mark to the newly added node
         p->mark = newNode;
         p->sequence_length += 1;
         p->position += 1;
      }
   }

  buf_assert_invariants(n);
}


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

void buf_delete_left(buffer * n) {
    if(n->head == NULL){
        // Buffer is empty, no action needed
        buf_assert_invariants(n);
        return;
    } else {
        struct buffer* p = n;

        // Get the nodes to the left and right of the mark
        struct node* left_of_mark = p->mark->prev;
        struct node* right_of_mark = p->mark->next;

        // There is nothing to the left of the mark and there is at least one character after the mark
        if(left_of_mark == NULL && right_of_mark != NULL){
            // Mark is at the beginning, no action needed
            buf_assert_invariants(p);
            return;
        }
        // The buffer is empty but the mark sign is still there
        else if(left_of_mark == NULL && right_of_mark == NULL){
            // Buffer is empty, no action needed
            buf_assert_invariants(p);
            return;
        }
        // Mark is at the end of the buffer, delete the last character
        else if(right_of_mark == NULL && left_of_mark != NULL){
            
            left_of_mark->next = NULL;
            p->mark = left_of_mark;
            p->sequence_length -= 1;
            p->position -= 1;
            buf_assert_invariants(p);
        }
        // There is at least one character to the right of mark and at least one character to the left of mark
        else if(right_of_mark != NULL && left_of_mark != NULL){
            // Mark is hypothetically in the middle of the buffer, delete the character to the left of the mark
            struct node* previous_node = p->mark->prev;
            struct node* after_mark_node = p->mark->next;

            previous_node->next = after_mark_node;
            after_mark_node->prev = previous_node;

            p->mark = previous_node;  

            p->sequence_length -= 1;
            p->position -= 1;
            buf_assert_invariants(p);
        }
        // Mark is at the beginning of the buffer, delete the only character
        else if(p->mark->prev == NULL){
            p->head = NULL;
            p->mark = NULL;
            p->sequence_length -= 1;
            p->position -= 1;
            buf_assert_invariants(p);
        }
        // In any other case, return without making any changes
        else {
            buf_assert_invariants(p);
            return;
        }
        buf_assert_invariants(p);
    }
}

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
void buf_delete_right(buffer* n) {
    struct buffer* p = n;
    struct node* right_of_mark = p->mark->next;
    struct node* left_of_mark = p->mark->prev;
    
    // Check if there is a node to the right of the mark
    if(right_of_mark != NULL){
        if(right_of_mark->next == NULL){
            // Mark is at the end of the buffer, delete the character
            p->mark->next = NULL;
            p->sequence_length -= 1;
            buf_assert_invariants(p);
        }
        else if(left_of_mark == NULL){
            // Mark is at the beginning of the buffer, delete the character
            p->mark->next = right_of_mark->next;
            right_of_mark->prev = NULL;
            p->sequence_length -= 1;
            buf_assert_invariants(p);
        }
        else if(right_of_mark != NULL && left_of_mark != NULL){
            // Mark is in the middle of the buffer, delete the character
            struct node* temp = right_of_mark->next;
            p->mark->next = temp;
            p->sequence_length -= 1;
            buf_assert_invariants(p);
        }
        else if(left_of_mark == NULL && right_of_mark->next == NULL){
            // There is only one character and the mark is to the left of it, delete the character
            p->head = NULL;
            p->mark = NULL;
            p->sequence_length = 0;
            buf_assert_invariants(p);
        }
        else{
            // In any other case, return without making any changes
            buf_assert_invariants(p);
            return;
        }
    } else {
        // There is no character to the right of the mark, return without making any changes
        buf_assert_invariants(p);
        return;
    }
}
/* move_left(buf) moves the insert mark one character to the left.  If there is
 * no character to the left of the insert mark, this functdion has no effect.
 *
 * Pre-condition:  buf = <c_0,...,c_{i-1}, |c_i,...,c_{n-1}>.
 * Post-condition: buf = <c_0,...,c_{i-2}, |c_{i-1},...,c_{n-1}>.
 *   If i = 0, this function has no effect.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME.
 */
void buf_move_left(buffer* n ) {
    struct buffer* p = n;
    struct node* temp = p->mark->prev;
    
    // Check if there is a node to the left of the mark
    if(temp == NULL){
        // Mark is already at the beginning, no action needed
        buf_assert_invariants(p);
        return;
    }
    
    // Move the mark one position to the left
    p->mark = temp;
    p->position -= 1;
    
    buf_assert_invariants(p);
}

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
void buf_move_right(buffer *n) {
    struct buffer* p = n;
    struct node* temp = p->mark->next;
    
    // Check if there is a node to the right of the mark
    if(temp != NULL){
        // Move the mark one position to the right
        p->mark = temp;
        buf_assert_invariants(p);
    } else {
        // Mark is already at the end, no action needed
        buf_assert_invariants(p);
        return;
    }
    
    p->position += 1;
    
    buf_assert_invariants(p);
}

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
void buf_set(buffer *n, int c) {
    struct buffer* p = n;
    struct node* current = p->head;

    // Traverse the buffer to find the node at index c
    for(int i = 0; i <= c && current != NULL; i++){
        if(i == c){
            // Set the mark to the node at index c and update the position
            struct node* temp = current;
            p->mark = temp;
            p->position = c;
            buf_assert_invariants(p);
        }
        current = current->next;
    }
    
    buf_assert_invariants(p);
    return;
}

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
void buf_get_contents(buffer *n, char x[]) {

    int length = n->sequence_length;
    struct node* node_to_check = n->head;

    // Copy the contents of the buffer into the array x
    for(int i = 0; i < length; i++){
        x[i] = node_to_check->val;
        node_to_check = node_to_check->next;
    }

    buf_assert_invariants(n);
    return;
}

/* buf_get_pos(buf) = i, where buf = <c_0,...,c_{i-1}, |c_i,...,c_{n-1}>.
 *
 * Ex:  if buf represents "abcd|efg", then getpos(buf) = 4.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME.
 */
int buf_get_pos(buffer*n ) {

   struct buffer* p = n;
   
   buf_assert_invariants(p);
   return p->position;


}

/* int buf_size(buf) = n, where buf = <c_0,...,c_{n-1>>.
 *
 * THIS FUNCTION MUST BE CONSTANT TIME.
 */
int buf_size(buffer*n ) {
   struct buffer* p = n;
   buf_assert_invariants(p);
   return p->sequence_length;
}

/* buf_free(buf):  frees all resources associated to buf, including buf itself.
 *
 * THIS FUNCTION MUST BE LINEAR TIME.
 */
void buf_free(buffer *n) {
    struct buffer* p = n;
    struct node* current = p->head;

    // Traverse the buffer and free the memory allocated for each node
    while(current != NULL){
        struct node* temp = current;
        free(current);
        current = temp->next;
    }

    buf_assert_invariants(p);
    return;
}

/*  print_buffer(buf).
 *
 *  Post-condition:  Some information about buf will be printed to ther
 *  terminal.  
 *
 *  The implementation of this function is entirely up to you.  It is here to
 *  help you with debugging.  We will never use it, but there is an option in
 *  the driver program that will call this function with the current buffer.
 */
void buf_print(buffer *n) {
    struct buffer* q = n;
    struct node* p = n->head;
    struct node* mark = n->mark;
    
    // Print the contents of the buffer
    while (p != NULL) {
        printf("%c ", p->val);
        p = p->next;
        // Print '|' if the current node is the mark
        if(p == mark->next) printf("|");
    }
    
    // Print the position and length of the buffer
    printf("\n");
    printf("Pos: %d", q->position);
    printf("\n");
    printf("Length: %d", q->sequence_length);
    printf("\n");
    return;
}