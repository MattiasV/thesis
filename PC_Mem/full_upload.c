#include "MemoryAPI.h"

int main(int argc, char* argv[])
{
    uint8_t mem_id;
    uint32_t offset;
    char *cmdstring;

    if(argc < 4 || argc > 4)
    {
        printf("Provide following arguments to upload a file : \n");
        printf(" -- upload <memory_id> <offset> <file_name> \n");
        printf(" \t <memory_id> \t : to select memory on starterkit \n");
        printf(" \t <offset> \t : offset on the specified memory uint32_t \n");
        printf(" \t <file_name> \t : string of which file to upload from directory \n");
        return 0;
    }

    /**
    Note we zouden de input nog kunnen controleren, maar er zitten al switch cases op
    de starterkit dus eigenlijk niet nodig. Maar eventueel om niet te veel naar de
    starterkit te sturen zouden we hier nog een controle kunnen toevoegen.
    */

    mem_id = atoi(argv[1]);
    offset = atoi(argv[2]);
    cmdstring = get_string_cmd_upload(argc, argv);
    printf("cmdstring: %s \n", cmdstring);

    upload_memory(mem_id, offset, cmdstring);

    return 0;
}
