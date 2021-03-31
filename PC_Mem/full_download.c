#include "MemoryAPI.h"

int main(int argc, char* argv[])
{
    uint8_t mem_id;
    uint32_t offset, size;
    char *cmdstring;

    if(argc < 5 || argc > 5)
    {
        printf("Provide following arguments to download function : \n");
        printf(" -- download <memory_id> <offset> <size> <file_name> \n");
        printf(" \t <memory_id> \t : to select memory on starterkit \n");
        printf(" \t <offset> \t : offset on the specified memory uint32_t \n");
        printf(" \t <size> \t : how many bytes to download uint32_t \n");
        printf(" \t <file_name> \t : string where to store file in current directory \n");
        return 0;
    }

    mem_id = atoi(argv[1]);
    offset = atoi(argv[2]);
    size = atoi(argv[3]);
    cmdstring = get_string_cmd_download(argc, argv);
    printf("cmdstring: %s \n", cmdstring);

    download_memory(mem_id, offset,size, cmdstring);


}
