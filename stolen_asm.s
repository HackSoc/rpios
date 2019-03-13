.globl enable_paging
.globl set_page_directory_phys

enable_paging:
STMFD sp!, {R4-R11, LR}
mov r0, #0x1
// set the dacr for domain 0 to 1...
mcr p15, 0, r0, c3, c0, 0

// set mmu and extended page table bits (aka, enable armv6 features)
mrc p15, 0, r0, c1, c0, 0
orr r0, r0, #(1)
bic r0, r0, #(1<<29)
mcr p15, 0, r0, c1, c0, 0
LDMFD sp!, {R4-R11, PC}

// set ttbr0
set_page_directory_phys:
STMFD sp!, {R4-R11, LR}
mcr p15, 0, r0, c2, c0, 0
LDMFD sp!, {R4-R11, PC}