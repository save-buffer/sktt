#include <algorithm>

#include <tt-metalium/bfloat16.hpp>
#include <tt-metalium/command_queue.hpp>
#include <tt-metalium/constants.hpp>
#include <tt-metalium/host_api.hpp>
#include <tt-metalium/tilize_utils.hpp>
#include <tt-metalium/util.hpp>

namespace ttm = tt::tt_metal;

int main()
{
    constexpr int device_id = 0;
    ttm::IDevice *device = ttm::CreateDevice(device_id);
    ttm::CommandQueue &cq = device->command_queue();
    ttm::Program program = ttm::CreateProgram();

    constexpr uint32_t tile_size = tt::constants::TILE_WIDTH * tt::constants::TILE_HEIGHT;
    constexpr uint32_t ntiles = 50;
    constexpr uint32_t buffer_size = tile_size * ntiles * sizeof(bfloat16);

    ttm::InterleavedBufferConfig l1_config =
    {
        .device = device,
        .size = buffer_size,
        .page_size = buffer_size,
        .buffer_type = ttm::BufferType::L1,
    };
    std::shared_ptr<ttm::Buffer> l1_buffer = ttm::CreateBuffer(l1_config);

    ttm::InterleavedBufferConfig dram_config =
    {
        .device = device,
        .size = buffer_size,
        .page_size = buffer_size,
        .buffer_type = ttm::BufferType::DRAM,
    };
    std::shared_ptr<ttm::Buffer> input_dram_buffer = ttm::CreateBuffer(dram_config);
    std::shared_ptr<ttm::Buffer> output_dram_buffer = ttm::CreateBuffer(dram_config);

    std::vector<bfloat16> input_host(ntiles * tile_size);
    std::iota(input_host.begin(), input_host.end(), 0.0f);
    EnqueueWriteBuffer(cq, input_dram_buffer, input_host, false);

    ttm::CoreCoord core = { 0, 0 };
    ttm::KernelHandle dram_copy_kernel = ttm::CreateKernel(
        program,
        "kernels/loopback.cpp",
        core,
        ttm::DataMovementConfig
        {
            .processor = ttm::DataMovementProcessor::RISCV_0,
            .noc = ttm::NOC::RISCV_0_default,
        });
    std::vector<uint32_t> runtime_args =
    {
        l1_buffer->address(),
        input_dram_buffer->address(),
        0,
        output_dram_buffer->address(),
        0,
        buffer_size,
    };

    ttm::SetRuntimeArgs(
        program,
        dram_copy_kernel,
        core,
        runtime_args);

    ttm::EnqueueProgram(cq, program, false);
    ttm::Finish(cq);

    std::vector<bfloat16> result;
    ttm::EnqueueReadBuffer(cq, output_dram_buffer, result, true);
    bool pass = true;
    for(int i = 0; i < input_host.size(); i++)
    {
        if(input_host[i] != result[i])
        {
            std::cout << "Mismatch at index " << i << '\n';
            pass = false;
        }
    }
    if(pass)
        tt::log_info(tt::LogTest, "PASSED!");
    else
        tt::log_info(tt::LogTest, "FAILED");
    ttm::CloseDevice(device);
    return 0;
}
