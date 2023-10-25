#include <jbinit.h>

void* read_file(char* path, size_t* size) {
    int fd = 0;
    LOG("opening %s\n", path);
    fd = open(path, O_RDONLY, 0);
    if (fd == -1) spin();
    size_t fsize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    void* fdata = mmap(NULL, (fsize & ~0x3fff) + 0x4000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (fdata == (void*)-1) {
        LOG("failed to map memory for %s\n", path);
        spin();
    }
    int didread = read(fd, fdata, fsize);
    LOG("read %d bytes\n", didread);
    *size = fsize;
    close(fd);
    return fdata;
}

int write_file(char* path, void* data, size_t size) {
    LOG("deploying %s\n", path);
    int fd = open(path, O_WRONLY | O_CREAT, 0755);
    LOG("%s write fd=%d\n", path, fd);
    if (fd == -1) {
        LOG("failed to open %s for writing\n", path);
        spin();
    }
    int didwrite = write(fd, data, size);
    LOG("didwrite=%d\n", didwrite);
    close(fd);
    return didwrite;
}
