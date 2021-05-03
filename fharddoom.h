#ifndef FHARDDOOM_H
#define FHARDDOOM_H

/* Section 1: PCI ids. */

#define FHARDDOOM_VENDOR_ID			0x0666
#define FHARDDOOM_DEVICE_ID			0x1996

/* Section 2: MMIO registers.  */

/* Section 2.1: main control area.  */

/* Enables active units of the device.  MMU and CACHE are passive and don't
 * have an enable (disable the client instead).  FIFOs also don't have enables
 * — disable the source and/or destination unit instead.  */
#define FHARDDOOM_ENABLE				0x0000
#define FHARDDOOM_ENABLE_CMD				0x00000001
#define FHARDDOOM_ENABLE_FE				0x00000002
#define FHARDDOOM_ENABLE_SRD				0x00000004
#define FHARDDOOM_ENABLE_SPAN				0x00000008
#define FHARDDOOM_ENABLE_COL				0x00000010
#define FHARDDOOM_ENABLE_FX				0x00000020
#define FHARDDOOM_ENABLE_SWR				0x00000040
#define FHARDDOOM_ENABLE_ALL				0x0000007f
/* Status of device units — 1 means they have work to do.  */
#define FHARDDOOM_STATUS				0x0004
#define FHARDDOOM_STATUS_CMD				0x00000001
#define FHARDDOOM_STATUS_FE				0x00000002
#define FHARDDOOM_STATUS_SRD				0x00000004
#define FHARDDOOM_STATUS_SPAN				0x00000008
#define FHARDDOOM_STATUS_COL				0x00000010
#define FHARDDOOM_STATUS_FX				0x00000020
#define FHARDDOOM_STATUS_SWR				0x00000040
#define FHARDDOOM_STATUS_FIFO_SRDCMD			0x00010000
#define FHARDDOOM_STATUS_FIFO_SPANCMD			0x00020000
#define FHARDDOOM_STATUS_FIFO_COLCMD			0x00040000
#define FHARDDOOM_STATUS_FIFO_FXCMD			0x00080000
#define FHARDDOOM_STATUS_FIFO_SWRCMD			0x00100000
#define FHARDDOOM_STATUS_FIFO_COLIN			0x00200000
#define FHARDDOOM_STATUS_FIFO_FXIN			0x00400000
#define FHARDDOOM_STATUS_FIFO_FESEM			0x01000000
#define FHARDDOOM_STATUS_FIFO_SRDSEM			0x02000000
#define FHARDDOOM_STATUS_FIFO_COLSEM			0x04000000
#define FHARDDOOM_STATUS_FIFO_SPANSEM			0x08000000
#define FHARDDOOM_STATUS_FIFO_SPANOUT			0x10000000
#define FHARDDOOM_STATUS_FIFO_COLOUT			0x20000000
#define FHARDDOOM_STATUS_FIFO_FXOUT			0x40000000
/* The reset register.  Punching 1 will clear all pending work and/or
 * cached data.  */
#define FHARDDOOM_RESET					0x0004
#define FHARDDOOM_RESET_CMD				0x00000001
#define FHARDDOOM_RESET_FE				0x00000002
#define FHARDDOOM_RESET_SRD				0x00000004
#define FHARDDOOM_RESET_SPAN				0x00000008
#define FHARDDOOM_RESET_COL				0x00000010
#define FHARDDOOM_RESET_FX				0x00000020
#define FHARDDOOM_RESET_SWR				0x00000040
#define FHARDDOOM_RESET_MMU				0x00000080
#define FHARDDOOM_RESET_STATS				0x00000100
#define FHARDDOOM_RESET_TLB				0x00000200
#define FHARDDOOM_RESET_CACHE_COL_CMAP_B		0x00001000
#define FHARDDOOM_RESET_CACHE_COL_SRC			0x00002000
#define FHARDDOOM_RESET_CACHE_SPAN_SRC			0x00004000
#define FHARDDOOM_RESET_CACHE_SWR_TRANSMAP		0x00008000
#define FHARDDOOM_RESET_FIFO_SRDCMD			0x00010000
#define FHARDDOOM_RESET_FIFO_SPANCMD			0x00020000
#define FHARDDOOM_RESET_FIFO_COLCMD			0x00040000
#define FHARDDOOM_RESET_FIFO_FXCMD			0x00080000
#define FHARDDOOM_RESET_FIFO_SWRCMD			0x00100000
#define FHARDDOOM_RESET_FIFO_COLIN			0x00200000
#define FHARDDOOM_RESET_FIFO_FXIN			0x00400000
#define FHARDDOOM_RESET_FIFO_FESEM			0x01000000
#define FHARDDOOM_RESET_FIFO_SRDSEM			0x02000000
#define FHARDDOOM_RESET_FIFO_COLSEM			0x04000000
#define FHARDDOOM_RESET_FIFO_SPANSEM			0x08000000
#define FHARDDOOM_RESET_FIFO_SPANOUT			0x10000000
#define FHARDDOOM_RESET_FIFO_COLOUT			0x20000000
#define FHARDDOOM_RESET_FIFO_FXOUT			0x40000000
#define FHARDDOOM_RESET_ALL				0x7f7ff3ff
/* Interrupt status.  */
#define FHARDDOOM_INTR					0x0008
#define FHARDDOOM_INTR_FENCE_WAIT			0x00000001
#define FHARDDOOM_INTR_FEED_ERROR			0x00000002
#define FHARDDOOM_INTR_CMD_ERROR			0x00000004
#define FHARDDOOM_INTR_FE_ERROR				0x00000008
#define FHARDDOOM_INTR_PAGE_FAULT(i)			(0x00000100 << (i))
#define FHARDDOOM_INTR_PAGE_FAULT_CMD_MAIN		0x00000100
#define FHARDDOOM_INTR_PAGE_FAULT_CMD_SUB		0x00000200
#define FHARDDOOM_INTR_PAGE_FAULT_SRD			0x00000400
#define FHARDDOOM_INTR_PAGE_FAULT_SWR_DST		0x00000800
#define FHARDDOOM_INTR_PAGE_FAULT_COL_CMAP_B		0x00001000
#define FHARDDOOM_INTR_PAGE_FAULT_COL_SRC		0x00002000
#define FHARDDOOM_INTR_PAGE_FAULT_SPAN_SRC		0x00004000
#define FHARDDOOM_INTR_PAGE_FAULT_SWR_TRANSMAP		0x00008000
#define FHARDDOOM_INTR_MASK				0x0000ff0f
/* And enable (same bitfields).  */
#define FHARDDOOM_INTR_ENABLE				0x000c

/* Section 2.2: CMD — command processor.  Fetches commands from the command
 * buffers and the manual feed, presents them to FE, reacts to FE requests.
 *
 * The three user-facing features of this units are main command buffer,
 * manual command feed, and fence functionality.
 */

/* The main command buffer: user-facing registers.
 *
 * The GET register is updated by hardware (and should not be written
 * unless PUT.ENABLE is off), the PUT register is updated by kernel.
 *
 * The pseudocode for the main command buffer fetcher is:
 *
 * while True:
 *     # Whenever we have available command space and GET != PUT (disregarding the ENABLE bit),
 *     # fetch commands into the internal FIFO.
 *     if internal_fifo.free != 0 and CMD_MAIN_GET != CMD_MAIN_PUT and CMD_MAIN_SETUP.ENABLE:
 *         # Read command from the GET address.
 *         cmd = mem_read_le32(CMD_MAIN_SETUP.SLOT, CMD_MAIN_GET)
 *         # Store it for execution.
 *         internal_fifo.push(cmd)
 *         # Increment GET, possibly wrapping.
 *         CMD_MAIN_GET += 4
 *         if CMD_MAIN_GET == CMD_MAIN_SETUP.WRAP:
 *             CMD_MAIN_GET = 0
 *
 */

#define FHARDDOOM_CMD_MAIN_SETUP			0x0080
#define FHARDDOOM_CMD_MAIN_SETUP_WRAP_MASK		0x003ffffc
#define FHARDDOOM_CMD_MAIN_SETUP_SLOT_MASK		0x3f000000
#define FHARDDOOM_CMD_MAIN_SETUP_SLOT_SHIFT		24
#define FHARDDOOM_CMD_MAIN_SETUP_ENABLE			0x80000000
#define FHARDDOOM_CMD_MAIN_SETUP_MASK			0xbf3ffffc

#define FHARDDOOM_CMD_MAIN_GET				0x0084
#define FHARDDOOM_CMD_MAIN_PUT				0x0088

#define FHARDDOOM_CMD_PTR_MASK				0x003ffffc

/* The manual feed: user-facing registers.
 *
 * The CMD_MANUAL_FREE register is read-only, and returns the current amount
 * of free space in the internal command FIFO available for writing via
 * CMD_MANUAL_FEED register, in words.  The values possible are 0-255.
 * Immediately after reset, or when otherwise idle, this register is at
 * the max value (255).  It decrements by one every time a command word is
 * sent via the CMD_MANUAL_FEED, and it increments by one every time such
 * a command word is consumed for execution by FE.  This register reads
 * as 0 whenever the automatic fetcher is enabled, or when the internal
 * FIFO still contains commands fetched by automatic fetcher.
 *
 * The CMD_MANUAL_FEED register is write only and is used to manually feed
 * command words for execution, bypassing the fetcher described above.
 * Every time it is written, the word written is stored into the internal
 * command FIFO as if it was just fetched from the main command buffer.
 *
 * Writing this register is disallowed (and will trigger the FEED_ERROR
 * interrupt) in the following cases:
 *
 * - CMD_MANUAL_FREE is 0 (no space in the FIFO)
 * - CMD_MAIN_SETUP.ENABLE is 1 (manual feed cannot be used while the fetcher
 *   is active)
 */

#define FHARDDOOM_CMD_MANUAL_FREE			0x008c
#define FHARDDOOM_CMD_MANUAL_FEED			0x008c

/* The fences: user-facing registers.
 *
 * The CMD_FENCE_LAST register contains the payload of the last FENCE command
 * executed.  It is writable by kernel, but should only be written as part
 * of the initialization sequence.
 *
 * The CMD_FENCE_WAIT register contains the FENCE payload that, when the
 * matching FENCE command is executed, will trigger a FENCE_WAIT interrupt.
 * This allows the kernel to selectively wait for the execution of a specific
 * batch of commands.  If FENCE_WAIT interrupt is not needed, the DISABLE bit
 * can be set here.
 */

#define FHARDDOOM_CMD_FENCE_LAST			0x0090
#define FHARDDOOM_CMD_FENCE_LAST_MASK			0x0fffffff

#define FHARDDOOM_CMD_FENCE_WAIT			0x0094
#define FHARDDOOM_CMD_FENCE_WAIT_DATA_MASK		0x0fffffff
#define FHARDDOOM_CMD_FENCE_WAIT_DISABLE		0x80000000
#define FHARDDOOM_CMD_FENCE_WAIT_MASK			0x8fffffff

/* Error reporting registers, to be read when CMD_ERROR interrupt happens.
 *
 * CMD_ERROR_CODE is the error code describing the problem.
 *
 * CMD_ERROR_DATA is some kind of extra information, the exact meaning varying
 * on the error code.
 *
 * CMD_INFO contains information regarding the command being executed,
 * which is the command that caused the error when CMD_ERROR happens.
 *
 * CMD_HEADER contains a copy of the first word of the failing command.
 */

#define FHARDDOOM_CMD_ERROR_CODE			0x0098
/* The subroutine command buffer ended in the middle of a command.
 * Extra data: buffer end address.  */
#define FHARDDOOM_CMD_ERROR_CODE_SUB_INCOMPLETE		0x00000000
/* Unknown command.  No extra data.  */
#define FHARDDOOM_CMD_ERROR_CODE_UNK_COMMAND		0x00000001
/* Privileged command in subroutine buffer.  No extra data.  */
#define FHARDDOOM_CMD_ERROR_CODE_PRIV_COMMAND		0x00000002
/* Command references an unbound slot.  Extra data: slot index.  */
#define FHARDDOOM_CMD_ERROR_CODE_INVALID_SLOT		0x00000003
/* User command references a non-USER slot.  Extra data: slot index.  */
#define FHARDDOOM_CMD_ERROR_CODE_KERNEL_SLOT		0x00000004
/* Command references a non-WRITABLE slot for writing.  Extra data: slot index.  */
#define FHARDDOOM_CMD_ERROR_CODE_RO_SLOT		0x00000005
/* Y coordinates for DRAW_COLUMNS in wrong order.  Extra data: offending command word.  */
#define FHARDDOOM_CMD_ERROR_CODE_DRAW_COLUMNS_Y_REV	0x00000006
/* X coordinates for DRAW_SPANS in wrong order.  Extra data: offending command word.  */
#define FHARDDOOM_CMD_ERROR_CODE_DRAW_SPANS_X_REV	0x00000007
#define FHARDDOOM_CMD_ERROR_CODE_MASK			0x0000000f

#define FHARDDOOM_CMD_ERROR_DATA			0x009c

#define FHARDDOOM_CMD_INFO				0x00a0
#define FHARDDOOM_CMD_INFO_PTR_MASK			0x003ffffc
#define FHARDDOOM_CMD_INFO_SLOT_MASK			0x3f000000
#define FHARDDOOM_CMD_INFO_SLOT_SHIFT			24
/* Set if the command came from the subroutine buffer.  */
#define FHARDDOOM_CMD_INFO_SUB				0x40000000
/* Set if the command came via CMD_MANUAL_FEED register.
 * If set, PTR and SLOT are meaningless.  */
#define FHARDDOOM_CMD_INFO_MANUAL			0x80000000
#define FHARDDOOM_CMD_INFO_MASK				0xff3ffffc

#define FHARDDOOM_CMD_HEADER				0x00a4

/* And the internal registers.  */

#define FHARDDOOM_CMD_SUB_GET				0x00a8
#define FHARDDOOM_CMD_SUB_GET_MASK			0x003ffffc

#define FHARDDOOM_CMD_SUB_LEN				0x00ac
#define FHARDDOOM_CMD_SUB_LEN_MASK			0x003ffffc

#define FHARDDOOM_CMD_SUB_STATE				0x00b0
/* Address of the next command word to be delivered to FE.  */
#define FHARDDOOM_CMD_SUB_STATE_PTR_MASK		0x003ffffc
#define FHARDDOOM_CMD_SUB_STATE_SLOT_MASK		0x3f000000
#define FHARDDOOM_CMD_SUB_STATE_SLOT_SHIFT		24
#define FHARDDOOM_CMD_SUB_STATE_MASK			0x3f3ffffc

#define FHARDDOOM_CMD_SUB_FIFO_GET			0x00b4
#define FHARDDOOM_CMD_SUB_FIFO_PUT			0x00b8
#define FHARDDOOM_CMD_SUB_FIFO_WINDOW			0x00bc
#define FHARDDOOM_CMD_SUB_FIFO_SIZE			0x00000100

#define FHARDDOOM_CMD_MAIN_STATE			0x00c0
/* Address of the next command word to be delivered to FE.  */
#define FHARDDOOM_CMD_MAIN_STATE_PTR_MASK		0x003ffffc
/* If set, any words currently in FIFO are from CMD_MANUAL_FEED.  */
#define FHARDDOOM_CMD_MAIN_STATE_MANUAL			0x80000000
#define FHARDDOOM_CMD_MAIN_STATE_MASK			0x803ffffc

#define FHARDDOOM_CMD_MAIN_FIFO_GET			0x00c4
#define FHARDDOOM_CMD_MAIN_FIFO_PUT			0x00c8
#define FHARDDOOM_CMD_MAIN_FIFO_WINDOW			0x00cc
#define FHARDDOOM_CMD_MAIN_FIFO_SIZE			0x00000100

/* Section 2.3: FE — the front end.  Runs firmware that reads the user
 * commands, and converts them to the proper low-level commands understood by
 * COL/SPAN/FX/SWR.  */

/* The FE RAM windows.  Reading/writing the WINDOW register reads/writes
 * a word of FE code/data RAM at address ADDR, then auto-increments ADDR by 4.  */
#define FHARDDOOM_FE_CODE_ADDR				0x0100
#define FHARDDOOM_FE_CODE_ADDR_MASK			0x0000fffc
#define FHARDDOOM_FE_CODE_WINDOW			0x0104
#define FHARDDOOM_FE_DATA_ADDR				0x0108
#define FHARDDOOM_FE_DATA_ADDR_MASK			0x0000fffc
#define FHARDDOOM_FE_DATA_WINDOW			0x010c
/* The current program counter.  */
#define FHARDDOOM_FE_PC					0x0110
#define FHARDDOOM_FE_PC_MASK				0xfffffffc
/* The current execution state.  */
#define FHARDDOOM_FE_STATE				0x0114
/* The core is actively executing instructions.  */
#define FHARDDOOM_FE_STATE_STATE_RUNNING		0x00000000
/* The core is halted due to an error.  */
#define FHARDDOOM_FE_STATE_STATE_ERROR			0x00000001
/* The core is blocked on CMD_FETCH_* read.  */
#define FHARDDOOM_FE_STATE_STATE_CMD_FETCH_HEADER	0x00000002
#define FHARDDOOM_FE_STATE_STATE_CMD_FETCH_ARG		0x00000003
/* The core is blocked on a FIFO write.  */
#define FHARDDOOM_FE_STATE_STATE_SRDCMD			0x00000004
#define FHARDDOOM_FE_STATE_STATE_SPANCMD		0x00000005
#define FHARDDOOM_FE_STATE_STATE_COLCMD			0x00000006
#define FHARDDOOM_FE_STATE_STATE_FXCMD			0x00000007
#define FHARDDOOM_FE_STATE_STATE_SWRCMD			0x00000008
/* The core is blocked on FESEM read.  */
#define FHARDDOOM_FE_STATE_STATE_FESEM			0x00000009
#define FHARDDOOM_FE_STATE_STATE_MASK			0x0000000f
/* The pending command code, when core is blocked on a FIFO write.  */
#define FHARDDOOM_FE_STATE_CMD_MASK			0x000000f0
#define FHARDDOOM_FE_STATE_CMD_SHIFT			4
/* The destination register, when core is blocked on a register read.  */
#define FHARDDOOM_FE_STATE_DST_MASK			0x00001f00
#define FHARDDOOM_FE_STATE_DST_SHIFT			8
#define FHARDDOOM_FE_STATE_MASK				0x00001fff
/* The pending write data, when core is blocked on a FIFO write.  */
#define FHARDDOOM_FE_WRITE_DATA				0x0118
/* The FE error data (set when FE_ERROR interrupt is triggered).  */
#define FHARDDOOM_FE_ERROR_DATA_A			0x0120
#define FHARDDOOM_FE_ERROR_DATA_B			0x0124
/* The FE error code (set when FE_ERROR interrupt is triggered).  */
#define FHARDDOOM_FE_ERROR_CODE				0x0128
/* The FE core encountered an illegal instruction.  A is address, B is
 * the instruction opcode.  */
#define FHARDDOOM_FE_ERROR_CODE_ILLEGAL_INSTRUCTION	0x00000000
/* The FE core tried to jump to an unaligned address.  A is current PC, B is would-be new PC.  */
#define FHARDDOOM_FE_ERROR_CODE_UNALIGNED_INSTRUCTION	0x00000001
/* The FE core encountered a load bus error.  A is the faulting
 * address.  */
#define FHARDDOOM_FE_ERROR_CODE_BUS_ERROR_LOAD		0x00000002
/* The FE core encountered a store bus error.  A is the faulting
 * address, B is the written data.  */
#define FHARDDOOM_FE_ERROR_CODE_BUS_ERROR_STORE		0x00000003
/* The FE core encountered an instruction fetch bus error.  A is the
 * faulting address.  */
#define FHARDDOOM_FE_ERROR_CODE_BUS_ERROR_EXEC		0x00000004
#define FHARDDOOM_FE_ERROR_CODE_MASK			0x0000000f
/* The registers.  */
#define FHARDDOOM_FE_REG(i)				(0x0180 + (i) * 4)
#define FHARDDOOM_FE_REG_NUM				0x20

/* Section 2.4: FIFOs.  */

#define FHARDDOOM_FIFO_SRDCMD_GET			0x0200
#define FHARDDOOM_FIFO_SRDCMD_PUT			0x0204
#define FHARDDOOM_FIFO_SRDCMD_CMD_WINDOW		0x0208
#define FHARDDOOM_FIFO_SRDCMD_DATA_WINDOW		0x020c
#define FHARDDOOM_FIFO_SRDCMD_SIZE			0x400
#define FHARDDOOM_FIFO_SPANCMD_GET			0x0210
#define FHARDDOOM_FIFO_SPANCMD_PUT			0x0214
#define FHARDDOOM_FIFO_SPANCMD_CMD_WINDOW		0x0218
#define FHARDDOOM_FIFO_SPANCMD_DATA_WINDOW		0x021c
#define FHARDDOOM_FIFO_SPANCMD_SIZE			0x400
#define FHARDDOOM_FIFO_COLCMD_GET			0x0220
#define FHARDDOOM_FIFO_COLCMD_PUT			0x0224
#define FHARDDOOM_FIFO_COLCMD_CMD_WINDOW		0x0228
#define FHARDDOOM_FIFO_COLCMD_DATA_WINDOW		0x022c
#define FHARDDOOM_FIFO_COLCMD_SIZE			0x400
#define FHARDDOOM_FIFO_FXCMD_GET			0x0230
#define FHARDDOOM_FIFO_FXCMD_PUT			0x0234
#define FHARDDOOM_FIFO_FXCMD_CMD_WINDOW			0x0238
#define FHARDDOOM_FIFO_FXCMD_DATA_WINDOW		0x023c
#define FHARDDOOM_FIFO_FXCMD_SIZE			0x400
#define FHARDDOOM_FIFO_SWRCMD_GET			0x0240
#define FHARDDOOM_FIFO_SWRCMD_PUT			0x0244
#define FHARDDOOM_FIFO_SWRCMD_CMD_WINDOW		0x0248
#define FHARDDOOM_FIFO_SWRCMD_DATA_WINDOW		0x024c
#define FHARDDOOM_FIFO_SWRCMD_SIZE			0x400
#define FHARDDOOM_FIFO_CMD_MASK				0x0000000f
/* The 4 semaphore registers.  Bumped by one by SWR commands, decreased by FE/SRD/SPAN/COL.  */
#define FHARDDOOM_FIFO_FESEM				0x0250
#define FHARDDOOM_FIFO_SRDSEM				0x0254
#define FHARDDOOM_FIFO_COLSEM				0x0258
#define FHARDDOOM_FIFO_SPANSEM				0x025c
#define FHARDDOOM_FIFO_SEM_MASK				0x00000001
#define FHARDDOOM_FIFO_COLIN_GET			0x0260
#define FHARDDOOM_FIFO_COLIN_PUT			0x0264
#define FHARDDOOM_FIFO_COLIN_SIZE			0x40
#define FHARDDOOM_FIFO_FXIN_GET				0x0268
#define FHARDDOOM_FIFO_FXIN_PUT				0x026c
#define FHARDDOOM_FIFO_FXIN_SIZE			0x40
#define FHARDDOOM_FIFO_COLIN_DATA_WINDOW		0x0280
#define FHARDDOOM_FIFO_FXIN_DATA_WINDOW			0x02c0
#define FHARDDOOM_FIFO_SPANOUT_GET			0x0300
#define FHARDDOOM_FIFO_SPANOUT_PUT			0x0304
#define FHARDDOOM_FIFO_SPANOUT_SIZE			0x40
#define FHARDDOOM_FIFO_COLOUT_GET			0x0310
#define FHARDDOOM_FIFO_COLOUT_PUT			0x0314
#define FHARDDOOM_FIFO_COLOUT_SIZE			0x40
#define FHARDDOOM_FIFO_COLOUT_MASK_WINDOW		0x0318
#define FHARDDOOM_FIFO_FXOUT_GET			0x0320
#define FHARDDOOM_FIFO_FXOUT_PUT			0x0324
#define FHARDDOOM_FIFO_FXOUT_SIZE			0x40
#define FHARDDOOM_FIFO_FXOUT_MASK_WINDOW		0x0328
#define FHARDDOOM_FIFO_SPANOUT_DATA_WINDOW		0x0340
#define FHARDDOOM_FIFO_COLOUT_DATA_WINDOW		0x0380
#define FHARDDOOM_FIFO_FXOUT_DATA_WINDOW		0x03c0

/* Section 2.5: MMU.  */

/* The MMU client indices.  */
#define FHARDDOOM_MMU_CLIENT_CMD_MAIN			0
#define FHARDDOOM_MMU_CLIENT_CMD_SUB			1
#define FHARDDOOM_MMU_CLIENT_SRD			2
#define FHARDDOOM_MMU_CLIENT_SWR_DST			3
#define FHARDDOOM_MMU_CLIENT_COL_CMAP_B			4
#define FHARDDOOM_MMU_CLIENT_COL_SRC			5
#define FHARDDOOM_MMU_CLIENT_SPAN_SRC			6
#define FHARDDOOM_MMU_CLIENT_SWR_TRANSMAP		7
#define FHARDDOOM_MMU_CLIENT_NUM			8
/* The master PT pointers, aka the slots.  */
#define FHARDDOOM_MMU_SLOT(i)				(0x400 + (i) * 8)
#define FHARDDOOM_MMU_SLOT_VALID			0x00000001
#define FHARDDOOM_MMU_SLOT_PA_MASK			0xfffffff0
#define FHARDDOOM_MMU_SLOT_PA_SHIFT			8
#define FHARDDOOM_MMU_SLOT_MASK				0xfffffff1
#define FHARDDOOM_MMU_SLOT_NUM				64
/* The per-client singular TLBs.  */
#define FHARDDOOM_MMU_CLIENT_TLB_TAG(i)			(0x0500 + (i) * 8)
#define FHARDDOOM_MMU_CLIENT_TLB_VALUE(i)		(0x0504 + (i) * 8)
/* The last translated virt address for each client (useful for page fault handling).  */
#define FHARDDOOM_MMU_CLIENT_VA(i)			(0x0540 + (i) * 4)
#define FHARDDOOM_MMU_CLIENT_VA_ADDR_MASK		0x003fffff
#define FHARDDOOM_MMU_CLIENT_VA_SLOT_MASK		0x3f000000
#define FHARDDOOM_MMU_CLIENT_VA_SLOT_SHIFT		24
/* The PTE cache pool.  */
#define FHARDDOOM_MMU_TLB_TAG(i)			(0x0600 + (i) * 8)
#define FHARDDOOM_MMU_TLB_VALUE(i)			(0x0604 + (i) * 8)
#define FHARDDOOM_MMU_TLB_SIZE				64
/* The tag fields.  */
#define FHARDDOOM_MMU_TLB_TAG_VALID			0x00000001
#define FHARDDOOM_MMU_TLB_TAG_MASK			0x3f3ff001

/* Section 2.6: STATS.  */

#define FHARDDOOM_STATS(i)				(0x0800 + (i) * 4)
#define FHARDDOOM_STATS_NUM				0x80
/* Stats 0:0x20 are for the fw.  */
/* A job has been processed.  */
#define FHARDDOOM_STAT_FW_JOB				0x00
/* A user command has been processed.  */
#define FHARDDOOM_STAT_FW_CMD				0x01
/* A fill rect command has been processed.  */
#define FHARDDOOM_STAT_FW_FILL_RECT			0x02
/* A fill rect span has been drawn.  */
#define FHARDDOOM_STAT_FW_FILL_RECT_SPAN		0x03
/* A horizontal draw line command has been processed.  */
#define FHARDDOOM_STAT_FW_DRAW_LINE_HORIZ		0x04
/* A horizontal draw line segment has been processed.  */
#define FHARDDOOM_STAT_FW_DRAW_LINE_HORIZ_SEG		0x05
/* A vertical draw line command has been processed.  */
#define FHARDDOOM_STAT_FW_DRAW_LINE_VERT		0x06
/* A vertical draw line segment has been processed.  */
#define FHARDDOOM_STAT_FW_DRAW_LINE_VERT_SEG		0x07
/* A simple blit command has been processed.  */
#define FHARDDOOM_STAT_FW_BLIT_SIMPLE			0x08
/* A simple blit span has been drawn.  */
#define FHARDDOOM_STAT_FW_BLIT_SIMPLE_SPAN		0x09
/* A background blit command has been processed.  */
#define FHARDDOOM_STAT_FW_BLIT_BG			0x0a
/* A background blit span has been drawn.  */
#define FHARDDOOM_STAT_FW_BLIT_BG_SPAN			0x0b
/* A complex blit command has been processed.  */
#define FHARDDOOM_STAT_FW_BLIT_COMPLEX			0x0c
/* A complex blit span has been drawn.  */
#define FHARDDOOM_STAT_FW_BLIT_COMPLEX_SPAN		0x0d
/* A draw spans command has been processed.  */
#define FHARDDOOM_STAT_FW_DRAW_SPANS			0x0e
/* A draw spans span has been drawn.  */
#define FHARDDOOM_STAT_FW_DRAW_SPANS_SPAN		0x0f
/* A wipe command has been processed.  */
#define FHARDDOOM_STAT_FW_WIPE				0x10
/* A wipe column has been drawn.  */
#define FHARDDOOM_STAT_FW_WIPE_COL			0x11
/* A wipe batch has been drawn.  */
#define FHARDDOOM_STAT_FW_WIPE_BATCH			0x12
/* A wipe segment has been drawn.  */
#define FHARDDOOM_STAT_FW_WIPE_SEG			0x13
/* A draw columns command has been processed.  */
#define FHARDDOOM_STAT_FW_DRAW_COLUMNS			0x14
/* A draw columns column has been drawn.  */
#define FHARDDOOM_STAT_FW_DRAW_COLUMNS_COL		0x15
/* A draw columns batch has been drawn.  */
#define FHARDDOOM_STAT_FW_DRAW_COLUMNS_BATCH		0x16
/* A draw columns segment has been drawn.  */
#define FHARDDOOM_STAT_FW_DRAW_COLUMNS_SEG		0x17
/* A draw fuzz command has been processed.  */
#define FHARDDOOM_STAT_FW_DRAW_FUZZ			0x18
/* A draw fuzz column has been drawn.  */
#define FHARDDOOM_STAT_FW_DRAW_FUZZ_COL			0x19
/* A draw fuzz batch has been drawn.  */
#define FHARDDOOM_STAT_FW_DRAW_FUZZ_BATCH		0x1a
/* A draw fuzz segment has been drawn.  */
#define FHARDDOOM_STAT_FW_DRAW_FUZZ_SEG			0x1b
/* 0x1c-0x1f unused and reserved for fw.  */
/* A TLB direct hit (per-client).  */
#define FHARDDOOM_STAT_TLB_HIT(i)			(0x20 + (i))
/* A TLB pool hit (per-client).  */
#define FHARDDOOM_STAT_TLB_POOL_HIT(i)			(0x28 + (i))
/* A TLB miss — PTE fetch (per-client).  */
#define FHARDDOOM_STAT_TLB_MISS(i)			(0x30 + (i))
/* A kernel PDE TLB hit.  */
#define FHARDDOOM_STAT_TLB_KERNEL_PDE_HIT		0x38
/* A kernel PDE TLB miss — PDE fetch.  */
#define FHARDDOOM_STAT_TLB_KERNEL_PDE_MISS		0x39
/* A user PDE TLB hit.  */
#define FHARDDOOM_STAT_TLB_USER_PDE_HIT			0x3a
/* A user PDE TLB miss — PDE fetch.  */
#define FHARDDOOM_STAT_TLB_USER_PDE_MISS		0x3b
/* A kernel TLB flush.  */
#define FHARDDOOM_STAT_TLB_KERNEL_FLUSH			0x3c
/* A user TLB flush.  */
#define FHARDDOOM_STAT_TLB_USER_FLUSH			0x3d
/* 0x3e-0x3f unused.  */
/* A cache hit (per-client).  */
#define FHARDDOOM_STAT_CACHE_HIT(i)			(0x40 + (i))
/* A cache miss (per-client).  */
#define FHARDDOOM_STAT_CACHE_MISS(i)			(0x44 + (i))
/* A cache speculative hit (per-client).  */
#define FHARDDOOM_STAT_CACHE_SPEC_HIT(i)		(0x48 + (i))
/* A cache speculative miss (per-client).  */
#define FHARDDOOM_STAT_CACHE_SPEC_MISS(i)		(0x4c + (i))
/* A cache flush (per-client).  */
#define FHARDDOOM_STAT_CACHE_FLUSH(i)			(0x50 + (i))
/* FENCE_WAIT interrupt triggered.  */
#define FHARDDOOM_STAT_FENCE_WAIT			0x54
/* 0x55 unused.  */
/* CMD block read.  */
#define FHARDDOOM_STAT_CMD_BLOCK			0x56
/* CMD word read.  */
#define FHARDDOOM_STAT_CMD_WORD				0x57
/* FE instruction executed.  */
#define FHARDDOOM_STAT_FE_INSN				0x58
/* FE load executed.  */
#define FHARDDOOM_STAT_FE_LOAD				0x59
/* FE store executed.  */
#define FHARDDOOM_STAT_FE_STORE				0x5a
/* 0x5b unused.  */
/* MMIO register read.  */
#define FHARDDOOM_STAT_MMIO_READ			0x5c
/* MMIO register write.  */
#define FHARDDOOM_STAT_MMIO_WRITE			0x5d
/* 0x5e-0x5f unused.  */
/* SRD command executed.  */
#define FHARDDOOM_STAT_SRD_CMD				0x60
/* SRD read executed.  */
#define FHARDDOOM_STAT_SRD_READ				0x61
/* SRD block read.  */
#define FHARDDOOM_STAT_SRD_BLOCK			0x62
/* SRD FESEM executed.  */
#define FHARDDOOM_STAT_SRD_FESEM			0x63
/* SPAN command executed.  */
#define FHARDDOOM_STAT_SPAN_CMD				0x64
/* SPAN draw executed.  */
#define FHARDDOOM_STAT_SPAN_DRAW			0x65
/* SPAN block drawn.  */
#define FHARDDOOM_STAT_SPAN_BLOCK			0x66
/* SPAN pixel drawn.  */
#define FHARDDOOM_STAT_SPAN_PIXEL			0x67
/* COL command executed.  */
#define FHARDDOOM_STAT_COL_CMD				0x68
/* COL LOAD_CMAP_A command executed.  */
#define FHARDDOOM_STAT_COL_LOAD_CMAP_A			0x69
/* COL draw executed.  */
#define FHARDDOOM_STAT_COL_DRAW				0x6a
/* COL draw executed with CMAP_A enabled.  */
#define FHARDDOOM_STAT_COL_DRAW_CMAP_A			0x6b
/* COL block drawn.  */
#define FHARDDOOM_STAT_COL_BLOCK			0x6c
/* COL block drawn with CMAP_A enabled.  */
#define FHARDDOOM_STAT_COL_BLOCK_CMAP_A			0x6d
/* COL pixel drawn.  */
#define FHARDDOOM_STAT_COL_PIXEL			0x6e
/* COL pixel drawn with CMAP_B enabled.  */
#define FHARDDOOM_STAT_COL_PIXEL_CMAP_B			0x6f
/* FX command executed.  */
#define FHARDDOOM_STAT_FX_CMD				0x70
/* FX LOAD_CMAP command executed.  */
#define FHARDDOOM_STAT_FX_LOAD_CMAP			0x71
/* FX LOAD_FUZZ command executed.  */
#define FHARDDOOM_STAT_FX_LOAD_FUZZ			0x72
/* FX DRAW command executed.  */
#define FHARDDOOM_STAT_FX_DRAW				0x73
/* FX block drawn.  */
#define FHARDDOOM_STAT_FX_BLOCK				0x74
/* FX block drawn with CMAP_A enabled.  */
#define FHARDDOOM_STAT_FX_BLOCK_CMAP_A			0x75
/* FX block drawn with CMAP_B enabled.  */
#define FHARDDOOM_STAT_FX_BLOCK_CMAP_B			0x76
/* FX block drawn with FUZZ enabled.  */
#define FHARDDOOM_STAT_FX_BLOCK_FUZZ			0x77
/* SWR command executed.  */
#define FHARDDOOM_STAT_SWR_CMD				0x78
/* SWR DRAW command executed.  */
#define FHARDDOOM_STAT_SWR_DRAW				0x79
/* SWR block drawn.  */
#define FHARDDOOM_STAT_SWR_BLOCK			0x7a
/* SWR block read.  */
#define FHARDDOOM_STAT_SWR_BLOCK_READ			0x7b
/* SWR block drawn with TRANS_EN.  */
#define FHARDDOOM_STAT_SWR_BLOCK_TRANS			0x7c
/* SWR SRDSEM executed.  */
#define FHARDDOOM_STAT_SWR_SRDSEM			0x7d
/* SWR COLSEM executed.  */
#define FHARDDOOM_STAT_SWR_COLSEM			0x7e
/* SWR SPANSEM executed.  */
#define FHARDDOOM_STAT_SWR_SPANSEM			0x7f

/* Section 2.7: SWR — surface write unit.  Gathers blocks from COL or FX,
 * possibly runs them through the TRANSMAP, writes them to memory.  */

#define FHARDDOOM_SWR_STATE				0x0a00
#define FHARDDOOM_SWR_STATE_DRAW_LENGTH_MASK		0x0000ffff
#define FHARDDOOM_SWR_STATE_COL_EN			0x00010000
#define FHARDDOOM_SWR_STATE_TRANS_EN			0x00020000
#define FHARDDOOM_SWR_STATE_SRC_BUF_FULL		0x00040000
#define FHARDDOOM_SWR_STATE_DST_BUF_FULL		0x00080000
#define FHARDDOOM_SWR_STATE_TRANS_POS_MASK		0x07f00000
#define FHARDDOOM_SWR_STATE_TRANS_POS_SHIFT		20
/* If 1, pending SRDSEM, COLSEM, SPANSEM.  */
#define FHARDDOOM_SWR_STATE_SRDSEM			0x10000000
#define FHARDDOOM_SWR_STATE_COLSEM			0x20000000
#define FHARDDOOM_SWR_STATE_SPANSEM			0x40000000
#define FHARDDOOM_SWR_STATE_MASK			0x77fff7ff
#define FHARDDOOM_SWR_TRANSMAP_VA			0x0a04
#define FHARDDOOM_SWR_TRANSMAP_VA_MASK			0x3f3f0000
#define FHARDDOOM_SWR_DST_VA				0x0a08
#define FHARDDOOM_SWR_DST_PITCH				0x0a0c
/* 64-bit */
#define FHARDDOOM_SWR_BLOCK_MASK			0x0a10
/* The three buffers: source data, orignal destination data, post-TRANSMAP
 * data.  If TRANSMAP is not enabled, only SRC_BUF is used.  */
#define FHARDDOOM_SWR_SRC_BUF(i)			(0x0a40 + (i))
#define FHARDDOOM_SWR_DST_BUF(i)			(0x0a80 + (i))
#define FHARDDOOM_SWR_TRANS_BUF(i)			(0x0ac0 + (i))

/* Section 2.8: SRD — surface read unit.  Reads raw surface or colorbuf
 * blocks, sends them to COL or FX.  */

#define FHARDDOOM_SRD_STATE				0x0b00
#define FHARDDOOM_SRD_STATE_READ_LENGTH_MASK		0x0000ffff
/* If 1, reads to COL, otherwise reads to FX.  */
#define FHARDDOOM_SRD_STATE_COL				0x00010000
/* If 1, waiting on SRDSEM.  */
#define FHARDDOOM_SRD_STATE_SRDSEM			0x10000000
#define FHARDDOOM_SRD_STATE_FESEM			0x20000000
#define FHARDDOOM_SRD_STATE_MASK			0x3001ffff
#define FHARDDOOM_SRD_SRC_VA				0x0b04
#define FHARDDOOM_SRD_SRC_PITCH				0x0b08

/* Section 2.9: SPAN — the span reader.  Used to read texture data for
 * the DRAW_SPAN function, and to read source data for the BLIT
 * function.  Sends the texels to the FX unit.
 *
 * The pseudocode for the main function (DRAW) is as follows:
 *
 * for _ in range(DRAW.LENGTH):
 *     u = (USTART >> 16) & ((1 << UVMASK.ULOG) - 1)
 *     v = (VSTART >> 16) & ((1 << UVMASK.VLOG) - 1)
 *     texel_ptr = src_ptr + v * src_pitch + u
 *     block[DRAW.XOFF] = cached_mem_read(USER_PD, texel_ptr)
 *     USTART += USTEP
 *     VSTART += VSTEP
 *     if DRAW.XOFF == 63:
 *         DRAW.XOFF = 0
 *         SPANOUT.send(block)
 *     else:
 *         DRAW.XOFF += 1
 * if DRAW.XOFF != 0:
 *     SPANOUT.send(block)
 */

#define FHARDDOOM_SPAN_STATE				0x0b40
#define FHARDDOOM_SPAN_STATE_DRAW_LENGTH_MASK		0x0000ffff
#define FHARDDOOM_SPAN_STATE_DRAW_XOFF_MASK		0x003f0000
#define FHARDDOOM_SPAN_STATE_DRAW_XOFF_SHIFT		16
/* If 1, waiting on SPANSEM.  */
#define FHARDDOOM_SPAN_STATE_SPANSEM			0x10000000
#define FHARDDOOM_SPAN_STATE_MASK			0x103fffff
#define FHARDDOOM_SPAN_SRC				0x0b44
#define FHARDDOOM_SPAN_SRC_PITCH_MASK			0x003fffc0
#define FHARDDOOM_SPAN_SRC_SLOT_MASK			0x3f000000
#define FHARDDOOM_SPAN_SRC_SLOT_SHIFT			24
#define FHARDDOOM_SPAN_SRC_MASK				0x3f3fffc0
/* The mask of the source uv coords.  */
#define FHARDDOOM_SPAN_UVMASK				0x0b48
#define FHARDDOOM_SPAN_UVMASK_MASK			0x00001f1f
#define FHARDDOOM_SPAN_USTART				0x0b50
#define FHARDDOOM_SPAN_VSTART				0x0b54
#define FHARDDOOM_SPAN_USTEP				0x0b58
#define FHARDDOOM_SPAN_VSTEP				0x0b5c

/* Section 2.10: FX — the effects unit.  Gets raw texture data from the SPAN
 * unit, or draws with a constant color.  Applies colormap for spans, handles
 * solid drawing, does the fuzz effect.  */

#define FHARDDOOM_FX_STATE				0x0b80
#define FHARDDOOM_FX_STATE_DRAW_LENGTH_MASK		0x0000ffff
#define FHARDDOOM_FX_STATE_DRAW_CMAP_A_EN		0x00010000
#define FHARDDOOM_FX_STATE_DRAW_CMAP_B_EN		0x00020000
#define FHARDDOOM_FX_STATE_DRAW_FUZZ_EN			0x00040000
#define FHARDDOOM_FX_STATE_DRAW_FETCH_SRD		0x00100000
#define FHARDDOOM_FX_STATE_DRAW_FETCH_SPAN		0x00200000
#define FHARDDOOM_FX_STATE_DRAW_FETCH_DONE		0x00400000
#define FHARDDOOM_FX_STATE_DRAW_NON_FIRST		0x00800000
#define FHARDDOOM_FX_STATE_DRAW_FUZZ_POS_MASK		0x03000000
#define FHARDDOOM_FX_STATE_DRAW_FUZZ_POS_SHIFT		24
#define FHARDDOOM_FX_STATE_LOAD_ACTIVE			0x04000000
#define FHARDDOOM_FX_STATE_LOAD_MODE_CMAP_A		0x00000000
#define FHARDDOOM_FX_STATE_LOAD_MODE_CMAP_B		0x10000000
#define FHARDDOOM_FX_STATE_LOAD_MODE_BLOCK		0x20000000
#define FHARDDOOM_FX_STATE_LOAD_MODE_FUZZ		0x30000000
#define FHARDDOOM_FX_STATE_LOAD_MODE_MASK		0x30000000
#define FHARDDOOM_FX_STATE_LOAD_CNT_MASK		0xc0000000
#define FHARDDOOM_FX_STATE_LOAD_CNT_SHIFT		30
#define FHARDDOOM_FX_STATE_MASK				0xf7f7ffff
#define FHARDDOOM_FX_SKIP				0x0b84
#define FHARDDOOM_FX_SKIP_BEGIN_MASK			0x0000003f
#define FHARDDOOM_FX_SKIP_END_MASK			0x00003f00
#define FHARDDOOM_FX_SKIP_END_SHIFT			8
#define FHARDDOOM_FX_SKIP_ALWAYS			0x00010000
#define FHARDDOOM_FX_SKIP_MASK				0x00013f3f
#define FHARDDOOM_FX_COL(i)				(0xc00 + (i) * 4)
#define FHARDDOOM_FX_COL_FUZZPOS_MASK			0x0000003f
#define FHARDDOOM_FX_COL_ENABLE				0x00000080
#define FHARDDOOM_FX_COL_MASK				0x000000bf
#define FHARDDOOM_FX_BUF(i)				(0xd00 + (i))
#define FHARDDOOM_FX_BUF_SIZE				0x100
#define FHARDDOOM_FX_CMAP_A(i)				(0xe00 + (i))
#define FHARDDOOM_FX_CMAP_B(i)				(0xf00 + (i))

/* Section 2.11: COL — the column reader.  Used to read texture data for
 * the DRAW_COLUMN function, and to read source data for the WIPE function.
 * Capable of handling 64 columns at once.  Can apply up to two colormaps:
 * colormap A applies to all columns, while colormap B is per-column.
 * The textured and colormapped pixels are sent to SWR, along with the valid
 * pixel mask.  */

#define FHARDDOOM_COL_STATE				0x2000
#define FHARDDOOM_COL_STATE_DRAW_LENGTH_MASK		0x0000ffff
#define FHARDDOOM_COL_STATE_CMAP_A_EN			0x00010000
/* The next column to be textured.  */
#define FHARDDOOM_COL_STATE_XOFF_MASK			0x03f00000
#define FHARDDOOM_COL_STATE_XOFF_SHIFT			20
/* If 1, waiting on COLSEM.  */
#define FHARDDOOM_COL_STATE_COLSEM			0x10000000
/* Set if LOAD_CMAP_A in progresss.  */
#define FHARDDOOM_COL_STATE_LOAD_CMAP_A			0x20000000
/* The current position (in blocks) for LOAD_CMAP_A.  */
#define FHARDDOOM_COL_STATE_CMAP_A_POS_MASK		0xc0000000
#define FHARDDOOM_COL_STATE_CMAP_A_POS_SHIFT		30
#define FHARDDOOM_COL_STATE_MASK			0xf3f1ffff
/* Per-column command state, to be moved to proper column RAM by COL_SETUP.  */
#define FHARDDOOM_COL_STAGE_CMAP_B_VA			0x2004
#define FHARDDOOM_COL_STAGE_SRC_VA			0x2008
#define FHARDDOOM_COL_STAGE_SRC_PITCH			0x200c
#define FHARDDOOM_COL_STAGE_USTART			0x2010
#define FHARDDOOM_COL_STAGE_USTEP			0x2014
/* Actual per-column state.  */
#define FHARDDOOM_COL_COLS_CMAP_B_VA(i)			(0x2100 + (i) * 4)
#define FHARDDOOM_COL_COLS_CMAP_B_VA_MASK		0x3f3fff00
#define FHARDDOOM_COL_COLS_SRC_VA(i)			(0x2200 + (i) * 4)
#define FHARDDOOM_COL_COLS_SRC_PITCH(i)			(0x2300 + (i) * 4)
#define FHARDDOOM_COL_COLS_SRC_PITCH_MASK		0x003fffff
#define FHARDDOOM_COL_COLS_USTART(i)			(0x2400 + (i) * 4)
#define FHARDDOOM_COL_COLS_USTEP(i)			(0x2500 + (i) * 4)
#define FHARDDOOM_COL_COLS_SRC_HEIGHT(i)		(0x2600 + (i) * 4)
#define FHARDDOOM_COL_COLS_SRC_HEIGHT_MASK		0x0000ffff
#define FHARDDOOM_COL_COLS_STATE(i)			(0x2700 + (i) * 4)
#define FHARDDOOM_COL_COLS_STATE_COL_EN			0x00000001
#define FHARDDOOM_COL_COLS_STATE_CMAP_B_EN		0x00000002
#define FHARDDOOM_COL_COLS_STATE_DATA_CMAP_MASK		0x00003f00
#define FHARDDOOM_COL_COLS_STATE_DATA_CMAP_SHIFT	8
#define FHARDDOOM_COL_COLS_STATE_DATA_GET_MASK		0x003f0000
#define FHARDDOOM_COL_COLS_STATE_DATA_GET_SHIFT		16
#define FHARDDOOM_COL_COLS_STATE_DATA_PUT_MASK		0x3f000000
#define FHARDDOOM_COL_COLS_STATE_DATA_PUT_SHIFT		24

#define FHARDDOOM_COL_COLS_STATE_MASK			0x3f3f3f03
/* The CMAP_A data (256 bytes).  */
#define FHARDDOOM_COL_CMAP_A(i)				(0x2800 + (i))
/* The pre-textured data.  64 bytes for each lane.  */
#define FHARDDOOM_COL_DATA(i)				(0x3000 + (i))
#define FHARDDOOM_COL_DATA_SIZE				64

/* Section 2.12: CACHEs.  */

#define FHARDDOOM_CACHE_CLIENT_COL_CMAP_B		0
#define FHARDDOOM_CACHE_CLIENT_COL_SRC			1
#define FHARDDOOM_CACHE_CLIENT_SPAN_SRC			2
#define FHARDDOOM_CACHE_CLIENT_SWR_TRANSMAP		3
#define FHARDDOOM_CACHE_CLIENT_NUM			4
#define FHARDDOOM_CACHE_TAG(i)				(0x8000 + (i) * 4)
#define FHARDDOOM_CACHE_DATA(i)				(0xc000 + (i))
#define FHARDDOOM_CACHE_SIZE				64
#define FHARDDOOM_CACHE_TAG_VA_MASK			0x3f3fffc0
#define FHARDDOOM_CACHE_TAG_VALID			0x00000001
#define FHARDDOOM_CACHE_TAG_MASK			0x3f3fffc1

/* Section 2.13: end.  */

#define FHARDDOOM_BAR_SIZE				0x10000


/* Section 3: Page tables.  */

#define FHARDDOOM_PAGE_SIZE				0x1000
#define FHARDDOOM_PAGE_SHIFT				12
/* Page table entry.  */
#define FHARDDOOM_PTE_PRESENT				0x00000001
#define FHARDDOOM_PTE_PA_MASK				0xfffffff0
#define FHARDDOOM_PTE_PA_SHIFT				8
#define FHARDDOOM_PTE_MASK				0xfffffff1
/* Splits the VA into SLOT + PTI + OFF */
#define FHARDDOOM_VA_SLOT(va)				((va) >> 24 & 0x3f)
#define FHARDDOOM_VA_ADDR(va)				((va) & 0x3fffff)
#define FHARDDOOM_VA_PTI(va)				((va) >> 12 & 0x3ff)
#define FHARDDOOM_VA_OFF(va)				((va) & 0xfff)
/* Makes a VA.  */
#define FHARDDOOM_VA(ptr, slot)				(((ptr) & 0x3fffff) | (slot) << 24)
#define FHARDDOOM_VA_MASK				0x3f3fffff
#define FHARDDOOM_SLOT_MASK				0x0000003f


/* Section 4: The driver commands.  */

/* The command type.  Always in word 0.  */
#define FHARDDOOM_USER_CMD_HEADER_EXTR_TYPE(w)		((w) & 0xf)
#define FHARDDOOM_USER_CMD_TYPE_NOP			0x0
#define FHARDDOOM_USER_CMD_TYPE_FILL_RECT		0x1
#define FHARDDOOM_USER_CMD_TYPE_DRAW_LINE		0x2
#define FHARDDOOM_USER_CMD_TYPE_BLIT			0x3
#define FHARDDOOM_USER_CMD_TYPE_WIPE			0x4
#define FHARDDOOM_USER_CMD_TYPE_DRAW_COLUMNS		0x5
#define FHARDDOOM_USER_CMD_TYPE_DRAW_FUZZ		0x6
#define FHARDDOOM_USER_CMD_TYPE_DRAW_SPANS		0x7
/* These four count as privileged.  */
#define FHARDDOOM_USER_CMD_TYPE_BIND_SLOT		0x8
#define FHARDDOOM_USER_CMD_TYPE_CLEAR_SLOTS		0x9
#define FHARDDOOM_USER_CMD_TYPE_CALL			0xa
#define FHARDDOOM_USER_CMD_TYPE_FENCE			0xb

/* Fill rectangle.  Fills the specified rectangle with a solid color.  */
/* Word 0: command type and fill color.  */
#define FHARDDOOM_USER_FILL_RECT_HEADER(slot_dst, color)	(FHARDDOOM_USER_CMD_TYPE_FILL_RECT | (slot_dst) << 4 | (color) << 24)
#define FHARDDOOM_USER_FILL_RECT_HEADER_EXTR_SLOT_DST(w)	((w) >> 4 & 0x3f)
#define FHARDDOOM_USER_FILL_RECT_HEADER_EXTR_COLOR(w)	((w) >> 24 & 0xff)
/* Word 1: X and Y coords of the left upper corner.  */
#define FHARDDOOM_USER_FILL_RECT_W1(x, y)		((x) | (y) << 16)
#define FHARDDOOM_USER_FILL_RECT_W1_EXTR_X(w)		((w) & 0xffff)
#define FHARDDOOM_USER_FILL_RECT_W1_EXTR_Y(w)		((w) >> 16 & 0xffff)
/* Word 2: width and height of the rectangle.  */
#define FHARDDOOM_USER_FILL_RECT_W2(w, h)		((w) | (h) << 16)
#define FHARDDOOM_USER_FILL_RECT_W2_EXTR_W(w)		((w) & 0xffff)
#define FHARDDOOM_USER_FILL_RECT_W2_EXTR_H(w)		((w) >> 16 & 0xffff)

/* Draw line.  */
#define FHARDDOOM_USER_DRAW_LINE_HEADER(slot_dst, color)	(FHARDDOOM_USER_CMD_TYPE_DRAW_LINE | (slot_dst) << 4 | (color) << 24)
#define FHARDDOOM_USER_DRAW_LINE_HEADER_EXTR_SLOT_DST(w)	((w) >> 4 & 0x3f)
#define FHARDDOOM_USER_DRAW_LINE_HEADER_EXTR_COLOR(w)	((w) >> 24 & 0xff)
/* Word 3: X and Y coords of one line endpoint.  */
#define FHARDDOOM_USER_DRAW_LINE_W1(x, y)		((x) | (y) << 16)
#define FHARDDOOM_USER_DRAW_LINE_W1_EXTR_X(w)		((w) & 0xffff)
#define FHARDDOOM_USER_DRAW_LINE_W1_EXTR_Y(w)		((w) >> 16 & 0xffff)
/* Word 4: X and Y coords of the other line endpoint.  */
#define FHARDDOOM_USER_DRAW_LINE_W2(x, y)		((x) | (y) << 16)
#define FHARDDOOM_USER_DRAW_LINE_W2_EXTR_X(w)		((w) & 0xffff)
#define FHARDDOOM_USER_DRAW_LINE_W2_EXTR_Y(w)		((w) >> 16 & 0xffff)

/* Blit.  */
/* Word 0: command type and source UV mask.  */
#define FHARDDOOM_USER_BLIT_HEADER(slot_dst, slot_src, ulog, vlog)	(FHARDDOOM_USER_CMD_TYPE_BLIT | (slot_dst) << 4 | (slot_src) << 16 | (ulog) << 22 | (vlog) << 27)
#define FHARDDOOM_USER_BLIT_HEADER_EXTR_SLOT_DST(w)	((w) >> 4 & 0x3f)
#define FHARDDOOM_USER_BLIT_HEADER_EXTR_SLOT_SRC(w)	((w) >> 16 & 0x3f)
#define FHARDDOOM_USER_BLIT_HEADER_EXTR_ULOG(w)		((w) >> 22 & 0x1f)
#define FHARDDOOM_USER_BLIT_HEADER_EXTR_VLOG(w)		((w) >> 27 & 0x1f)
/* Word 1: X and Y coords of the destination left upper corner.  */
#define FHARDDOOM_USER_BLIT_W1(x, y)			((x) | (y) << 16)
#define FHARDDOOM_USER_BLIT_W1_EXTR_X(w)		((w) & 0xffff)
#define FHARDDOOM_USER_BLIT_W1_EXTR_Y(w)		((w) >> 16 & 0xffff)
/* Word 2: width and height of the destination rectangle.  */
#define FHARDDOOM_USER_BLIT_W2(w, h)			((w) | (h) << 16)
#define FHARDDOOM_USER_BLIT_W2_EXTR_W(w)		((w) & 0xffff)
#define FHARDDOOM_USER_BLIT_W2_EXTR_H(w)		((w) >> 16 & 0xffff)
/* Word 3: X and Y coords of the source left upper corner.  */
#define FHARDDOOM_USER_BLIT_W3(x, y)			((x) | (y) << 16)
#define FHARDDOOM_USER_BLIT_W3_EXTR_X(w)		((w) & 0xffff)
#define FHARDDOOM_USER_BLIT_W3_EXTR_Y(w)		((w) >> 16 & 0xffff)
/* Word 4: width and height of the source rectangle.  */
#define FHARDDOOM_USER_BLIT_W4(w, h)			((w) | (h) << 16)
#define FHARDDOOM_USER_BLIT_W4_EXTR_W(w)		((w) & 0xffff)
#define FHARDDOOM_USER_BLIT_W4_EXTR_H(w)		((w) >> 16 & 0xffff)

/* Wipe.  */
/* Word 0: just the command type.  */
#define FHARDDOOM_USER_WIPE_HEADER(slot_dst, slot_src_a, slot_src_b)	(FHARDDOOM_USER_CMD_TYPE_WIPE | (slot_dst) << 4 | (slot_src_a) << 16 | (slot_src_b) << 24)
#define FHARDDOOM_USER_WIPE_HEADER_EXTR_SLOT_DST(w)	((w) >> 4 & 0x3f)
#define FHARDDOOM_USER_WIPE_HEADER_EXTR_SLOT_SRC_A(w)	((w) >> 16 & 0x3f)
#define FHARDDOOM_USER_WIPE_HEADER_EXTR_SLOT_SRC_B(w)	((w) >> 24 & 0x3f)
/* Word 1: X and Y coords of the destination left upper corner.  */
#define FHARDDOOM_USER_WIPE_W1(x, y)			((x) | (y) << 16)
#define FHARDDOOM_USER_WIPE_W1_EXTR_X(w)		((w) & 0xffff)
#define FHARDDOOM_USER_WIPE_W1_EXTR_Y(w)		((w) >> 16 & 0xffff)
/* Word 2: width and height of the destination rectangle.  */
#define FHARDDOOM_USER_WIPE_W2(w, h)			((w) | (h) << 16)
#define FHARDDOOM_USER_WIPE_W2_EXTR_W(w)		((w) & 0xffff)
#define FHARDDOOM_USER_WIPE_W2_EXTR_H(w)		((w) >> 16 & 0xffff)
/* The following word is repeated once for every X coordinate.  */
/* Repeat word 0: Y offset of a given column */

/* Draw columns.  */
/* Word 0: command type, enables, number of columns.  */
#define FHARDDOOM_USER_DRAW_COLUMNS_HEADER(slot_dst, cae, cbe, te, nc)	(FHARDDOOM_USER_CMD_TYPE_DRAW_COLUMNS | (slot_dst) << 4 | (cae) << 12 | (cbe) << 13 | (te) << 14 | (nc) << 16)
#define FHARDDOOM_USER_DRAW_COLUMNS_HEADER_EXTR_SLOT_DST(w)	((w) >> 4 & 0x3f)
#define FHARDDOOM_USER_DRAW_COLUMNS_HEADER_EXTR_CMAP_A_EN(w)	((w) >> 12 & 1)
#define FHARDDOOM_USER_DRAW_COLUMNS_HEADER_EXTR_CMAP_B_EN(w)	((w) >> 13 & 1)
#define FHARDDOOM_USER_DRAW_COLUMNS_HEADER_EXTR_TRANS_EN(w)	((w) >> 14 & 1)
#define FHARDDOOM_USER_DRAW_COLUMNS_HEADER_EXTR_NUM_COLS(w)	((w) >> 16 & 0xffff)
/* Word 1 (if colormap A or transmap enabled): colormap A and transmap.  */
#define FHARDDOOM_USER_DRAW_COLUMNS_W1(slot_cmap_a, cmap_a_idx, slot_transmap, transmap_idx)	((slot_cmap_a) | (cmap_a_idx) << 6 | (slot_transmap) << 20 | (transmap_idx) << 26)
#define FHARDDOOM_USER_DRAW_COLUMNS_W1_EXTR_SLOT_CMAP_A(w)	((w) & 0x3f)
#define FHARDDOOM_USER_DRAW_COLUMNS_W1_EXTR_CMAP_A_IDX(w)	((w) >> 6 & 0x3fff)
#define FHARDDOOM_USER_DRAW_COLUMNS_W1_EXTR_SLOT_TRANSMAP(w)	((w) >> 20 & 0x3f)
#define FHARDDOOM_USER_DRAW_COLUMNS_W1_EXTR_TRANSMAP_IDX(w)	((w) >> 26 & 0x3f)

/* The following 5 or 6 words are repeated once for every column.  */
/* Repeat word 0: x, texture height.  */
#define FHARDDOOM_USER_DRAW_COLUMNS_WR0(x, ulog)	((x) | (ulog) << 16)
#define FHARDDOOM_USER_DRAW_COLUMNS_WR0_EXTR_X(w)	((w) & 0xffff)
#define FHARDDOOM_USER_DRAW_COLUMNS_WR0_EXTR_SRC_HEIGHT(w)	((w) >> 16 & 0xffff)
/* Repeat word 1: y0 and y1.  */
#define FHARDDOOM_USER_DRAW_COLUMNS_WR1(y0, y1)		((y0) | (y1) << 16)
#define FHARDDOOM_USER_DRAW_COLUMNS_WR1_EXTR_Y0(w)	((w) & 0xffff)
#define FHARDDOOM_USER_DRAW_COLUMNS_WR1_EXTR_Y1(w)	((w) >> 16 & 0xffff)
/* Repeat word 2: texture pointer.  */
#define FHARDDOOM_USER_DRAW_COLUMNS_WR2(slot_tex, tex_ptr)	((slot_tex) << 24 | (tex_ptr))
#define FHARDDOOM_USER_DRAW_COLUMNS_WR2_EXTR_SLOT_TEX(w)	((w) >> 24 & 0x3f)
#define FHARDDOOM_USER_DRAW_COLUMNS_WR2_EXTR_TEX_PTR(w)		((w) & 0x3fffff)
/* Repeat word 3: ustart.  */
/* Repeat word 4: ustep.  */
/* Repeat word 5 (if enabled in header): colormap B.  */
#define FHARDDOOM_USER_DRAW_COLUMNS_WR5(slot_cmap_b, cmap_b_idx)	((slot_cmap_b) | (cmap_b_idx) << 6)
#define FHARDDOOM_USER_DRAW_COLUMNS_WR5_EXTR_SLOT_CMAP_B(w)	((w) & 0x3f)
#define FHARDDOOM_USER_DRAW_COLUMNS_WR5_EXTR_CMAP_B_IDX(w)	((w) >> 6 & 0x3fff)

/* Draw fuzz.  */
/* Word 0: command type, number of columns.  */
#define FHARDDOOM_USER_DRAW_FUZZ_HEADER(slot_dst, nc)		(FHARDDOOM_USER_CMD_TYPE_DRAW_FUZZ | (slot_dst) << 4 | (nc) << 16)
#define FHARDDOOM_USER_DRAW_FUZZ_HEADER_EXTR_SLOT_DST(w)	((w) >> 4 & 0x3f)
#define FHARDDOOM_USER_DRAW_FUZZ_HEADER_EXTR_NUM_COLS(w)	((w) >> 16 & 0xffff)
/* Word 1: fuzzstart and fuzzend.  */
#define FHARDDOOM_USER_DRAW_FUZZ_W1(fs, fe)		((fs) | (fe) << 16)
#define FHARDDOOM_USER_DRAW_FUZZ_W1_EXTR_FUZZSTART(w)	((w) & 0xffff)
#define FHARDDOOM_USER_DRAW_FUZZ_W1_EXTR_FUZZEND(w)	((w) >> 16 & 0xffff)
/* Word 2: colormap.  */
#define FHARDDOOM_USER_DRAW_FUZZ_W2(slot_cmap, cmap_idx)	((slot_cmap) | (cmap_idx) << 6)
#define FHARDDOOM_USER_DRAW_FUZZ_W2_EXTR_SLOT_CMAP(w)	((w) & 0x3f)
#define FHARDDOOM_USER_DRAW_FUZZ_W2_EXTR_CMAP_IDX(w)	((w) >> 6 & 0x3fff)
/* The following 2 words are repeated once for every column.  */
/* Repeat word 0: x, U mask.  */
#define FHARDDOOM_USER_DRAW_FUZZ_WR0(x, fuzzpos)	((x) | (fuzzpos) << 16)
#define FHARDDOOM_USER_DRAW_FUZZ_WR0_EXTR_X(w)		((w) & 0xffff)
#define FHARDDOOM_USER_DRAW_FUZZ_WR0_EXTR_FUZZPOS(w)	((w) >> 16 & 0x3f)
/* Repeat word 1: y0 and y1.  */
#define FHARDDOOM_USER_DRAW_FUZZ_WR1(y0, y1)		((y0) | (y1) << 16)
#define FHARDDOOM_USER_DRAW_FUZZ_WR1_EXTR_Y0(w)		((w) & 0xffff)
#define FHARDDOOM_USER_DRAW_FUZZ_WR1_EXTR_Y1(w)		((w) >> 16 & 0xffff)

/* Draw spans.  */
/* Word 0: command type, enables, UV mask.  */
#define FHARDDOOM_USER_DRAW_SPANS_HEADER(slot_dst, cae, cbe, te, slot_src, ulog, vlog)	(FHARDDOOM_USER_CMD_TYPE_DRAW_SPANS | (slot_dst) << 4 | (cae) << 12 | (cbe) << 13 | (te) << 14 | (slot_src) << 16 | (ulog) << 22 | (vlog) << 27)
#define FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_SLOT_DST(w)	((w) >> 4 & 0x3f)
#define FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_CMAP_A_EN(w)	((w) >> 12 & 1)
#define FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_CMAP_B_EN(w)	((w) >> 13 & 1)
#define FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_TRANS_EN(w)	((w) >> 14 & 1)
#define FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_SLOT_SRC(w)	((w) >> 16 & 0x3f)
#define FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_ULOG(w)	((w) >> 22 & 0x1f)
#define FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_VLOG(w)	((w) >> 27 & 0x1f)
/* Word 1 (if colormap A or transmap enabled): colormap A and transmap.  */
#define FHARDDOOM_USER_DRAW_SPANS_W1(slot_cmap_a, cmap_a_idx, slot_transmap, transmap_idx)	((slot_cmap_a) | (cmap_a_idx) << 6 | (slot_transmap) << 20 | (transmap_idx) << 26)
#define FHARDDOOM_USER_DRAW_SPANS_W1_EXTR_SLOT_CMAP_A(w)	((w) & 0x3f)
#define FHARDDOOM_USER_DRAW_SPANS_W1_EXTR_CMAP_A_IDX(w)	((w) >> 6 & 0x3fff)
#define FHARDDOOM_USER_DRAW_SPANS_W1_EXTR_SLOT_TRANSMAP(w)	((w) >> 20 & 0x3f)
#define FHARDDOOM_USER_DRAW_SPANS_W1_EXTR_TRANSMAP_IDX(w)	((w) >> 26 & 0x3f)
/* Word 2: y0 and y1.  */
#define FHARDDOOM_USER_DRAW_SPANS_W2(y0, y1)		((y0) | (y1) << 16)
#define FHARDDOOM_USER_DRAW_SPANS_W2_EXTR_Y0(w)		((w) & 0xffff)
#define FHARDDOOM_USER_DRAW_SPANS_W2_EXTR_Y1(w)		((w) >> 16 & 0xffff)
/* The following 5 or 6 words are repeated once for every x coordinate.  */
/* Repeat word 0: x0 and x1.  */
#define FHARDDOOM_USER_DRAW_SPANS_WR0(x0, x1)		((x0) | (x1) << 16)
#define FHARDDOOM_USER_DRAW_SPANS_WR0_EXTR_X0(w)	((w) & 0xffff)
#define FHARDDOOM_USER_DRAW_SPANS_WR0_EXTR_X1(w)	((w) >> 16 & 0xffff)
/* Repeat word 1: ustart.  */
/* Repeat word 2: vstart.  */
/* Repeat word 3: ustep.  */
/* Repeat word 4: vstep.  */
/* Repeat word 5 (if enabled in header): colormap B.  */
#define FHARDDOOM_USER_DRAW_SPANS_WR5(slot_cmap_b, cmap_b_idx)	((slot_cmap_b) | (cmap_b_idx) << 6)
#define FHARDDOOM_USER_DRAW_SPANS_WR5_EXTR_SLOT_CMAP_B(w)	((w) & 0x3f)
#define FHARDDOOM_USER_DRAW_SPANS_WR5_EXTR_CMAP_B_IDX(w)	((w) >> 6 & 0x3fff)

/* Bind slot.  */
/* Word 0: slot, pitch.  */
#define FHARDDOOM_USER_BIND_SLOT_HEADER(slot, pitch)	(FHARDDOOM_USER_CMD_TYPE_BIND_SLOT | (slot) << 4 | (pitch) >> 6 << 10)
#define FHARDDOOM_USER_BIND_SLOT_HEADER_EXTR_SLOT(w)	((w) >> 4 & 0x3f)
#define FHARDDOOM_USER_BIND_SLOT_HEADER_EXTR_PITCH(w)	((w) >> 4 & 0x3fffc0)
/* Word 1: flags, PT address.  */
#define FHARDDOOM_USER_BIND_SLOT_DATA(valid, writable, user, pt_addr)	((valid) | (writable) << 1 | (user) << 2 | (pt_addr >> 12 << 4))
#define FHARDDOOM_USER_BIND_SLOT_DATA_VALID		0x00000001
#define FHARDDOOM_USER_BIND_SLOT_DATA_WRITABLE		0x00000002
#define FHARDDOOM_USER_BIND_SLOT_DATA_USER		0x00000004

/* Clear slots.  */
/* Word 0.  */
#define FHARDDOOM_USER_CLEAR_SLOTS_HEADER		FHARDDOOM_USER_CMD_TYPE_CLEAR_SLOTS
/* Word 1: slot mask, slots 0-31.  */
/* Word 2: slot mask, slots 32-63.  */

/* Call subroutine.  */
/* Word 0: slot, address.  */
#define FHARDDOOM_USER_CALL_HEADER(slot, addr)		(FHARDDOOM_USER_CMD_TYPE_CALL | (slot) << 4 | (addr) >> 2 << 10)
#define FHARDDOOM_USER_CALL_HEADER_EXTR_SLOT(w)		((w) >> 4 & 0x3f)
#define FHARDDOOM_USER_CALL_HEADER_EXTR_ADDR(w)		((w) >> 8 & 0x3ffffc)
/* Word 1: buffer length.  */

/* Fence.  */
#define FHARDDOOM_USER_FENCE_HEADER(val)		(FHARDDOOM_USER_CMD_TYPE_FENCE | (val) << 4)
#define FHARDDOOM_USER_FENCE_HEADER_EXTR_VAL(w)		((w) >> 4 & 0xfffffff)


/* Section 5: Misc definitions.  */

/* The block size used for drawing etc (64 pixels).  */
#define FHARDDOOM_BLOCK_SIZE				0x40
#define FHARDDOOM_BLOCK_MASK				0x3f
#define FHARDDOOM_BLOCK_SHIFT				6
#define FHARDDOOM_BLOCK_PTR_MASK			0x003fffc0
#define FHARDDOOM_BLOCK_VA_MASK				0x3f3fffc0
#define FHARDDOOM_BLOCK_PITCH_MASK			0x003fffc0
#define FHARDDOOM_COORD_MASK				0xffff
#define FHARDDOOM_COLORMAP_SIZE				0x100


/* Section 6: FE core internal memory map.  */

/* Read to get the next command word, as header word (updates CMD_INFO).  */
#define FHARDDOOM_FEMEM_CMD_FETCH_HEADER		0x00000020
/* Read to get the next command word, as argument word (does not update CMD_INFO,
 * makes sure we haven't run out of subroutine).  */
#define FHARDDOOM_FEMEM_CMD_FETCH_ARG			0x00000024
/* Read only alias of CMD_INFO.  */
#define FHARDDOOM_FEMEM_CMD_INFO			0x00000028
/* Pokes the FENCE logic with a given payload.  */
#define FHARDDOOM_FEMEM_CMD_FENCE			0x0000002c
/* Write these three in order to perform a call; when done, next command words
 * will be fetched from the subroutine buffer, until it runs out.
 * LEN needs to be written last in the sequence, as it will immediately
 * trigger fetching the subroutine.  */
#define FHARDDOOM_FEMEM_CMD_CALL_SLOT			0x00000030
#define FHARDDOOM_FEMEM_CMD_CALL_ADDR			0x00000034
#define FHARDDOOM_FEMEM_CMD_CALL_LEN			0x00000038
#define FHARDDOOM_FEMEM_CMD_FLUSH_CACHES		0x0000003c
/* When written, triggers CMD_ERROR and halts the core.
 * Index goes to CMD_ERROR_CODE, data goes to CMD_ERROR_DATA.  */
#define FHARDDOOM_FEMEM_CMD_ERROR(t)			(0x00000040 + (t) * 4)
/* Write to submit a command to a FIFO.  */
#define FHARDDOOM_FEMEM_SRDCMD(t)			(0x00000080 + (t) * 4)
#define FHARDDOOM_FEMEM_SPANCMD(t)			(0x000000c0 + (t) * 4)
#define FHARDDOOM_FEMEM_COLCMD(t)			(0x00000100 + (t) * 4)
#define FHARDDOOM_FEMEM_FXCMD(t)			(0x00000140 + (t) * 4)
#define FHARDDOOM_FEMEM_SWRCMD(t)			(0x00000180 + (t) * 4)
/* Read to wait for a signal from SWR on the FESEM interface.  */
#define FHARDDOOM_FEMEM_FESEM				0x000001c0
/* Write to bind a slot.  Payload is the same as the argument word, with USER
 * and WRITABLE bits ignored.  */
#define FHARDDOOM_FEMEM_MMU_BIND(t)			(0x00000200 + (t) * 4)
/* Write to bump a STATS counter.  */
#define FHARDDOOM_FEMEM_STAT_BUMP(t)			(0x00000300 + (t) * 4)
/* The code RAM — read only from the core.  */
#define FHARDDOOM_FEMEM_CODE_BASE			0x80000000
#define FHARDDOOM_FEMEM_CODE_SIZE			0x10000
/* The data RAM.  */
#define FHARDDOOM_FEMEM_DATA_BASE			0xc0000000
#define FHARDDOOM_FEMEM_DATA_SIZE			0x10000


/* Section 7: Internal commands.  */

/* Section 7.1: SRDCMD — SRD unit internal commands.  */

#define FHARDDOOM_SRDCMD_TYPE_NOP			0x0
/* The slot of the source.  */
#define FHARDDOOM_SRDCMD_TYPE_SRC_SLOT			0x1
/* The virtual base address of the source.  */
#define FHARDDOOM_SRDCMD_TYPE_SRC_PTR			0x2
/* The pitch of the source.  */
#define FHARDDOOM_SRDCMD_TYPE_SRC_PITCH			0x3
/* Reads blocks.  */
#define FHARDDOOM_SRDCMD_TYPE_READ			0x4
/* Wait for a signal on SRDSEM.  */
#define FHARDDOOM_SRDCMD_TYPE_SRDSEM			0x5
/* Send a signal on FESEM.  */
#define FHARDDOOM_SRDCMD_TYPE_FESEM			0x6
#define FHARDDOOM_SRDCMD_DATA_READ(len, col)		((len) | (col) << 16)
#define FHARDDOOM_SRDCMD_DATA_EXTR_READ_LENGTH(cmd)	((cmd) & 0xffff)
/* If set, send blocks to COL, otherwise to FX.  */
#define FHARDDOOM_SRDCMD_DATA_EXTR_READ_COL(cmd)	((cmd) >> 16 & 1)

/* Section 7.2: SPANCMD — SPAN unit internal commands.  */

#define FHARDDOOM_SPANCMD_TYPE_NOP			0x0
/* The slot of the source.  */
#define FHARDDOOM_SPANCMD_TYPE_SRC_SLOT			0x1
/* The pitch of the source.  */
#define FHARDDOOM_SPANCMD_TYPE_SRC_PITCH		0x2
/* The mask of the source uv coords.  */
#define FHARDDOOM_SPANCMD_TYPE_UVMASK			0x3
/* Straight from the command words.  */
#define FHARDDOOM_SPANCMD_TYPE_USTART			0x4
#define FHARDDOOM_SPANCMD_TYPE_VSTART			0x5
#define FHARDDOOM_SPANCMD_TYPE_USTEP			0x6
#define FHARDDOOM_SPANCMD_TYPE_VSTEP			0x7
/* Emits a given number of texels to the FX.  */
#define FHARDDOOM_SPANCMD_TYPE_DRAW			0x8
/* Waits for a signal from SWR on the SPANSEM interface, then flushes cache.  */
#define FHARDDOOM_SPANCMD_TYPE_SPANSEM			0x9

#define FHARDDOOM_SPANCMD_DATA_UVMASK(ulog, vlog)	((ulog) | (vlog) << 8)
#define FHARDDOOM_SPANCMD_DATA_EXTR_UVMASK_ULOG(cmd)	((cmd) & 0x3f)
#define FHARDDOOM_SPANCMD_DATA_EXTR_UVMASK_VLOG(cmd)	((cmd) >> 8 & 0x3f)
#define FHARDDOOM_SPANCMD_DATA_DRAW(len, xoff)		((len) | (xoff) << 16)
/* Number of actual pixels to be drawn.  */
#define FHARDDOOM_SPANCMD_DATA_EXTR_DRAW_LENGTH(cmd)	((cmd) & 0xffff)
/* Number of dummy pixels to be sent at the beginning of the first block.  */
#define FHARDDOOM_SPANCMD_DATA_EXTR_DRAW_XOFF(cmd)	((cmd) >> 16 & 0x3f)

/* Section 7.3: COLCMD — COL unit internal commands.  */

#define FHARDDOOM_COLCMD_TYPE_NOP			0x0
/* Per-column colormap B virtual address (incl. slot at bits 24-29).  */
#define FHARDDOOM_COLCMD_TYPE_COL_CMAP_B_VA		0x1
/* Per-column source virtual address (incl. slot at bits 24-29).  */
#define FHARDDOOM_COLCMD_TYPE_COL_SRC_VA		0x2
/* Per-column source pitch.  */
#define FHARDDOOM_COLCMD_TYPE_COL_SRC_PITCH		0x3
/* Per-column U coord.  */
#define FHARDDOOM_COLCMD_TYPE_COL_USTART		0x4
#define FHARDDOOM_COLCMD_TYPE_COL_USTEP			0x5
/* Sets up a column (use after the above per-column commands).  */
#define FHARDDOOM_COLCMD_TYPE_COL_SETUP			0x6
/* Load colormap A from SRD.  */
#define FHARDDOOM_COLCMD_TYPE_LOAD_CMAP_A		0x7
/* Emits a given number of blocks to the SWR.  */
#define FHARDDOOM_COLCMD_TYPE_DRAW			0x8
/* Waits for a signal from SWR on the COLSEM interface, then flushes cache.  */
#define FHARDDOOM_COLCMD_TYPE_COLSEM			0x9
#define FHARDDOOM_COLCMD_DATA_COL_SETUP(x, col_en, cmap_b_en, height)	((x) | (col_en) << 8 | (cmap_b_en) << 9 | (height) << 16)
/* The column X coord in the block.  */
#define FHARDDOOM_COLCMD_DATA_EXTR_COL_SETUP_X(cmd)	((cmd) & 0x3f)
/* Column enable — if unset, this column will be disabled and skipped in
 * blocks sent to SWR.  */
#define FHARDDOOM_COLCMD_DATA_EXTR_COL_SETUP_COL_EN(cmd)	((cmd) >> 8 & 1)
/* Colormap B enable.  */
#define FHARDDOOM_COLCMD_DATA_EXTR_COL_SETUP_CMAP_B_EN(cmd)	((cmd) >> 9 & 1)
/* U coord mask.  */
#define FHARDDOOM_COLCMD_DATA_EXTR_COL_SETUP_SRC_HEIGHT(cmd)	((cmd) >> 16 & 0xffff)
/* If enabled, U is mapped to y coord within the source (ie. multiplied by
 * source pitch), otherwise x.  */
#define FHARDDOOM_COLCMD_DATA_DRAW(len, cmap_a_en)	((len) | (cmap_a_en) << 16)
/* Number of blocks to be drawn.  */
#define FHARDDOOM_COLCMD_DATA_EXTR_DRAW_LENGTH(cmd)	((cmd) & 0xffff)
/* Colormap A enable.  */
#define FHARDDOOM_COLCMD_DATA_EXTR_DRAW_CMAP_A_EN(cmd)	((cmd) >> 16 & 1)

/* Section 7.4: FXCMD — FX unit internal commands.  */

#define FHARDDOOM_FXCMD_TYPE_NOP			0x0
/* Loads 4 blocks from FXIN as the colormap.  Param is 0 for colormap A, 1 for colormap B.  */
#define FHARDDOOM_FXCMD_TYPE_LOAD_CMAP			0x1
/* Loads 1 block from FXIN to the buffer.  */
#define FHARDDOOM_FXCMD_TYPE_LOAD_BLOCK			0x2
/* Loads 2 blocks from FXIN to the buffer.  */
#define FHARDDOOM_FXCMD_TYPE_LOAD_FUZZ			0x3
/* Fills the whole buffer with a single color.  */
#define FHARDDOOM_FXCMD_TYPE_FILL_COLOR			0x4
/* Sets FUZZ state for a column.  */
#define FHARDDOOM_FXCMD_TYPE_COL_SETUP			0x5
/* Sets how many columns should be masked off at start/end.  */
#define FHARDDOOM_FXCMD_TYPE_SKIP			0x6
/* Draws stuff.  */
#define FHARDDOOM_FXCMD_TYPE_DRAW			0x7
#define FHARDDOOM_FXCMD_DATA_COL_SETUP(x, pos, en)		((x) | (pos) << 8 | (en) << 15)
#define FHARDDOOM_FXCMD_DATA_EXTR_COL_SETUP_X(cmd)	((cmd) & 0x3f)
#define FHARDDOOM_FXCMD_DATA_EXTR_COL_SETUP_FUZZPOS(cmd)	((cmd) >> 8 & 0x3f)
#define FHARDDOOM_FXCMD_DATA_EXTR_COL_SETUP_ENABLE(cmd)	((cmd) >> 15 & 1)
#define FHARDDOOM_FXCMD_DATA_SKIP(b, e, a)		((b) | (e) << 8 | (a) << 16)
#define FHARDDOOM_FXCMD_DATA_EXTR_SKIP_BEGIN(cmd)	((cmd) & 0x3f)
#define FHARDDOOM_FXCMD_DATA_EXTR_SKIP_END(cmd)		((cmd) >> 8 & 0x3f)
/* If true, SKIP will be applied to all blocks; otherwise, BEGIN will
 * only be applied to the first block, and END only to the last block.  */
#define FHARDDOOM_FXCMD_DATA_EXTR_SKIP_ALWAYS(cmd)	((cmd) >> 16 & 1)
#define FHARDDOOM_FXCMD_DATA_DRAW(len, cae, cbe, fe, srd, span)	((len) | (cae) << 16 | (cbe) << 17 | (fe) << 18 | (srd) << 20 | (span) << 21)
/* Number of blocks to be drawn.  */
#define FHARDDOOM_FXCMD_DATA_EXTR_DRAW_LENGTH(cmd)	((cmd) & 0xffff)
/* If true, the pixels sent to SWR will be translated through the colormap.  */
#define FHARDDOOM_FXCMD_DATA_EXTR_DRAW_CMAP_A_EN	((cmd) >> 16 & 1)
#define FHARDDOOM_FXCMD_DATA_EXTR_DRAW_CMAP_B_EN	((cmd) >> 17 & 1)
/* Extra-special DRAW_FUZZ mode.  */
#define FHARDDOOM_FXCMD_DATA_EXTR_DRAW_FUZZ_EN		((cmd) >> 18 & 1)
/* Selects the data source.  If neither is set, just draw whatever is in
 * the buffer.  */
#define FHARDDOOM_FXCMD_DATA_EXTR_DRAW_FETCH_SRD	((cmd) >> 20 & 1)
#define FHARDDOOM_FXCMD_DATA_EXTR_DRAW_FETCH_SPAN	((cmd) >> 21 & 1)

/* Section 7.5: SWRCMD — SWR unit internal commands.  */

#define FHARDDOOM_SWRCMD_TYPE_NOP			0x0
/* The virtual base address of the TRANSMAP.  */
#define FHARDDOOM_SWRCMD_TYPE_TRANSMAP_VA		0x1
/* The virtual base address of the destination.  */
#define FHARDDOOM_SWRCMD_TYPE_DST_SLOT			0x2
/* The virtual base address of the destination.  */
#define FHARDDOOM_SWRCMD_TYPE_DST_PTR			0x3
/* The pitch of the destination.  */
#define FHARDDOOM_SWRCMD_TYPE_DST_PITCH			0x4
/* Draw blocks from COL or FX.  */
#define FHARDDOOM_SWRCMD_TYPE_DRAW			0x5
/* Send a signal on SRDSEM.  */
#define FHARDDOOM_SWRCMD_TYPE_SRDSEM			0x6
/* Send a signal on COLSEM.  */
#define FHARDDOOM_SWRCMD_TYPE_COLSEM			0x7
/* Send a signal on SPANSEM.  */
#define FHARDDOOM_SWRCMD_TYPE_SPANSEM			0x8
#define FHARDDOOM_SWRCMD_DATA_DRAW(len, c_en, t_en)	((len) | (c_en) << 16 | (t_en) << 17)
#define FHARDDOOM_SWRCMD_DATA_EXTR_DRAW_LENGTH(cmd)	((cmd) & 0xffff)
/* If set, draw from COL, otherwise from FX.  */
#define FHARDDOOM_SWRCMD_DATA_EXTR_DRAW_COL_EN(cmd)	((cmd) >> 16 & 1)
/* If set, enable the TRANSMAP.  */
#define FHARDDOOM_SWRCMD_DATA_EXTR_DRAW_TRANS_EN(cmd)	((cmd) >> 17 & 1)

#endif
