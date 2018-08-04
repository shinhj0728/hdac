//
// Copyright (c) 2018 Hdac Technology AG
// Hdac code distributed under the GPLv3 license
//============================================================================================
// dumphdac.c
//	Hdac block & tx dump tool
//
// History
//	2018/01/00	Initial code
//============================================================================================
//

//
// Output: block.json / tx.json
//

#include "rpc.h"
#include "hdaclib.h"


int	main()
{
	hdac_t	*hdac = NULL;
	FILE	*blockfp = NULL, *txfp = NULL;
	json	js, subjs;
	char	*retstr = NULL, buf[256] = {0};

	hdac = hdac_open("192.168.1.7", 8822, "hdacwallet", "hdac.wallet");

	if (hdac)
	{
		int	blocks = 0;

		blockfp = fopen("_block.json", "wb");
		txfp = fopen("_tx.json", "wb");

		assert(blockfp != NULL);
		assert(txfp != NULL);

		js = hdac_getinfo(hdac);
		js.print();
		js["result"]["blocks"].tostr(buf, sizeof(buf));

		blocks = atoi(buf);
		printf("TOTAL BLOCKS=%d\n", blocks);

		for (int bidx = 0; bidx <= blocks; bidx++)
		{
			char	*blockjson = NULL;

			printf("BLOCK=%d    \r", bidx);

			retstr = hdac_getblock(hdac, bidx, 1);
			if (json_rpc_error(retstr))
			{
				printf("WARNING: Block %d not found!\n", bidx);
				continue;
			}

			js = retstr;
			js.print(4, blockfp);

			for (int txidx = 0; txidx < 30000; txidx++)
			{
				char	txid[256] = {0};

				js["result"]["tx"][txidx].tostr(txid, sizeof(txid));
				if (txid[0] == 0)
					break;

				subjs = hdac_getrawtx(hdac, txid, 1);
				subjs.print(4, txfp);
			}
		}

		hdac_close(hdac);
	}

	fclose(blockfp);
	fclose(txfp);

	return 0;
}
