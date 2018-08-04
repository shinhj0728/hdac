//
// Copyright (c) 2018 Hdac Technology AG
// Hdac code distributed under the GPLv3 license
//============================================================================================
// rpc.h
//	Hdac RPC API functions
//
// History
//	2018/03/10	Initial code
//============================================================================================
//

#ifndef __RPCCMD_H__
#define __RPCCMD_H__


#include <limits.h>
#include "hdaclib.h"

using namespace std;


#define RPC_SECURE		0	// Do not allow RPC security functions
#define SCALABLE_WALLET		1	// Upgraded wallet - some functions are not supported.


// == Blockchain ==
char	*hdac_getbestblockhash(hdac_t *hdac);
char	*hdac_getblockbyhash(hdac_t *hdac, const char *hash, const int verbose = 1);
char	*hdac_getblock(hdac_t *hdac, const uint32_t height, const int verbose = 1);
char	*hdac_getblockchaininfo(hdac_t *hdac);
char	*hdac_getblockcount(hdac_t *hdac);
char	*hdac_getblockhash(hdac_t *hdac, const uint32_t index);
char	*hdac_getchaintips(hdac_t *hdac);
char	*hdac_getdifficulty(hdac_t *hdac);
char	*hdac_getmempoolinfo(hdac_t *hdac);
char	*hdac_getrawmempool(hdac_t *hdac, const bool verbose = false);
char	*hdac_gettxout(hdac_t *hdac, const char *txid, const uint32_t nvout, const bool includemempool = false);
char	*hdac_gettxoutsetinfo(hdac_t *hdac);
char	*hdac_listassets(hdac_t *hdac, const char *asset_id, const bool verbose = false, const long int count = INT_MAX, const int start = 0);
char	*hdac_listblocks(hdac_t *hdac, const char *block_set_id, const bool verbose = false);
char	*hdac_listpermissions(hdac_t *hdac, const char *perm = "", const char *addr = "*", const bool verbose = false);
char	*hdac_liststreams(hdac_t *hdac, const char *stream_id = "*", const bool verbose = false, const long int count = INT_MAX, const int start = 0);
//char	*hdac_listupgrades(hdac_t *hdac, const char *upgrade_id = "*");
//char	*hdac_verifychain(hdac_t *hdac, const int checklevel = 3, const int numblocks = 288);

// == Control ==
char	*hdac_clearmempool(hdac_t *hdac);
char	*hdac_getblockchainparams(hdac_t *hdac, const bool displaynames = true, const bool with_upgrades = true);
char	*hdac_getinfo(hdac_t *hdac);
char	*hdac_getruntimeparams(hdac_t *hdac);
char	*hdac_pause_task(hdac_t *hdac, const char *tasks);
char	*hdac_resume_task(hdac_t *hdac, const char *tasks);
char	*hdac_setlastblockbyhash(hdac_t *hdac, const char *hash);
char	*hdac_setlastblock(hdac_t *hdac, const int height);
char	*hdac_setruntimeparam(hdac_t *hdac, const char *param_name, const char *param_value = "");

// == Generating ==
char	*hdac_getgenerate(hdac_t *hdac);
char	*hdac_gethashespersec(hdac_t *hdac);

// == Mining ==
char	*hdac_getblocktemplate(hdac_t *hdac, const char *jsonobj);
char	*hdac_getblockwindowsize(hdac_t *hdac);
char	*hdac_getmininginfo(hdac_t *hdac);
char	*hdac_getnetworkhashps(hdac_t *hdac, const int blocks = 120, const int height = -1);
//char	*hdac_prioritiestx(hdac_t *hdac, const char *txid, const int priority_delta, const int fee_delta);

// == Network ==
char	*hdac_addnode(hdac_t *hdac, const char *node, const char *op);
char	*hdac_getaddednodeinfo(hdac_t *hdac, const bool dns, const char *node = "");
char	*hdac_getconnectioncount(hdac_t *hdac);
char	*hdac_getnettotals(hdac_t *hdac);
char	*hdac_getnetworkinfo(hdac_t *hdac);
char	*hdac_getpeerinfo(hdac_t *hdac);
//char	*hdac_ping(hdac_t *hdac);

// == Util ==
char	*hdac_createkeypairs(hdac_t *hdac, const int count = 1);
char	*hdac_estimatefee(hdac_t *hdac, const int nblocks);
char	*hdac_estimatepriority(hdac_t *hdac, const int nblocks);
char	*hdac_validateaddress(hdac_t *hdac, const char *addr);
char	*hdac_verifymessage(hdac_t *hdac, const char *addr, const char *signature, const char *message);

// == Wallet ==
char	*hdac_approvefrom(hdac_t *hdac, const char *from_addr, const char *upgrade_id, const bool approve);
char	*hdac_backupwallet(hdac_t *hdac, const char *dest);
char	*hdac_combineunspent(hdac_t *hdac, const char *addr, const int minconf = 1, const int maxcombines = 100, const int mininputs = 2, const int maxinputs = 100, const int maxtime = 15);
char	*hdac_dumpprivkey(hdac_t *hdac, const char *addr);
char	*hdac_dumpwallet(hdac_t *hdac, const char *filename);
char	*hdac_encryptwallet(hdac_t *hdac, const char *passphrase);
char	*hdac_walletpassphrase(hdac_t *hdac, const char *passphrase, const int timeout_sec);
#ifndef SCALABLE_WALLET
char	*hdac_getaccount(hdac_t *hdac, const char *addr);
char	*hdac_getaccountaddress(hdac_t *hdac, const char *account);
#endif
char	*hdac_getaddressbalances(hdac_t *hdac, const char *addr, const int minconf = 1, const bool includeLocked = false);
char	*hdac_getaddresses(hdac_t *hdac, const bool verbose = false, const int type = 0);
#ifndef SCALABLE_WALLET
char	*hdac_getaddressesbyaccount(hdac_t *hdac, const char *account);
#endif
char	*hdac_getaddresstx(hdac_t *hdac, const char *addr, const char *txid, const bool verbose = false);
char	*hdac_getassetbalances(hdac_t *hdac, const char *account, const int minconf = 1, const bool includeWatchonly = false, const bool includeLocked = false);
char	*hdac_getassettx(hdac_t *hdac, const char *asset_id, const char *txid, const bool verbose = false);
char	*hdac_getbalance(hdac_t *hdac, const char *account = "", const int minconf = 1, const bool includeWatchonly = false);
char	*hdac_getmultibalances(hdac_t *hdac, const char *addr = "", const char *assets = "*", const int minconf = 1, const bool includeLocked = false, const bool includeWatchonly = false);
char	*hdac_getnewaddress(hdac_t *hdac, const char *account = "");
#ifndef SCALABLE_WALLET
char	*hdac_getreceivedbyaccount(hdac_t *hdac, const char *account, const int minconf = 1);
#endif
char	*hdac_getreceivedbyaddress(hdac_t *hdac, const char *addr, const int minconf = 1);
char	*hdac_gettotalbalances(hdac_t *hdac, const int minconf = 1, const bool includeWatchonly = false, const bool includeLocked = false);
char	*hdac_gettx(hdac_t *hdac, const char *txid, const bool includeWatchonly = false);
char	*hdac_getrawtx(hdac_t *hdac, const char *txid, const int verbose = 1);
char	*hdac_gettxoutdata(hdac_t *hdac, const char *txid, const int vout, const long int count_bytes = INT_MAX, const int start_byte = 0);
char	*hdac_getunconfirmedbalance(hdac_t *hdac);
char	*hdac_getwalletinfo(hdac_t *hdac);
char	*hdac_getwallettx(hdac_t *hdac, const char *txid, const bool includeWatchonly = false, const bool verbose = false);
char	*hdac_importaddress(hdac_t *hdac, const char *addrs, const char *label = "", const bool rescan = true);
char	*hdac_importprivkey(hdac_t *hdac, const char *privkeys, const char *label = "", const bool rescan = true);
char	*hdac_importwallet(hdac_t *hdac, const char *filename);
char	*hdac_issue(hdac_t *hdac, const char *addr, const char *asset_params, const double quantity, const double smallest_unit = 1, const double native_amount = 0, const char *custom_fields = "");
char	*hdac_issuefrom(hdac_t *hdac, const char *fromaddr, const char *toaddr, const char *asset_params, const double quantity, const double smallest_unit = 1, const double native_amount = 0, const char *custom_fields = "");
char	*hdac_issuemore(hdac_t *hdac, const char *addr, const char *asset_id, const double quantity, const double native_amount = 0, const char *custom_fields = "");
char	*hdac_issuemorefrom(hdac_t *hdac, const char *fromaddr, const char *toaddr, const char *asset_id, const double quantity, const double native_amount = 0, const char *custom_fields = "");
char	*hdac_keypoolrefill(hdac_t *hdac, const int newsize = 100);
char	*hdac_listaccounts(hdac_t *hdac, const int minconf = 1, const bool includeWatchonly = false);
char	*hdac_listaddresses(hdac_t *hdac, const char *addrs = "*", const bool verbose = false, const long int count = INT_MAX, const int start = 0);
char	*hdac_listaddressgroupings(hdac_t *hdac);
char	*hdac_listaddresstxs(hdac_t *hdac, const char *addr, const int count = 10, const int skip = 0, const bool verbose = false);
char	*hdac_listassettxs(hdac_t *hdac, const char *asset_id, const bool verbose = false, const int count = 10, const int start = 0, const bool local_ordering = false);
char	*hdac_listlockunspent(hdac_t *hdac);
#ifndef SCALABLE_WALLET
char	*hdac_listreceivedbyaccount(hdac_t *hdac, const int minconf = 1, const bool includeempty = false, const bool includeWatchonly = false);
char	*hdac_listreceivedbyaddress(hdac_t *hdac, const int minconf = 1, const bool includeempty = false, const bool includeWatchonly = false);
#endif
char	*hdac_listsinceblock(hdac_t *hdac, const char *blockhash = "", const int target_confirms = 1, const bool includeWatchonly = false);
char	*hdac_listtxs(hdac_t *hdac, const char *account, const int count = 10, const int from = 0, const bool includeWatchonly = false);
char	*hdac_listunspent(hdac_t *hdac, const int minconf = 1, const int maxconf = 9999999, const char *addrs = "");
char	*hdac_listwallettxs(hdac_t *hdac, const int count = 10, const int skip = 0, const bool includeWatchonly = false, const bool verbose = false);
char	*hdac_lockunspent(hdac_t *hdac, const bool unlock, const char *tx);
#ifndef SCALABLE_WALLET
char	*hdac_move(hdac_t *hdac, const char *fromaccount, const char *toaccount, const double amount, const int minconf = 1, const char *comment = "");
#endif
char	*hdac_preparelockunspent(hdac_t *hdac, const char *asset_qty, const bool lock = true);
char	*hdac_preparelockunspentfrom(hdac_t *hdac, const char *fromaddr, const char *asset_qty, const bool lock = true);
char	*hdac_resendwallettxs(hdac_t *hdac);
char	*hdac_send(hdac_t *hdac, const char *addr, const double amount, const char *comment = "", const char *comment_to = "");
char	*hdac_send_asset(hdac_t *hdac, const char *addr, const char *asset_qty, const char *comment = "", const char *comment_to = "");
char	*hdac_sendasset(hdac_t *hdac, const char *addr, const char *asset_id, const double asset_qty, const double native_amount, const char *comment = "", const char *comment_to = "");
char	*hdac_sendassetfrom(hdac_t *hdac, const char *fromaddr, const char *toaddr, const char *asset_id, const double asset_qty, const double native_amount = 0.0, const char *comment = "", const char *comment_to = "");
char	*hdac_sendfrom(hdac_t *hdac, const char *fromaddr, const char *toaddr, const double amount, const char *comment = "", const char *comment_to = "");
char	*hdac_sendfrom_asset(hdac_t *hdac, const char *fromaddr, const char *toaddr, const char *asset_qty, const char *comment = "", const char *comment_to = "");
#ifndef SCALABLE_WALLET
char	*hdac_sendfromaccount(hdac_t *hdac, const char *fromaccount, const char *toaddr, const double amount, const int minconf = 1, const char *comment = "", const char *comment_to = "");
char	*hdac_sendmany(hdac_t *hdac, const char *fromaccount, const char *addr_amounts, const int minconf = 1, const char *comment = "");
#endif
char	*hdac_sendwithdata(hdac_t *hdac, const char *addr, const double amount, const char *data, int datalen = 0);
char	*hdac_sendwithdata_asset(hdac_t *hdac, const char *addr, const char *asset_qty, const char *data, int datalen = 0);
char	*hdac_sendwithdatafrom(hdac_t *hdac, const char *fromaddr, const char *toaddr, const double amount, const char *data, int datalen = 0);
char	*hdac_sendwithdatafrom_asset(hdac_t *hdac, const char *fromaddr, const char *toaddr, const char *asset_qty, const char *data, int datalen = 0);
#ifndef SCALABLE_WALLET
char	*hdac_setaccount(hdac_t *hdac, const char *addr, const char *account);
#endif
char	*hdac_settxfee(hdac_t *hdac, const double amount);
char	*hdac_signmessage(hdac_t *hdac, const char *addr_or_privkey, const char *message);


// Wallet:GRANT
char	*hdac_grant(hdac_t *hdac, const char *addrs, const char *perms, const double native_amount = 0.0, const int startblock = 0, const int endblock = 4294967295, const char *comment = "", const char *comment_to = "");
char	*hdac_grantfrom(hdac_t *hdac, const char *fromaddr, const char *toaddrs, const char *perms, const double native_amount = 0.0, const int startblock = 0, const int endblock = 4294967295, const char *comment = "", const char *comment_to = "");
char	*hdac_grantwithdata(hdac_t *hdac, const char *addrs, const char *perms, const char *data, int datalen = 0, const double native_amount = 0.0, const int startblock = 0, const int endblock = 4294967295);
char	*hdac_grantwithdatafrom(hdac_t *hdac, const char *fromaddr, const char *toaddrs, const char *perms, const char *data, int datalen = 0, const double native_amount = 0.0, const int startblock = 0, const int endblock = 4294967295);
char	*hdac_revoke(hdac_t *hdac, const char *addrs, const char *perms, const double native_amount = 0, const char *comment = "", const char *comment_to = "");
char	*hdac_revokefrom(hdac_t *hdac, const char *fromaddr, const char *toaddrs, const char *perms, const double native_amount = 0, const char *comment = "", const char *comment_to = "");


// Wallet:STREAM
char	*hdac_create(hdac_t *hdac, const char *entity_type, const char *entity_name, const bool open, const char *custom_fields = "");
char	*hdac_createfrom(hdac_t *hdac, const char *from_addr, const char *entity_type, const char *entity_name, const bool open, const char *custom_fields);
char	*hdac_getstreamitem(hdac_t *hdac, const char *stream_id, const char *txid, const bool verbose = false);
char	*hdac_liststreamblockitems(hdac_t *hdac, const char *stream_id, const char *block_set_id, const bool verbose = false, const long int count = INT_MAX, const int start = 0);
char	*hdac_liststreamitems(hdac_t *hdac, const char *stream_id, const bool verbose = false, const long int count = INT_MAX, const int start = 0, const bool local_ordering = false);
char	*hdac_liststreamkeyitems(hdac_t *hdac, const char *stream_id, const char *stream_key, const bool verbose = false, const long int count = 10, const int start = 0, const bool local_ordering = false);
char	*hdac_liststreamkeys(hdac_t *hdac, const char *stream_id, const char *stream_key = "*", const bool verbose = false, const long int count = INT_MAX, const int start = 0, const bool local_ordering = false);
char	*hdac_liststreampublisheritems(hdac_t *hdac, const char *stream_id, const char *addr, const bool verbose = false, const int count = 10, const int start = 0, const bool local_ordering = false);
char	*hdac_liststreampublishers(hdac_t *hdac, const char *stream_id, const char *addrs = "*", const bool verbose = false, const long int count = INT_MAX, const int start = 0, const bool local_ordering = false);
char	*hdac_publish(hdac_t *hdac, const char *stream_id, const char *item_key, const char *data, int datalen = 0);
char	*hdac_publishfrom(hdac_t *hdac, const char *fromaddr, const char *stream_id, const char *item_key, const char *data, int datalen = 0, bool ishex = false);
char	*hdac_subscribe(hdac_t *hdac, const char *entity_ids, const bool rescan = true);
char	*hdac_unsubscribe(hdac_t *hdac, const char *json_id);


// Wallet:MULTISIG
char	*hdac_addmultisigaddress(hdac_t *hdac, const int nrequired, const char *keys, const char *account = "");
char	*hdac_createmultisig(hdac_t *hdac, const int nrequired, const char *keys);
char	*hdac_createrawsendfrom(hdac_t *hdac, const char *from_addr, const char *json_data, const char *data = "", const char *action = "");
char	*hdac_signrawtx(hdac_t *hdac, const char *tx_hex, const char *prevtxs = "", const char *privatekeys = "", const char *sighashtype = "ALL");
char	*hdac_sendrawtx(hdac_t *hdac, const char *tx_hex, bool allowhighfees = false);


// Wallet:EXCHANGE
char	*hdac_createrawexchange(hdac_t *hdac, const char *txid, const int vout, const char *ask_assets);
char	*hdac_appendrawexchange(hdac_t *hdac, const char *hex, const char *txid, const int vout, const char *ask_assets);
char	*hdac_decoderawexchange(hdac_t *hdac, const char *tx_hex, const bool verbose = false);
char	*hdac_completerawexchange(hdac_t *hdac, const char *hex, const char *txid, const int vout, const char *ask_assets, const char *data = "",int datalen = 0);
char	*hdac_decoderawtx(hdac_t *hdac, const char *tx_hex);
char	*hdac_disablerawtx(hdac_t *hdac, const char *tx_hex);
char	*hdac_getrawchangeaddress(hdac_t *hdac);



//
// Hdac RPC command interface
//
class hdac {
public:
	hdac_t	*hdacp;

	hdac(const char *nodeip, const int port, const char *userid, const char *password)
		{ hdacp = hdac_open(nodeip, port, userid, password); }

	~hdac()
		{ hdac_close(hdacp); }


	string cmd(const char *data)
		{ return hdac_cmd(hdacp, data); }

	string receipt(const char *txid)
		{ return hdac_receipt(hdacp, txid); }

	int set_limit(const hdac_send_limit_t *lp)
		{ return hdac_set_send_limit(hdacp, lp); }

	hdac_send_limit_t *get_limit(const char *name)
		{ return hdac_get_send_limit(hdacp, name); }


	int name_set(const char *addr, const char *name)
		{ return hdac_name_set(hdacp, addr, name); }

	int name_unset(const char *addr, const char *name)
		{ return hdac_name_unset(hdacp, addr, name); }

	string name_get(const char *name)
		{
			char	outbuf[256] = {0};
			hdac_name_get(hdacp, name, outbuf, sizeof(outbuf));
			string retstr = outbuf;
			return retstr;
		}


	// == Blockchain ==
	string getbestblockhash()
		{ return hdac_getbestblockhash(hdacp); }
	string getblockbyhash(const char *hash, const int verbose = 1)
		{ return hdac_getblockbyhash(hdacp, hash, verbose); }
	string getblock(const uint32_t height, const int verbose = 1)
		{ return hdac_getblock(hdacp, height, verbose); }
	string getblockchaininfo()
		{ return hdac_getblockchaininfo(hdacp); }
	string getblockcount()
		{ return hdac_getblockcount(hdacp); }
	string getblockhash(const uint32_t index)
		{ return hdac_getblockhash(hdacp, index); }
	string getchaintips()
		{ return hdac_getchaintips(hdacp); }
	string getdifficulty()
		{ return hdac_getdifficulty(hdacp); }
	string getmempoolinfo()
		{ return hdac_getmempoolinfo(hdacp); }
	string getrawmempool(const bool verbose = false)
		{ return hdac_getrawmempool(hdacp, verbose); }
	string gettxout(const char *txid, const uint32_t nvout, const bool includemempool = false)
		{ return hdac_gettxout(hdacp, txid, nvout, includemempool); }
	string gettxoutsetinfo()
		{ return hdac_gettxoutsetinfo(hdacp); }
	string listassets(const char *asset_id, const bool verbose = false, const long int count = INT_MAX, const int start = 0)
		{ return hdac_listassets(hdacp, asset_id, verbose, count, start); }
	string listblocks(const char *block_set_id, const bool verbose = false)
		{ return hdac_listblocks(hdacp, block_set_id, verbose); }
	string listpermissions(const char *perm = "", const char *addr = "*", const bool verbose = false)
		{ return hdac_listpermissions(hdacp, perm, addr, verbose); }
	string liststreams(const char *stream_id = "*", const bool verbose = false, const long int count = INT_MAX, const int start = 0)
		{ return hdac_liststreams(hdacp, stream_id, verbose, count, start); }
//	string listupgrades(const char *upgrade_id = "*")
//		{ return hdac_listupgrades(hdacp, upgrade_id); }
//	string verifychain(const int checklevel = 3, const int numblocks = 288)
//		{ return hdac_verifychain(hdacp, checklevel, numblocks); }
	
	// == Control ==
	string clearmempool()
		{ return hdac_clearmempool(hdacp); }
	string getblockchainparams(const bool displaynames = true, const bool with_upgrades = true)
		{ return hdac_getblockchainparams(hdacp, displaynames, with_upgrades); }
	string getinfo()
		{ return hdac_getinfo(hdacp); }
	string getruntimeparams()
		{ return hdac_getruntimeparams(hdacp); }
	string pause_task(const char *tasks)
		{ return hdac_pause_task(hdacp, tasks); }
	string resume_task(const char *tasks)
		{ return hdac_resume_task(hdacp, tasks); }
	string setlastblockbyhash(const char *hash)
		{ return hdac_setlastblockbyhash(hdacp, hash); }
	string setlastblock(const int height)
		{ return hdac_setlastblock(hdacp, height); }
	string setruntimeparam(const char *param_name, const char *param_value = "")
		{ return hdac_setruntimeparam(hdacp, param_name, param_value); }
	
	// == Generating ==
	string getgenerate()
		{ return hdac_getgenerate(hdacp); }
	string gethashespersec()
		{ return hdac_gethashespersec(hdacp); }
	
	// == Mining ==
	string getblocktemplate(const char *jsonobj)
		{ return hdac_getblocktemplate(hdacp, jsonobj); }
	string getblockwindowsize()
		{ return hdac_getblockwindowsize(hdacp); }
	string getmininginfo()
		{ return hdac_getmininginfo(hdacp); }
	string getnetworkhashps(const int blocks = 120, const int height = -1)
		{ return hdac_getnetworkhashps(hdacp, blocks, height); }
//	string prioritiestx(const char *txid, const int priority_delta, const int fee_delta)
//		{ return hdac_prioritiestx(hdacp, txid, priority_delta, fee_delta); }
	
	// == Network ==
	string addnode(const char *node, const char *op)
		{ return hdac_addnode(hdacp, node, op); }
	string getaddednodeinfo(const bool dns, const char *node = "")
		{ return hdac_getaddednodeinfo(hdacp, dns, node); }
	string getconnectioncount()
		{ return hdac_getconnectioncount(hdacp); }
	string getnettotals()
		{ return hdac_getnettotals(hdacp); }
	string getnetworkinfo()
		{ return hdac_getnetworkinfo(hdacp); }
	string getpeerinfo()
		{ return hdac_getpeerinfo(hdacp); }
//	string ping()
//		{ return hdac_ping(hdacp); }
	
	// == Util ==
	string createkeypairs(const int count = 1)
		{ return hdac_createkeypairs(hdacp, count); }
	string estimatefee(const int nblocks)
		{ return hdac_estimatefee(hdacp, nblocks); }
	string estimatepriority(const int nblocks)
		{ return hdac_estimatepriority(hdacp, nblocks); }
	string validateaddress(const char *addr)
		{ return hdac_validateaddress(hdacp, addr); }
	string verifymessage(const char *addr, const char *signature, const char *message)
		{ return hdac_verifymessage(hdacp, addr, signature, message); }
	
	// == Wallet ==
	string addmultisigaddress(const int nrequired, const char *keys, const char *account = "")
		{ return hdac_addmultisigaddress(hdacp, nrequired, keys, account); }
	string approvefrom(const char *from_addr, const char *upgrade_id, const bool approve)
		{ return hdac_approvefrom(hdacp, from_addr, upgrade_id, approve); }
	string backupwallet(const char *dest)
		{ return hdac_backupwallet(hdacp, dest); }
	string combineunspent(const char *addr, const int minconf = 1, const int maxcombines = 100, const int mininputs = 2, const int maxinputs = 100, const int maxtime = 15)
		{ return hdac_combineunspent(hdacp, addr, minconf, maxcombines, mininputs, maxinputs, maxtime); }
	string dumpprivkey(const char *addr)
		{ return hdac_dumpprivkey(hdacp, addr); }
	string dumpwallet(const char *filename)
		{ return hdac_dumpwallet(hdacp, filename); }
	string encryptwallet(const char *passphrase)
		{ return hdac_encryptwallet(hdacp, passphrase); }
	string walletpassphrase(const char *passphrase, const int timeout_sec)
		{ return hdac_walletpassphrase(hdacp, passphrase, timeout_sec); }
#ifndef SCALABLE_WALLET
	string getaccount(const char *addr)
		{ return hdac_getaccount(hdacp, addr); }
	string getaccountaddress(const char *account)
		{ return hdac_getaccountaddress(hdacp, account); }
#endif
	string getaddressbalances(const char *addr, const int minconf = 1, const bool includeLocked = false)
		{ return hdac_getaddressbalances(hdacp, addr, minconf, includeLocked); }
	string getaddresses(const bool verbose = false)
		{ return hdac_getaddresses(hdacp, verbose); }
#ifndef SCALABLE_WALLET
	string getaddressesbyaccount(const char *account)
		{ return hdac_getaddressesbyaccount(hdacp, account); }
#endif
	string getaddresstx(const char *addr, const char *txid, const bool verbose = false)
		{ return hdac_getaddresstx(hdacp, addr, txid, verbose); }
	string getassetbalances(const char *account, const int minconf = 1, const bool includeWatchonly = false, const bool includeLocked = false)
		{ return hdac_getassetbalances(hdacp, account, minconf, includeWatchonly, includeLocked); }
	string getassettx(const char *asset_id, const char *txid, const bool verbose = false)
		{ return hdac_getassettx(hdacp, asset_id, txid, verbose); }
	string getbalance(const char *account = "", const int minconf = 1, const bool includeWatchonly = false)
		{ return hdac_getbalance(hdacp, account, minconf, includeWatchonly); }
	string getmultibalances(const char *addr = "", const char *assets = "*", const int minconf = 1, const bool includeLocked = false, const bool includeWatchonly = false)
		{ return hdac_getmultibalances(hdacp, addr, assets, minconf, includeLocked, includeWatchonly); }
	string getnewaddress(const char *account = "")
		{ return hdac_getnewaddress(hdacp, account); }
#ifndef SCALABLE_WALLET
	string getreceivedbyaccount(const char *account, const int minconf = 1)
		{ return hdac_getreceivedbyaccount(hdacp, account, minconf); }
#endif
	string getreceivedbyaddress(const char *addr, const int minconf = 1)
		{ return hdac_getreceivedbyaddress(hdacp, addr, minconf); }
	string gettotalbalances(const int minconf = 1, const bool includeWatchonly = false, const bool includeLocked = false)
		{ return hdac_gettotalbalances(hdacp, minconf, includeWatchonly, includeLocked); }
	string gettx(const char *txid, const bool includeWatchonly = false)
		{ return hdac_gettx(hdacp, txid, includeWatchonly); }
	string getrawtx(const char *txid, const int verbose)
		{ return hdac_getrawtx(hdacp, txid, verbose); }
	string gettxoutdata(const char *txid, const int vout, const long int count_bytes = INT_MAX, const int start_byte = 0)
		{ return hdac_gettxoutdata(hdacp, txid, vout, count_bytes, start_byte); }
	string getunconfirmedbalance()
		{ return hdac_getunconfirmedbalance(hdacp); }
	string getwalletinfo()
		{ return hdac_getwalletinfo(hdacp); }
	string getwallettx(const char *txid, const bool includeWatchonly = false, const bool verbose = false)
		{ return hdac_getwallettx(hdacp, txid, includeWatchonly, verbose); }
	string importaddress(const char *addrs, const char *label = "", const bool rescan = true)
		{ return hdac_importaddress(hdacp, addrs, label, rescan); }
	string importprivkey(const char *privkeys, const char *label = "", const bool rescan = true)
		{ return hdac_importprivkey(hdacp, privkeys, label, rescan); }
	string importwallet(const char *filename)
		{ return hdac_importwallet(hdacp, filename); }
	string issue(const char *addr, const char *asset_params, const double quantity, const double smallest_unit = 1, const double native_amount = 0, const char *custom_fields = "")
		{ return hdac_issue(hdacp, addr, asset_params, quantity, smallest_unit, native_amount, custom_fields); }
	string issuefrom(const char *fromaddr, const char *toaddr, const char *asset_params, const double quantity, const double smallest_unit = 1, const double native_amount = 0, const char *custom_fields = "")
		{ return hdac_issuefrom(hdacp, fromaddr, toaddr, asset_params, quantity, smallest_unit, native_amount, custom_fields); }
	string issuemore(const char *addr, const char *asset_id, const double quantity, const double native_amount = 0, const char *custom_fields = "")
		{ return hdac_issuemore(hdacp, addr, asset_id, quantity, native_amount, custom_fields); }
	string issuemorefrom(const char *fromaddr, const char *toaddr, const char *asset_id, const double quantity, const double native_amount = 0, const char *custom_fields = "")
		{ return hdac_issuemorefrom(hdacp, fromaddr, toaddr, asset_id, quantity, native_amount, custom_fields); }
	string keypoolrefill(const int newsize = 100)
		{ return hdac_keypoolrefill(hdacp, newsize); }
	string listaccounts(const int minconf = 1, const bool includeWatchonly = false)
		{ return hdac_listaccounts(hdacp, minconf, includeWatchonly); }
	string listaddresses(const char *addrs = "*", const bool verbose = false, const long int count = INT_MAX, const int start = 0)
		{ return hdac_listaddresses(hdacp, addrs, verbose, count, start); }
	string listaddressgroupings()
		{ return hdac_listaddressgroupings(hdacp); }
	string listaddresstxs(const char *addr, const int count = 10, const int skip = 0, const bool verbose = false)
		{ return hdac_listaddresstxs(hdacp, addr, count, skip, verbose); }
	string listassettxs(const char *asset_id, const bool verbose = false, const int count = 10, const int start = 0, const bool local_ordering = false)
		{ return hdac_listassettxs(hdacp, asset_id, verbose, count, start, local_ordering); }
	string listlockunspent()
		{ return hdac_listlockunspent(hdacp); }
#ifndef SCALABLE_WALLET
	string listreceivedbyaccount(const int minconf = 1, const bool includeempty = false, const bool includeWatchonly = false)
		{ return hdac_listreceivedbyaccount(hdacp, minconf, includeempty, includeWatchonly); }
	string listreceivedbyaddress(const int minconf = 1, const bool includeempty = false, const bool includeWatchonly = false)
		{ return hdac_listreceivedbyaddress(hdacp, minconf, includeempty, includeWatchonly); }
#endif
	string listsinceblock(const char *blockhash = "", const int target_confirms = 1, const bool includeWatchonly = false)
		{ return hdac_listsinceblock(hdacp, blockhash, target_confirms, includeWatchonly); }
	string listtxs(const char *account, const int count = 10, const int from = 0, const bool includeWatchonly = false)
		{ return hdac_listtxs(hdacp, account, count, from, includeWatchonly); }
	string listunspent(const int minconf = 1, const int maxconf = 9999999, const char *addrs = "")
		{ return hdac_listunspent(hdacp, minconf, maxconf, addrs); }
	string listwallettxs(const int count = 10, const int skip = 0, const bool includeWatchonly = false, const bool verbose = false)
		{ return hdac_listwallettxs(hdacp, count, skip, includeWatchonly, verbose); }
	string lockunspent(const bool unlock, const char *tx)
		{ return hdac_lockunspent(hdacp, unlock, tx); }
#ifndef SCALABLE_WALLET
	string move(const char *fromaccount, const char *toaccount, const double amount, const int minconf = 1, const char *comment = "")
		{ return hdac_move(hdacp, fromaccount, toaccount, amount, minconf, comment); }
#endif
	string preparelockunspent(const char *asset_qty, const bool lock = true)
		{ return hdac_preparelockunspent(hdacp, asset_qty, lock); }
	string preparelockunspentfrom(const char *fromaddr, const char *asset_qty, const bool lock = true)
		{ return hdac_preparelockunspentfrom(hdacp, fromaddr, asset_qty, lock); }
	string resendwallettxs()
		{ return hdac_resendwallettxs(hdacp); }
	string send(const char *addr, const double amount, const char *comment = "", const char *comment_to = "")
		{ return hdac_send(hdacp, addr, amount, comment, comment_to); }
	string send_asset(const char *addr, const char *asset_qty, const char *comment = "", const char *comment_to = "")
		{ return hdac_send_asset(hdacp, addr, asset_qty, comment, comment_to); }
	string sendasset(const char *addr, const char *asset_id, const double asset_qty, const double native_amount, const char *comment = "", const char *comment_to = "")
		{ return hdac_sendasset(hdacp, addr, asset_id, asset_qty, native_amount, comment, comment_to); }
	string sendassetfrom(const char *fromaddr, const char *toaddr, const char *asset_id, const double asset_qty, const double native_amount, const char *comment = "", const char *comment_to = "")
		{ return hdac_sendassetfrom(hdacp, fromaddr, toaddr, asset_id, asset_qty, native_amount, comment, comment_to); }
	string sendfrom(const char *fromaddr, const char *toaddr, const double amount, const char *comment = "", const char *comment_to = "")
		{ return hdac_sendfrom(hdacp, fromaddr, toaddr, amount, comment, comment_to); }
	string sendfrom_asset(const char *fromaddr, const char *toaddr, const char *asset_qty, const char *comment = "", const char *comment_to = "")
		{ return hdac_sendfrom_asset(hdacp, fromaddr, toaddr, asset_qty, comment, comment_to); }
#ifndef SCALABLE_WALLET
	string sendfromaccount(const char *fromaccount, const char *toaddr, const double amount, const int minconf = 1, const char *comment = "", const char *comment_to = "")
		{ return hdac_sendfromaccount(hdacp, fromaccount, toaddr, amount, minconf, comment, comment_to); }
	string sendmany(const char *fromaccount, const char *addr_amounts, const int minconf = 1, const char *comment = "")
		{ return hdac_sendmany(hdacp, fromaccount, addr_amounts, minconf, comment); }
#endif
	string sendwithdata(const char *addr, const double amount, const char *data, int datalen = 0)
		{ return hdac_sendwithdata(hdacp, addr, amount, data, datalen); }
	string sendwithdata_asset(const char *addr, const char *asset_qty, const char *data, int datalen = 0)
		{ return hdac_sendwithdata_asset(hdacp, addr, asset_qty, data, datalen); }
	string sendwithdatafrom(const char *fromaddr, const char *toaddr, const double amount, const char *data, int datalen = 0)
		{ return hdac_sendwithdatafrom(hdacp, fromaddr, toaddr, amount, data, datalen); }
	string sendwithdatafrom_asset(const char *fromaddr, const char *toaddr, const char *asset_qty, const char *data, int datalen = 0)
		{ return hdac_sendwithdatafrom_asset(hdacp, fromaddr, toaddr, asset_qty, data, datalen); }
#ifndef SCALABLE_WALLET
	string setaccount(const char *addr, const char *account)
		{ return hdac_setaccount(hdacp, addr, account); }
#endif
	string settxfee(const double amount)
		{ return hdac_settxfee(hdacp, amount); }
	string signmessage(const char *addr_or_privkey, const char *message)
		{ return hdac_signmessage(hdacp, addr_or_privkey, message); }
	
	
	// Wallet:GRANT
	string grant(const char *addrs, const char *perms, const double native_amount = 0.0, const int startblock = 0, const int endblock = 4294967295, const char *comment = "", const char *comment_to = "")
		{ return hdac_grant(hdacp, addrs, perms, native_amount, startblock, endblock, comment, comment_to); }
	string grantfrom(const char *fromaddr, const char *toaddrs, const char *perms, const double native_amount = 0.0, const int startblock = 0, const int endblock = 4294967295, const char *comment = "", const char *comment_to = "")
		{ return hdac_grantfrom(hdacp, fromaddr, toaddrs, perms, native_amount, startblock, endblock, comment, comment_to); }
	string grantwithdata(const char *addrs, const char *perms, const char *data, int datalen = 0, const double native_amount = 0.0, const int startblock = 0, const int endblock = 4294967295)
		{ return hdac_grantwithdata(hdacp, addrs, perms, data, datalen, native_amount, startblock, endblock); }
	string grantwithdatafrom(const char *fromaddr, const char *toaddrs, const char *perms, const char *data, int datalen = 0, const double native_amount = 0.0, const int startblock = 0, const int endblock = 4294967295)
		{ return hdac_grantwithdatafrom(hdacp, fromaddr, toaddrs, perms, data, datalen, native_amount, startblock, endblock); }
	string revoke(const char *addrs, const char *perms, const double native_amount = 0, const char *comment = "", const char *comment_to = "")
		{ return hdac_revoke(hdacp, addrs, perms, native_amount, comment, comment_to); }
	string revokefrom(const char *fromaddr, const char *toaddrs, const char *perms, const double native_amount = 0, const char *comment = "", const char *comment_to = "")
		{ return hdac_revokefrom(hdacp, fromaddr, toaddrs, perms, native_amount, comment, comment_to); }
	
	
	// Wallet:STREAM
	string create(const char *entity_type, const char *entity_name, const bool open, const char *custom_fields = "")
		{ return hdac_create(hdacp, entity_type, entity_name, open, custom_fields); }
	string createfrom(const char *from_addr, const char *entity_type, const char *entity_name, const bool open, const char *custom_fields)
		{ return hdac_createfrom(hdacp, from_addr, entity_type, entity_name, open, custom_fields); }
	string getstreamitem(const char *stream_id, const char *txid, const bool verbose = false)
		{ return hdac_getstreamitem(hdacp, stream_id, txid, verbose); }
	string liststreamblockitems(const char *stream_id, const char *block_set_id, const bool verbose = false, const long int count = INT_MAX, const int start = 0)
		{ return hdac_liststreamblockitems(hdacp, stream_id, block_set_id, verbose, count, start); }
	string liststreamitems(const char *stream_id, const bool verbose = false, const long int count = INT_MAX, const int start = 0, const bool local_ordering = false)
		{ return hdac_liststreamitems(hdacp, stream_id, verbose, count, start, local_ordering); }
	string liststreamkeyitems(const char *stream_id, const char *stream_key, const bool verbose = false, const long int count = 10, const int start = 0, const bool local_ordering = false)
		{ return hdac_liststreamkeyitems(hdacp, stream_id, stream_key, verbose, count, start, local_ordering); }
	string liststreamkeys(const char *stream_id, const char *stream_key = "*", const bool verbose = false, const long int count = INT_MAX, const int start = 0, const bool local_ordering = false)
		{ return hdac_liststreamkeys(hdacp, stream_id, stream_key, verbose, count, start, local_ordering); }
	string liststreampublisheritems(const char *stream_id, const char *addr, const bool verbose = false, const int count = 10, const int start = 0, const bool local_ordering = false)
		{ return hdac_liststreampublisheritems(hdacp, stream_id, addr, verbose, count, start, local_ordering); }
	string liststreampublishers(const char *stream_id, const char *addrs = "*", const bool verbose = false, const long int count = INT_MAX, const int start = 0, const bool local_ordering = false)
		{ return hdac_liststreampublishers(hdacp, stream_id, addrs, verbose, count, start, local_ordering); }
	string publish(const char *stream_id, const char *item_key, const char *data, int datalen = 0)
		{ return hdac_publish(hdacp, stream_id, item_key, data, datalen); }
	string publishfrom(const char *fromaddr, const char *stream_id, const char *item_key, const char *data, int datalen = 0)
		{ return hdac_publishfrom(hdacp, fromaddr, stream_id, item_key, data, datalen); }
	string subscribe(const char *entity_ids, const bool rescan = true)
		{ return hdac_subscribe(hdacp, entity_ids, rescan); }
	string unsubscribe(const char *json_id)
		{ return hdac_unsubscribe(hdacp, json_id); }
	
	
	// Wallet:MULTISIG
	string createmultisig(const int nrequired, const char *keys)
		{ return hdac_createmultisig(hdacp, nrequired, keys); }
	string createrawsendfrom(const char *from_addr, const char *json_data, const char *data = "", const char *action = "")
		{ return hdac_createrawsendfrom(hdacp, from_addr, json_data, data, action); }
	string signrawtx(const char *tx_hex, const char *prevtxs = "", const char *privatekeys = "", const char *sighashtype = "ALL")
		{ return hdac_signrawtx(hdacp, tx_hex, prevtxs, privatekeys, sighashtype); }
	string sendrawtx(const char *tx_hex, bool allowhighfees = false)
		{ return hdac_sendrawtx(hdacp, tx_hex, allowhighfees); }

	// Wallet:EXCHANGE
	string createrawexchange(const char *txid, const int vout, const char *ask_assets)
		{ return hdac_createrawexchange(hdacp, txid, vout, ask_assets); }
	string appendrawexchange(const char *hex, const char *txid, const int vout, const char *ask_assets)
		{ return hdac_appendrawexchange(hdacp, hex, txid, vout, ask_assets); }
	string decoderawexchange(const char *tx_hex, const bool verbose = false)
		{ return hdac_decoderawexchange(hdacp, tx_hex, verbose); }
	string completerawexchange(const char *hex, const char *txid, const int vout, const char *ask_assets, const char *data = "")
		{ return hdac_completerawexchange(hdacp, hex, txid, vout, ask_assets, data); }
	string decoderawtx(const char *tx_hex)
		{ return hdac_decoderawtx(hdacp, tx_hex); }
	string disablerawtx(const char *tx_hex)
		{ return hdac_disablerawtx(hdacp, tx_hex); }
	string getrawchangeaddress()
		{ return hdac_getrawchangeaddress(hdacp); }
};


#endif	// __RPCCMD_H__
