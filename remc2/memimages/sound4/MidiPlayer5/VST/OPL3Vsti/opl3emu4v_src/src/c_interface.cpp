#include "OPLSynth.h"

#include "c_types.h"
#include "c_interface.h"


struct opl3emu_data {
	OPLSynth *synth;	
};


// C-visible implementation

extern "C" {


opl3emu_context opl3emu_create_context(void *instance_data) {
	opl3emu_data *data = new opl3emu_data;
	data->synth = new OPLSynth();	
	return data;
}

void opl3emu_free_context(opl3emu_context data) {
	if (data == NULL) return;
	delete data->synth;
	data->synth = NULL;
	delete data;
}

void opl3emu_init(opl3emu_context context, int samplerate) {
	if (context == NULL) return;
	context->synth->Init(samplerate);
}

void opl3emu_writemididata(opl3emu_context context, DWORD dwdata) {
	if (context == NULL) return;
	context->synth->WriteMidiData(dwdata);
}
	
void opl3emu_getsample(opl3emu_context context, short *samplem, int len) {
	if (context == NULL) return;
	context->synth->GetSample(samplem, len);
}

void opl3emu_boardreset(opl3emu_context context) {
	if (context == NULL) return;
	context->synth->BoardReset();
}

void opl3emu_playsysex(opl3emu_context context, Bit8u *bufpos, DWORD len) {
	if (context == NULL) return;
	context->synth->PlaySysex(bufpos, len);
}

void opl3emu_setpatch_sbi(opl3emu_context context, Bit8u bankNum, Bit8u patchNum, Bit8u *buf, DWORD len) {
	if (context == NULL) return;
	context->synth->SetPatch_Sbi(bankNum, patchNum, buf, len);
}

void opl3emu_setbank(opl3emu_context context, int banknum) {
	if (context == NULL) return;
	context->synth->SetBank(banknum);
}

int opl3emu_getactivevoicecount(opl3emu_context context) {
	if (context == NULL) return 0;
	return context->synth->GetActiveVoiceCount();
}

void opl3emu_setvolume(opl3emu_context context, WORD volume_atten) {
	if (context == NULL) return;
	context->synth->SetVolume(volume_atten);
}

BYTE opl3emu_getpatch(opl3emu_context context, BYTE channel) {
	if (context == NULL) return 0;
	return context->synth->GetPatch(channel);
}

void opl3emu_usenukedopl3(opl3emu_context context, bool value) {
	if (context == NULL) return;
	context->synth->UseNukedOPL3(value);

}


} // extern "C"
