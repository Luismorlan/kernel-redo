/******************************************************************************/
/* Important Fall 2017 CSCI 402 usage information:                            */
/*                                                                            */
/* This fils is part of CSCI 402 kernel programming assignments at USC.       */
/*         53616c7465645f5f2e8d450c0c5851acd538befe33744efca0f1c4f9fb5f       */
/*         3c8feabc561a99e53d4d21951738da923cd1c7bbd11b30a1afb11172f80b       */
/*         984b1acfbbf8fae6ea57e0583d2610a618379293cb1de8e1e9d07e6287e8       */
/*         de7e82f3d48866aa2009b599e92c852f7dbf7a6e573f1c7228ca34b9f368       */
/*         faaef0c0fcf294cb                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

#pragma once

#include "types.h"

#include "test/kshell/kshell.h"

typedef enum kshell_token_type {
        KTT_WORD,
        KTT_REDIRECT_IN,         /* '<' */
        KTT_REDIRECT_OUT,        /* '>' */
        KTT_REDIRECT_OUT_APPEND, /* '>>' */
        KTT_EOL,

        KTT_MAX /* Number of token types */
} kshell_token_type_t;

typedef struct kshell_token {
        kshell_token_type_t kt_type;
        char *kt_text;
        size_t kt_textlen;
} kshell_token_t;

/**
 * Finds the next token in the input line.
 *
 * Note: To find multiple tokens from the same line, you increment the
 * line pointer by the number of bytes processed before the next call
 * to kshell_next token.
 *
 * @param ksh the kshell
 * @param line the input line to tokenize
 * @param token out parameter containing the next token found
 * @return 0 if no more tokens, otherwise, number of bytes processed
 */
int kshell_next_token(kshell_t *ksh, char *line, kshell_token_t *token);

const char *kshell_token_type_str(kshell_token_type_t type);
