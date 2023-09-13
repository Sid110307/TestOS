@echo -off
mode 80 25

cls
if exist .\EFI\BOOT\BOOT.EFI then
    .\EFI\BOOT\BOOT.EFI
    goto END
endif

if exist fs0:\EFI\BOOT\BOOT.EFI then
	fs0:
	echo Found bootloader on fs0:
	EFI\BOOT\BOOT.EFI
	goto END
endif

if exist fs1:\EFI\BOOT\BOOT.EFI then
	fs1:
	echo Found bootloader on fs1:
	EFI\BOOT\BOOT.EFI
	goto END
endif

if exist fs2:\EFI\BOOT\BOOT.EFI then
	fs2:
	echo Found bootloader on fs2:
	EFI\BOOT\BOOT.EFI
	goto END
endif

if exist fs3:\EFI\BOOT\BOOT.EFI then
	fs3:
	echo Found bootloader on fs3:
	EFI\BOOT\BOOT.EFI
	goto END
endif

if exist fs4:\EFI\BOOT\BOOT.EFI then
	fs4:
	echo Found bootloader on fs4:
	EFI\BOOT\BOOT.EFI
	goto END
endif

if exist fs5:\EFI\BOOT\BOOT.EFI then
	fs5:
	echo Found bootloader on fs5:
	EFI\BOOT\BOOT.EFI
	goto END
endif

if exist fs6:\EFI\BOOT\BOOT.EFI then
	fs6:
	echo Found bootloader on fs6:
	EFI\BOOT\BOOT.EFI
	goto END
endif

if exist fs7:\EFI\BOOT\BOOT.EFI then
	fs7:
	echo Found bootloader on fs7:
	EFI\BOOT\BOOT.EFI
	goto END
endif

echo "No bootloader found!"
:END
