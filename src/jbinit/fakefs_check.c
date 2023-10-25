#include <jbinit.h>

void pinfo_check(bool* use_fakefs_p, char* bootargs, char* dev_rootdev) {
  struct stat statbuf;
  if (checkrain_options_enabled(pinfo.flags, palerain_option_rootful)) {
    snprintf(dev_rootdev, 0x20, "/dev/%s", pinfo.rootdev);
    *use_fakefs_p = true;
  }

  if (checkrain_options_enabled(info.flags, checkrain_option_force_revert)) {
    *use_fakefs_p = false;
    if (checkrain_options_enabled(pinfo.flags, palerain_option_clean_fakefs)) {
      LOG("Cannot force revert and clean fakefs at the same time\n");
      spin();
    }
  }

  if (checkrain_options_enabled(pinfo.flags, palerain_option_clean_fakefs) && 
    !checkrain_options_enabled(pinfo.flags, palerain_option_rootful)) {
      LOG("cannot clean fakefs while not in rootful mode");
      spin();
    } 

  if (checkrain_options_enabled(pinfo.flags, palerain_option_setup_rootful)) {
    *use_fakefs_p = false;
    if (!checkrain_options_enabled(pinfo.flags, palerain_option_rootful)) {
      LOG("cannot have palerain_option_setup_rootful when palerain_option_rootful is unset\n");
      spin();
    }
    if (strstr(bootargs, "wdt=-1") == NULL) {
      LOG("cannot have palerain_option_setup_rootful without wdt=-1 in boot-args\n");
      spin();
    }
    if (stat(dev_rootdev, &statbuf) == 0) {
      if (!checkrain_options_enabled(pinfo.flags, palerain_option_setup_rootful_forced) &&
      !checkrain_options_enabled(info.flags, checkrain_option_force_revert) ) {
        LOG("cannot create fakefs over an existing one without checkrain_option_force_revert\n");
        spin();
      }
    }
    if (checkrain_options_enabled(pinfo.flags, palerain_option_setup_rootful_forced)) {
      puts("Warning: this flag is deprecated, use checkrain_option_force_revert with palerain_option_setup_rootful/palerain_option_setup_partial_root to recreate fakefs/partial fakefs");
    }

    if (checkrain_options_enabled(pinfo.flags, palerain_option_clean_fakefs)) {
      LOG("cannot clean and setup fakefs at the same time\n");
      spin();
    }

  } else if (checkrain_options_enabled(pinfo.flags, palerain_option_setup_partial_root)) {
    LOG("cannot have palerain_option_setup_partial_root without palerain_option_setup_rootful\n");
    spin();
  }
}
