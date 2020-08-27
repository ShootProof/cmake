#include <stdio.h>
#include <stdlib.h>
#include "DynamsoftBarcodeReader.h"
#include "BarcodeReaderConfig.h"
#include <iostream>
#include <fstream>

using namespace std;

char* read_file_text(const char* filename) {
	FILE *fp = fopen(filename, "r");
	size_t size;
	char *text = NULL;
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
	}
	else {
		cout << "Fail to open file" << endl;
		return NULL;
	}

	rewind(fp);
	text = (char *)calloc((size + 1), sizeof(char));
	if (text == NULL) {fputs ("Memory error",stderr); return NULL;}

	char c;
	char *tmp = text;
	do {
		c = fgetc (fp);
		*tmp = c;
		tmp++;
	} while (c != EOF);
	fclose (fp);
	return text;
}

unsigned char * read_file_binary(const char* filename, int* out_size) {
	FILE *fp = fopen(filename, "rb");
	size_t size;
	unsigned char *buffer = NULL;
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
	}
	else {
		cout << "Fail to open file" << endl;
		return NULL;
	}

	rewind(fp);
	buffer = ( unsigned char *)malloc(sizeof( unsigned char) * size);
	if (buffer == NULL) {fputs ("Memory error",stderr); return NULL;}

	size_t result = fread(buffer, 1, size, fp);
	*out_size = size;
	if (result != size) {fputs ("Reading error",stderr); return NULL;}
	fclose (fp);
	return buffer;
}

int barcode_decoding(const unsigned char* buffer, int size, int formats, char* license)
{
	// Initialize Dynamsoft Barcode Reader
	CBarcodeReader reader;
	reader.InitLicense(license);

	// Get and update settings
	char sError[512];
	PublicRuntimeSettings* runtimeSettings = new PublicRuntimeSettings();
	reader.GetRuntimeSettings(runtimeSettings);
	// Four is the defalt value here, we can experiment with timing and adjust as needed
	runtimeSettings->maxAlgorithmThreadCount = 4;
	runtimeSettings->barcodeFormatIds = formats;
	reader.UpdateRuntimeSettings(runtimeSettings, sError, 512);
	delete runtimeSettings;

	// Read barcodes from file stream
	int iRet = reader.DecodeFileInMemory(buffer, (int)size, "");

	// Output barcode result
	if (iRet != DBR_OK && iRet != DBRERR_MAXICODE_LICENSE_INVALID && iRet != DBRERR_AZTEC_LICENSE_INVALID && iRet != DBRERR_LICENSE_EXPIRED && iRet != DBRERR_QR_LICENSE_INVALID && iRet != DBRERR_GS1_COMPOSITE_LICENSE_INVALID &&
		iRet != DBRERR_1D_LICENSE_INVALID && iRet != DBRERR_PDF417_LICENSE_INVALID && iRet != DBRERR_DATAMATRIX_LICENSE_INVALID && iRet != DBRERR_GS1_DATABAR_LICENSE_INVALID && iRet != DBRERR_PATCHCODE_LICENSE_INVALID)
	{
		printf("Failed to read barcode: %s\n", CBarcodeReader::GetErrorString(iRet));
		return 1;
	}

	TextResultArray *paryResult = NULL;
	reader.GetAllTextResults(&paryResult);

	if (paryResult->resultsCount == 0)
	{
		printf("No barcode found.\n");
		CBarcodeReader::FreeTextResults(&paryResult);
		return 4;
	}

	// Since more than one barcode can be scanned, the results set is an array.
	for (int iIndex = 0; iIndex < paryResult->resultsCount; iIndex++)
	{
		// Print only the decoded text
		printf("%s", paryResult->results[iIndex]->barcodeText);
	}

	CBarcodeReader::FreeTextResults(&paryResult);
	return 0;
}

int main(int argc, const char* argv[])
{
	if (argc < 3) {
		printf("Usage: BarcodeReader [image-file] [license-file]\n");
		return 2;
	}

	char* license = NULL;

	// Read the license and exit with code 2 if it's invalid
	license = read_file_text(argv[2]);
	if (license == NULL) {
		printf("License is null\n");
		return 1;
	}

	int size = 0;
	unsigned char* buffer = NULL;
	buffer = read_file_binary(argv[1], &size);
	if (buffer == NULL) {
		printf("Image is null\n");
		return 1;
	}

	int exitCode = 0;
	exitCode = barcode_decoding(buffer, size, BF_QR_CODE, license);

	free(license);
	free(buffer);

	return exitCode;
}
