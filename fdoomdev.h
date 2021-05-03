#ifndef FDOOMDEV_H
#define FDOOMDEV_H

#ifdef __KERNEL__
#include <linux/kernel.h>
#else
#include <stdint.h>
#endif

#include <linux/ioctl.h>

#define FDOOMDEV_MAX_USER_SLOTS		60

/* Creates a new buffer on the device, returns its fd.  */
struct fdoomdev_ioctl_create_buffer {
	uint32_t size;
	uint32_t pitch;
};
#define FDOOMDEV_IOCTL_CREATE_BUFFER _IOW('D', 0x00, struct fdoomdev_ioctl_create_buffer)

struct fdoomdev_ioctl_buffer_resize {
	uint32_t size;
};
#define FDOOMDEV_IOCTL_BUFFER_RESIZE _IOW('D', 0x01, struct fdoomdev_ioctl_buffer_resize)

/* Runs a batch of commands on the current context, returns 0.  */
struct fdoomdev_ioctl_run {
	uint32_t cmd_fd;
	uint32_t cmd_addr;
	uint32_t cmd_size;
	uint32_t buffers_num;
	uint32_t buffer_fd[FDOOMDEV_MAX_USER_SLOTS];
};
#define FDOOMDEV_IOCTL_RUN _IOW('D', 0x02, struct fdoomdev_ioctl_run)

/* Waits for all but `num_back` last commands submitted on this context
 * to complete, returns 0.  */
struct fdoomdev_ioctl_wait {
	uint32_t num_back;
};
#define FDOOMDEV_IOCTL_WAIT _IOW('D', 0x03, struct fdoomdev_ioctl_wait)

#endif
