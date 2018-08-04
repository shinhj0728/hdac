//
// Copyright (c) 2018 Hdac Technology AG
// Hdac code distributed under the GPLv3 license
//============================================================================================
// node.h
//	Hdac node management
//
// History
//	2018/04/10	Initial code
//============================================================================================
//

#ifndef __NODE_H__
#define __NODE_H__


#include "hdaclib.h"
#include "rpc.h"


typedef struct {
	char	addr[HDAC_ADDR_LEN];
	char	txid[HDAC_TX_LEN];
	time_t	time;
	double	amount;
} hdac_in_t;


class hdac_node
{
private:
	hdac_t	*hdac;
	bool	local_opened;

public:
	hdac_node()
		{ hdac = NULL; local_opened = false; }
	~hdac_node()
		{ close(); }

	void	set_hdac(hdac_t *newhdac)
		{ close(); hdac = newhdac; local_opened = false; }
	hdac_t	*open(const char *nodeip, const int port, const char *userid, const char *password)
		{ hdac = hdac_open(nodeip, port, userid, password); local_opened = true; return hdac; }
	void	close()
		{ if (local_opened) hdac_close(hdac); hdac = NULL; }

	hdac_in_t& operator [](char *addr);
	hdac_in_t& balance(char *toaddr);
	string	txs(char *addr);
};


class hdac_virtual_device
{
private:
	hdac_t	*hdac;
	bool	local_opened;

public:
	hdac_virtual_device()
		{ hdac = NULL; local_opened = false; }
	~hdac_virtual_device()
		{ close(); }

	void	set_hdac(hdac_t *newhdac)
		{ close(); hdac = newhdac; local_opened = false; }
	hdac_t	*open(const char *nodeip, const int port, const char *userid, const char *password)
		{ hdac = hdac_open(nodeip, port, userid, password); local_opened = true; return hdac; }
	void	close()
		{ if (local_opened) hdac_close(hdac); hdac = NULL; }

	hdac_node& add(char *addr, void callback(char *json_data));
	bool	exist(char *addr);
	void	remove(char *addr);

	string	balance(char *addr);
	string	getstatus(char *addr, char *cmd = NULL);
	string	control(char *addr, char *cmd);
	string	txs(char *addr);
};





#endif	// __NODE_H__
