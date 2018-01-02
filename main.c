/*******************************************************************************
 * (c) Copyright 2016-2017 Microsemi SoC Products Group. All rights reserved.
 * 
 * This SoftConsole example project demonstrates how to cppcheck can be used
 * to detect bugs and mistakes which would not cause syntex errors. To avoid
 * cases where the code compiles correctly, might run, but might not behave
 * as desired.
 *
 * The project is loosely based on the miv-rv32im-systick-blinky and has the
 * hardware configuration in common, but the hardware is optional.
 * Demonstration of found bug is working fully without any hardware present.
 *
 * List what check the tool is capable is listed on the project page:
 * https://sourceforge.net/p/cppcheck/wiki/ListOfChecks/
 *
 * If security is significant concern then using multiple analysis tools at
 * the same time would be recommended, but for a free bundled tool, the cppcheck
 * is decent first line defense. Each project can set separately how thoroughly
 * the cppcheck will analyse the project. By default only errors are enabled,
 * but optional style, performance and portability checks can be enabled as
 * well.
 *
 * Publicly known vulnerabilities detected with cppcheck:
 * http://seclists.org/oss-sec/2017/q3/397
 * https://lists.x.org/archives/xorg-announce/2014-January/002389.html
 * https://bugzilla.redhat.com/show_bug.cgi?id=CVE-2012-1147
 *
 */
#define BUFFER_SIZE 38

#include <stdio.h>

char hello_world[BUFFER_SIZE] ="\r\nHello World CPP check example\r\n";

typedef struct Staff {
    int days_worked;
    int salary;
    int paid_total;
} Staff;

Staff joe = {
    .days_worked = 16,
    .salary      = 900,
    .paid_total  = 1800
};

void pay_staff(Staff *person) {
    // Following can happen:
    //
    // A) [possible BUG] The person is already checked for null pointer outside
    //    this function and it's already safe and the null pointer check is
    //    redundant.
    //
    // B) [severe BUG] Person is not checked and could contain null pointer,
    //    but the code will break before the null pointer check.
    //
    // Either the code is redundant at best or broken at worst, indication to
    // rewrite the code given by cppcheck.
    person->paid_total += person->salary;

    if (person) {
        // Check for null pointer then use the struct
        person->days_worked+=7;
    }
}

Staff* hire_staff(int copy_default_values) {
    Staff default_staff = {
        .days_worked = 0,
        .salary      = 850,
        .paid_total  = 0
    };


    Staff *ret = NULL;
    if (copy_default_values == 1) {
        ret = &default_staff;
    }

    // if "copy_default_values" is not 1 then "ret" will stay NULL and shouldn't
    // be accessed yet, probably missing else statement, or default
    // initialization [possible BUG]

    ret->days_worked = 1;
    return(ret);
}

// cppcheck-suppress unusedFunction
void interrupt_handler(void) {
    // handlers are not called by software directly and in this case you want
    // to supress the warning.
}

void change_salary(Staff *person) {
    // this function is never called, show warning
}


int main(int argc, char **argv) {
    uint8_t rx_char; // [not a BUG] Unused variable
    char *pointer;   // [severe BUG] Variable will be used without initialising.

    printf("%s", hello_world);

    for (int i=0; i< 100; i++) {
        if (i > 90 || i < 100) {
            // [possibly a BUG] condition which will always evaluate to be true
            printf("Condition meet all the time at i=%d\r\n", i);
        }

        if (i == 99 || 99 == i) {
            // [possibly a BUG] condition meet at i==99, probably has a typo
            printf("Condition met at i=%d\r\n", i);
        }
    }

    // Clearing buffer in a for loop.
    // [severe BUG] Flawed condition will cause out-of-bounds error
    for (int i=0; i<=BUFFER_SIZE; i++) {
        hello_world[i] = 0;
    }

    // [possible BUG] Setting j to 10 but never using j with this value.
    for (int i=0; i<=BUFFER_SIZE; i++) {
        int j = 10;
        j = i + 33;  // First value is unused, possibly missing a functionality
        printf("Result of i + 33 = %d\r\n", j);
    }

    *pointer = 0; // [severe BUG] Write to pointer which is uninitialized

    Staff *jack_p = hire_staff(1);
    pay_staff(&joe);
    pay_staff(jack_p);

    return 0;
}

