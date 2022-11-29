/*
 * can_cli.c
 *
 *  Created on: 19 10 2022
 *      Author: Olegbom
 */

#include "can_cli.h"
#include <stddef.h>
#include <string.h>

/* ----------- FUNCTIONS ------------------- */

void canCliStart( CanCliTx_s *cliTx, txCliFunc func, uint8_t *dstCanData )
{
    cliTx->txCliFunc = func;
    cliTx->canData = dstCanData;
    cliTx->remainder = 0;
    cliTx->txIndex = 0;
    cliTx->canData[0] = 0;
}

void canCliTx( CanCliTx_s *cliTx, const uint8_t *data, uint32_t length )
{
    if( ( cliTx->txCliFunc == NULL ) ||
        ( cliTx->canData == NULL) )
        return;

    uint32_t dataTxByteIndex = 0;

    if( cliTx->remainder > 0 )
    {
        uint8_t numberOfFreeBytes = 7 - cliTx->remainder;
        if( numberOfFreeBytes > length )
        {
            memcpy( cliTx->canData + 1 + cliTx->remainder, data, length );
            cliTx->remainder += length;
            return;
        }
        else
        {
            memcpy( cliTx->canData + 1 + cliTx->remainder, data, numberOfFreeBytes );

            cliTx->txCliFunc();
            cliTx->txIndex++;
            dataTxByteIndex += numberOfFreeBytes;
            cliTx->remainder = 0;
        }
    }

    while( dataTxByteIndex + 7 <= length )
    {
        cliTx->canData[0] = cliTx->txIndex;
        memcpy( cliTx->canData + 1, data + dataTxByteIndex, 7 );
        cliTx->txCliFunc();
        cliTx->txIndex++;
        dataTxByteIndex += 7;
    }

    cliTx->remainder = length - dataTxByteIndex;
    cliTx->canData[0] = cliTx->txIndex;
    memcpy( cliTx->canData + 1, data + dataTxByteIndex, cliTx->remainder );

}

void canCliEnd( CanCliTx_s *cliTx )
{
    if( ( cliTx->txCliFunc == NULL ) ||
        ( cliTx->canData == NULL) )
        return;

    memset( cliTx->canData + 1 + cliTx->remainder, 0, 7 - cliTx->remainder );
    if( cliTx->txCliFunc )
        cliTx->txCliFunc();
    cliTx->txCliFunc = NULL;
    cliTx->canData = NULL;
}
