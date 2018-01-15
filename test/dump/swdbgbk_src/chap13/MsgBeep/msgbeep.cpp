#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

static CONST LPCTSTR UserSoundsAlias[] = {
    ".Default",
    "AppGPFault",
	"CCSelect",
    "Close",
	"CriticalBatteryAlarm",
	"DeviceConnect",
	"DeviceDisconnect",
	"DeviceFail",
	"FaxBeep",
	"LowBatteryAlarm",
    "MailBeep",
    "Maximize",
    "MenuCommand",
    "MenuPopup",
    "Minimize",
    "Open",
	"PrintComplete",
    "RestoreDown",
    "RestoreUp",
	"ShowBand",
    "SnapShot",
    "SystemAsterisk",
    "SystemExclamation",
	"SystemExit",
    "SystemHand",
    "SystemQuestion",
	"SystemStart",
	"WindowsLogoff",
	"WindowsLogon",
	"WindowsUAC",
	"TestForNotExisted"
};
void PlayEventSound()
{
	for(int i=0;i<sizeof(UserSoundsAlias)/sizeof(LPCTSTR);i++)
	{
		printf("Any key to play [%s] sound...\n",UserSoundsAlias[i]);
		getchar();
		if(!PlaySound(UserSoundsAlias[i],
			NULL,
			SND_ALIAS|SND_SYNC))
		{
			printf("Failed to play the sound with error %d.\n", 
				GetLastError());
			getchar();
		}
	}
}
void main()
{
	printf("Any key beep...\n"); getchar();
	Beep( 750, 300 );

	printf("Any key to call MessageBeep...\n");getchar();
	MessageBeep(MB_ICONEXCLAMATION);
	PlayEventSound();
}