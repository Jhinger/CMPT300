#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

/**
 * Sample test routine for executing each function at least once.
 * Copyright Brian Fraser, 2020
 */

// Macro for custom testing; does exit(1) on failure.
#define CHECK(condition) do{ \
    if (!(condition)) { \
        printf("ERROR: %s (@%d): failed condition \"%s\"\n", __func__, __LINE__, #condition); \
        exit(1);\
    }\
} while(0)



// For checking the "free" function called
static int complexTestFreeCounter = 0;
static void complexTestFreeFn(void* pItem) 
{
    CHECK(pItem != NULL);
    complexTestFreeCounter++;
}

//For List_search()
static bool searchCheck(void * pItem, void * pComparisonArg) {
    if (pItem == pComparisonArg) return true;
    return false;
}

// For searching
static bool itemEquals(void* pItem, void* pArg) 
{
    return (pItem == pArg);
}

static void testComplex()
{
    
    // Empty list
    List* pList = List_create();
    CHECK(pList != NULL);
    CHECK(List_count(pList) == 0);

    // Add 
    int added = 41;
    CHECK(List_add(pList, &added) == 0);
    CHECK(List_count(pList) == 1);
    CHECK(List_curr(pList) == &added);

    // Insert
    int inserted = 42;
    CHECK(List_insert(pList, &inserted) == 0);
    CHECK(List_count(pList) == 2);
    CHECK(List_curr(pList) == &inserted);
    
    // Prepend
    int prepended = 43;
    CHECK(List_prepend(pList, &prepended) == 0);
    CHECK(List_count(pList) == 3);
    CHECK(List_curr(pList) == &prepended);
    
    // Append
    int appended = 44;
    CHECK(List_append(pList, &appended) == 0);
    CHECK(List_count(pList) == 4);
    CHECK(List_curr(pList) == &appended);
    
    // Next through it all (from before list)
    CHECK(List_first(pList) == &prepended);
    CHECK(List_prev(pList) == NULL);
    CHECK(List_next(pList) == &prepended);
    CHECK(List_next(pList) == &inserted);
    CHECK(List_next(pList) == &added);
    CHECK(List_next(pList) == &appended);
    CHECK(List_next(pList) == NULL);
    CHECK(List_next(pList) == NULL);

    // Prev through it all
    //   starting from past end
    CHECK(List_last(pList) == &appended);
    CHECK(List_next(pList) == NULL);
    CHECK(List_prev(pList) == &appended);
    CHECK(List_prev(pList) == &added);
    CHECK(List_prev(pList) == &inserted);
    CHECK(List_prev(pList) == &prepended);
    CHECK(List_prev(pList) == NULL);
    CHECK(List_prev(pList) == NULL);

    // Remove first
    CHECK(List_first(pList) == &prepended);
    CHECK(List_remove(pList) == &prepended);
    CHECK(List_curr(pList) == &inserted);

    // Trim last
    CHECK(List_trim(pList) == &appended);
    CHECK(List_curr(pList) == &added);

    //Free remaining 2 elements
    complexTestFreeCounter = 0;
    List_free(pList, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 2);

    // Concat
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    List* pList1 = List_create();
    List_add(pList1, &one);
    List_add(pList1, &two);
    List* pList2 = List_create();
    List_add(pList2, &three);
    List_add(pList2, &four);
    
    List_concat(pList1, pList2);
    CHECK(List_count(pList1) == 4);
    CHECK(List_first(pList1) == &one);
    CHECK(List_last(pList1) == &four);      //free list = back increments, add list front increments


    // Search
    List_first(pList1);
    CHECK(List_search(pList1, itemEquals, &two) == &two);
    CHECK(List_search(pList1, itemEquals, &two) == &two);
    CHECK(List_search(pList1, itemEquals, &one) == NULL); 

    //---------------Gurshan Jhinger - Beginning my own testing here.---------------
    //---------------List_free(pList1, complexTestFreeFn);--------------------------

    List_free(pList1, complexTestFreeFn);

    //Test creating an 11th list.
    List * list1 = List_create();       
    List * list2 = List_create();     
    List * list3 = List_create();
    List * list4 = List_create();
    List * list5 = List_create();
    List * list6 = List_create();
    List * list7 = List_create();
    List * list8 = List_create();
    List * list9 = List_create();
    List * list10 = List_create();
    List * listOutOfRange = List_create();
    CHECK(listOutOfRange == NULL); 
    CHECK(list10 != NULL);
    CHECK(list9 != NULL);

    //Test that no nodes are in list after create
    CHECK(List_count(list10) >= 0);
    CHECK(List_first(list10) == NULL);
    CHECK(List_last(list3) == NULL);
    CHECK(List_curr(list5) == NULL);
    CHECK(List_remove(list6) == NULL);

    //Check that freeing a list, and then creating one works.
    List_free(list10, complexTestFreeFn);
    List * list11 = List_create();
    CHECK(list11 != NULL);                           

    int item1 = 1;
    int item2 = 2;
    int item3 = 3;
    int item4 = 4;
    int item5 = 5;

    //Testing adding items to the list
    CHECK(List_add(list1, &item1) == 0);
    CHECK(List_curr(list1) == &item1);
    CHECK(List_prev(list1) == NULL);
    CHECK(List_next(list1) == &item1);
    CHECK(List_insert(list1, &item2) == 0);
    CHECK(List_next(list1) == &item1);
    CHECK(List_append(list1, &item3) == 0);
    CHECK(List_curr(list1) == &item3);
    CHECK(List_append(list1, &item4) == 0);
    CHECK(List_last(list1) == &item4);
    CHECK(List_curr(list1) == &item4);
    CHECK(List_prepend(list1, &item5) == 0);
    CHECK(List_first(list1) == &item5);
    CHECK(List_count(list1) == 5);
    CHECK(List_prev(list1) == NULL);
    CHECK(List_prev(list1) == NULL);
    CHECK(List_next(list1) == &item5);
    List_last(list1);
    CHECK(List_remove(list1) == &item4);
    CHECK(List_last(list1) == &item3);

    //Test overfill of nodes;
    int nodeItem1 = 1;
    int nodeItem2 = 2;
    int nodeItem3 = 3;
    int nodeItem4 = 4;
    int nodeItem5 = 5;
    int nodeItem6 = 6;
    int nodeItem7 = 7;
    int nodeItem8 = 8;
    int nodeItem9 = 9;
    int nodeItem10 = 10;
    int nodeItem11 = 11;
    int nodeItem12 = 12;
    int nodeItem13 = 13;
    int nodeItem14 = 14;
    int nodeItem15 = 15;
    int nodeItem16 = 16;
    int nodeItem17 = 17;
    int nodeItem18 = 18;
    int nodeItem19 = 19;
    int nodeItem20 = 20;
    int nodeItem21 = 21;
    int nodeItem22 = 22;
    int nodeItem23 = 23;
    int nodeItem24 = 24;
    int nodeItem25 = 25;
    int nodeItem26 = 26;
    int nodeItem27 = 27;
    int nodeItem28 = 28;
    int nodeItem29 = 29;
    int nodeItem30 = 30;
    int nodeItem31 = 31;
    int nodeItem32 = 32;
    int nodeItem33 = 33;
    int nodeItem34 = 34;
    int nodeItem35 = 35;
    int nodeItem36 = 36;
    int nodeItem37 = 37;
    int nodeItem38 = 38;
    int nodeItem39 = 39;
    int nodeItem40 = 40;
    int nodeItem41 = 41;
    int nodeItem42 = 42;
    int nodeItem43 = 43;
    int nodeItem44 = 44;
    int nodeItem45 = 45;
    int nodeItem46 = 46;
    int nodeItem47 = 47;
    int nodeItem48 = 48;
    int nodeItem49 = 49;
    int nodeItem50 = 50;
    int nodeItem51 = 51;
    int nodeItem52 = 52;
    int nodeItem53 = 53;
    int nodeItem54 = 54;
    int nodeItem55 = 55;
    int nodeItem56 = 56;
    int nodeItem57 = 57;
    int nodeItem58 = 58;
    int nodeItem59 = 59;
    int nodeItem60 = 60;
    int nodeItem61 = 61;
    int nodeItem62 = 62;
    int nodeItem63 = 63;
    int nodeItem64 = 64;
    int nodeItem65 = 65;
    int nodeItem66 = 66;
    int nodeItem67 = 67;
    int nodeItem68 = 68;
    int nodeItem69 = 69;
    int nodeItem70 = 70;
    int nodeItem71 = 71;
    int nodeItem72 = 72;
    int nodeItem73 = 73;
    int nodeItem74 = 74;
    int nodeItem75 = 75;
    int nodeItem76 = 76;
    int nodeItem77 = 77;
    int nodeItem78 = 78;
    int nodeItem79 = 79;
    int nodeItem80 = 80;
    int nodeItem81 = 81;
    int nodeItem82 = 82;
    int nodeItem83 = 83;
    int nodeItem84 = 84;
    int nodeItem85 = 85;
    int nodeItem86 = 86;
    int nodeItem87 = 87;
    int nodeItem88 = 88;
    int nodeItem89 = 89;
    int nodeItem90 = 90;
    int nodeItem91 = 91;
    int nodeItem92 = 92;
    int nodeItem93 = 93;
    int nodeItem94 = 94;
    int nodeItem95 = 95;
    int nodeItem96 = 96;
    int nodeItem97 = 97;
    int nodeItem98 = 98;
    int nodeItem99 = 99;
    int nodeItem100 = 100;
    int nodeItemOverflow = 101;

    List_free(list1, complexTestFreeFn);
    List_free(list2, complexTestFreeFn);
    List_free(list3, complexTestFreeFn);
    List_free(list4, complexTestFreeFn);
    List_free(list5, complexTestFreeFn);
    List_free(list6, complexTestFreeFn);
    List_free(list7, complexTestFreeFn);
    List_free(list8, complexTestFreeFn);
    List_free(list9, complexTestFreeFn);
    List_free(list11, complexTestFreeFn);

    List * list13 = List_create();
    List * list12 = List_create();
    List * list14 = List_create();
    CHECK(list13 != NULL);
    CHECK(list13 != NULL);
    CHECK(list14 != NULL);
    CHECK(List_add(list12, &nodeItem97) == 0);
    CHECK(List_add(list12, &nodeItem98) == 0);
    CHECK(List_add(list12, &nodeItem99) == 0);

    List_free(list13, complexTestFreeFn);
    List_free(list14, complexTestFreeFn);
    List_free(list12, complexTestFreeFn);

    List * testList = List_create();
    CHECK(testList != NULL);
    CHECK(List_add(testList, &nodeItem1) == 0);
    CHECK(List_insert(testList, &nodeItem2) == 0);
    CHECK(List_append(testList, &nodeItem3) == 0);
    CHECK(List_prepend(testList, &nodeItem4) == 0);
    CHECK(List_add(testList, &nodeItem5) == 0);
    CHECK(List_prepend(testList, &nodeItem6) == 0);
    CHECK(List_insert(testList, &nodeItem7) == 0);
    CHECK(List_insert(testList, &nodeItem8) == 0);
    CHECK(List_add(testList, &nodeItem9) == 0);
    CHECK(List_insert(testList, &nodeItem10) == 0);
    CHECK(List_prepend(testList, &nodeItem11) == 0);
    CHECK(List_append(testList, &nodeItem12) == 0);
    CHECK(List_prepend(testList, &nodeItem13) == 0);
    CHECK(List_add(testList, &nodeItem14) == 0);
    CHECK(List_add(testList, &nodeItem15) == 0);
    CHECK(List_insert(testList, &nodeItem16) == 0);
    CHECK(List_append(testList, &nodeItem17) == 0);
    CHECK(List_append(testList, &nodeItem18) == 0);
    CHECK(List_insert(testList, &nodeItem19) == 0);
    CHECK(List_prepend(testList, &nodeItem20) == 0);
    CHECK(List_add(testList, &nodeItem21) == 0);
    CHECK(List_prepend(testList, &nodeItem22) == 0);
    CHECK(List_insert(testList, &nodeItem23) == 0);
    CHECK(List_add(testList, &nodeItem24) == 0);
    CHECK(List_add(testList, &nodeItem25) == 0);
    CHECK(List_add(testList, &nodeItem26) == 0);
    CHECK(List_insert(testList, &nodeItem27) == 0);
    CHECK(List_prepend(testList, &nodeItem28) == 0);
    CHECK(List_append(testList, &nodeItem29) == 0);
    CHECK(List_append(testList, &nodeItem30) == 0);
    CHECK(List_insert(testList, &nodeItem31) == 0);
    CHECK(List_append(testList, &nodeItem32) == 0);
    CHECK(List_prepend(testList, &nodeItem33) == 0);
    CHECK(List_add(testList, &nodeItem34) == 0);
    CHECK(List_add(testList, &nodeItem35) == 0);
    CHECK(List_insert(testList, &nodeItem36) == 0);
    CHECK(List_append(testList, &nodeItem37) == 0);
    CHECK(List_append(testList, &nodeItem38) == 0);
    CHECK(List_insert(testList, &nodeItem39) == 0);
    CHECK(List_count(testList) == 39);
    CHECK(List_prepend(testList, &nodeItem40) == 0);
    CHECK(List_curr(testList) == &nodeItem40);
    CHECK(List_insert(testList, &nodeItem41) == 0);
    CHECK(List_prepend(testList, &nodeItem42) == 0);
    CHECK(List_append(testList, &nodeItem43) == 0);
    CHECK(List_insert(testList, &nodeItem44) == 0);
    CHECK(List_add(testList, &nodeItem45) == 0);
    CHECK(List_add(testList, &nodeItem46) == 0);
    CHECK(List_append(testList, &nodeItem47) == 0);
    CHECK(List_prepend(testList, &nodeItem48) == 0);
    CHECK(List_insert(testList, &nodeItem49) == 0);
    CHECK(List_insert(testList, &nodeItem50) == 0);
    CHECK(List_prepend(testList, &nodeItem51) == 0);

    //Checking that insert inserts at the start of the list if current is before the List.
    List_prev(testList);
    CHECK(List_curr(testList) == NULL);
    CHECK(List_insert(testList, &nodeItem52) == 0);
    CHECK(List_first(testList) == &nodeItem52);

    //Checking that insert inserts at the end of the list if current is beyond the List.
    CHECK(List_append(testList, &nodeItem53) == 0); //Move current to tail.
    CHECK(List_curr(testList) == &nodeItem53);
    List_next(testList);
    CHECK(List_curr(testList) == NULL);
    CHECK(List_insert(testList, &nodeItem54) == 0);
    CHECK(List_last(testList) == &nodeItem54);      //Checking that new current is on the tail of the list.

    CHECK(List_add(testList, &nodeItem55) == 0);
    CHECK(List_insert(testList, &nodeItem56) == 0);
    CHECK(List_append(testList, &nodeItem57) == 0);
    CHECK(List_add(testList, &nodeItem58) == 0);    //Regularly add the rest as we have confirmed insert, add, prepend, append work as intended.
    CHECK(List_add(testList, &nodeItem59) == 0);
    CHECK(List_add(testList, &nodeItem60) == 0);
    CHECK(List_add(testList, &nodeItem61) == 0);
    CHECK(List_add(testList, &nodeItem62) == 0);
    CHECK(List_add(testList, &nodeItem63) == 0);
    CHECK(List_add(testList, &nodeItem64) == 0);
    CHECK(List_add(testList, &nodeItem65) == 0);
    CHECK(List_add(testList, &nodeItem66) == 0);
    CHECK(List_add(testList, &nodeItem67) == 0);
    CHECK(List_add(testList, &nodeItem68) == 0);
    CHECK(List_add(testList, &nodeItem69) == 0);
    CHECK(List_add(testList, &nodeItem70) == 0);
    CHECK(List_add(testList, &nodeItem71) == 0);
    CHECK(List_add(testList, &nodeItem72) == 0);
    CHECK(List_add(testList, &nodeItem73) == 0);
    CHECK(List_add(testList, &nodeItem74) == 0);
    CHECK(List_add(testList, &nodeItem75) == 0);
    CHECK(List_add(testList, &nodeItem76) == 0);
    CHECK(List_add(testList, &nodeItem77) == 0);
    CHECK(List_add(testList, &nodeItem78) == 0);
    CHECK(List_add(testList, &nodeItem79) == 0);
    CHECK(List_add(testList, &nodeItem80) == 0);
    CHECK(List_add(testList, &nodeItem81) == 0);
    CHECK(List_add(testList, &nodeItem82) == 0);
    CHECK(List_add(testList, &nodeItem83) == 0);
    CHECK(List_add(testList, &nodeItem84) == 0);
    CHECK(List_add(testList, &nodeItem85) == 0);
    CHECK(List_add(testList, &nodeItem86) == 0);
    CHECK(List_add(testList, &nodeItem87) == 0);
    CHECK(List_add(testList, &nodeItem88) == 0);
    CHECK(List_add(testList, &nodeItem89) == 0);
    CHECK(List_add(testList, &nodeItem90) == 0);
    CHECK(List_add(testList, &nodeItem91) == 0);
    CHECK(List_add(testList, &nodeItem92) == 0);
    CHECK(List_add(testList, &nodeItem93) == 0);
    CHECK(List_add(testList, &nodeItem94) == 0);
    CHECK(List_add(testList, &nodeItem95) == 0);
    CHECK(List_add(testList, &nodeItem96) == 0);
    CHECK(List_add(testList, &nodeItem97) == 0);
    CHECK(List_add(testList, &nodeItem98) == 0);
    CHECK(List_add(testList, &nodeItem99) == 0);
    CHECK(List_append(testList, &nodeItem100) == 0);

    CHECK(List_add(testList, &nodeItemOverflow) == -1);
    CHECK(List_prepend(testList, &nodeItemOverflow) == -1);
    CHECK(List_append(testList, &nodeItemOverflow) == -1);
    CHECK(List_insert(testList, &nodeItemOverflow) == -1);

    //Testing Removing
    //Testing removing the last item in the list
    List_last(testList);
    CHECK(List_remove(testList) == &nodeItem100);
    CHECK(List_curr(testList) == NULL);

    //Testing removing the first item in the list.
    List_first(testList);
    CHECK(List_remove(testList) == &nodeItem52);
    CHECK(List_curr(testList) == &nodeItem51);
    
    //Testing Freeing
    //Checking that list is NULL after free'd
    CHECK(List_count(testList) == 98);
    List_free(testList, complexTestFreeFn);
    CHECK(List_count(testList) == 0);

    //Testing Concatenating:
    List * concatTest1 = List_create();
    List * concatTest2 = List_create();
    List * concatTest3 = List_create();
    List * concatTest4 = List_create();
    List * concatTest5 = List_create();
    List * concatTest6 = List_create();
    List * concatTest7 = List_create();
    List * concatTest8 = List_create();
    int concatItem1 = 1;
    int concatItem2 = 2;
    int concatItem3 = 3;
    int concatItem4 = 4;
    int concatItem5 = 5;
    int concatItem6 = 6;
    int concatItem7 = 7;
    int concatItem8 = 8;

    //empty list + empty list
    List_concat(concatTest1, concatTest2);
    CHECK(List_count(concatTest2) == 0);
    CHECK(List_count(concatTest1) == 0);
    List_free(concatTest1, complexTestFreeFn);

    //unempty list + empty list
    CHECK(List_add(concatTest3, &concatItem1) == 0);
    CHECK(List_add(concatTest3, &concatItem2) == 0);
    List_concat(concatTest3, concatTest4);
    CHECK(List_count(concatTest3) == 2);
    List_free(concatTest3, complexTestFreeFn);

    //empty list + unempty list
    CHECK(List_add(concatTest6, &concatItem3) == 0);
    CHECK(List_add(concatTest6, &concatItem4) == 0);
    List_concat(concatTest5, concatTest6);
    CHECK(List_count(concatTest5) == 2);
    CHECK(List_count(concatTest6) == 0);
    List_free(concatTest5, complexTestFreeFn);

    //unempty list + unempty list
    CHECK(List_add(concatTest7, &concatItem5) == 0);
    CHECK(List_add(concatTest7, &concatItem6) == 0);
    CHECK(List_add(concatTest8, &concatItem7) == 0);
    CHECK(List_add(concatTest8, &concatItem8) == 0);
    List_concat(concatTest7, concatTest8);
    CHECK(List_count(concatTest7) == 4);
    CHECK(List_count(concatTest8) == 0);
    List_free(concatTest7, complexTestFreeFn);

    //Testing Searching:
    //Test searching when the item exists
    List * searchList = List_create();
    int searchItem1 = 1;
    int searchItem2 = 2;
    int searchItem3 = 3;
    int searchItem4 = 4;
    int searchItem5 = 5;
    int searchItem6 = 6;
    int searchItem7 = 7;
    int searchItem8 = 8;
    int searchItem9 = 9;
    int searchItem10 = 10;
    int searchItemDNE = 11;
    CHECK(List_add(searchList, &searchItem1) == 0);
    CHECK(List_add(searchList, &searchItem2) == 0);
    CHECK(List_add(searchList, &searchItem3) == 0);
    CHECK(List_add(searchList, &searchItem4) == 0);
    CHECK(List_add(searchList, &searchItem5) == 0);
    CHECK(List_add(searchList, &searchItem6) == 0);
    CHECK(List_add(searchList, &searchItem7) == 0);
    CHECK(List_add(searchList, &searchItem8) == 0);
    CHECK(List_add(searchList, &searchItem9) == 0);
    CHECK(List_add(searchList, &searchItem10) == 0);
    List_first(searchList);
    CHECK(List_search(searchList, searchCheck, &searchItem10) == &searchItem10);
    CHECK(List_curr(searchList) == &searchItem10);

    //Test searching when the item does not exist.
    List_first(searchList);
    CHECK(List_search(searchList, searchCheck, &searchItemDNE) == NULL);
    CHECK(List_curr(searchList) == NULL);
    
    //Testing Trimming:
    CHECK(List_trim(searchList) == &searchItem10);
    CHECK(List_curr(searchList) == &searchItem9);
    CHECK(List_trim(searchList) == &searchItem9);
    CHECK(List_curr(searchList) == &searchItem8);

    //Testing trim returns NULL if list is empty
    List_free(searchList, complexTestFreeFn);
    CHECK(List_trim(searchList) == NULL);

    //
}

int main(int argCount, char *args[]) 
{
    testComplex();

    // We got here?!? PASSED!
    printf("********************************\n");
    printf("           PASSED\n");
    printf("********************************\n");
    return 0;
}  
 