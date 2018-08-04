#include "libhash.h"


int	main(int ac, char **av)
{
	char	bin[200] = {0}, tmp[200] = {0};
	char	b58[200] = {0};
	char	outbin[200] = {0};
	size_t	b58sz = 0, outbinsz = 0;

	if (ac <= 1)
		strcpy(bin, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	else
		strcpy(bin, av[1]);

	b58sz = sizeof(b58);
	base58_encode(b58, &b58sz, bin, strlen(bin));
	printf("base58_encode:\nout=%s b58sz=%ld\nbin=%s (%ld)\n\n", 
		b58, b58sz, bin, strlen(bin));
	for (int nn = 0; nn < b58sz; nn++)
	{
		printf("[%2d] = 0x%02X ", nn, b58[nn]);
	}

	outbinsz = sizeof(outbin);
	base58_decode(outbin, &outbinsz, b58, strlen(b58));
	printf("base58_decode: b58sz=%ld out=%s (%ld)\n\n",
		b58sz, outbin, strlen(outbin));

	if (outbinsz != strlen(bin) || strcmp(bin, outbin) != 0)
		printf("Base58: Error!\n");
	else
		printf("Base58: Passed!\n");

	return 0;
}

