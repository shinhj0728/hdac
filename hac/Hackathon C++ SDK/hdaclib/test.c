//
// Copyright (c) 2018 Hdac Technology AG
// Hdac code distributed under the GPLv3 license
//============================================================================================
// test.c
//	Hdac SDK/API test program
//
// History
//	2018/05/00	Initial code
//============================================================================================
//

#include "rpc.h"
#include "hdaclib.h"
#include "contract.h"


using namespace std;


void	init(hdac_t *hdac);
void	test_basic_rpc(hdac_t *hdac);
void	test_api(hdac_t *hdac);
void	test_auth(hdac_t *hdac);
void	test_auth2(hdac_t *hdac);
void	test_dns(hdac_t *hdac);
void	test_rpc(hdac_t *hdac);
void	test_cert_doc(hdac_t *hdac);
void	test_multisig(hdac_t *hdac);

void	test_rpc_blockchain(hdac_t *hdac);
void	test_rpc_control(hdac_t *hdac);
void	test_rpc_generating(hdac_t *hdac);
void	test_rpc_mining(hdac_t *hdac);
void	test_rpc_network(hdac_t *hdac);
void	test_rpc_block_tx(hdac_t *hdac);
void	test_rpc_util(hdac_t *hdac);
void	test_rpc_grant(hdac_t *hdac);
void	test_rpc_stream(hdac_t *hdac);
void	test_rpc_wallet_send(hdac_t *hdac);
void	test_rpc_wallet(hdac_t *hdac);
void	test_rpc_raw_xch(hdac_t *hdac);

void	read_enter(int level);


static	char	_fromaddr[128] = {0};
static	char	_toaddr[128] = {0};
static	char	_toaddr2[128] = {0};

static	char	_txid[128] = {0};
static	char	_assettxid[128] = {0};
static	char	_metatxid[128] = {0};


int	main()
{
	hdac_rpc_t rpc = {0};
	hdac_t	*hdac = NULL;

	if (hdac_load_config("rpc.cfg", &rpc) == E_OK)
		hdac = hdac_open(&rpc);
	else
		hdac = hdac_open("192.168.1.7", 38822, "hdacwallet", "hdac.wallet");

	if (hdac)
	{
		printf("\nhdac_open(%s, %d, %s, %s) OK\n\n", 
			hdac->rpc.ip, hdac->rpc.port, hdac->rpc.user, hdac->rpc.password);
		read_enter(0);

		init(hdac);
		read_enter(0);

		test_basic_rpc(hdac);
		read_enter(0);

		test_api(hdac);
		read_enter(0);

		test_auth(hdac);	// 하나의 주소에 등록하는 방식 
		read_enter(0);

		test_auth2(hdac);	// 등록시 새로운 주소 생성하는 방식 
		read_enter(0);

		test_dns(hdac);		// name-address mapping service
		read_enter(0);

		test_rpc(hdac);	// RPC 명령어 전체 테스트 

		test_cert_doc(hdac);
		read_enter(0);

		test_multisig(hdac);
		read_enter(0);

		hdac_close(hdac);
	}

	return 0;
}


void	read_enter(int level)
{
	printf("\n\nReady.. type ENTER ");
	while (1)
	{
		int	ch = getc(stdin);

		if (ch == EOF)
			break;
		if (ch == '\n')
			break;
	}
	if (level == 0)
		printf("================================================================================\n");
	else if (level == 1)
		printf("--------------------\n");
}


void	init(hdac_t *hdac)
{
	json	addrlist, list;
	char	tmp[256] = {0}, cmd[256] = {0};

	printf("=====INITIALIZE\n\n");

	// 최초의 주소는 _fromaddr
	sprintf(cmd, "getaddresses");
	list = hdac_cmd(hdac, cmd);

	addrlist = list["result"];
	if (addrlist.size() < 3)
	{
		printf("Create new 2nd address...\n");
		sprintf(cmd, "getnewaddress");
		list = hdac_cmd(hdac, cmd);
		printf("CMD=%s\n\n", cmd);
	//	list.print();
	//	printf("\n\n");

		printf("Create new 3rd address...\n");
		sprintf(cmd, "getnewaddress");
		list = hdac_cmd(hdac, cmd);
		printf("CMD=%s\n\n", cmd);
	//	list.print();
	//	printf("\n\n");
	}

	sprintf(cmd, "getaddresses");
	list = hdac_cmd(hdac, cmd);
	printf("CMD=%s\n\n", cmd);
//	list.print();
//	printf("\n\n");

	list["result"][0].tostr(_fromaddr, sizeof(_fromaddr));
	printf("FROM addr = %s\n", _fromaddr);

	list["result"][1].tostr(_toaddr, sizeof(_toaddr));
	printf("TO   addr = %s\n", _toaddr);

	list["result"][2].tostr(_toaddr2, sizeof(_toaddr2));
	printf("TO   addr2 = %s\n", _toaddr2);

	printf("\n\n");


	printf("Create HDACT asset...\n");
	sprintf(cmd, "issue %s HDACT 100000000 0.00000001 ", _fromaddr);
	list = hdac_cmd(hdac, cmd);

	printf("Create USD asset...\n");
	sprintf(cmd, "issue %s USD 1000000 0.00000001 ", _fromaddr);
	list = hdac_cmd(hdac, cmd);
	printf("CMD=%s\n\n", cmd);

	sprintf(cmd, "getaddressbalances \"%s\"", _fromaddr);
	list = hdac_cmd(hdac, cmd);
	printf("CMD=%s\n\n", cmd);
	list.print();

	for (int nn = 0; nn < list["result"].size(); nn++)
	{
		string name, qty;
		name = list["result"][nn]["name"].tostr();
		qty = list["result"][nn]["qty"].tostr();
		if (name == "")
			name = "HDAC*C";
		printf("%s	%s\n", name.c_str(), qty.c_str());
	}

	printf("\njson[0][0]=%s\n", list[0][0].tostr().c_str());

	for (int nn = 0; nn < 3; nn++)
	{
		printf("\njson[\"result\"][%d]=%s\n", nn, list["result"][nn].tostr().c_str());
	}
	printf("\n\n");
}


void	test_basic_rpc(hdac_t *hdac)
{
	json	list;
	char	cmd[512] = {0};

	printf("=====BASIC RPC\n\n");

	strcpy(cmd, "{\"jsonrpc\": \"1.0\", \"id\":\"xrpc\", \"method\": \"getinfo\", \"params\": [] }");

	list = hdac_cmd(hdac, cmd);
	printf("CMD=%s\n\n", cmd);
	list.print();
	printf("\n\n");
	read_enter(1);

	sprintf(cmd, "{\"jsonrpc\": \"1.0\", \"id\":\"xrpc\", \"method\": \"getaddressbalances\", \"params\": [ \"%s\" ] }", _fromaddr);
	list = hdac_cmd(hdac, cmd);
	printf("CMD=%s\n\n", cmd);
	list.print();
	printf("\n\n");

	sprintf(cmd, "sendasset %s HDACT 0.1", _toaddr);
	list = hdac_cmd(hdac, cmd);
	printf("CMD=%s\n\n", cmd);
//	list.print();
//	printf("\n\n");

	list["result"].tostr(_assettxid, sizeof(_assettxid));
	if (_txid[0] == 0)
		strcpy(_txid, _assettxid);
	printf("TXID=%s\n", _txid);
	printf("\n\n");
}


// API를 테스트하기 위한 부분 
void	test_api(hdac_t *hdac)
{
	json	list;
	char	cmd[256] = {0};


	printf("=====RECEIPT\n\n");

	list = hdac_receipt(hdac, _txid);
	printf("CMD=hdac_receipt(%s)\n", _txid);
	list.print();
	printf("\n\n");
	read_enter(0);


	printf("=====LIMIT\n\n");

	hdac_send_limit_t	limit = {0};

	strcpy(limit.name, "HDACT");
	limit.seconds = 24 * 3600;
	limit.max_send = 100;
	limit.max_amount = 10.0;

	printf("hdac_set_send_limit(%s %d %d %lf %s %s)\n",
		limit.name, limit.seconds, limit.max_send, limit.max_amount, limit.fromaddr, limit.toaddr);
	if (hdac_set_send_limit(hdac, &limit) != E_OK)
		printf("	FAILED\n");

	strcpy(limit.name, "HDACT");
	limit.seconds = 300;
	limit.max_send = 10;
	limit.max_amount = 1.0;

	printf("hdac_set_send_limit(%s %d %d %lf %s %s)\n",
		limit.name, limit.seconds, limit.max_send, limit.max_amount, limit.fromaddr, limit.toaddr);
	if (hdac_set_send_limit(hdac, &limit) != E_OK)
		printf("	FAILED\n");

	strcpy(limit.name, "HDAC");
	limit.seconds = 3600;
	limit.max_send = 10;
	limit.max_amount = 10.0;
	strcpy(limit.fromaddr, _fromaddr);

	printf("hdac_set_send_limit(%s %d %d %lf %s %s)\n",
		limit.name, limit.seconds, limit.max_send, limit.max_amount, limit.fromaddr, limit.toaddr);
	if (hdac_set_send_limit(hdac, &limit) != E_OK)
		printf("	FAILED\n");
	
	strcpy(limit.name, "HDACT");
	limit.seconds = 600;
	limit.max_send = 10;
	limit.max_amount = 10.0;
	strcpy(limit.fromaddr, _fromaddr);
	strcpy(limit.toaddr, _toaddr);

	printf("hdac_set_send_limit(%s %d %d %lf %s %s)\n",
		limit.name, limit.seconds, limit.max_send, limit.max_amount, limit.fromaddr, limit.toaddr);
	if (hdac_set_send_limit(hdac, &limit) != E_OK)
		printf("	FAILED\n");
	read_enter(0);

	printf("hdac_get_send_limit(HDACT)\n");
	int	nlimit = 0;
	hdac_send_limit_t *limits = hdac_get_send_limit(hdac, "HDACT", &nlimit);
	for (int nn = 0; nn < nlimit; nn++)
	{
		hdac_send_limit_t *lp = &limits[nn];

		printf("[%2d] name=%-10s sec=%-8d max_send=%-6d max_amount=%lf from=%s to=%s\n",
			nn, lp->name, lp->seconds, lp->max_send, lp->max_amount, lp->fromaddr, lp->toaddr);
	}
	free(limits);
	printf("\n\n");
	read_enter(0);

	printf("hdac_load_all_send_limit()\n");
	hdac_load_all_send_limit(hdac);
	for (int nn = 0; nn < hdac->nlimit; nn++)
	{
		hdac_send_limit_t *lp = &hdac->limits[nn];

		if (lp->name[0] == 0 && lp->seconds <= 0)
			continue;
		printf("[%2d] name=%-10s sec=%-8d max_send=%-6d max_amount=%lf from=%s to=%s\n",
			nn, lp->name, lp->seconds, lp->max_send, lp->max_amount, lp->fromaddr, lp->toaddr);
	}
	printf("\n\n");
	read_enter(0);

	printf("sendfrom(101): LIMIT ERROR condition from=%s to=%s\n", _fromaddr, _toaddr);
	list = hdac_sendfrom(hdac, _fromaddr, _toaddr, 101.0);
	list.print();
	printf("\n\n");
	read_enter(0);
}


void	test_auth(hdac_t *hdac)
{
	int	ret = 0;
	char	uniqid[200] = {0};


	// 하나의 주소를 이용해서 데이터 저장/조회 방법 (id 사용)
	printf("=====AUTH\n\n");

	// 사용자 인증 
	hdac_auth_txdata_t auth_txdata = {0};

	strcpy(auth_txdata.fromaddr, _fromaddr);
	strcpy(auth_txdata.toaddr, _toaddr);
	strcpy(auth_txdata.asset_name, "HDACT");
	strcpy(auth_txdata.asset_amount, "1");
	strcpy(auth_txdata.amount, "1");

	hdac_auth_init(hdac, &auth_txdata);

	hdac_auth2_t	authi = {0};

	// 인증 정보 등록 
	strcpy(authi.id, "leejm");
	strcpy(authi.data[0].name, "userid");
	strcpy(authi.data[0].value, "leejm");
	strcpy(authi.data[1].name, "password");
	strcpy(authi.data[1].value, "qwer1234");
	strcpy(authi.data[2].name, "jumin");
	strcpy(authi.data[2].value, "661220");
	strcpy(authi.data[3].name, "mobile");
	strcpy(authi.data[3].value, "01084887188");
	strcpy(authi.data[4].name, "email");
	strcpy(authi.data[4].value, "firewire@empal.com");

	ret = hdac_auth_reg(hdac, &authi, "force");
	printf("	REG result=%d key=%s\n", ret, authi.result);
	ret = hdac_auth_reg(hdac, &authi, "force");
	printf("	REG result=%d key=%s\n", ret, authi.result);
	strcpy(uniqid, authi.result);


	// 인증 정보 검증 (테스트)
	memset(&authi.data, 0, sizeof(authi.data));

	strcpy(authi.id, "leejm");
	strcpy(authi.data[1].name, "password");
	strcpy(authi.data[1].value, "qwer1234");
	strcpy(authi.data[2].name, "jumin");
	strcpy(authi.data[2].value, "112233");
	strcpy(authi.data[3].name, "email");
	strcpy(authi.data[3].value, "firewire@empal.com");
	strcpy(authi.data[4].name, "mobile");
	strcpy(authi.data[4].value, "01011112222");

	ret = hdac_auth_test(hdac, &authi);
	printf("	TEST nmatched=%d %s\n", ret, authi.result);

	// 인증 정보 삭제 
	ret = hdac_auth_delete(hdac, uniqid);
	printf("	DELETE result=%d %s\n", ret, authi.result);

	ret = hdac_auth_test(hdac, &authi);
	printf("	TEST nmatched=%d %s\n", ret, authi.result);
}


void	test_auth2(hdac_t *hdac)
{
	int	ret = 0;
	char	uniqid[200] = {0};


	// 하나의 주소를 이용해서 데이터 저장/조회 방법 (id 사용)
	printf("=====AUTH2\n\n");

	// 사용자 인증 
	hdac_auth_txdata_t auth_txdata = {0};

	strcpy(auth_txdata.fromaddr, _fromaddr);
	strcpy(auth_txdata.toaddr, _toaddr);
	strcpy(auth_txdata.asset_name, "HDACT");
	strcpy(auth_txdata.asset_amount, "1");
	strcpy(auth_txdata.amount, "1");

	hdac_auth_init(hdac, &auth_txdata);

	hdac_auth2_t	authi = {0};

	// 인증 정보 등록 
	strcpy(authi.id, "");
	strcpy(authi.data[0].name, "userid");
	strcpy(authi.data[0].value, "leejm");
	strcpy(authi.data[1].name, "password");
	strcpy(authi.data[1].value, "qwer1234");
	strcpy(authi.data[2].name, "jumin");
	strcpy(authi.data[2].value, "661220");
	strcpy(authi.data[3].name, "mobile");
	strcpy(authi.data[3].value, "01084887188");
	strcpy(authi.data[4].name, "email");
	strcpy(authi.data[4].value, "firewire@empal.com");

	ret = hdac_auth2_reg(hdac, &authi, "force");
	printf("	REG result=%d key=%s\n", ret, authi.result);
	strcpy(uniqid, authi.result);

	// 인증 정보 검증 (테스트)
	memset(&authi.data, 0, sizeof(authi.data));
	strcpy(authi.id, uniqid);
	strcpy(authi.data[1].name, "password");
	strcpy(authi.data[1].value, "qwer1234");
	strcpy(authi.data[2].name, "jumin");
	strcpy(authi.data[2].value, "112233");
	strcpy(authi.data[3].name, "mobile");
	strcpy(authi.data[3].value, "01011112222");
	strcpy(authi.data[4].name, "email");
	strcpy(authi.data[4].value, "firewire@empal.com");

	ret = hdac_auth2_test(hdac, &authi);
	printf("	TEST result=%d %s\n", ret, authi.result);

	// 인증 정보 삭제 
	ret = hdac_auth2_delete(hdac, uniqid);
	printf("	DELETE result=%d %s\n", ret, authi.result);

	ret = hdac_auth2_test(hdac, &authi);
	printf("	TEST result=%d %s\n", ret, authi.result);
}


void	test_dns(hdac_t *hdac)
{
	const	char	*addr = _toaddr;
	const	char	*name = "수신주소.1";
	char	buf[256] = {0};
	int	ret = 0;


	printf("=====DNS\n\n");

	printf("     hdac_name_set() test...\n\n");
	ret = hdac_name_set(hdac, addr, name);
	printf("hdac_name_set() ret=%d\n\n", ret);

	printf("hdac_name_get(addr로 name 입수) test...\n\n");
	buf[0] = 0;
	hdac_name_get(hdac, addr, buf, sizeof(buf));
	printf("	addr=%s ==> name=%s\n", addr, buf);

	printf("hdac_name_get(name로 addr 입수) test...\n\n");
	buf[0] = 0;
	hdac_name_get(hdac, name, buf, sizeof(buf));
	printf("	name=%s ==> addr=%s\n", name, buf);
	printf("\n\n");

	printf("hdac_name_unset(%s, %s) test...\n\n", addr, name);
	ret = hdac_name_unset(hdac, addr, name);
	printf("    hdac_name_unset() ret=%d\n", ret);

	printf("hdac_name_get(name로 addr 입수) test...\n\n");
	buf[0] = 0;
	hdac_name_get(hdac, name, buf, sizeof(buf));
	printf("	name=%s ==> addr=%s\n", name, buf);
	printf("\n\n");
}


void	test_rpc(hdac_t *hdac)
{
/*
	read_enter(0);
	test_rpc_blockchain(hdac);

	read_enter(0);
	test_rpc_control(hdac);

	read_enter(0);
	test_rpc_generating(hdac);

	read_enter(0);
	test_rpc_mining(hdac);

	read_enter(0);
	test_rpc_network(hdac);

	read_enter(0);
	test_rpc_block_tx(hdac);

	read_enter(0);
	test_rpc_util(hdac);

	read_enter(0);
	test_rpc_grant(hdac);

	read_enter(0);
	test_rpc_stream(hdac);

	read_enter(0);
	test_rpc_wallet_send(hdac);

	read_enter(0);
	test_rpc_wallet(hdac);
*/
	read_enter(0);
	test_rpc_raw_xch(hdac);
}


void	test_rpc_blockchain(hdac_t *hdac)
{
	json	list;
	char	hash[256] = {0}, buf[512] = {0};

	printf("=====RPC CMD: BLOCKCHAIN\n\n");

	list = hdac_getbestblockhash(hdac);
	printf("CMD=getbestblockhash()\n");
	list["result"].tostr(hash, sizeof(hash));
	printf("	HASH=%s\n", hash);
	printf("\n\n");
	read_enter(1);

	list = hdac_getblockbyhash(hdac, hash, 1);
	printf("CMD=getblock(%s, %d)\n", hash, 1);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getblock(hdac, 1, 1);
	printf("CMD=getblock(%d, %d)\n", 1, 1);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getblockchaininfo(hdac);
	printf("CMD=getblockchaininfo()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getblockcount(hdac);
	printf("CMD=getblockcount()\n");
	list.print();
	printf("\n");

	list["result"].tostr(buf, sizeof(buf));
	printf("    BLOCKCOUNT=%s\n\n", buf);
	printf("\n\n");
	read_enter(1);

	list = hdac_getblockhash(hdac, 1);
	printf("CMD=getblockhash(%d)\n", 1);
	list.print();
	printf("\n");

	list["result"].tostr(hash, sizeof(hash));
	printf("    HASH=%s\n\n", hash);
	printf("\n\n");
	read_enter(1);


	// 문제점: hdac은 block 정보가 나오지 않음. 
	// multichain은 잘 나옴 ==> 용량이 너무 커서 json_parse 10MB 버퍼로 처리 불가 
	list = hdac_getchaintips(hdac);
	printf("CMD=getchaintips()\n");
	printf("    내용이 너무 많은 경우, json_parse() 오류 발생함.\n\n");
	read_enter(1);

	list = hdac_getdifficulty(hdac);
	printf("CMD=getdifficulty()\n");
	list.print();
	printf("\n");

	list["result"].tostr(hash, sizeof(hash));
	printf("    DIFFICULTY=%s\n\n", hash);
	printf("\n\n");
	read_enter(1);

	list = hdac_getmempoolinfo(hdac);
	printf("CMD=getmempoolinfo()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getrawmempool(hdac, true);
	printf("CMD=getrawmempool(true)\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	strcpy(_txid, "d0d3d20a4c65244be68fa48e26e038062db50581b6095d635beb69eb4f638985");
	list = hdac_gettxout(hdac, _txid, 0);
	printf("CMD=gettxout(%s, %d)\n", _txid, 0);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_gettxoutsetinfo(hdac);
	printf("CMD=gettxoutsetinfo()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listassets(hdac, "HDACT", true);
	printf("CMD=listassets(%s, %d)\n", "HDACT", true);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listblocks(hdac, "10-20", true);
	printf("CMD=listblocks(%s, %d)\n", "1000-1002", true);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listpermissions(hdac);
	printf("CMD=listpermissions()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listpermissions(hdac, "receive", _toaddr, true);
	printf("CMD=listpermissions(%s, %s, %d)\n", "receive", _toaddr, true);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_liststreams(hdac);
	printf("CMD=liststreams()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_liststreams(hdac, "root", true);
	printf("CMD=liststreams(%s, %d)\n", "root", true);
	list.print();
	printf("\n\n");
	read_enter(1);

	/** Depredicated
	list = hdac_listupgrades(hdac);
	printf("CMD=listupgrades()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_verifychain(hdac);
	printf("CMD=verifychain()\n");
	list.print();
	**/
}


void	test_rpc_control(hdac_t *hdac)
{
	json	list;
	char	hash[256] = {0}, buf[512] = {0};

	printf("=====RPC CMD: CONTROL\n\n");

	list = hdac_clearmempool(hdac);		// incoming,mining should be paused
	printf("CMD=clearmempool()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getblockchainparams(hdac);
	printf("CMD=getblockchainparams()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getinfo(hdac);
	printf("CMD=getinfo()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getruntimeparams(hdac);
	printf("CMD=getruntimeparams()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_pause_task(hdac, "mining");		// pause incoming,mining
	printf("CMD=pause_task(%s)\n", "mining");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_resume_task(hdac, "mining");		// resume incoming,mining
	printf("CMD=resume_task(%s)\n", "mining");
	list.print();
	printf("\n\n");
	read_enter(1);

	strcpy(hash, "00000094e9541d02120db77d6f765f907998925837829720689d3dd3d0d04f5e");
	list = hdac_setlastblockbyhash(hdac, hash);
	printf("CMD=setlastblockbyhash(%s)\n", hash);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_setlastblock(hdac, 99999);
	printf("CMD=setlastblock(%d)\n", 99999);
	list.print();
	printf("\n\n");
	read_enter(1);

	// miningrequirespeers, mineemptyrounds, miningturnover, lockadminminerounds, maxshowndata, 
	// bantx, lockblock, autosubscribe, handshakelocal, hideknownopdrops
	list = hdac_setruntimeparam(hdac, "miningrequirespeers", "1");
	printf("CMD=setruntimeparam(%s, %s)\n", "miningrequirespeers", "1");
	list.print();
	printf("\n\n");
}


void	test_rpc_generating(hdac_t *hdac)
{
	json	list;
	char	outbuf[256] = {0};

	printf("=====RPC CMD: GENERATING\n\n");

	list = hdac_getgenerate(hdac);
	printf("CMD=getgenerate()\n");
	list.print();
	printf("\n");

	list["result"].tostr(outbuf, sizeof(outbuf));
	printf("    gen=%s\n", outbuf);
	printf("\n\n");
	read_enter(1);

	list = hdac_gethashespersec(hdac);
	printf("CMD=gethashspersec()\n");
	list.print();
	printf("\n\n");

	list["result"].tostr(outbuf, sizeof(outbuf));
	printf("    hashspersec=%s\n", outbuf);
	printf("\n\n");
}


void	test_rpc_mining(hdac_t *hdac)
{
	json	list;
	char	outbuf[256] = {0};

	printf("=====RPC CMD: MINING\n\n");

	// HDAC mining 되는 버전에서만 가능 
	const char	*jsonstr = "{\"capabilities\": [\"coinbasetxn\", \"workid\", \"coinbase/append\"]}";

	list = hdac_getblocktemplate(hdac, jsonstr);
	printf("CMD=getblocktemplate('%s')\n", jsonstr);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getblockwindowsize(hdac);
	printf("CMD=getblockwindowsize()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getmininginfo(hdac);
	printf("CMD=getmininginfo()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getnetworkhashps(hdac);
	printf("CMD=getnetworkhashps()\n");
	list["result"].tostr(outbuf, sizeof(outbuf));
	printf("    hashps=%s\n", outbuf);
	printf("\n\n");
	read_enter(1);

	strcpy(_txid, "d0d3d20a4c65244be68fa48e26e038062db50581b6095d635beb69eb4f638985");

	/*****
	list = hdac_prioritiestx(hdac, _txid, 0.0, 10000);
	printf("CMD=prioritiestx(%s)\n", _txid);
	list.print();
	printf("\n\n");
	*****/
}


void	test_rpc_network(hdac_t *hdac)
{
	json	list;

	printf("=====RPC CMD: NETWORK\n\n");

	list = hdac_addnode(hdac, "192.168.1.15:38823", "onetry");	// add/remove/onetry
	printf("CMD=addnode(%s, %s)\n", "192.168.1.15:38823", "onetry");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getaddednodeinfo(hdac, true, "192.168.1.15");
	printf("CMD=getaddednodeinfo(%d, %s)\n", true, "192.168.1.15");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getconnectioncount(hdac);
	printf("CMD=getconnectioncount()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getnettotals(hdac);
	printf("CMD=getnettotals()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getnetworkinfo(hdac);
	printf("CMD=getnetworkinfo()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getpeerinfo(hdac);
	printf("CMD=getpeerinfo()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	/*****
	list = hdac_ping(hdac);
	printf("CMD=ping()\n");
	list.print();
	printf("\n\n");
	*****/
}


void	test_rpc_block_tx(hdac_t *hdac)
{
	json	list;
	char	hash[256] = {0};

	list = hdac_getblock(hdac, 1, 1);
	printf("CMD=getblock(%d, %d)\n", 1, 1);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listblocks(hdac, "1000-1002", true);
	printf("CMD=listblocks(%s, %d)\n", "1000-1002", true);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getaddresstx(hdac, _fromaddr, _txid, true);
	printf("CMD=getaddresstx(%s %s %s)\n", _fromaddr, _txid, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getassettx(hdac, "HDACT", _txid, true);
	printf("CMD=getassettx(%s %s %s)\n", "HDACT", _txid, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_gettx(hdac, _txid, true);
	printf("CMD=getassettx(%s %s)\n", _txid, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getwallettx(hdac, _txid, true, true);
	printf("CMD=getwallettx(%s %s %s)\n", _txid, BOOL_STR(true), BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listsinceblock(hdac, hash, 1, true);
	printf("CMD=listsinceblock(%s %d %s)\n", hash, 1, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);


	list = hdac_listaddresstxs(hdac, _fromaddr, 2, 0, true);
	printf("CMD=listaddresstxs(%s %d %d %s)\n", _fromaddr, 2, 0, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listassettxs(hdac, "HDACT", true, 2);
	printf("CMD=listassettxs(%s %s %d)\n", "HDACT", BOOL_STR(true), 2);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listtxs(hdac, "", 2, 0, true);
	printf("CMD=listtxs(%s %d %d %s)\n", "", 2, 0, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listwallettxs(hdac, 2, 0, true, true);
	printf("CMD=listwallettxs(%d %d %s %s)\n", 2, 0, BOOL_STR(true), BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_resendwallettxs(hdac);
	printf("CMD=resendwallettxs()\n");
	list.print();
	printf("\n\n");
}


void	test_rpc_util(hdac_t *hdac)
{
	json	list;
	char	cmd[256] = {0}, outbuf[256] = {0};


	printf("=====RPC CMD: UTIL\n\n");

	list = hdac_createkeypairs(hdac);
	printf("CMD=createkeypairs()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	sprintf(cmd, "[\"%s\",\"%s\"]", _fromaddr, _toaddr);
	list = hdac_createmultisig(hdac, 2, cmd);
	printf("CMD=createmultisig(%d, %s)\n", 2, cmd);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_estimatefee(hdac, 4096);
	printf("CMD=estimatefee(%d)\n", 4096);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_estimatepriority(hdac, 1);
	printf("CMD=estimatepriority(%d)\n", 1);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_validateaddress(hdac, _fromaddr);
	printf("CMD=validateaddress(%s)\n", _fromaddr);
	list.print();
	printf("\n\n");
	read_enter(1);

	// mcc mc signmessage _fromaddr "This is message"
	list = hdac_signmessage(hdac, _fromaddr, "This is message");
	printf("CMD=signmessage(%s, %s)\n", _fromaddr, "This is message");
	list.print();
	printf("\n");

	list["result"].tostr(outbuf, sizeof(outbuf));
	printf("SIGN=%s\n", outbuf);
	printf("\n\n");
	read_enter(1);

	list = hdac_verifymessage(hdac, _fromaddr, outbuf, "This is message");
	printf("CMD=verifymessage(%s, %s, %s)\n", _fromaddr, outbuf, "This is message");
	list.print();
	printf("\n\n");
}


void	test_rpc_grant(hdac_t *hdac)
{
	json	list;
	string	hexstr;
	char	tmp[512] = {0}, tmp2[512] = {0}, *tp = NULL;


	printf("=====RPC CMD: GRANT\n\n");

	list = hdac_grant(hdac, _toaddr, "mine");
	printf("CMD=grant(%s %s)\n", _toaddr, "mine");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_grantfrom(hdac, _fromaddr, _toaddr, "mine");
	printf("CMD=grantfrom(%s %s %s)\n", _fromaddr, _toaddr, "mine");
	list.print();
	printf("\n\n");
	read_enter(1);

	strcpy(tmp, "grantwithdata로 저장된 hex 문자열입니다.");
	list = hdac_grantwithdata(hdac, _toaddr, "mine", tmp);
	printf("CMD=grantwithdata(%s %s \"%s\")\n", _toaddr, "mine", tmp);
	list.print();
	list["result"].tostr(_txid, sizeof(_txid));
	printf("\n\n");

	list = hdac_getrawtx(hdac, _txid, 4);
	list.print();
	list["data"][0].tostr(tmp, sizeof(tmp));
	printf("DATA=%s\n", (char *)hex2bin(tmp2, tmp, strlen(tmp)));
	printf("\n\n");
	read_enter(1);


	strcpy(tmp, "grantwithdatafrom로 저장된 hex string입니다.");
	list = hdac_grantwithdatafrom(hdac, _fromaddr, _toaddr, "mine", tmp);
	printf("CMD=grantwithdatafrom(%s %s %s \"%s\")\n", _fromaddr, _toaddr, "mine", tmp);
	list.print();
	list["data"].tostr(tmp, sizeof(tmp));
	printf("\n\n");

	list = hdac_getrawtx(hdac, _txid);
	list.print();
	list["data"][0].tostr(tmp, sizeof(tmp));
	printf("DATA=%s\n", (char *)hex2bin(tmp2, tmp, strlen(tmp)));
	printf("\n\n");
	read_enter(1);


	list = hdac_listpermissions(hdac, "mine", "*", true);
	printf("CMD=listpermissions(%s %s)\n", "*", BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_gettx(hdac, _txid);
	printf("CMD=gettx(%s)\n", _txid);
	list.print();
	hexstr = list["result"]["hex"].tostr();
//	printf("	hexstr=%s\n", hexstr.c_str());
	printf("\n\n");
	read_enter(1);

	list = hdac_decoderawtx(hdac, hexstr.c_str());
	printf("CMD=decoderawtx(%s)\n", hexstr.c_str());
	list.print();
	printf("\n");

	list["data"][0].tostr(tmp, sizeof(tmp));
//	printf("	data=%s\n", tmp);
	hex2bin(tmp2, &tmp[1], strlen(tmp) - 2);		// "" 제외 
	printf("\nData=%s ==> %s\n", tmp, tmp2);
	printf("\n\n");
	read_enter(1);


	list = hdac_revoke(hdac, _toaddr, "mine");
	printf("CMD=revoke(%s %s)\n", _toaddr, "mine");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_revokefrom(hdac, _fromaddr, _toaddr, "mine");
	printf("CMD=revokefrom(%s %s %s)\n", _fromaddr, _toaddr, "mine");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listpermissions(hdac);
	printf("CMD=listpermissions()\n");
	list.print();
	printf("\n\n");
}


void	test_rpc_stream(hdac_t *hdac)
{
	json	list;
	char	tmp[512] = {0}, tmp2[512] = {0};

	printf("=====RPC CMD: STREAM\n\n");

	list = hdac_create(hdac, "stream", "mystream", BOOL_STR(true), 
			"{\"Creator\":\"Hyundai-Pay\",\"CreationDate\":\"2018-02-07\"}");
	printf("CMD=create(%s %s %s %s)\n", "stream", "mystream", BOOL_STR(true), 
		"{\"Creator\":\"Hyundai-Pay\",\"CreationDate\":\"2018-02-07\"}");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_createfrom(hdac, _fromaddr, "stream", "mystream2", BOOL_STR(true), 
			"{\"Creator\":\"Hyundai-Pay\",\"CreationDate\":\"2018-02-07\"}");
	printf("CMD=createfrom(%s %s %s %s %s)\n", _fromaddr, "stream", "mystream2", BOOL_STR(true), 
		"{\"Creator\":\"Hyundai-Pay\",\"CreationDate\":\"2018-02-07\"}");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_unsubscribe(hdac, "mystream2");
	printf("CMD=unsubscribe(%s)\n", "mystream2");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_subscribe(hdac, "mystream2", true);
	printf("CMD=subscribe(%s %s)\n", "mystream2", BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	strcpy(tmp, "publish로 저장된 hex 문자열입니다.");
	list = hdac_publish(hdac, "mystream2", "key1", tmp);
	printf("CMD=publish(%s %s %s)\n", "mystream2", "key1", tmp);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_publishfrom(hdac, _fromaddr, "mystream2", "key1", tmp);
	printf("CMD=publishfrom(%s %s  %s %s)\n", _fromaddr, "mystream2", "key1", tmp);
	list.print();
	printf("\n");

	list["data"].tostr(_txid, sizeof(_txid));
	list["result"].tostr(_txid, sizeof(_txid));
	printf("\n\n");
	read_enter(1);

	list = hdac_getstreamitem(hdac, "mystream2", (const char *) _txid, true);
	printf("CMD=getstreamitem(%s %s %s)\n", "mystream2", _txid, BOOL_STR(true));
	list.print();
	printf("\n");

	list["data"].tostr(tmp, sizeof(tmp));
	printf("\nData=%s\n", tmp);
	printf("\n\n");
	read_enter(1);

	list = hdac_liststreamblockitems(hdac, "mystream2", "100", true);
	printf("CMD=liststreamblockitems(%s %s %s)\n", "mystream2", "100", BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_liststreamitems(hdac, "mystream2", true);
	printf("CMD=liststreamitems(%s %s)\n", "mystream2", BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_liststreamkeyitems(hdac, "mystream2", "key1", true);
	printf("CMD=liststreamkeyitems(%s %s %s)\n", "mystream2", "key1", BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_liststreamkeys(hdac, "mystream2", "*", true);
	printf("CMD=liststreamkeys(%s %s %s)\n", "mystream2", "*", BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_liststreampublisheritems(hdac, "mystream2", _fromaddr, true);
	printf("CMD=liststreampublisheritems(%s %s %s)\n", "mystream2", _fromaddr, BOOL_STR(true));
	list.print();
	printf("\n\n");
}


void	test_rpc_wallet_send(hdac_t *hdac)
{
	json	list;
	char	tmp[256] = {0};

	printf("=====RPC CMD: SEND\n\n");

	list = hdac_send(hdac, _toaddr, 10.0, "10.0 물품 대금 지급", "홍길동이 물품 대금 10.0 송금");
	printf("CMD=send(%s %lf \"%s\" \"%s\")\n", _toaddr, 10.0, 
		"10.0 물품 대금 지급", "홍길동이 물품 대금 10.0 송금");
	list.print();
	list["result"].tostr(_txid, sizeof(_txid));
	printf("txid=%s\n", _txid);
	printf("\n\n");
	read_enter(1);

	char	comment[256] = {0}, comment_to[256] = {0};

	list = hdac_gettx(hdac, _txid);
	list["result"]["comment"].tostr(comment, sizeof(comment));
	list["result"]["to"].tostr(comment_to, sizeof(comment_to));
	printf("CMD=gettx %s\n\nComment=%s\nCommentTo=%s\n", 
		_txid,
		(char *)hex2bin(tmp, comment, strlen(comment)),
		(char *)hex2bin(tmp, comment_to, strlen(comment_to)));
	printf("\n\n");
	read_enter(1);

	list = hdac_send_asset(hdac, _toaddr, "{\"HDACT\":10000.0}", 
			"HDACT 10000.0 물품 대금 지급", "홍길동이 물품 대금 HDACT 10000.0 송금");
	printf("CMD=send_asset(%s %s \"%s\" \"%s\")\n", _toaddr, "{\"HDACT\":10000.0}", 
		"HDACT 10000.0 물품 대금 지급", "홍길동이 물품 대금 HDACT 10000.0 송금");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_sendasset(hdac, _toaddr, "HDACT", 10000.0, 0.0, 
			"HDACT 10000.0 물품 대금 지급", "홍길동이 물품 대금 HDACT 10000.0 송금");
	printf("CMD=sendasset(%s %s %lf %lf \"%s\" \"%s\")\n", _toaddr, "HDACT", 10000.0, 0.0, 
		"HDACT 10000.0 물품 대금 지급", "홍길동이 물품 대금 HDACT 10000.0 송금");
	list.print();
	list["result"].tostr(_assettxid, sizeof(_assettxid));
	printf("\n\n");
	read_enter(1);

	list = hdac_sendassetfrom(hdac, _fromaddr, _toaddr, "HDACT", 10000.0, 0.0, 
			"HDACT 10000.0 물품 대금 지급", "홍길동이 물품 대금 HDACT 10000.0 송금");
	printf("CMD=sendassetfrom(%s %s %s %lf %lf \"%s\" \"%s\")\n", _fromaddr, _toaddr, "HDACT", 10000.0, 0.0, 
		"HDACT 10000.0 물품 대금 지급", "홍길동이 물품 대금 HDACT 10000.0 송금");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_sendfrom(hdac, _fromaddr, _toaddr, 10.0, 
			"10.0 물품 대금 지급", "홍길동이 물품 대금 10.0 송금");
	printf("CMD=sendfrom(%s %s %lf \"%s\" \"%s\")\n", _fromaddr, _toaddr, 10.0, 
		"10.0 물품 대금 지급", "홍길동이 물품 대금 10.0 송금");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_sendfrom_asset(hdac, _fromaddr, _toaddr, "{\"HDACT\":10000.0}", 
			"HDACT 10000.0 물품 대금 지급", "홍길동이 물품 대금 HDACT 10000.0 송금");
	printf("CMD=sendfrom_asset(%s %s %s \"%s\" \"%s\")\n", _fromaddr, _toaddr, "{\"HDACT\":10000.0}", 
		"HDACT 10000.0 물품 대금 지급", "홍길동이 물품 대금 HDACT 10000.0 송금");
	list.print();
	printf("\n\n");
	read_enter(1);

#ifndef SCALABLE_WALLET
	list = hdac_sendfromaccount(hdac, "", _toaddr, 10.0, 1, 
			"10.0 물품 대금 지급", "홍길동이 물품 대금 10.0 송금");
	printf("CMD=sendfromaccount(%s %s %lf %d \"%s\" \"%s\")\n", _fromaddr, _toaddr, 10.0, 1,
		"10.0 물품 대금 지급", "홍길동이 물품 대금 10.0 송금");
	list.print();
	printf("\n\n");
	read_enter(1);

	sprintf(tmp, "{\"%s\":10.0,\"%s\":20.0}", _toaddr, _toaddr);
	list = hdac_sendmany(hdac, "", tmp, 1, "각각에 10.0 20.0 물품 대금 지급");
	printf("CMD=sendmany(%s %s %d \"%s\")\n", "", tmp, 1, "각각에 10.0 20.0 물품 대금 지급");
	list.print();
	printf("\n\n");
	read_enter(1);
#endif

	list = hdac_sendwithdata(hdac, _toaddr, 10.0, "10.0 물품 대금 지급");
	printf("CMD=sendwithdata(%s %lf \"%s\")\n", _toaddr, 10.0, "10.0 물품 대금 지급");
	list.print();
	list["result"].tostr(_metatxid, sizeof(_metatxid));
	printf("META TXID=%s\n", _metatxid);
	printf("\n\n");
	read_enter(1);

	list = hdac_sendwithdata_asset(hdac, _toaddr, "{\"HDACT\":10000.0}", "10000.0 물품 대금 지급");
	printf("CMD=sendwithdata_asset(%s %s \"%s\")\n", _toaddr, "{\"HDACT\":10000.0}", "10000.0 물품 대금 지급");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_sendwithdatafrom(hdac, _fromaddr, _toaddr, 10.0, "10.0 물품 대금 지급");
	printf("CMD=sendwithdatafrom(%s %s %lf \"%s\")\n", _fromaddr, _toaddr, 10.0, "10.0 물품 대금 지급");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_sendwithdatafrom_asset(hdac, _fromaddr, _toaddr, 
			"{\"HDACT\":10000.0}", "10000.0 물품 대금 지급");
	printf("CMD=sendwithdatafrom_asset(%s %s %s \"%s\")\n", _fromaddr, _toaddr, 
		"{\"HDACT\":10000.0}", "10000.0 물품 대금 지급");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getaddressbalances(hdac, _toaddr);
	printf("CMD=getaddressbalances(%s)\n\n", _toaddr);
	list.print();
	printf("\n\n");
}


void	test_rpc_wallet(hdac_t *hdac)
{
	json	list;
	char	tmp[512] = {0}, privkey[256] = {0}, txid[256] = {0};

	printf("=====RPC CMD: WALLET\n\n");

	if (_txid[0] == 0)
	{
		list = hdac_send(hdac, _toaddr, 10.0, "10.0 물품 대금 지급", "홍길동이 물품 대금 10.0 송금");
		printf("CMD=send(%s %lf \"%s\" \"%s\")\n", _toaddr, 10.0, 
			"10.0 물품 대금 지급", "홍길동이 물품 대금 10.0 송금");
		list.print();
		list["result"].tostr(_txid, sizeof(_txid));
		printf("\n\n");
		read_enter(1);
	}

	sprintf(tmp, "[\"%s\",\"%s\"]", _fromaddr, _toaddr);
	list = hdac_addmultisigaddress(hdac, 2, tmp);
	printf("CMD=addmultisigaddress(%d, %s)\n", 2, tmp);
	list.print();
	printf("\n\n");
	read_enter(1);

	/*****
	list = hdac_approvefrom(hdac, _toaddr, "upgrade1", true);
	printf("CMD=approvefrom(%s, %s, %s)\n", _toaddr, "upgrade1", BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);
	*****/

	sprintf(tmp, "/tmp");			// wallet.dat 저장됨 
	list = hdac_backupwallet(hdac, tmp);
	printf("CMD=backupwallet(%s)\n", tmp);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_combineunspent(hdac, _fromaddr);
	printf("CMD=combineunspent(%s)\n", _fromaddr);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_dumpprivkey(hdac, _toaddr);
	printf("CMD=dumpprivkey(%s)\n", _toaddr);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_dumpwallet(hdac, "/tmp/wallet.dat");
	printf("CMD=dumpwallet(%s)\n", "/tmp/wallet.dat");
	list.print();
	list["result"].tostr(privkey, sizeof(privkey));
	printf("\n\n");

	int retcode = system("ls -l /tmp/wallet.*");
	printf("\n\n");
	read_enter(1);

#if RPC_SECURE > 0
	list = hdac_encryptwallet(hdac, "PASSWORD");
	printf("CMD=encryptwallet(%s)\n", "PASSWORD");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_walletpassphrase(hdac, "PASSWORD", 3600);
	printf("CMD=walletpassphrase(%s %d sec)\n", "PASSWORD", 3600);
	list.print();
	printf("\n\n");
	read_enter(1);
#endif

#ifndef SCALABLE_WALLET
	list = hdac_getaccount(hdac, _fromaddr);
	printf("CMD=getaccount(%s)\n", _fromaddr);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getaccountaddress(hdac, "account");
	printf("CMD=getaccountaddress(%s)\n", "account");
	list.print();
	printf("\n\n");
	read_enter(1);
#endif

	list = hdac_getaddressbalances(hdac, _fromaddr, 1);
	printf("CMD=getaddressbalances(%s %d)\n", _fromaddr, 1);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getaddresses(hdac, true);
	printf("CMD=getaddresses(%s)\n", BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

#ifndef SCALABLE_WALLET
	list = hdac_getaddressesbyaccount(hdac, "account");
	printf("CMD=getaddressesbyaccount(%s)\n", "account");
	list.print();
	printf("\n\n");
	read_enter(1);
#endif

	list = hdac_getaddresstx(hdac, _toaddr, _txid, true);
	printf("CMD=getaddresstx(%s %s %s)\n", _toaddr, _txid, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getassetbalances(hdac, "", 1, true);
	printf("CMD=getassetbalances(%s %d %s)\n", "", 1, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_subscribe(hdac, "HDACT");
	printf("CMD=subscribe(%s)\n", "HDACT");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getassettx(hdac, "HDACT", _assettxid, true);
	printf("CMD=getassettx(%s %s %s)\n", "HDACT", _assettxid, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getbalance(hdac, "", 1, true);
	printf("CMD=getbalance(%s %d %s)\n", "", 1, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getmultibalances(hdac, _fromaddr, "*", 1, true, true);
	printf("CMD=getmultibalances(%s %s %d %s %s)\n", _fromaddr, "*", 1, BOOL_STR(true), BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getnewaddress(hdac);
	printf("CMD=getnewaddress()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

#ifndef SCALABLE_WALLET
	list = hdac_getreceivedbyaccount(hdac, "account", 1);
	printf("CMD=getreceivedbyaccount(%s %d)\n", "account", 1);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getreceivedbyaddress(hdac, _fromaddr, 1);
	printf("CMD=getreceivedbyaddress(%s %d)\n", _fromaddr, 1);
	list.print();
	printf("\n\n");
	read_enter(1);
#endif

	list = hdac_gettotalbalances(hdac, 1, true);
	printf("CMD=gettotalbalances(%d %s)\n", 1, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_gettx(hdac, _txid, true);
	printf("CMD=gettx(%s %s)\n", _txid, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_send(hdac, _toaddr, 10.0, "10.0 물품 대금 지급", "홍길동이 물품 대금 10.0 송금");
	printf("CMD=send(%s %lf \"%s\" \"%s\")\n", _toaddr, 10.0, 
		"10.0 물품 대금 지급", "홍길동이 물품 대금 10.0 송금");
	list.print();
	printf("\n\n");

	list["result"].tostr(txid, sizeof(txid));
	printf("TXID=%s\n", txid);
	printf("\n\n");

	list = hdac_gettxoutdata(hdac, _metatxid, 1);
	printf("CMD=gettxoutdata(%s %d)\n", _metatxid, 1);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getunconfirmedbalance(hdac);
	printf("CMD=getunconfirmedbalance()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_getwalletinfo(hdac);
	printf("CMD=getwalletinfo()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_importaddress(hdac, _toaddr, "LABEL", false);
	printf("CMD=importaddress(%s %s %s)\n", _toaddr, "LABEL", BOOL_STR(false));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_importprivkey(hdac, "LVBoSoZtPB9mM59BkT2H57ogDBrvS5fGZVyDYWG4EpksaZAizCXc");
	printf("CMD=importprivkey(%s)\n", "LVBoSoZtPB9mM59BkT2H57ogDBrvS5fGZVyDYWG4EpksaZAizCXc");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_importwallet(hdac, "/tmp/wallet.dat");
	printf("CMD=importwallet(%s)\n", "/tmp/wallet.dat");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_issue(hdac, _fromaddr, "{\"name\":\"WON\",\"open\":true}", 100000000.0, 0.0001);
	printf("CMD=issue(%s %s %lf %lf)\n", _fromaddr, "{\"name\":\"WON\",\"open\":true}", 100000000.0, 0.0001);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_issuefrom(hdac, _fromaddr, _toaddr, "WON2", 100000000.0, 0.0001);
	printf("CMD=issuefrom(%s %s %s %lf %lf)\n", _fromaddr, _toaddr, "WON2", 100000000.0, 0.0001);
	list.print();
	printf("\n\n");
	read_enter(1);

	// "open": true required
	list = hdac_issuemore(hdac, _fromaddr, "WON", 10000.0, 0.0001);
	printf("CMD=issuemore(%s %s %lf %lf)\n", _fromaddr, "WON", 10000.0, 0.0001);
	list.print();
	printf("\n\n");
	read_enter(1);

	// "open": true required
	list = hdac_issuemorefrom(hdac, _fromaddr, _toaddr, "WON", 10000.0, 0.0001);
	printf("CMD=issuemorefrom(%s %s %s %lf %lf)\n", _fromaddr, _toaddr, "WON", 10000.0, 0.0001);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_keypoolrefill(hdac, 100);
	printf("CMD=keypoolrefill(%d)\n", 100);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listaccounts(hdac, 1, true);
	printf("CMD=listaccounts(%d %s)\n", 1, BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listaddresses(hdac, "*", true);
	printf("CMD=listaddresses(%s %s)\n", "*", BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listaddressgroupings(hdac);
	printf("CMD=listaddressgroupings()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

#ifndef SCALABLE_WALLET
	list = hdac_listreceivedbyaccount(hdac, 1, true, true);
	printf("CMD=listreceivedbyaccount(%d %s %s)\n", 1, BOOL_STR(true), BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listreceivedbyaddress(hdac, 1, true, true);
	printf("CMD=listreceivedbyaddress(%d %s %s)\n", 1, BOOL_STR(true), BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);
#endif

#ifndef SCALABLE_WALLET
	list = hdac_move(hdac, "fromaccount", "toaccount", 1.0, 1, "move 명령 처리");
	printf("CMD=move(%s %s %lf %d %s)\n", "fromaccount", "toaccount", 1.0, 1, "move 명령 처리");
	list.print();
	printf("\n\n");
	read_enter(1);
#endif

	list = hdac_listunspent(hdac, 1);
	printf("CMD=listunspent(%d)\n", 1);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_preparelockunspent(hdac, "{\"WON\":100}");
	printf("CMD=preparelockunspent(%s)\n", "{\"WON\":100}");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_preparelockunspentfrom(hdac, _fromaddr, "{\"WON\":100}");
	printf("CMD=preparelockunspentfrom(%s %s)\n", _fromaddr, "{\"WON\":100}");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_listlockunspent(hdac);
	printf("CMD=listlockunspent()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

#ifndef SCALABLE_WALLET
	list = hdac_setaccount(hdac, _fromaddr, "myacc");
	printf("CMD=setaccount(%s %s)\n", _fromaddr, "myacc");
	list.print();
	printf("\n\n");
	read_enter(1);
#endif

	list = hdac_settxfee(hdac, 0.0001);
	printf("CMD=settxfee(%lf)\n", 0.0001);
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_signmessage(hdac, _fromaddr, "Sign message test.");
	printf("CMD=signmessage(%s \"%s\")\n", _fromaddr, "Sign message test.");
	list.print();
	printf("\n\n");
}


// https://www.multichain.com/developers/atomic-exchange-txs/
// 여기서 생성해야 함: HDACT 10500 ==> USD 100으로 교환
void	test_rpc_raw_xch(hdac_t *hdac)
{
	json	list;

	printf("=====RPC CMD: EXCHANGE\n\n");

	list = hdac_getrawchangeaddress(hdac);
	printf("CMD=getrawchangeaddress()\n");
	list.print();
	printf("\n\n");
	read_enter(1);

//	char	*hdac_createrawsendfrom(hdac_t *hdac, const char *from_addr, const char *json_data, const char *data = "", const char *action = "");
//	char	*hdac_disablerawtx(hdac_t *hdac, const char *tx_hex);

	list = hdac_createrawexchange(hdac, "cc401cf6ca95662329c2bbc83d40c077906ed5c1f7cd00de1c2a255ec757393e", 1, "{\"HDACT\":100}");
	printf("CMD=createrawexchange(%s, %d %s)\n", 
		"cc401cf6ca95662329c2bbc83d40c077906ed5c1f7cd00de1c2a255ec757393e", 1, "{\"HDACT\":100}");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_appendrawexchange(hdac, "0100000001e25330a648d15257c92d8fc9adb34b95c4ac14e594606d3366fb30ecd902b8aa010000006a47304402207e15afff52316b26592c70b8d48f467aea793e8b32bcf258b8fa87865e8bf9640220737c3c735caf8ec6e03dc78c0a0f11734dc00e010e084d16460bf3667645fcf283210339157df9a712fc55ab4eca75e053a8b860a1b5f871a51904725157af8d3ddf1fffffffff0100000000000000003776a914ae8872240bd75ca6049c1d4da3a88ee85e1fd65788ac1c73706b71e2a0bec1db1251f28e47f272c16802fe00e40b54020000007500000000", "b5f28968a9fba57209dd33c79ff47e4f2485660c432c82c605ad2428a5c25c9b", 1, "{\"HDACT\":10500}");
	printf("CMD=appendrawexchange(%s, %s, %d, %s)\n", "0100000001e25330a648d15257c92d8fc9adb34b95c4ac14e594606d3366fb30ecd902b8aa010000006a47304402207e15afff52316b26592c70b8d48f467aea793e8b32bcf258b8fa87865e8bf9640220737c3c735caf8ec6e03dc78c0a0f11734dc00e010e084d16460bf3667645fcf283210339157df9a712fc55ab4eca75e053a8b860a1b5f871a51904725157af8d3ddf1fffffffff0100000000000000003776a914ae8872240bd75ca6049c1d4da3a88ee85e1fd65788ac1c73706b71e2a0bec1db1251f28e47f272c16802fe00e40b54020000007500000000", "b5f28968a9fba57209dd33c79ff47e4f2485660c432c82c605ad2428a5c25c9b", 1, "{\"HDACT\":10500}");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_decoderawexchange(hdac, "0100000001e25330a648d15257c92d8fc9adb34b95c4ac14e594606d3366fb30ecd902b8aa010000006a47304402207e15afff52316b26592c70b8d48f467aea793e8b32bcf258b8fa87865e8bf9640220737c3c735caf8ec6e03dc78c0a0f11734dc00e010e084d16460bf3667645fcf283210339157df9a712fc55ab4eca75e053a8b860a1b5f871a51904725157af8d3ddf1fffffffff0100000000000000003776a914ae8872240bd75ca6049c1d4da3a88ee85e1fd65788ac1c73706b71e2a0bec1db1251f28e47f272c16802fe00e40b54020000007500000000", true);
	printf("CMD=decoderawexchange(%s, %s)\n",
		"0100000001e25330a648d15257c92d8fc9adb34b95c4ac14e594606d3366fb30ecd902b8aa010000006a47304402207e15afff52316b26592c70b8d48f467aea793e8b32bcf258b8fa87865e8bf9640220737c3c735caf8ec6e03dc78c0a0f11734dc00e010e084d16460bf3667645fcf283210339157df9a712fc55ab4eca75e053a8b860a1b5f871a51904725157af8d3ddf1fffffffff0100000000000000003776a914ae8872240bd75ca6049c1d4da3a88ee85e1fd65788ac1c73706b71e2a0bec1db1251f28e47f272c16802fe00e40b54020000007500000000", BOOL_STR(true));
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_createrawexchange(hdac, "cc401cf6ca95662329c2bbc83d40c077906ed5c1f7cd00de1c2a255ec757393e", 1, "{\"HDACT\":100}");

	printf("CMD=createrawexchange(%s, %d %s)\n", 
		"cc401cf6ca95662329c2bbc83d40c077906ed5c1f7cd00de1c2a255ec757393e", 1, "{\"HDACT\":100}");
	list.print();
	printf("\n\n");
	read_enter(1);

	list = hdac_completerawexchange(hdac, "0100000001e25330a648d15257c92d8fc9adb34b95c4ac14e594606d3366fb30ecd902b8aa010000006a47304402207e15afff52316b26592c70b8d48f467aea793e8b32bcf258b8fa87865e8bf9640220737c3c735caf8ec6e03dc78c0a0f11734dc00e010e084d16460bf3667645fcf283210339157df9a712fc55ab4eca75e053a8b860a1b5f871a51904725157af8d3ddf1fffffffff0100000000000000003776a914ae8872240bd75ca6049c1d4da3a88ee85e1fd65788ac1c73706b71e2a0bec1db1251f28e47f272c16802fe00e40b54020000007500000000", "b5f28968a9fba57209dd33c79ff47e4f2485660c432c82c605ad2428a5c25c9b", 1.0, "{\"HDACT\":10500}", "010203040506070809");
	printf("CMD=completerawexchange(%s, %s %lf, %s, %s)\n",
		"0100000001e25330a648d15257c92d8fc9adb34b95c4ac14e594606d3366fb30ecd902b8aa010000006a47304402207e15afff52316b26592c70b8d48f467aea793e8b32bcf258b8fa87865e8bf9640220737c3c735caf8ec6e03dc78c0a0f11734dc00e010e084d16460bf3667645fcf283210339157df9a712fc55ab4eca75e053a8b860a1b5f871a51904725157af8d3ddf1fffffffff0100000000000000003776a914ae8872240bd75ca6049c1d4da3a88ee85e1fd65788ac1c73706b71e2a0bec1db1251f28e47f272c16802fe00e40b54020000007500000000", "b5f28968a9fba57209dd33c79ff47e4f2485660c432c82c605ad2428a5c25c9b", 1.0, "{\"HDACT\":10500}", "010203040506070809");
	list.print();
	printf("\n\n");
}


void	test_cert_doc(hdac_t *hdac)
{
	hdac_cert_t	certi = {0};
	hdac_cert_find_t findi = {0};
	struct stat st;

	printf("\n================================================================================\n");
	strcpy(certi.groupname, "Confidential");
	strcpy(certi.keyname, "conf-100-1001");
	certi.ispublic = false;

	certi.comment = (char *) "Confidential document\n\
\n\
date: 2018-02-08\n\
creator: Hyundai-Pay\n\
\n\
Title: Hdac main-net open\n";

	if (stat("test.c", &st) < 0)
	{
		printf("Cannot find %s\n", "test.c");
	}
	else
	{
		FILE	*fp = fopen("test.c", "rb");
		if (fp)
		{
			certi.contents = (char *) calloc(1, st.st_size + 1);
			int nread = fread(certi.contents, st.st_size, 1, fp);
			fclose(fp);
			certi.contents_len = nread;
			printf("Read %d bytes\n", nread);

			printf("\nRegister docuement information to Hdac blockchain\n\n");
			hdac_cert_doc_reg(hdac, &certi);

			printf("RESULT:\n");
			printf("    addr=%s\n", certi.addr);
			printf("    txid=%s\n", certi.txid);
			printf("    comment_hash=%s\n", certi.comment_hash);
			printf("    contents_hash=%s\n", certi.contents_hash);
		}

	}
	printf("\n\n");
	read_enter(1);

	printf("\n================================================================================\n");
	printf("Search document information in Hdac blockchain by txid\n\n");
	strcpy(findi.txid, certi.txid);
	hdac_cert_doc_find(hdac, &findi);

	printf("RESULT:\n");
	printf("    txid=%s\n", findi.txid);
	printf("    comment_hash=%s\n", findi.comment_hash);
	printf("    contents_hash=%s\n", findi.contents_hash);
	printf("    comment=%s\n", findi.comment);
	printf("\n\n");
	read_enter(1);

	printf("\n================================================================================\n");
	printf("\nSearch document information in Hdac blockchain by [groupname, keyname]\n\n");
	memset(&findi, 0, sizeof(findi));
	strcpy(findi.groupname, certi.groupname);
	strcpy(findi.keyname, certi.keyname);
	hdac_cert_doc_find(hdac, &findi);

	printf("RESULT:\n");
	printf("    txid=%s\n", findi.txid);
	printf("    comment_hash=%s\n", findi.comment_hash);
	printf("    contents_hash=%s\n", findi.contents_hash);
	printf("    comment=%s\n", findi.comment);
	printf("\n\n");

}


void	test_multisig(hdac_t *hdac)
{
	multisig_t msig = {0};
	json	js;

	// 2개의 주소로만 multisig
	js = hdac_getaddresses(hdac, true);
	js = js["result"];
	for (int nn = 0; nn < js.size() && msig.nsig < 2; nn++)
	{
		string pubkey = js[nn]["pubkey"].tostr();
		string addr = js[nn]["address"].tostr();
		string isscript = js[nn]["isscript"].tostr();

		if (pubkey == "" || isscript == "true")
			continue;

		if (msig.nsig < HDAC_MAX_MULTISIG && msig.addr[msig.nsig][0] == 0)
		{
			strcpy(msig.addr[msig.nsig], addr.c_str());
			strcpy(msig.pubkey[msig.nsig], pubkey.c_str());
			printf("Add multisig address[%d]: %s pubkey=%s\n", 
				msig.nsig, msig.addr[msig.nsig], msig.pubkey[msig.nsig]);
			msig.nsig++;
		}
	}
	strcpy(msig.asset_name, "USD");
	strcpy(msig.send_value, "111.0");
	printf("\nUse token: USD 111.0\n\n");

	printf("\n================================================================================\n");

	hdac_multisig(hdac, &msig);
	printf("SIGN = %s\n", msig.sign1);
	read_enter(1);

	printf("\n================================================================================\n");

	if (hdac_multisig2(hdac, &msig, 1) == E_OK)
		printf("Multisig sign2(1):\n\nRESULT = %s\n", msig.sign2);
	else
		printf("FAIL = %s\n", _hdac_error_msg);	// 오류..
	read_enter(1);

	if (hdac_multisig_send(hdac, &msig) == E_OK)
		printf("Multisig SUCCESS(1):\n\nRESULT = %s\n", msig.txid);
	else
		printf("FAIL = %s\n", _hdac_error_msg);	// 오류..
	read_enter(1);

}

