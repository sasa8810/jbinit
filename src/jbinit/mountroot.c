#include <jbinit.h>
#include <common.h>

void findvolume(void);

void mountroot(char* rootdev, uint64_t rootlivefs, int rootopts) {
    struct stat statbuf;
    char buf[0x100];
    struct apfs_mountarg arg = {
        rootdev,
        0,
        rootlivefs, // 1 mount without snapshot, 0 mount snapshot
        0,
    };
retry_rootfs_mount:
    LOG("mounting rootfs %s\n", rootdev);
    int err = mount("apfs", "/", rootopts | MNT_RDONLY, &arg);
    if (!err) {
      LOG("mount rootfs OK\n");
    } else {
      LOG("mount rootfs %s FAILED with err=%d!\n", rootdev, err);
      sleep(1);
      goto retry_rootfs_mount;
      // spin();
    }
    if ((err = stat("/private/", &statbuf))) {
      LOG("stat %s FAILED with err=%d!\n", "/private/", err);
      sleep(1);
      goto retry_rootfs_mount;
    } else {
      LOG("stat %s OK\n", "/private/");
    }
//    findvolume(void);
//    int err = mount("apfs", "/private/var/", rootopts | MNT_UPDATE, &arg);
//    if (!err) {
//      LOG("mount data OK\n");
//    } else {
//      LOG("mount data %s FAILED with err=%d!\n", rootdev, err);
//      sleep(1);
//      goto retry_rootfs_mount;
//      // spin();
//    }
//    findvolume(void);
//    int err = mount("apfs", "/private/preboot/", rootopts | MNT_ROOTFS, &arg);
//    if (!err) {
//      LOG("mount preboot OK\n");
//    } else {
//      LOG("mount preboot %s FAILED with err=%d!\n", rootdev, err);
//      sleep(1);
//      goto retry_rootfs_mount;
//      // spin();
//    }
}

//void findvolume(void) {
//    LOG("find rootfs at %s\n", rootdev); // 文字列を配列にコピー
//    
//    int length = strlen(rootdev);  // 文字列の長さを取得
//
//    // 最後の文字が数字かどうかを確認
//    if (length > 0 && isdigit(rootdev[length - 1])) {
//        // 最後の文字を整数に変換して1を足す
//        int lastDigit = rootdev[length - 1] - '0';
//        lastDigit += 1;
//
//        // 新しい数字を文字に変換して代入
//        rootdev[length - 1] = '0' + lastDigit;
//
//        //printf("変更後の文字列: %s\n", str);
//    }
//}

