#include <stdint.h>
void set_page_directory_phys(uint32_t phys);
void enable_paging();
uint32_t __attribute__((aligned(16384))) page_table[4096];

void go()
{
	uint32_t pa = 0;
	uint32_t va = 0;
	for(int i = 0; i < 1; i++) // identity map the first whatever
	{
		uint32_t section = pa | 2 | 1<<11;
		page_table[i] = section; // At 0
		pa += 1024*1024;
	}

	// peripherals
	page_table[1008] = 0x3f000000 | 2 | 1<<11;
	page_table[1009] = 0x3f100000 | 2 | 1<<11;
	page_table[1010] = 0x3f200000 | 2 | 1<<11;

	// map fb?
	pa = get_fb_ptr();
	for(int i = 0; i < 8; i++) // identity map the first whatever
	{
		uint32_t section = pa | 2 | 1<<11 | 1 << 3 | 1 << 2;
		page_table[pa >> 20] = section;
		pa += 1024*1024;
	}

	set_page_directory_phys((uint32_t)page_table);
	enable_paging();
}