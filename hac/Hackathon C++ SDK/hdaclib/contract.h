//
// Copyright (c) 2018 Hdac Technology AG
// Hdac code distributed under the GPLv3 license
//============================================================================================
// contract.h
//	Smart contract PoC
//
// History
//	2018/05/10	Add hdac_map_* functions
//	2018/05/20	Add hdac_contract class
//	2018/06/07	Merge hdac_admin and hdac_contract class
//============================================================================================
//

#ifndef __CONTRACT_H__
#define __CONTRACT_H__


#include <time.h>
#include <stdio.h>

#include "hdaclib.h"
#include "rpc.h"


//
// map structure
//
typedef struct {
// IN
	char	admin[HDAC_ADDR_LEN];		// admin address
	char	handler[HDAC_ADDR_LEN];		// contract handler, receive address
	char	sender[HDAC_ADDR_LEN];		// contract sender

	char	stream[32];			// stream for contract permission handling
	char	key[256];			// contract key

	char	token[128];			// Token for this contract
	double	amount;				// amount
	double	unit;				// smallest unit: 0.00000001

	char	progfile[256];			// Contract program filename

// OUT
	char	txid[HDAC_TX_LEN];		// Contract tx id
	time_t	lasttime;
} hdac_map_t;


// mapping functions
int	hdac_map_grant(hdac_t *hdac, const char *addr, const char *perm);
int	hdac_map_add(hdac_t *hdac, const char *stream, const char *sender, const char *handler);
int	hdac_map_remove(hdac_t *hdac, const char *stream, const char *sender);
int	hdac_map_validate(hdac_t *hdac, const char *stream, const char *perm, const char *sender);
string	hdac_get_permission(hdac_t *hdac, const char *perm, const char *sender);

// contract functions
int	hdac_contract_open(hdac_t *hdac, const hdac_map_t *map);
int	hdac_contract_grant(hdac_t *hdac, const char *stream, const char *sender, const char *handler);
int	hdac_contract_close(hdac_t *hdac, const hdac_map_t *map);
int	hdac_contract_closed(hdac_t *hdac, const char *stream);
int	hdac_contract_issue_token(hdac_t *hdac, const char *addr, const char *tokenname, const double amount, const double unit);
int	hdac_contract_revoke(hdac_t *hdac, const char *stream, const char *sender);
int	hdac_contract_abort(hdac_t *hdac, const char *stream);
int	hdac_contract_send(hdac_t *hdac, hdac_map_t *map);
string	hdac_contract_receive(hdac_t *hdac, hdac_map_t *map);


#ifdef __cplusplus

class hdac_contract
{
private:
	bool	local_opened;

public:
	hdac_t	*hdac;
	hdac_rpc_t rpc;
	hdac_map_t map;

public:
	hdac_contract()
		{ hdac = NULL; init(); }
	hdac_contract(hdac_t *newhdac)
		{ hdac = newhdac; init(); }
	~hdac_contract()
		{ close_hdac(); }
	void	init()
		{ local_opened = false; memset(&rpc, 0, sizeof(rpc)); memset(&map, 0, sizeof(map)); }

	// Hdac full node RPC connection
	int	load_config(const char *cfgfile);
	void	set_hdac(hdac_t *newhdac)
		{ close_hdac(); hdac = newhdac; local_opened = false; }
	hdac_t	*open_hdac()
		{ return open_hdac(rpc.ip, rpc.port, rpc.user, rpc.password); }
	hdac_t	*open_hdac(hdac_rpc_t *rpc)
		{ return open_hdac(rpc->ip, rpc->port, rpc->user, rpc->password); }
	hdac_t	*open_hdac(const char *nodeip, const int port, const char *userid, const char *password)
		{ hdac = hdac_open(nodeip, port, userid, password); local_opened = true; return hdac; }
	int	hdac_opened()
		{ return hdac ? true : false; }
	void	close_hdac()
		{ if (local_opened) hdac_close(hdac); hdac = NULL; 
		  memset(&rpc, 0, sizeof(rpc)); memset(&map, 0, sizeof(map)); }

	// Admin operations
	int	setmap(hdac_map_t& map);
	hdac_map_t& getmap(hdac_map_t& map);
	hdac_rpc_t *getrpc()
		{ return &rpc; }

	int	open()
		{ return open(&map); }
	int	open(const hdac_map_t *map)
		{ return hdac_contract_open(hdac, map); }

	int	grant()
		{ return grant(&map); }
	int	grant(const hdac_map_t *map)
		{ return grant(map->stream, map->sender, map->handler); }
	int	grant(const char *stream, const char *sender, const char *handler)
		{ return hdac_contract_grant(hdac, stream, sender, handler); }

	int	close()
		{ return close(&map); }
	int	close(const hdac_map_t *map)	// Close contract stream: send/receive not available
		{ return hdac_contract_close(hdac, map); }

	int	closed()
		{ return closed(map.stream); }
	int	closed(const char *stream)
		{ return hdac_contract_closed(hdac, stream); }

	int	issue_token(const char *addr, const char *tokenname, const double amount, const double unit)
		{ return hdac_contract_issue_token(hdac, addr, tokenname, amount, unit); }

//	int	grant(const char *stream, const char *sender)
//		{ return hdac_contract_grant(hdac, stream, sender); }
	int	revoke(const hdac_map_t *map)
		{ return revoke(map->stream, map->sender); }
	int	revoke(const char *stream, const char *sender)
		{ return hdac_contract_revoke(hdac, stream, sender); }

	// Contract operations
	int	send(hdac_map_t *map)		// Send contract
		{ return hdac_contract_send(hdac, map); }
	string	receive(hdac_map_t *map)	// Read contract
		{ return hdac_contract_receive(hdac, map); }

	int	pause(hdac_map_t *map);		// Pause contract
	int	resume(hdac_map_t *map);	// Resume contrat
	int	abort(const char*stream);	// Stop all contract execution immediately
};

#endif	// __cplusplus


#endif	// __CONTRACT_H__
