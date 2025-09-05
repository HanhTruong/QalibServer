
QalibServerps.dll: dlldata.obj QalibServer_p.obj QalibServer_i.obj
	link /dll /out:QalibServerps.dll /def:QalibServerps.def /entry:DllMain dlldata.obj QalibServer_p.obj QalibServer_i.obj \
		mtxih.lib mtx.lib mtxguid.lib \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
		ole32.lib advapi32.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		/MD \
		$<

clean:
	@del QalibServerps.dll
	@del QalibServerps.lib
	@del QalibServerps.exp
	@del dlldata.obj
	@del QalibServer_p.obj
	@del QalibServer_i.obj
