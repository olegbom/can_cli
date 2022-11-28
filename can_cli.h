/*
 * can_cli.h
 *
 *  Created on: 19 10 2022
 *      Author: Olegbom
 */

#pragma once


/* ----------- INCLUDES ------------------- */

#include <stdint.h>

/* ----------- TYPEDEFS ------------------- */

typedef void ( *txCliFunc )( void );

typedef struct
{
    txCliFunc txCliFunc;
    uint8_t *canData;
    uint8_t remainder;
    uint8_t txIndex;
} CanCliTx_s;


/* ----------- FUNCTIONS PROTOTYPES ------------------- */

void canCliStart( CanCliTx_s *cliTx, txCliFunc func, uint8_t *dstCanData );
void canCliTx( CanCliTx_s *cliTx, const uint8_t *data, uint32_t length );
void canCliEnd( CanCliTx_s *cliTx );
