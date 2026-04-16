#include "nvme_passthru.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/nvme_ioctl.h>
#include <fcntl.h>
#include <cstring>
#include <cstdint>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include <inttypes.h>

using namespace std;

const unsigned int PAGE_SIZE = 4096;
const unsigned int MAX_BUFLEN = 16*1024*1024; /* Maximum transfer size (can be adjusted if needed) */
const unsigned int NSID = 1; /* NSID can be checked using 'sudo nvme list' */

int Embedded::Proj1::Open(const std::string &dev) {
    int err;
    err = open(dev.c_str(), O_RDONLY);
    if (err < 0)
        return -1;
    fd_ = err;

    struct stat nvme_stat;
    err = fstat(fd_, &nvme_stat);
    if (err < 0)
        return -1;
    if (!S_ISCHR(nvme_stat.st_mode) && !S_ISBLK(nvme_stat.st_mode))
        return -1;

    return 0;
}

int Embedded::Proj1::ImageWrite(const std::vector<uint8_t> &buf) {
    if (buf.empty()) return -EINVAL;
    if (buf.size() > MAX_BUFLEN) {
        cerr << "[ERROR] Image size exceeds the maximum transfer size limit." << endl;
        return -EINVAL;
    }

    size_t offset = 0;
    __u32 lba = 0;
    const size_t chunk_size = 128 * 1024;
    
    while(offset < buf.size()){
        size_t len = min(chunk_size, buf.size() - offset);
        
        size_t aligned_len = (len + 4095) & ~4095UL;
        
        void *tmp = nullptr;
        posix_memalign(&tmp, PAGE_SIZE, aligned_len);
        memset(tmp, 0, aligned_len);
        memcpy(tmp, buf.data() + offset, len);
        
        __u32 nlb = aligned_len / 4096;
        
        int ret = nvme_passthru(NVME_CMD_WRITE,NSID,(__u64)tmp, aligned_len, lba, nlb - 1);
        
        free(tmp);
        
        if(ret < 0 ) return ret;
        
        offset += len;
        lba += nlb;
    }
    
    /* ------------------------------------------------------------------
     * TODO: Implement this function.
     * 
     * Return 0 on success, or a negative error code on failure.
     * ------------------------------------------------------------------ */

    return 0;
}

int Embedded::Proj1::ImageRead(std::vector<uint8_t> &buf, size_t size) {
    if (size == 0) return -EINVAL;
    if (size > MAX_BUFLEN) {
        cerr << "[ERROR] Requested read size exceeds the maximum transfer size limit." << endl;
        return -EINVAL;
    }
    buf.resize(size);
    
    size_t offset = 0;
    __u32 lba = 0;
    const size_t chunk_size = 128*1024;
    
    while(offset < size){
        size_t len = min(chunk_size, size - offset);
        size_t aligned_len = (len + 4095) & ~4095UL;
        
        void *tmp = nullptr;
        posix_memalign(&tmp, PAGE_SIZE, aligned_len);
        memset(tmp, 0 , aligned_len);
        
        __u32 nlb = aligned_len / 4096;
        
        int ret = nvme_passthru(NVME_CMD_READ, NSID, (__u64)tmp, aligned_len, lba, nlb - 1);
        
        if(ret < 0) {free(tmp); return ret; }
        
        memcpy(buf.data() + offset, tmp, len);
        free(tmp);
        
        offset += len;
        lba += nlb;
    }
    
    
    /* ------------------------------------------------------------------
     * TODO: Implement this function.
     * 
     * Return 0 on success, or a negative error code on failure.
     * ------------------------------------------------------------------ */

    return 0; // placeholder
}

int Embedded::Proj1::Hello() {
    /* ------------------------------------------------------------------
     * TODO: Implement this function.
     * 
     * Return 0 on success, or a negative error code on failure.
     * ------------------------------------------------------------------ */

    return nvme_passthru(NVME_CMD_HELLO, NSID, 0, 0, 0, 0);
}

int Embedded::Proj1::nvme_passthru(__u8 opcode, __u32 nsid, __u64 addr, __u32 data_len, __u32 cdw10, __u32 cdw12)
{
    /* ------------------------------------------------------------------
     * TODO: Implement this function.
     * This function should serve as the low-level interface for issuing
     * passthru NVMe commands. Make sure to include appropriate arguments
     * (e.g., opcode, namespace ID, command dwords, buffer pointer, length,
     * and result field) so that higher-level methods (ImageWrite, ImageRead,
     * Hello) can be implemented using this helper.
     *
     * Hint: refer to the Linux nvme_ioctl.h header and the struct nvme_passthru_cmd definition.
     * - Link: https://elixir.bootlin.com/linux/v5.15/source/include/uapi/linux/nvme_ioctl.h
     * ------------------------------------------------------------------ */
    struct nvme_passthru_cmd cmd;
    memset(&cmd, 0 ,  sizeof(cmd));
    
    cmd.opcode = opcode;
    cmd.nsid = nsid;
    cmd.addr = addr;
    cmd.data_len = data_len;
    cmd.cdw10 = cdw10;
    cmd.cdw12 = cdw12;
    
    return ioctl(fd_, NVME_IOCTL_IO_CMD, &cmd);
    
}

