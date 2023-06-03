#include "HumanPyramids.h"
#include "error.h"
using namespace std;

/* TODO: Refer to HumanPyramids.h for more information about what this function should do.
 * Then, delete this comment.
 */
double weightOnBackRec(int row, int col, int pyramidHeight, Grid<double>& table) {
    if (row < 0 || row >= pyramidHeight || col < 0 || col > row) {
        error("Location out of range");
    }

    if (row == 0 && col == 0) {
        return 0;
    } else if (table[row][col] != 0) {
        return table[row][col];
    } else {
        double ans = 0;
        if (col > 0) {
            ans += 80 + weightOnBackRec(row-1, col-1, pyramidHeight, table)/2;
        }
        if (col < row) {
            ans += 80 + weightOnBackRec(row-1, col, pyramidHeight, table)/2;
        }
        table[row][col] = ans;

        return ans;
    }
}


double weightOnBackOf(int row, int col, int pyramidHeight) {
    /* TODO: Delete the next few lines and implement this function.
     * (void) row;
     * (void) col;
     * (void) pyramidHeight;
     * return 0;
     */
    Grid<double> table(pyramidHeight, pyramidHeight);

    return weightOnBackRec(row, col, pyramidHeight, table);
}


/* * * * * * Test Cases * * * * * */
#include "GUI/SimpleTest.h"

/* TODO: Add your own tests here. You know the drill - look for edge cases, think about
 * very small and very large cases, etc.
 */


STUDENT_TEST("Check Person at(0,0)") {
    EXPECT_EQUAL(weightOnBackOf(0, 0, 3), 0);
}


STUDENT_TEST("Check Person at(0,0)") {
    EXPECT_EQUAL(weightOnBackOf(4, 2, 5), 500);
}



/* * * * * * Test cases from the starter files below this point. * * * * * */

PROVIDED_TEST("Check Person E from the handout.") {
    /* Person E is located at row 2, column 1. */
    EXPECT_EQUAL(weightOnBackOf(2, 1, 5), 240);
}

PROVIDED_TEST("Function reports errors in invalid cases.") {
    EXPECT_ERROR(weightOnBackOf(-1, 0, 10));
    EXPECT_ERROR(weightOnBackOf(10, 10, 5));
    EXPECT_ERROR(weightOnBackOf(-1, 10, 20));
}

PROVIDED_TEST("Stress test: Memoization is implemented (should take under a second)") {
    /* TODO: Yes, we are asking you to make a change to this test case! Delete the
     * line immediately after this one - the one that starts with SHOW_ERROR - once
     * you have implemented memoization to test whether it works correctly.
     */
//    SHOW_ERROR("This test is configured to always fail until you delete this line from\n         HumanPyramids.cpp. Once you have implemented memoization and want\n         to check whether it works correctly, remove the indicated line.");

    /* Do not delete anything below this point. :-) */

    /* This will take a LONG time to complete if memoization isn't implemented.
     * We're talking "heat death of the universe" amounts of time. :-)
     *
     * If you did implement memoization but this test case is still hanging, make
     * sure that in your recursive function (not the wrapper) that your recursive
     * calls are to your new recursive function and not back to the wrapper. If you
     * call the wrapper again, you'll get a fresh new memoization table rather than
     * preserving the one you're building up in your recursive exploration, and the
     * effect will be as if you hadn't implemented memoization at all.
     */
    EXPECT(weightOnBackOf(100, 50, 200) >= 10000);
}

/* TODO: Add your own tests here. You know the drill - look for edge cases, think about
 * very small and very large cases, etc.
 */
