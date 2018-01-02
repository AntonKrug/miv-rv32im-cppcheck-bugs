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
 * is decent first line defense.
 *
 * Public known vulnerabilities detected with cppcheck:
 * http://seclists.org/oss-sec/2017/q3/397
 * https://lists.x.org/archives/xorg-announce/2014-January/002389.html
 * https://bugzilla.redhat.com/show_bug.cgi?id=CVE-2012-1147
 *
 */

#include "riscv_hal.h"
#include "hw_platform.h"
#include "core_uart_apb.h"

const char * g_hello_msg ="\r\n    Hello World Example   \r\n\r\n";

/*-----------------------------------------------------------------------------
 * UART instance data.
 */
UART_instance_t g_uart;

/*-----------------------------------------------------------------------------
 * main
 */
int main(int argc, char **argv) {
    uint8_t rx_char;
    uint8_t rx_count;

    PLIC_init();

    UART_init(&g_uart,
              COREUARTAPB0_BASE_ADDR,
              BAUD_VALUE_115200,
              (DATA_8_BITS | NO_PARITY));

    UART_polled_tx_string(&g_uart, (const uint8_t *)g_hello_msg);

    /*
     * Loop. Echo back characters received on UART.
     */
    do {
        rx_count = UART_get_rx(&g_uart, &rx_char, 1);
        if (rx_count > 0) {
            UART_send(&g_uart, &rx_char, 1);
        }
    } while (1);
  
  
    return 0;
}

