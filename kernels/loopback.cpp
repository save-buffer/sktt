#include <cstdint>

void kernel_main()
{
    uint32_t l1_addr = get_arg_val<uint32_t>(0);
    uint32_t src_addr = get_arg_val<uint32_t>(1);
    uint32_t src_bank = get_arg_val<uint32_t>(2);
    uint32_t dst_addr = get_arg_val<uint32_t>(3);
    uint32_t dst_bank = get_arg_val<uint32_t>(4);
    uint32_t size = get_arg_val<uint32_t>(5);

    uint64_t src_noc_addr = get_noc_addr_from_bank_id<true>(src_bank, src_addr);
    noc_async_read(src_noc_addr, l1_addr, size);
    noc_async_read_barrier();

    uint64_t dst_noc_addr = get_noc_addr_from_bank_id<true>(dst_bank, dst_addr);
    noc_async_write(l1_addr, dst_noc_addr, size);
    noc_async_write_barrier();
}
