/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2022-2025, Advanced Micro Devices, Inc.
 */

#ifndef _AIE2_MAILBOX_H_
#define _AIE2_MAILBOX_H_

struct mailbox;
struct mailbox_channel;

/*
 * xdna_mailbox_msg - message struct
 *
 * @opcode:	opcode for firmware
 * @handle:	handle used for the notify callback
 * @notify_cb:  callback function to notify the sender when there is response
 * @send_data:	pointing to sending data
 * @send_size:	size of the sending data
 * @id:		ID of the msg eventually pushed to device
 */
struct xdna_mailbox_msg {
	u32		opcode;
	void		*handle;
	int		(*notify_cb)(void *handle, void __iomem *data, size_t size);
	u8		*send_data;
	size_t		send_size;
	int		id;
};

/*
 * xdna_mailbox_res - mailbox hardware resource
 *
 * @ringbuf_base:	ring buffer base address
 * @ringbuf_size:	ring buffer size
 * @mbox_base:		mailbox base address
 * @mbox_size:		mailbox size
 */
struct xdna_mailbox_res {
	void __iomem	*ringbuf_base;
	size_t		ringbuf_size;
	void __iomem	*mbox_base;
	size_t		mbox_size;
	const char	*name;
};

/*
 * xdna_mailbox_chann_res - resources
 *
 * @rb_start_addr:	ring buffer start address
 * @rb_size:		ring buffer size
 * @mb_head_ptr_reg:	mailbox head pointer register
 * @mb_tail_ptr_reg:	mailbox tail pointer register
 */
struct xdna_mailbox_chann_res {
	u32 rb_start_addr;
	u32 rb_size;
	u32 mb_head_ptr_reg;
	u32 mb_tail_ptr_reg;
};

/*
 * xdna_mailbox_chann_info - channel information
 *
 * @x2i: host to firmware mailbox resources
 * @i2x: firmware to host mailbox resources
 * @intr_reg: register addr of MSI-X interrupt
 * @msix_id: mailbox MSI-X interrupt vector index
 */
struct xdna_mailbox_chann_info {
	struct xdna_mailbox_chann_res	x2i;
	struct xdna_mailbox_chann_res	i2x;
	u32				intr_reg;
	u32				msix_id;
};

/*
 * xdna_mailbox_create() -- create mailbox subsystem and initialize
 *
 * @dev: device pointer
 * @res: SRAM and mailbox resources
 *
 * Return: If success, return a handle of mailbox subsystem.
 * Otherwise, return NULL pointer.
 */
struct mailbox *xdna_mailbox_create(struct device *dev,
				    const struct xdna_mailbox_res *res);

/*
 * xdna_mailbox_destroy() -- destroy mailbox subsystem
 *
 * @mailbox: the handle return from xdna_mailbox_create()
 */
void xdna_mailbox_destroy(struct mailbox *mailbox);

enum xdna_mailbox_channel_type {
	MB_CHANNEL_MGMT = 0,
	MB_CHANNEL_USER_NORMAL,
	MB_CHANNEL_USER_POLL,
};

/*
 * xdna_mailbox_create_channel() -- Create a mailbox channel instance
 *
 * @mailbox: the handle return from xdna_mailbox_create()
 * @info: information to create a channel
 * @type: type of channel
 *
 * Return: If success, return a handle of mailbox channel. Otherwise, return NULL.
 */
struct mailbox_channel *
xdna_mailbox_create_channel(struct mailbox *mailbox,
			    struct xdna_mailbox_chann_info *info,
			    enum xdna_mailbox_channel_type type);

/*
 * xdna_mailbox_release_channel() -- release mailbox channel
 *
 * @mailbox_chann: the handle return from xdna_mailbox_create_channel()
 *
 * Release all resources, including messages, list entries, interrupt etc.
 * After this function all, the channel is not functional at all.
 * This is added for more complex synchronization secnario.
 */
void xdna_mailbox_release_channel(struct mailbox_channel *mailbox_chann);

/*
 * xdna_mailbox_free_channel() -- free mailbox channel
 *
 * @mailbox_chann: the handle return from xdna_mailbox_create_channel()
 *
 * Free all resources. This must be called after xdna_mailbox_release_channel().
 */
void xdna_mailbox_free_channel(struct mailbox_channel *mailbox_chann);

/*
 * xdna_mailbox_destroy_channel() -- destroy mailbox channel
 *
 * @mailbox_chann: the handle return from xdna_mailbox_create_channel()
 *
 * Destroy the channel, it release all the resources that the mailbox channel is
 * holding and then free all the resources.
 */
void xdna_mailbox_destroy_channel(struct mailbox_channel *mailbox_chann);

/*
 * xdna_mailbox_stop_channel() -- stop mailbox channel
 *
 * @mailbox_chann: the handle return from xdna_mailbox_create_channel()
 *
 * Stop receiving response and sending messages
 */
void xdna_mailbox_stop_channel(struct mailbox_channel *mailbox_chann);

/*
 * xdna_mailbox_send_msg() -- Send a message
 *
 * @mailbox_chann: Mailbox channel handle
 * @msg: message struct for message information
 * @tx_timeout: the timeout value for sending the message in ms.
 *
 * Return: If success return 0, otherwise, return error code
 */
int xdna_mailbox_send_msg(struct mailbox_channel *mailbox_chann,
			  struct xdna_mailbox_msg *msg, u64 tx_timeout);

#if defined(CONFIG_DEBUG_FS)
/*
 * xdna_mailbox_info_show() -- Show mailbox info for debug
 *
 * @mailbox: the handle return from xdna_mailbox_create()
 * @m: the seq_file handle
 *
 * Return: if success, return 0; otherwise return error code
 */
int xdna_mailbox_info_show(struct mailbox *mailbox,
			   struct seq_file *m);

/*
 * xdna_mailbox_ringbuf_show() -- Show ringbuf for debug
 *
 * @mailbox: the handle return from xdna_mailbox_create()
 * @m: the seq_file handle
 *
 * Return: if success, return 0; otherwise return error code
 */
int xdna_mailbox_ringbuf_show(struct mailbox *mailbox,
			      struct seq_file *m);
#endif

#endif /* _AIE2_MAILBOX_ */
