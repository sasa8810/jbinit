#include <jbloader.h>
#include <common.h>

void findvolume(void);

int remount(char *rootdev)
{
  int ret = 0;
  if (fakefs_is_in_use || access("/.mount_rw", F_OK) == 0) {
    char dev_rootdev[0x20];
    snprintf(dev_rootdev, 0x20, "/dev/%s", rootdev);
    struct apfs_mountarg arg = {
      .path = dev_rootdev,
      ._null = 0,
      .apfs_flags = 1,
      ._pad = 0,
    };
    ret = mount("apfs", "/", MNT_UPDATE, &arg);
    if (ret) {
      fprintf(stderr, "could not remount /: %d (%s)\n", errno, strerror(errno));
      return ret;
    }
  }
    findvolume();
    struct apfs_mountarg data_arg = {
      .path = dev_rootdev,
      ._null = 0,
      .apfs_flags = 1,
      ._pad = 0,
    };
    ret = mount("apfs", "/private/var", MNT_UPDATE, &data_arg);
    if (ret) {
      fprintf(stderr, "could not remount /private/var: %d (%s)\n", errno, strerror(errno));
      return ret;
    }
    
  struct statfs fs;
  if ((ret = statfs("/private/preboot", &fs))) {
    fprintf(stderr, "could not statfs /private/preboot: %d (%s)\n", errno, strerror(errno));
    return ret;
  }
    findvolume();
  struct apfs_mountarg preboot_arg = {
    .path = dev_rootdev,
    ._null = 0,
    .apfs_flags = 1,
    ._pad = 0,
  };
  ret = mount("apfs", "/private/preboot", MNT_UPDATE, &preboot_arg);
  if (ret) {
    fprintf(stderr, "could not remount /private/preboot: %d (%s)\n", errno, strerror(errno));
    return ret;
  }
  return ret;
}

void findvolume(void) {
    LOG("find rootfs at %s\n", dev_rootdev); // 文字列を配列にコピー
    
    int length = strlen(dev_rootdev);  // 文字列の長さを取得

    // 最後の文字が数字かどうかを確認
    if (length > 0 && isdigit(dev_rootdev[length - 1])) {
        // 最後の文字を整数に変換して1を足す
        int lastDigit = dev_rootdev[length - 1] - '0';
        lastDigit += 1;

        // 新しい数字を文字に変換して代入
        dev_rootdev[length - 1] = '0' + lastDigit;

        //printf("変更後の文字列: %s\n", str);
    }
}
