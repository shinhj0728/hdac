
@echo hdacd.exe hdac-cli.exe 프로그램이 C:\\hdac 에 있어야 합니다
@echo .
@echo 준비가 되었으면 Enter를 누르세요
@echo .
@pause

@c:
@cd \hdac

C:\hdac\hdacd.exe -datadir=C:\hdac testprvhdac@13.209.25.117:28823 -daemon

