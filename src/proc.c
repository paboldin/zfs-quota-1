
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#include <linux/stat.h>

#include "tree.h"
#include "proc.h"

static struct proc_dir_entry *zfsquota_proc_root;

static const char quota_user[] = "quota.user";
static const char quota_group[] = "quota.group";
static const char aquota_user[] = "aquota.user";
static const char aquota_group[] = "aquota.group";

int zqproc_get_sb_type(struct inode *inode, struct super_block **psb,
		       int *ptype)
{
	if (psb)
		*psb = (struct super_block *)proc_get_parent_data(inode);
	if (ptype)
		*ptype = (int)(unsigned long)PDE_DATA(inode);
	return 0;
}

extern struct file_operations zfs_aquotf_vfsv2r1_file_operations;

struct proc_dir_entry* zqproc_register_handle(struct super_block *sb)
{
	struct proc_dir_entry *dev_dir;
	char buf[32];

	sprintf(buf, "%08x", new_encode_dev(sb->s_dev));
	dev_dir = proc_mkdir_data(buf, S_IRWXU, zfsquota_proc_root, sb);

	proc_create_data(aquota_user, S_IRWXU, dev_dir,
			 &zfs_aquotf_vfsv2r1_file_operations,
			 (void *)USRQUOTA);

	proc_create_data(aquota_group, S_IRWXU, dev_dir,
			 &zfs_aquotf_vfsv2r1_file_operations,
			 (void *)GRPQUOTA);

	return dev_dir;
}

int __init zfsquota_proc_init(void)
{
	zfsquota_proc_root = proc_mkdir_mode("zfsquota", S_IRWXU, NULL);
	return 0;
}

void __exit zfsquota_proc_exit(void)
{
	proc_remove(zfsquota_proc_root);
}
