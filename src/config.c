#include "config.h"

#include "io.h"
#include "log.h"

#define DFT_Volume					Max_Volume
#define DFT_AutoPlay				AutoPlay_All
#define DFT_WaitTimePerChar			60
#define DFT_WaitTimeDialog			800
#define DFT_WaitTimeDialogWithVoice	500
#define DFT_SkipVoice				1
#define DFT_DisableDialogTextSE		1
#define DFT_DisableDialogSwitchSE	1
#define DFT_DisableOriginalVoice	1
#define DFT_ShowInfo				1
#define DFT_PPSSPP					0


#define STR_Volume					"Volume"
#define STR_AutoPlay				"AutoPlay"
#define STR_WaitTimePerChar			"WaitTimePerChar"
#define STR_WaitTimeDialog			"WaitTimeDialog"
#define STR_WaitTimeDialogWithVoice	"WaitTimeDialogWithVoice"
#define STR_SkipVoice				"SkipVoice"
#define STR_DisableDialogTextSE		"DisableDialogTextSE"
#define STR_DisableDialogSwitchSE	"DisableDialogSwitchSE"
#define STR_DisableOriginalVoice	"DisableOriginalVoice"
#define STR_ShowInfo				"ShowInfo"
#define STR_PPSSPP					"PPSSPP"

#define STR_EQUAL "="
#define STR_SPACE " "
#define STR_CRLF    "\r\n"
#define MAX_NAME_LEN 31

#define SET_DFT(CONFIG, NAME) CONFIG->NAME = DFT_##NAME

static void inline _LoadDefault(Config* config) {
	SET_DFT(config, Volume);
	SET_DFT(config, AutoPlay);
	SET_DFT(config, WaitTimePerChar);
	SET_DFT(config, WaitTimeDialog);
	SET_DFT(config, WaitTimeDialogWithVoice);
	SET_DFT(config, SkipVoice);
	SET_DFT(config, DisableDialogTextSE);
	SET_DFT(config, DisableDialogSwitchSE);
	SET_DFT(config, DisableOriginalVoice);
	SET_DFT(config, ShowInfo);
	SET_DFT(config, PPSSPP);
}

static inline int _StrCmp(const char* s1, const char *s2) {
	while(*s1 == *s2 && *s1) s1++, s2++;
	return (unsigned char)*s1 - (unsigned char)*s2;
}

#define SET_VALUE(CONFIG, NAME, BUFF_NAME, VALUE) \
	if(!_StrCmp(STR_##NAME, BUFF_NAME)) CONFIG->NAME = VALUE;

#define FIX_VALUE_MAX(CONFIG, NAME) \
		if(CONFIG->NAME < 0) CONFIG->NAME = 0; \
		else if(CONFIG->NAME > Max_##NAME) CONFIG->NAME = Max_##NAME;

#define FIX_VALUE_BOOL(CONFIG, NAME) \
		if(CONFIG->NAME) CONFIG->NAME = 1; \
		else CONFIG->NAME = 0;

bool LoadConfig(struct Config* config, const char* cfg_file) {
	_LoadDefault(config);

	IoHandle ioh = IoFOpen(cfg_file, IO_O_RDONLY);
	if(ioh == NULL) return false;

	char buff[MAX_NAME_LEN + 1];
	char buff_equal[4];
	const char endMarks[] = { ' ', '\t', '\r', '\n', '\0', '=' };
	unsigned value = 0;

	for(;;) {
		int len = IoFReadStr(ioh, buff, MAX_NAME_LEN, endMarks, sizeof(endMarks));
		if(len == 0) break;

		len = IoFReadStr(ioh, buff_equal, 1, endMarks, sizeof(endMarks) - 1);
		if(len == 0 || buff_equal[0] != '=') break;

		len = IoFReadUInt(ioh, &value);
		if(len == 0) break;

		SET_VALUE(config, Volume, buff, value);
		SET_VALUE(config, AutoPlay, buff, value);
		SET_VALUE(config, WaitTimePerChar, buff, value);
		SET_VALUE(config, WaitTimeDialog, buff, value);
		SET_VALUE(config, WaitTimeDialogWithVoice, buff, value);
		SET_VALUE(config, SkipVoice, buff, value);
		SET_VALUE(config, DisableDialogTextSE, buff, value);
		SET_VALUE(config, DisableDialogSwitchSE, buff, value);
		SET_VALUE(config, DisableOriginalVoice, buff, value);
		SET_VALUE(config, ShowInfo, buff, value);
		SET_VALUE(config, PPSSPP, buff, value);
	}

	IoFClose(ioh);

	FIX_VALUE_MAX(config, Volume);
	FIX_VALUE_MAX(config, AutoPlay);
	FIX_VALUE_MAX(config, WaitTimePerChar);
	FIX_VALUE_MAX(config, WaitTimeDialog);
	FIX_VALUE_MAX(config, WaitTimeDialogWithVoice);

	FIX_VALUE_BOOL(config, SkipVoice);
	FIX_VALUE_BOOL(config, DisableDialogTextSE);
	FIX_VALUE_BOOL(config, DisableDialogSwitchSE);
	FIX_VALUE_BOOL(config, DisableOriginalVoice);
	FIX_VALUE_BOOL(config, ShowInfo);
	FIX_VALUE_BOOL(config, PPSSPP);

	return true;
}


#define WRITE_CONFIG(IOH, NAME) \
	IoFWriteStr(ioh, STR_##NAME); \
	IoFWriteStr(ioh, STR_SPACE STR_EQUAL STR_SPACE);\
	IoFWriteUInt(ioh, config->NAME);\
	IoFWriteStr(ioh, STR_CRLF);\

bool SaveConfig(const struct Config* config, const char* cfg_file) {
	IoHandle ioh = IoFOpen(cfg_file, IO_O_CREAT | IO_O_WRONLY | IO_O_TRUNC);
	if(ioh == NULL) return false;

	WRITE_CONFIG(ioh, Volume);
	WRITE_CONFIG(ioh, AutoPlay);
	WRITE_CONFIG(ioh, WaitTimePerChar);
	WRITE_CONFIG(ioh, WaitTimeDialog);
	WRITE_CONFIG(ioh, WaitTimeDialogWithVoice);
	WRITE_CONFIG(ioh, SkipVoice);
	WRITE_CONFIG(ioh, DisableDialogTextSE);
	WRITE_CONFIG(ioh, DisableDialogSwitchSE);
	WRITE_CONFIG(ioh, DisableOriginalVoice);
	WRITE_CONFIG(ioh, ShowInfo);
	WRITE_CONFIG(ioh, PPSSPP);

	IoFClose(ioh);
	return true;
}

