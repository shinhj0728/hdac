
@echo hdacd.exe hdac-cli.exe 프로그램이 C:\\hdac 에 있어야 합니다
@echo .
@echo 준비가 되었으면 Enter를 누르세요
@echo .
@pause

@mkdir c:\hdac\newbc
@copy params-priv.dat c:\hdac\newbc\params.dat
@copy hdac.conf c:\hdac\newbc\hdac.conf

@echo .
@echo 블록체인 가동 확인: cmd 창을 열어서 C:\hdac\hdac-cli.exe newbc getinfo 명령을 입력해 보세요
@echo .

@c:
@cd \hdac

C:\hdac\hdacd.exe -datadir=C:\hdac newbc -daemon -gen=1 -miningrequirespeers=0

@pause
