#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <imagehlp.h>
#include <fileapi.h>

#pragma comment(lib, "Imagehlp.lib")

int main(int argc, char* argv[]) {

	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	const char* fileName = argv[1];
	DWORD header = 0, checksum = 0;

	printf("Calculating the checksum ...\n");

	DWORD result = MapFileAndCheckSumA(
		fileName,
		&header,
		&checksum
	);

	if (CHECKSUM_SUCCESS != result) {
		if (result == CHECKSUM_MAP_FAILURE) {
			printf("Error:  Could not map the file. \n");
		}
		else if (result == CHECKSUM_MAPVIEW_FAILURE) {
			printf("Error:  Could not map a view of the file. \n");
		}
		else if (result == CHECKSUM_OPEN_FAILURE) {
			printf("Error:  Could not open the file. \n");
		}
		else if (result == CHECKSUM_UNICODE_FAILURE) {
			printf("Error:  Could not convert the file name to Unicode. \n");
		}
		return 1;
	}

	PLOADED_IMAGE loaded_image = ImageLoad(
		"vmware.exe",
		"C:\\Program Files (x86)\\VMware\\VMware Workstation\\"
	);

	if (NULL == loaded_image) {
		printf("Error: while loading the image: %d\n", GetLastError());
		return 1;
	}

	printf("Computed Checksum: %u, Actual Checksum in the PE file: %u\n", checksum, loaded_image->FileHeader->OptionalHeader.CheckSum);

	ImageUnload(loaded_image);
}