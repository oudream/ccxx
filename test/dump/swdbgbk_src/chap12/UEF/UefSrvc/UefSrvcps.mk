
UefSrvcps.dll: dlldata.obj UefSrvc_p.obj UefSrvc_i.obj
	link /dll /out:UefSrvcps.dll /def:UefSrvcps.def /entry:DllMain dlldata.obj UefSrvc_p.obj UefSrvc_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del UefSrvcps.dll
	@del UefSrvcps.lib
	@del UefSrvcps.exp
	@del dlldata.obj
	@del UefSrvc_p.obj
	@del UefSrvc_i.obj
