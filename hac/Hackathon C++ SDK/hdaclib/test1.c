#include "rpc.h"
#include "hdaclib.h"


int	main()
{
	hdac_t	*hdac = NULL;

	hdac = hdac_open("192.168.1.7", 28822, "hdacrpc", "hdac.anyone");

	if (hdac)
	{
		json	js;

		js = hdac_getinfo(hdac);
		js["result"].print();

		hdac_close(hdac);
	}
}
