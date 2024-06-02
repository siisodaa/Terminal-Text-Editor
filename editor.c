/* COMP 211 HW 9:  Using linked lists to implement an editor buffer
 *
 * Driver program.
 *
 * N. Danner
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "editbuffer.h"

/* The maximum length of the string of characters that the user can enter when
 * inserting characters.
 */
#define MAX_INSERT_CHARS 48

/* The maximum number of characters that will be printed when displaying the
 * contents of the editor buffer.
 */
#define MAX_PRINT_CHARS 80

/* A menu_opt structure describes a menu item.  A menu item consists of a
 * description and a function to call.
 *
 * This makes use of some C that we haven't learned---a structure field can be
 * a function!  The definition of the act field in this structure definition
 * says that act is a void function (procedure) that takes no parameters.
 */
struct menu_opt {
    char* desc ;
    void (*act)(void) ;
} ;

/* Yes, I know I have said to not use global variables, except for constants.
 * But this program does in fact require a single global variable for the
 * (pointer to the) editor buffer, because it defines many different functions
 * that all need to modify it.
 */
buffer *buf = NULL ;


/* Menu actions.  Each action corresponds to one of the actions that can be
 * performed on an editor buffer.
 */

/* Exit the program.
 */
void do_exit() {
    if (buf != NULL) buf_free(buf) ;
    exit(0) ;
}

/* Set buf to a pointer to an empty editor buffer.
 */
void do_make_empty() {
    if (buf != NULL) buf_free(buf) ;
    buf = buf_empty() ;
}

/* Get a string from the user and then call buf_insert on each of the
 * characters in that string.
 */
void do_insert() {
    char s[MAX_INSERT_CHARS] ;
    printf("Enter characters (max %d, ends at newline, "
            "other whitespace ignored): ", 
            MAX_INSERT_CHARS) ;
    scanf(" %s", s) ;
    for (int i=0; s[i] != '\0'; i+=1) buf_insert(buf, s[i]) ;
    return ;
}

/* Call buf_delete_left(buf).
 */
void do_delete_left() {
    buf_delete_left(buf) ;
} ;

/* Call buf_delete_right(buf).
 */
void do_delete_right() {
    buf_delete_right(buf) ;
} ;

/* Call buf_move_left(buf).
 */
void do_move_left() {
    buf_move_left(buf) ;
} ;

/* Call buf_move_right(buf).
 */
void do_move_right() {
    buf_move_right(buf) ;
} ;

/* Get a position n from the user and then call buf_set_pos(buf, n).
 */
void do_set() {
    int n ;
    printf("Enter position: ") ;
    scanf("%d", &n) ;
    buf_set(buf, n) ;
}

/* Call buf_print(buf).
 */
void do_print() {
    buf_print(buf) ;
}

int main(void) {

    /* Menu items.  A menu item consists of:
     * - A string to print to the user.
     * - A function to call if the user selects that item.
     */
    struct menu_opt menu[] = {
        {"Exit", do_exit},
        {"Create empty buffer", do_make_empty},
        {"Insert characters", do_insert},
        {"Delete left", do_delete_left},
        {"Delete right", do_delete_right},
        {"Move insert mark left", do_move_left},
        {"Move insert mark right", do_move_right},
        {"Set insert mark position", do_set} ,
        {"Print buffer", do_print}
    } ;

    /* WARNING:  this method for computing the size of an array only works when
     * the array is declared in the local binding table.  In particular, it is
     * not valid when the array is allocated using malloc, calloc, etc., nor
     * when the array is passed as a parameter.
    */
    int n_menu = sizeof(menu)/sizeof(struct menu_opt) ;

    /* User's menu choice.
     */
    int choice ;

    while (true) {

        /* Display the menu item descriptions (menu[i].desc) and let the user
         * choose one.
         */
        for (int i=0; i<n_menu; ++i) {
            printf("(%d) %s\n", i, menu[i].desc) ;
        }
        do {
            printf("Enter choice: ") ;
            scanf("%d", &choice) ;
        } while (choice < 0 || choice >= n_menu) ;

        /* Call the function part of the menu item (menu[choice].act).
         */
        menu[choice].act() ;

        /* Print the contents of the buffer.
         */

        /* Contents of the buffer and location of the insert mark.
         */
        int n = buf_size(buf) ;
        char s[n] ;
        int insert ;

        buf_get_contents(buf, s) ;
        insert = buf_get_pos(buf) ;

        /* Print up to MAX_PRINT_CHARS characters from the buffer.  This is a
         * safety issue, in case buf_size() returns a nonsensically large
         * value.  Notice the guard on printing "|" for the insertion mark, so
         * if there are more than MAX_PRINT_CHARS in the buffer, "|" will not
         * be printed.
         */
        printf("Buffer contents (up to %d characters):\n", MAX_PRINT_CHARS) ;
        int pc = n <= MAX_PRINT_CHARS ? n : MAX_PRINT_CHARS ;
        int i ;
        for (i=0; i<insert && i < pc; i+=1) printf("%c", s[i]) ;
        if (i == insert) printf("|") ;
        for (int i=insert; i < pc; i+=1) 
            printf("%c", s[i]) ;
        printf("\n") ;

        printf("\n") ;
    }

    return 0 ;
}