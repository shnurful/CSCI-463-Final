#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include "hex.h"
#include "memory.h"

static void usage();

int main(int argc, char **argv)
{
	uint32_t memory_limit = 0x100;	// default memory size is 0x100

	int opt;
	while ((opt = getopt(argc, argv, "m:")) != -1)
	{
		switch(opt)
		{
		case 'm':
			{
				std::istringstream iss(optarg);
				iss >> std::hex >> memory_limit;
			}
			break;

		default:
			usage();
		}
	}

	if (optind >= argc)
		usage();	// missing filename

	memory mem(memory_limit);
	mem.dump();

	if (!mem.load_file(argv[optind]))
		usage();

	mem.dump();

	std::cout << mem.get_size() << std::endl;
	std::cout << hex::to_hex32(mem.get8(0)) << std::endl;
	std::cout << hex::to_hex32(mem.get16(0)) << std::endl;
	std::cout << hex::to_hex32(mem.get32(0)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get8(0)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get16(0)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get32(0)) << std::endl;
	std::cout << hex::to_hex8(mem.get8(0)) << std::endl;
	std::cout << hex::to_hex8(mem.get16(0)) << std::endl;
	std::cout << hex::to_hex8(mem.get32(0)) << std::endl;

	std::cout << hex::to_hex0x32(mem.get32(0x1000)) << std::endl;

	mem.set8(0x10, 0x12);
	mem.set16(0x14, 0x1234);
	mem.set32(0x18, 0x87654321);


	std::cout << hex::to_hex0x32(mem.get8_sx(0x0f)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get8_sx(0x7f)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get8_sx(0x80)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get8_sx(0xe3)) << std::endl;

	std::cout << hex::to_hex0x32(mem.get16_sx(0xe0)) << std::endl;
	std::cout << hex::to_hex0x32(mem.get32_sx(0xe0)) << std::endl;

	mem.dump();

	return 0;
}

static void usage ()
 {
    std::cerr << " Usage : rv32i [ - m hex - mem - size ] infile " << std::endl ;
    std::cerr << " -m specify memory size ( default = 0 x100 )" << std::endl ;
    exit (1);
 }
