#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <asm/errno.h>

asmlinkage long sys_set_myFlag(pid_t pid, int flag_val)
{
	if (capable(CAP_SYS_ADMIN))
	{
		struct task_struct *tsk = find_task_by_vpid(pid);
		
		if (tsk != NULL)
		{
			if (flag_val == 1 || flag_val == 0)
			{
				tsk->myFlag = flag_val;
				return 0;
			}
			return -EINVAL;
		}
		return -ESRCH;
	}
	return -EPERM;
}
