
@echo hdacd.exe hdac-cli.exe ���α׷��� C:\\hdac �� �־�� �մϴ�
@echo .
@echo �غ� �Ǿ����� Enter�� ��������
@echo .
@pause

@mkdir c:\hdac\newbc
@copy params-priv.dat c:\hdac\newbc\params.dat
@copy hdac.conf c:\hdac\newbc\hdac.conf

@echo .
@echo ���ü�� ���� Ȯ��: cmd â�� ��� C:\hdac\hdac-cli.exe newbc getinfo ����� �Է��� ������
@echo .

@c:
@cd \hdac

C:\hdac\hdacd.exe -datadir=C:\hdac newbc -daemon -gen=1 -miningrequirespeers=0

@pause
