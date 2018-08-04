//
// Copyright (c) 2018 Hdac Technology AG
// Hdac code distributed under the GPLv3 license
//============================================================================================
// hdaclib.h
//	Hdac SDK/API header file
//
// History
//	2018/02/00	Add hdac_auth_* functions
//	2018/03/00	Add rpc command APIs
//	2018/04/00	Add document certification functions
//	2018/04/00	Add multisig functions (example)
//	2018/06/07	Smart contract PoC integrated
//============================================================================================
//

// hdaclib.h
//
#ifndef __HDACLIB_H__
#define __HDACLIB_H__


#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>

#include <curl/curl.h>

#include "libhash.h"
#include "json2.h"


#define HDAC_ADDR_LEN		40	// hdac=34 
#define HDAC_TX_LEN		80	// 64


enum {
	E_OK			= 0,
	E_NOT_FOUND		= -1,
	E_EXIST			= -2,
	E_WRITE_ERROR		= -3,
	E_READ_ERROR		= -4,
	E_FAIL			= -5,
	E_HDAC_SEND_LIMIT	= -9,

	// map
	E_NULL_ARG		= -100,
	E_WRONG_PERM		= -101,
	E_CREATE_STREAM		= -102,
	E_GRANT_PERM		= -103,
	E_STREAM_NOT_FOUND	= -104,
	E_STREAM_NO_PERM	= -105,

	// multisig
	E_MULTISIG_ADDRESS	= -200,
	E_IMPORT_ADDRESS	= -201,
	E_CREATE_RAW_SEND	= -202,
	E_SIGN_RAW_TX		= -203,
	E_SEND_RAW_TX		= -204,

	// wallet
	E_ISSUE			= -301,

	E_END_ERROR		= -999
};


//
// Authentication 
//

#define MAX_AUTH_DATA		20

typedef struct {
	char	fromaddr[HDAC_ADDR_LEN];
	char	toaddr[HDAC_ADDR_LEN];
	char	asset_name[128];
	char	asset_amount[32];
	char	amount[32];
}	hdac_auth_txdata_t;

typedef struct {
	char	name[20];
	char	value[256];
	int	matched;
}	hdac_auth_data2_t;

typedef struct {
	char	id[100];
	hdac_auth_data2_t data[MAX_AUTH_DATA];
	int	hashed[MAX_AUTH_DATA];
	char	op[20];
	char	chksum[100];
	int	retval;
	char	result[HDAC_TX_LEN];	// txid
	int	num;		// nitem
}	hdac_auth2_t;


// 
// Document original certification 
//
typedef struct {
	char	groupname[128];		// group name
	char	keyname[128];		// document searching key
	bool	ispublic;		// true if public data

	char	*comment;
	void	*contents;
	size_t	contents_len;

	// result
	char	addr[HDAC_ADDR_LEN];
	char	txid[HDAC_TX_LEN];
	char	comment_hash[128];
	char	contents_hash[128];
}	hdac_cert_t;


typedef struct {
	// input
	char	txid[HDAC_TX_LEN];	// txid of document tx

	char	groupname[128];		// group name
	char	keyname[128];		// document searching key

	// output
	char	comment_hash[128];
	char	contents_hash[128];
	char	*comment;		// comment (allocated)
	char	*jsonstr;		// json result (allocated)
}	hdac_cert_find_t;


//
// Send limit
//

#define	HDAC_MAX_SEND_LIMIT	100

typedef struct {
	char	name[128];
	int	seconds;
	int	max_send;
	double	max_amount;
	char	fromaddr[HDAC_ADDR_LEN];	// from address
	char	toaddr[HDAC_ADDR_LEN];		// to address

	time_t	starttime;			// check start time
	bool	overflow;			// stop sending: when name or seconds exceeds
}	hdac_send_limit_t;


typedef struct {
	char	ip[64];		// RPC IP
	int	port;		// RPC Port
	char	user[128];	// RPC UserId
	char	password[128];	// RPC Password
}	hdac_rpc_t;

//
// hdac i/o
//
typedef struct {
	hdac_rpc_t rpc;
	void	*callback;		// RPC Callback function 
	CURL	*curl;
	char	curl_errbuf[CURL_ERROR_SIZE + 1];

	char	*cmdjson;		// response JSON
	char	*retjson;		// response JSON
	int	retjsonlen;		// response JSON length
	char	message[256];
	char	txid[256];		// transaction id of command result

	int	nlimit;
	hdac_send_limit_t	*limits;
	hdac_send_limit_t	*send_stat;

	hdac_auth_txdata_t	txdata;
}	hdac_t;


//
// multisig
//

#define HDAC_PUBKEY_LEN		128
#define HDAC_MAX_MULTISIG	10


typedef struct {
	int	nsig;				// IN
	char	addr[HDAC_MAX_MULTISIG][HDAC_ADDR_LEN];		// IN
	char	pubkey[HDAC_MAX_MULTISIG][HDAC_PUBKEY_LEN];	// IN
	
	char	stream[32];			// IN (optional) default=MULTISIG
	char	asset_name[128];		// IN (optional)
	char	send_value[64];			// IN (optional)

	char	recvaddr[HDAC_ADDR_LEN];	// OUT from hdac_multisig()
	char	txid[HDAC_TX_LEN];		// OUT

	char	sign1[1600];			// 1'st OUT
	char	sign2[2048];			// 2'nd OUT
} multisig_t;



extern	int	_hdac_error_code;
extern	char	_hdac_error_msg[512];


// Authentication
extern	hdac_auth_txdata_t	_auth_txdata;

int	hdac_auth_init(hdac_t *hdac, const hdac_auth_txdata_t *txdata);

int	hdac_auth_reg(hdac_t *hdac, hdac_auth2_t *authi, const char *opt);
int	hdac_auth_test(hdac_t *hdac, hdac_auth2_t *authi);
int	hdac_auth_delete(hdac_t *hdac, const char *id);

int	hdac_auth2_reg(hdac_t *hdac, hdac_auth2_t *authi, const char *opt);
int	hdac_auth2_test(hdac_t *hdac, hdac_auth2_t *authi);
int	hdac_auth2_delete(hdac_t *hdac, const char *id);


hdac_cert_t	*hdac_cert_doc_reg(hdac_t *hdac, hdac_cert_t *certi);
hdac_cert_find_t	*hdac_cert_doc_find(hdac_t *hdac, hdac_cert_find_t *findi);


// multisig
int	hdac_multisig(hdac_t *hdac, multisig_t *msig);
int	hdac_multisig2(hdac_t *hdac, multisig_t *msig, int nth_sign);
int	hdac_multisig_send(hdac_t *hdac, multisig_t *msig);


// send limit
int	hdac_load_all_send_limit(hdac_t *hdac);
hdac_send_limit_t	*hdac_get_send_limit(hdac_t *hdac, const char *name, int *retcount = NULL);
int	hdac_set_send_limit(hdac_t *hdac, const hdac_send_limit_t *data);
int	hdac_check_send_stat(hdac_t *hdac, const char *name, const int count, const double amount, const char *fromaddr, const char *toaddr, const char *jsonstr);
int	hdac_check_send_stat_asset(hdac_t *hdac, const char *asset_qty, const char *fromaddr, const char *toaddr, const char *jsonstr);
int	hdac_cmp_send_limit(hdac_t *hdac, hdac_send_limit_t *cmplp);


// DNS
int	hdac_name_set(hdac_t *hdac, const char *addr, const char *name);
int	hdac_name_unset(hdac_t *hdac, const char *addr, const char *name);
char	*hdac_name_get(hdac_t *hdac, const char *name, char *outbuf, size_t outbuflen);

// old DNS
int	hdac_name_set_file(const char *addr, const char *name);
int	hdac_name_unset_file(const char *addr, const char *name);
char	*hdac_name_get_file(const char *name, char *outbuf, size_t outbuflen);

// utility functions
int	split_token(const char *data, char *tokenv[], int maxnumtoken);
char	*strip_quote(char *data);
bool	valid_address(const char *addr);


// hdaclib
int	hdac_load_config(const char *cfgfile, hdac_rpc_t *rpc);
hdac_t	*hdac_open(const hdac_rpc_t *rpc);
hdac_t	*hdac_open(const char *nodeip, const int port, const char *userid, const char *password);
void	hdac_close(hdac_t *hdac);
char	*hdac_cmd(hdac_t *hdac, const char *data);

char	*hdac_receipt(hdac_t *hdac, const char *txid);

void	hdac_seterror(int errcode, char *errmsg);
char	*hdac_geterror();

char	*autoreadfile(char *filename, size_t *filesz);
size_t	writefile(const char *filename, const char *buffer, const size_t bufsz);

string	bin2hex(const string binbuf);
string	hex2bin(const string hexbuf);


extern	const	char	*_hdac_debug;


#endif	// __HDACLIB_H__
