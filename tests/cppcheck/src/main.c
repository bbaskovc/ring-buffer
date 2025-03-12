/***********************************************************************************************************************
 *
 * @file        main.c
 * @brief       Windows / Linux platform start-up file to test DATA-LOGGER component.
 * @author      Blaz Baskovc
 * @copyright   Copyright 2025 Blaž Basškovč
 * @date        2025-02-17
 *
 **********************************************************************************************************************/

// --- Includes --------------------------------------------------------------------------------------------------------

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// --- Application Start-Up function -----------------------------------------------------------------------------------

int32_t main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    // Disable stdout buffering (needed to print even if one character is in buffer)
    setvbuf(stdout, NULL, _IONBF, 0);

    // Return execute results
    return EXIT_SUCCESS;
}

// --- Private Functions Defenitions -----------------------------------------------------------------------------------

// --- EOF -------------------------------------------------------------------------------------------------------------
