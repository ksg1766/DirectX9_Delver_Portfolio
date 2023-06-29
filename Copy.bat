// 명령어		옵션		원본파일 위치					사본파일 저장 위치
	
xcopy		/y		.\Engine\Header\*.*						.\Reference\Header\		

xcopy		/y		.\Engine\System\Bin\System.dll			.\Client\Bin\
xcopy		/y		.\Engine\System\Bin\System.lib			.\Reference\Library\

xcopy		/y		.\Engine\Utility\Bin\Utility.dll			.\Client\Bin\
xcopy		/y		.\Engine\Utility\Bin\Utility.lib			.\Reference\Library\