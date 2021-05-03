#include "fharddoom.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>

__asm__(
	".section start, \"ax\"\n"
	"_start:\n"
	"lui	sp, 0xc0010\n"
	"lui	a0, %hi(_data_image_start)\n"
	"addi	a0, a0, %lo(_data_image_start)\n"
	"lui	a1, %hi(_data_start)\n"
	"addi	a1, a1, %lo(_data_start)\n"
	"lui	a2, %hi(_data_end)\n"
	"addi	a2, a2, %lo(_data_end)\n"
	"beq	a1, a2, .Linit_bss\n"
	".Linit_data_loop:\n"
	"lb	a3, (a0)\n"
	"sb	a3, (a1)\n"
	"addi	a0, a0, 1\n"
	"addi	a1, a1, 1\n"
	"bne	a1, a2, .Linit_data_loop\n"
	".Linit_bss:\n"
	"lui	a2,%hi(_bss_end)\n"
	"addi	a2,a2,%lo(_bss_end)\n"
	"beq	a1, a2, .Linit_bss_done\n"
	".Linit_bss_loop:\n"
	"sb	zero, (a1)\n"
	"addi	a1, a1, 1\n"
	"bne	a1, a2, .Linit_bss_loop\n"
	".Linit_bss_done:\n"
	"j	main\n"
	".text\n"
);

static volatile uint32_t *const CMD_FETCH_HEADER = (void *)FHARDDOOM_FEMEM_CMD_FETCH_HEADER;
static volatile uint32_t *const CMD_FETCH_ARG = (void *)FHARDDOOM_FEMEM_CMD_FETCH_ARG;
static volatile uint32_t *const CMD_INFO = (void *)FHARDDOOM_FEMEM_CMD_INFO;
static volatile uint32_t *const CMD_FENCE = (void *)FHARDDOOM_FEMEM_CMD_FENCE;
static volatile uint32_t *const CMD_CALL_SLOT = (void *)FHARDDOOM_FEMEM_CMD_CALL_SLOT;
static volatile uint32_t *const CMD_CALL_ADDR = (void *)FHARDDOOM_FEMEM_CMD_CALL_ADDR;
static volatile uint32_t *const CMD_CALL_LEN = (void *)FHARDDOOM_FEMEM_CMD_CALL_LEN;
static volatile uint32_t *const CMD_FLUSH_CACHES = (void *)FHARDDOOM_FEMEM_CMD_FLUSH_CACHES;
static volatile uint32_t *const CMD_ERROR = (void *)FHARDDOOM_FEMEM_CMD_ERROR(0);

static volatile uint32_t *const SRDCMD = (void *)FHARDDOOM_FEMEM_SRDCMD(0);
static volatile uint32_t *const SPANCMD = (void *)FHARDDOOM_FEMEM_SPANCMD(0);
static volatile uint32_t *const COLCMD = (void *)FHARDDOOM_FEMEM_COLCMD(0);
static volatile uint32_t *const FXCMD = (void *)FHARDDOOM_FEMEM_FXCMD(0);
static volatile uint32_t *const SWRCMD = (void *)FHARDDOOM_FEMEM_SWRCMD(0);

static volatile uint32_t *const FESEM = (void *)FHARDDOOM_FEMEM_FESEM;

static volatile uint32_t *const MMU_BIND = (void *)FHARDDOOM_FEMEM_MMU_BIND(0);

static volatile uint32_t *const STAT_BUMP = (void *)FHARDDOOM_FEMEM_STAT_BUMP(0);

static noreturn void error(uint32_t code, uint32_t data) {
	CMD_ERROR[code] = data;
	__builtin_trap();
}

static void srdcmd_src_slot(uint32_t val) {
	SRDCMD[FHARDDOOM_SRDCMD_TYPE_SRC_SLOT] = val;
}

static void srdcmd_src_ptr(uint32_t val) {
	SRDCMD[FHARDDOOM_SRDCMD_TYPE_SRC_PTR] = val;
}

static void srdcmd_src_pitch(uint32_t val) {
	SRDCMD[FHARDDOOM_SRDCMD_TYPE_SRC_PITCH] = val;
}

static void srdcmd_read_fx(uint32_t num) {
	SRDCMD[FHARDDOOM_SRDCMD_TYPE_READ] = FHARDDOOM_SRDCMD_DATA_READ(num, false);
}

static void srdcmd_read_col(uint32_t num) {
	SRDCMD[FHARDDOOM_SRDCMD_TYPE_READ] = FHARDDOOM_SRDCMD_DATA_READ(num, true);
}

static void srdsem(void) {
	SWRCMD[FHARDDOOM_SWRCMD_TYPE_SRDSEM] = 0;
	SRDCMD[FHARDDOOM_SRDCMD_TYPE_SRDSEM] = 0;
}

static void spancmd_src_slot(uint32_t val) {
	SPANCMD[FHARDDOOM_SPANCMD_TYPE_SRC_SLOT] = val;
}

static void spancmd_src_pitch(uint32_t val) {
	SPANCMD[FHARDDOOM_SPANCMD_TYPE_SRC_PITCH] = val;
}

static void spancmd_uvmask(uint32_t ulog, uint32_t vlog) {
	SPANCMD[FHARDDOOM_SPANCMD_TYPE_UVMASK] = FHARDDOOM_SPANCMD_DATA_UVMASK(ulog, vlog);
}

static void spancmd_ustart(uint32_t val) {
	SPANCMD[FHARDDOOM_SPANCMD_TYPE_USTART] = val;
}

static void spancmd_ustep(uint32_t val) {
	SPANCMD[FHARDDOOM_SPANCMD_TYPE_USTEP] = val;
}

static void spancmd_vstart(uint32_t val) {
	SPANCMD[FHARDDOOM_SPANCMD_TYPE_VSTART] = val;
}

static void spancmd_vstep(uint32_t val) {
	SPANCMD[FHARDDOOM_SPANCMD_TYPE_VSTEP] = val;
}

static void spancmd_draw(uint32_t num, uint32_t xoff) {
	SPANCMD[FHARDDOOM_SPANCMD_TYPE_DRAW] = FHARDDOOM_SPANCMD_DATA_DRAW(num, xoff);
}

static void spansem(void) {
	SWRCMD[FHARDDOOM_SWRCMD_TYPE_SPANSEM] = 0;
	SPANCMD[FHARDDOOM_SPANCMD_TYPE_SPANSEM] = 0;
}

static void colcmd_col_cmap_b_va(uint32_t val) {
	COLCMD[FHARDDOOM_COLCMD_TYPE_COL_CMAP_B_VA] = val;
}

static void colcmd_col_src_va(uint32_t val) {
	COLCMD[FHARDDOOM_COLCMD_TYPE_COL_SRC_VA] = val;
}

static void colcmd_col_src_pitch(uint32_t val) {
	COLCMD[FHARDDOOM_COLCMD_TYPE_COL_SRC_PITCH] = val;
}

static void colcmd_col_ustart(uint32_t val) {
	COLCMD[FHARDDOOM_COLCMD_TYPE_COL_USTART] = val;
}

static void colcmd_col_ustep(uint32_t val) {
	COLCMD[FHARDDOOM_COLCMD_TYPE_COL_USTEP] = val;
}

static void colcmd_col_enable(uint32_t x, bool cmap_b_en, uint32_t height) {
	COLCMD[FHARDDOOM_COLCMD_TYPE_COL_SETUP] = FHARDDOOM_COLCMD_DATA_COL_SETUP(x, true, cmap_b_en, height);
}

static void colcmd_col_disable(uint32_t x) {
	COLCMD[FHARDDOOM_COLCMD_TYPE_COL_SETUP] = FHARDDOOM_COLCMD_DATA_COL_SETUP(x, false, false, 0);
}

static void colcmd_load_cmap_a(void) {
	COLCMD[FHARDDOOM_COLCMD_TYPE_LOAD_CMAP_A] = 0;
}

static void colcmd_draw(uint32_t num, bool cmap_a_en) {
	COLCMD[FHARDDOOM_COLCMD_TYPE_DRAW] = FHARDDOOM_COLCMD_DATA_DRAW(num, cmap_a_en);
}

static void colsem(void) {
	SWRCMD[FHARDDOOM_SWRCMD_TYPE_COLSEM] = 0;
	COLCMD[FHARDDOOM_COLCMD_TYPE_COLSEM] = 0;
}

static void fxcmd_load_cmap_a(void) {
	FXCMD[FHARDDOOM_FXCMD_TYPE_LOAD_CMAP] = 0;
}

static void fxcmd_load_cmap_b(void) {
	FXCMD[FHARDDOOM_FXCMD_TYPE_LOAD_CMAP] = 1;
}

static void fxcmd_load_block(void) {
	FXCMD[FHARDDOOM_FXCMD_TYPE_LOAD_BLOCK] = 0;
}

static void fxcmd_load_fuzz(void) {
	FXCMD[FHARDDOOM_FXCMD_TYPE_LOAD_FUZZ] = 0;
}

static void fxcmd_fill_color(uint32_t color) {
	FXCMD[FHARDDOOM_FXCMD_TYPE_FILL_COLOR] = color;
}

static void fxcmd_col_enable(uint32_t x, uint32_t fuzzpos) {
	FXCMD[FHARDDOOM_FXCMD_TYPE_COL_SETUP] = FHARDDOOM_FXCMD_DATA_COL_SETUP(x, fuzzpos, true);
}

static void fxcmd_col_disable(uint32_t x) {
	FXCMD[FHARDDOOM_FXCMD_TYPE_COL_SETUP] = FHARDDOOM_FXCMD_DATA_COL_SETUP(x, 0, false);
}

static void fxcmd_skip(uint32_t sb, uint32_t se, bool sa) {
	FXCMD[FHARDDOOM_FXCMD_TYPE_SKIP] = FHARDDOOM_FXCMD_DATA_SKIP(sb, se, sa);
}

static void fxcmd_draw_buf(uint32_t num) {
	FXCMD[FHARDDOOM_FXCMD_TYPE_DRAW] = FHARDDOOM_FXCMD_DATA_DRAW(num, false, false, false, false, false);
}

static void fxcmd_draw_span(uint32_t num, bool cmap_a_en, bool cmap_b_en) {
	FXCMD[FHARDDOOM_FXCMD_TYPE_DRAW] = FHARDDOOM_FXCMD_DATA_DRAW(num, cmap_a_en, cmap_b_en, false, false, true);
}

static void fxcmd_draw_srd(uint32_t num, bool cmap_a_en, bool cmap_b_en) {
	FXCMD[FHARDDOOM_FXCMD_TYPE_DRAW] = FHARDDOOM_FXCMD_DATA_DRAW(num, cmap_a_en, cmap_b_en, false, true, false);
}

static void fxcmd_draw_fuzz(uint32_t num) {
	FXCMD[FHARDDOOM_FXCMD_TYPE_DRAW] = FHARDDOOM_FXCMD_DATA_DRAW(num, false, false, true, true, false);
}

static void swrcmd_transmap_va(uint32_t val) {
	SWRCMD[FHARDDOOM_SWRCMD_TYPE_TRANSMAP_VA] = val;
}

static void swrcmd_dst_slot(uint32_t val) {
	SWRCMD[FHARDDOOM_SWRCMD_TYPE_DST_SLOT] = val;
}

static void swrcmd_dst_ptr(uint32_t val) {
	SWRCMD[FHARDDOOM_SWRCMD_TYPE_DST_PTR] = val;
}

static void swrcmd_dst_pitch(uint32_t val) {
	SWRCMD[FHARDDOOM_SWRCMD_TYPE_DST_PITCH] = val;
}

static void swrcmd_draw_fx(uint32_t num, bool trans_en) {
	SWRCMD[FHARDDOOM_SWRCMD_TYPE_DRAW] = FHARDDOOM_SWRCMD_DATA_DRAW(num, false, trans_en);
}

static void swrcmd_draw_col(uint32_t num, bool trans_en) {
	SWRCMD[FHARDDOOM_SWRCMD_TYPE_DRAW] = FHARDDOOM_SWRCMD_DATA_DRAW(num, true, trans_en);
}

static void fesem(void) {
	SRDCMD[FHARDDOOM_SRDCMD_TYPE_FESEM] = 0;
	*FESEM;
}

static void assert(bool x) {
	if (!x)
		__builtin_trap();
}

static uint32_t slot_pitch[FHARDDOOM_MMU_SLOT_NUM];
static uint32_t slot_data[FHARDDOOM_MMU_SLOT_NUM];

static uint32_t validate_slot_kernel(uint32_t idx) {
	if (!(slot_data[idx] & FHARDDOOM_USER_BIND_SLOT_DATA_VALID))
		error(FHARDDOOM_CMD_ERROR_CODE_INVALID_SLOT, idx);
	return idx;
}

static uint32_t validate_slot(uint32_t idx) {
	if (!(slot_data[idx] & FHARDDOOM_USER_BIND_SLOT_DATA_VALID))
		error(FHARDDOOM_CMD_ERROR_CODE_INVALID_SLOT, idx);
	if (!(slot_data[idx] & FHARDDOOM_USER_BIND_SLOT_DATA_USER))
		error(FHARDDOOM_CMD_ERROR_CODE_KERNEL_SLOT, idx);
	return idx;
}

static uint32_t validate_slot_dst(uint32_t idx) {
	if (!(slot_data[idx] & FHARDDOOM_USER_BIND_SLOT_DATA_VALID))
		error(FHARDDOOM_CMD_ERROR_CODE_INVALID_SLOT, idx);
	if (!(slot_data[idx] & FHARDDOOM_USER_BIND_SLOT_DATA_USER))
		error(FHARDDOOM_CMD_ERROR_CODE_KERNEL_SLOT, idx);
	if (!(slot_data[idx] & FHARDDOOM_USER_BIND_SLOT_DATA_WRITABLE))
		error(FHARDDOOM_CMD_ERROR_CODE_RO_SLOT, idx);
	return idx;
}

static void cmd_fill_rect(uint32_t cmd_header) {
	STAT_BUMP[FHARDDOOM_STAT_FW_FILL_RECT] = 1;
	fxcmd_fill_color(FHARDDOOM_USER_FILL_RECT_HEADER_EXTR_COLOR(cmd_header));
	uint32_t slot_dst = validate_slot_dst(FHARDDOOM_USER_FILL_RECT_HEADER_EXTR_SLOT_DST(cmd_header));
	swrcmd_dst_slot(slot_dst);
	uint32_t w1 = *CMD_FETCH_ARG;
	uint32_t x = FHARDDOOM_USER_FILL_RECT_W1_EXTR_X(w1);
	uint32_t y = FHARDDOOM_USER_FILL_RECT_W1_EXTR_Y(w1);
	uint32_t w2 = *CMD_FETCH_ARG;
	uint32_t w = FHARDDOOM_USER_FILL_RECT_W2_EXTR_W(w2);
	uint32_t h = FHARDDOOM_USER_FILL_RECT_W2_EXTR_H(w2);
	uint32_t dst_pitch = slot_pitch[slot_dst];
	uint32_t dst_ptr = y * dst_pitch + (x & ~FHARDDOOM_BLOCK_MASK);
	x &= FHARDDOOM_BLOCK_MASK;
	uint32_t skip_end = -(w + x) & FHARDDOOM_BLOCK_MASK;
	uint32_t blocks = (w + x + skip_end) >> FHARDDOOM_BLOCK_SHIFT;
	fxcmd_skip(x, skip_end, false);
	swrcmd_dst_pitch(FHARDDOOM_BLOCK_SIZE);
	while (h--) {
		STAT_BUMP[FHARDDOOM_STAT_FW_FILL_RECT_SPAN] = 1;
		fxcmd_draw_buf(blocks);
		swrcmd_dst_ptr(dst_ptr);
		swrcmd_draw_fx(blocks, false);
		dst_ptr += dst_pitch;
	}
}

static void draw_line_horiz_seg(uint32_t dst_ptr, uint32_t xb, uint32_t xe) {
	STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_LINE_HORIZ_SEG] = 1;
	uint32_t skip_begin = xb & FHARDDOOM_BLOCK_MASK;
	uint32_t skip_end = -xe & FHARDDOOM_BLOCK_MASK;
	uint32_t blocks = (skip_begin + skip_end + xe - xb) >> FHARDDOOM_BLOCK_SHIFT;
	fxcmd_skip(skip_begin, skip_end, false);
	fxcmd_draw_buf(blocks);
	swrcmd_dst_ptr(dst_ptr + (xb & ~FHARDDOOM_BLOCK_MASK));
	swrcmd_draw_fx(blocks, false);
}

static void draw_line_vert_seg(uint32_t dst_ptr, uint32_t x, uint32_t num) {
	STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_LINE_VERT_SEG] = 1;
	uint32_t xl = x & FHARDDOOM_BLOCK_MASK;
	fxcmd_skip(xl, FHARDDOOM_BLOCK_MASK - xl, true);
	fxcmd_draw_buf(num);
	swrcmd_dst_ptr(dst_ptr + (x & ~FHARDDOOM_BLOCK_MASK));
	swrcmd_draw_fx(num, false);
}

static void cmd_draw_line(uint32_t cmd_header) {
	fxcmd_fill_color(FHARDDOOM_USER_DRAW_LINE_HEADER_EXTR_COLOR(cmd_header));
	uint32_t slot_dst = validate_slot_dst(FHARDDOOM_USER_DRAW_LINE_HEADER_EXTR_SLOT_DST(cmd_header));
	swrcmd_dst_slot(slot_dst);
	uint32_t dst_pitch = slot_pitch[slot_dst];
	uint32_t w1 = *CMD_FETCH_ARG;
	uint32_t x1 = FHARDDOOM_USER_DRAW_LINE_W1_EXTR_X(w1);
	uint32_t y1 = FHARDDOOM_USER_DRAW_LINE_W1_EXTR_Y(w1);
	uint32_t w2 = *CMD_FETCH_ARG;
	uint32_t x2 = FHARDDOOM_USER_DRAW_LINE_W2_EXTR_X(w2);
	uint32_t y2 = FHARDDOOM_USER_DRAW_LINE_W2_EXTR_Y(w2);
	/* Make sure x1/y1 is the leftmost endpoint.  */
	if (x1 > x2) {
		uint32_t tmp = x1;
		x1 = x2;
		x2 = tmp;
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	uint32_t dx = x2 - x1;
	/* Set dst_ptr vertically to the starting line.  */
	uint32_t dst_ptr = dst_pitch * y1;
	uint32_t dy;
	if (y2 > y1) {
		/* Drawing line downwards.  */
		dy = y2 - y1;
	} else {
		/* Drawing line upwards.  Adjust the pitch.  */
		dy = y1 - y2;
		dst_pitch = -dst_pitch;
	}
	if (dx > dy) {
		/* Mostly-horizontal line.  */
		STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_LINE_HORIZ] = 1;
		swrcmd_dst_pitch(FHARDDOOM_BLOCK_SIZE);
		int32_t delta = 2 * (int32_t)dy - (int32_t)dx;
		uint32_t xlast = x1;
		for (uint32_t x = x1; x <= x2; x++) {
			if (delta > 0) {
				draw_line_horiz_seg(dst_ptr, xlast, x+1);
				dst_ptr += dst_pitch;
				xlast = x+1;
				delta -= 2 * dx;
			}
			delta += 2 * dy;
		}
		if (xlast != x2 + 1)
			draw_line_horiz_seg(dst_ptr, xlast, x2 + 1);
	} else {
		/* Mostly-vertical line.  */
		STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_LINE_VERT] = 1;
		swrcmd_dst_pitch(dst_pitch);
		int32_t delta = 2 * (int32_t)dx - (int32_t)dy;
		uint32_t num = 0;
		uint32_t n = dy + 1;
		while (n--) {
			num++;
			if (delta > 0) {
				draw_line_vert_seg(dst_ptr, x1, num);
				dst_ptr += dst_pitch * num;
				num = 0;
				x1++;
				delta -= 2 * dy;
			}
			delta += 2 * dx;
		}
		if (num)
			draw_line_vert_seg(dst_ptr, x1, num);
	}
}

static void cmd_blit(uint32_t cmd_header) {
	uint32_t slot_dst = validate_slot_dst(FHARDDOOM_USER_BLIT_HEADER_EXTR_SLOT_DST(cmd_header));
	swrcmd_dst_slot(slot_dst);
	uint32_t slot_src = validate_slot(FHARDDOOM_USER_BLIT_HEADER_EXTR_SLOT_SRC(cmd_header));
	uint32_t ulog = FHARDDOOM_USER_BLIT_HEADER_EXTR_ULOG(cmd_header);
	uint32_t vlog = FHARDDOOM_USER_BLIT_HEADER_EXTR_VLOG(cmd_header);
	uint32_t dst_pitch = slot_pitch[slot_dst];
	uint32_t w1 = *CMD_FETCH_ARG;
	uint32_t dst_x = FHARDDOOM_USER_BLIT_W1_EXTR_X(w1);
	uint32_t dst_y = FHARDDOOM_USER_BLIT_W1_EXTR_Y(w1);
	uint32_t w2 = *CMD_FETCH_ARG;
	uint32_t dst_w = FHARDDOOM_USER_BLIT_W2_EXTR_W(w2);
	uint32_t dst_h = FHARDDOOM_USER_BLIT_W2_EXTR_H(w2);
	uint32_t src_pitch = slot_pitch[slot_src];
	uint32_t w3 = *CMD_FETCH_ARG;
	uint32_t src_x = FHARDDOOM_USER_BLIT_W3_EXTR_X(w3);
	uint32_t src_y = FHARDDOOM_USER_BLIT_W3_EXTR_Y(w3);
	uint32_t w4 = *CMD_FETCH_ARG;
	uint32_t src_w = FHARDDOOM_USER_BLIT_W4_EXTR_W(w4);
	uint32_t src_h = FHARDDOOM_USER_BLIT_W4_EXTR_H(w4);
	/* Compute the pointers.  */
	uint32_t dst_ptr = dst_y * dst_pitch + (dst_x & ~FHARDDOOM_BLOCK_MASK);
	dst_x &= FHARDDOOM_BLOCK_MASK;
	swrcmd_dst_pitch(FHARDDOOM_BLOCK_SIZE);
	/* Decide on the path.  */
	if (src_w == dst_w && src_h == dst_h && (src_x & FHARDDOOM_BLOCK_MASK) == dst_x && ulog == 0x10 && vlog == 0x10) {
		/* Simple case, no scaling, no intra-block shift — use SRD.  */
		STAT_BUMP[FHARDDOOM_STAT_FW_BLIT_SIMPLE] = 1;
		uint32_t src_ptr = src_y * src_pitch + (src_x & ~FHARDDOOM_BLOCK_MASK);
		src_x &= FHARDDOOM_BLOCK_MASK;
		/* Finish SWR work.  */
		srdsem();
		/* Prepare the skip.  */
		uint32_t skip_end = -(dst_w + dst_x) & FHARDDOOM_BLOCK_MASK;
		uint32_t blocks = (dst_w + dst_x + skip_end) >> FHARDDOOM_BLOCK_SHIFT;
		fxcmd_skip(dst_x, skip_end, false);
		srdcmd_src_slot(slot_src);
		srdcmd_src_pitch(FHARDDOOM_BLOCK_SIZE);
		while (dst_h--) {
			STAT_BUMP[FHARDDOOM_STAT_FW_BLIT_SIMPLE_SPAN] = 1;
			srdcmd_src_ptr(src_ptr);
			srdcmd_read_fx(blocks);
			fxcmd_draw_srd(blocks, false, false);
			swrcmd_dst_ptr(dst_ptr);
			swrcmd_draw_fx(blocks, false);
			src_ptr += src_pitch;
			dst_ptr += dst_pitch;
		}
	} else if (src_w == dst_w && src_h == dst_h && (src_x & FHARDDOOM_BLOCK_MASK) == dst_x && ulog == 6) {
		/* Background case, no scaling, no intra-block shift, and source is 64×64 repeating — use SRD.  */
		STAT_BUMP[FHARDDOOM_STAT_FW_BLIT_BG] = 1;
		/* Finish SWR work.  */
		srdsem();
		/* Prepare the skip.  */
		uint32_t skip_end = -(dst_w + dst_x) & FHARDDOOM_BLOCK_MASK;
		uint32_t blocks = (dst_w + dst_x + skip_end) >> FHARDDOOM_BLOCK_SHIFT;
		fxcmd_skip(dst_x, skip_end, false);
		srdcmd_src_slot(slot_src);
		srdcmd_src_ptr(src_y * src_pitch);
		srdcmd_src_pitch(src_pitch);
		while (dst_h--) {
			STAT_BUMP[FHARDDOOM_STAT_FW_BLIT_BG_SPAN] = 1;
			srdcmd_read_fx(1);
			fxcmd_load_block();
			fxcmd_draw_buf(blocks);
			swrcmd_dst_ptr(dst_ptr);
			swrcmd_draw_fx(blocks, false);
			dst_ptr += dst_pitch;
			src_y++;
			if (!(src_y & ((1 << vlog) - 1))) {
				src_y -= 1 << vlog;
				srdcmd_src_ptr(src_y * src_pitch);
			}
		}
	} else {
		/* Complex case, use SPAN.  */
		STAT_BUMP[FHARDDOOM_STAT_FW_BLIT_COMPLEX] = 1;
		spancmd_src_slot(slot_src);
		spancmd_src_pitch(src_pitch);
		spancmd_uvmask(ulog, vlog);
		uint32_t ustep = (src_w << 16) / dst_w;
		uint32_t ustart = (src_x << 16);
		uint32_t vstep = (src_h << 16) / dst_h;
		uint32_t vstart = (src_y << 16);
		spancmd_ustep(ustep);
		spancmd_vstep(0);
		/* Finish SWR work.  */
		spansem();
		/* Prepare the skip.  */
		uint32_t skip_end = -(dst_w + dst_x) & FHARDDOOM_BLOCK_MASK;
		uint32_t blocks = (dst_w + dst_x + skip_end) >> FHARDDOOM_BLOCK_SHIFT;
		fxcmd_skip(dst_x, skip_end, false);
		while (dst_h--) {
			STAT_BUMP[FHARDDOOM_STAT_FW_BLIT_COMPLEX_SPAN] = 1;
			spancmd_ustart(ustart);
			spancmd_vstart(vstart);
			spancmd_draw(dst_w, dst_x);
			fxcmd_draw_span(blocks, false, false);
			swrcmd_dst_ptr(dst_ptr);
			swrcmd_draw_fx(blocks, false);
			vstart += vstep;
			dst_ptr += dst_pitch;
		}
	}
}

#define HEAP_MAX 0x400

static uint32_t heap[HEAP_MAX];
static uint32_t heap_size;

static inline uint32_t heap_left() {
	return HEAP_MAX - heap_size;
}
static inline bool heap_empty() {
	return heap_size == 0;
}

static inline uint32_t heap_pidx(uint32_t idx) {
	return (idx - 1) >> 1;
}

static inline uint32_t heap_cidx(uint32_t idx) {
	return (idx << 1) + 1;
}

static void heap_put(uint32_t word) {
	uint32_t idx = heap_size++;
	while (idx && word < heap[heap_pidx(idx)]) {
		heap[idx] = heap[heap_pidx(idx)];
		idx = heap_pidx(idx);
	}
	heap[idx] = word;
}

static uint32_t heap_get() {
	uint32_t res = heap[0];
	uint32_t word = heap[--heap_size];
	uint32_t idx = 0;
	while (heap_cidx(idx) < heap_size) {
		if (heap_cidx(idx) + 1 < heap_size) {
			/* Two children.  */
			uint32_t c0 = heap[heap_cidx(idx)];
			uint32_t c1 = heap[heap_cidx(idx) + 1];
			if (c0 < word && c0 < c1) {
				heap[idx] = c0;
				idx = heap_cidx(idx);
			} else if (c1 < word) {
				heap[idx] = c1;
				idx = heap_cidx(idx) + 1;
			} else {
				break;
			}
		} else {
			/* One child.  */
			if (heap[heap_cidx(idx)] < word) {
				heap[idx] = heap[heap_cidx(idx)];
				idx = heap_cidx(idx);
			} else {
				break;
			}
		}
	}
	heap[idx] = word;
	return res;
}

enum {
	WIPE_OP_START_A = 0,
	WIPE_OP_STOP_A = 1,
	WIPE_OP_START_B = 2,
	WIPE_OP_STOP_B = 3,
};

#define WIPE_OP(op, x, y) ((y) << 12 | (op) << 8 | (x))

static void wipe_flush(uint32_t dst_ptr, uint32_t dst_pitch, uint32_t src_a_ptr, uint32_t src_a_pitch, uint32_t src_b_ptr, uint32_t src_b_pitch, uint32_t slot_src_a, uint32_t slot_src_b) {
	uint32_t active = 0;
	uint32_t ylast = 0;
	STAT_BUMP[FHARDDOOM_STAT_FW_WIPE_BATCH] = 1;
	while (!heap_empty()) {
		uint32_t opw = heap_get();
		uint32_t opy = opw >> 12;
		uint32_t op = opw >> 8 & 3;
		uint32_t opx = opw & 0x3f;
		if (opy != ylast && active) {
			uint32_t num = opy - ylast;
			colcmd_draw(num, false);
			swrcmd_dst_ptr(dst_ptr + ylast * dst_pitch);
			swrcmd_draw_col(num, false);
			STAT_BUMP[FHARDDOOM_STAT_FW_WIPE_SEG] = 1;
		}
		ylast = opy;
		switch (op) {
			case WIPE_OP_START_A:
				colcmd_col_src_va(FHARDDOOM_VA(src_a_ptr + opx, slot_src_a));
				colcmd_col_src_pitch(src_a_pitch);
				colcmd_col_enable(opx, false, 0);
				active++;
				break;
			case WIPE_OP_START_B:
				colcmd_col_src_va(FHARDDOOM_VA(src_b_ptr + opx, slot_src_b));
				colcmd_col_src_pitch(src_b_pitch);
				colcmd_col_enable(opx, false, 0);
				active++;
				break;
			case WIPE_OP_STOP_A:
			case WIPE_OP_STOP_B:
				colcmd_col_disable(opx);
				assert(active != 0);
				active--;
				break;
		}
	}
	assert(active == 0);
}

static void cmd_wipe(uint32_t cmd_header) {
	uint32_t slot_dst = validate_slot_dst(FHARDDOOM_USER_WIPE_HEADER_EXTR_SLOT_DST(cmd_header));
	swrcmd_dst_slot(slot_dst);
	uint32_t slot_src_a = validate_slot(FHARDDOOM_USER_WIPE_HEADER_EXTR_SLOT_SRC_A(cmd_header));
	uint32_t slot_src_b = validate_slot(FHARDDOOM_USER_WIPE_HEADER_EXTR_SLOT_SRC_B(cmd_header));
	STAT_BUMP[FHARDDOOM_STAT_FW_WIPE] = 1;
	uint32_t w1 = *CMD_FETCH_ARG;
	uint32_t x = FHARDDOOM_USER_WIPE_W1_EXTR_X(w1);
	uint32_t y = FHARDDOOM_USER_WIPE_W1_EXTR_Y(w1);
	uint32_t w2 = *CMD_FETCH_ARG;
	uint32_t w = FHARDDOOM_USER_WIPE_W2_EXTR_W(w2);
	uint32_t h = FHARDDOOM_USER_WIPE_W2_EXTR_H(w2);
	uint32_t dst_pitch = slot_pitch[slot_dst];
	uint32_t src_a_pitch = slot_pitch[slot_src_a];
	uint32_t src_b_pitch = slot_pitch[slot_src_b];
	uint32_t dst_ptr = y * dst_pitch + (x & ~FHARDDOOM_BLOCK_MASK);
	uint32_t src_a_ptr = y * src_a_pitch + (x & ~FHARDDOOM_BLOCK_MASK);
	uint32_t src_b_ptr = y * src_b_pitch + (x & ~FHARDDOOM_BLOCK_MASK);
	x &= FHARDDOOM_BLOCK_MASK;
	colcmd_col_ustart(0);
	colcmd_col_ustep(0x10000);
	swrcmd_dst_pitch(dst_pitch);
	colsem();
	while (w--) {
		STAT_BUMP[FHARDDOOM_STAT_FW_WIPE_COL] = 1;
		uint32_t yoff = *CMD_FETCH_ARG & FHARDDOOM_COORD_MASK;
		if (yoff > h)
			yoff = h;
		if (yoff) {
			heap_put(WIPE_OP(WIPE_OP_START_A, x, 0));
			heap_put(WIPE_OP(WIPE_OP_STOP_A, x, yoff));
		}
		if (yoff != h) {
			heap_put(WIPE_OP(WIPE_OP_START_B, x, yoff));
			heap_put(WIPE_OP(WIPE_OP_STOP_B, x, h));
		}
		x++;
		if (x == FHARDDOOM_BLOCK_SIZE) {
			wipe_flush(dst_ptr, dst_pitch, src_a_ptr, src_a_pitch, src_b_ptr, src_b_pitch, slot_src_a, slot_src_b);
			x = 0;
			dst_ptr += FHARDDOOM_BLOCK_SIZE;
			src_a_ptr += FHARDDOOM_BLOCK_SIZE;
			src_b_ptr += FHARDDOOM_BLOCK_SIZE;
		}
	}
	if (!heap_empty())
		wipe_flush(dst_ptr, dst_pitch, src_a_ptr, src_a_pitch, src_b_ptr, src_b_pitch, slot_src_a, slot_src_b);
}

#define DC_MEM_SIZE 0x200

static uint32_t dc_mem_idx = 0;
static uint32_t dc_mem_wr0[DC_MEM_SIZE];
static uint32_t dc_mem_tex_va[DC_MEM_SIZE];
static uint32_t dc_mem_ustart[DC_MEM_SIZE];
static uint32_t dc_mem_ustep[DC_MEM_SIZE];
static uint32_t dc_mem_cmap_b_va[DC_MEM_SIZE];

enum {
	/* Stop operations must be processed before start operations for given Y.  */
	DC_OP_STOP = 0,
	DC_OP_START = 1,
};

#define DC_OP(op, idx, y) ((y) << 12 | (op) << 10 | (idx))

static void draw_columns_flush(uint32_t dst_pitch, bool cmap_a_en, bool cmap_b_en, bool trans_en) {
	if (heap_empty())
		return;
	STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_COLUMNS_BATCH] = 1;
	uint32_t dst_ptr = FHARDDOOM_USER_DRAW_COLUMNS_WR0_EXTR_X(dc_mem_wr0[0]) & ~FHARDDOOM_BLOCK_MASK;
	uint32_t active = 0;
	uint32_t ylast = 0;
	while (!heap_empty()) {
		uint32_t opw = heap_get();
		uint32_t opy = opw >> 12;
		uint32_t op = opw >> 10 & 1;
		uint32_t opi = opw & 0x1ff;
		if (opy != ylast && active) {
			uint32_t num = opy - ylast;
			colcmd_draw(num, cmap_a_en);
			swrcmd_dst_ptr(dst_ptr + ylast * dst_pitch);
			swrcmd_draw_col(num, trans_en);
			STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_COLUMNS_SEG] = 1;
		}
		ylast = opy;
		uint32_t x = FHARDDOOM_USER_DRAW_COLUMNS_WR0_EXTR_X(dc_mem_wr0[opi]) & FHARDDOOM_BLOCK_MASK;
		if (op == DC_OP_START) {
			uint32_t height = FHARDDOOM_USER_DRAW_COLUMNS_WR0_EXTR_SRC_HEIGHT(dc_mem_wr0[opi]);
			colcmd_col_src_va(dc_mem_tex_va[opi]);
			colcmd_col_ustart(dc_mem_ustart[opi]);
			colcmd_col_ustep(dc_mem_ustep[opi]);
			colcmd_col_cmap_b_va(dc_mem_cmap_b_va[opi]);
			colcmd_col_enable(x, cmap_b_en, height);
			active++;
		} else {
			colcmd_col_disable(x);
			assert(active != 0);
			active--;
		}
	}
	assert(active == 0);
	dc_mem_idx = 0;
}

static void cmd_draw_columns(uint32_t cmd_header) {
	STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_COLUMNS] = 1;
	uint32_t slot_dst = validate_slot_dst(FHARDDOOM_USER_DRAW_COLUMNS_HEADER_EXTR_SLOT_DST(cmd_header));
	swrcmd_dst_slot(slot_dst);
	bool cmap_a_en = FHARDDOOM_USER_DRAW_COLUMNS_HEADER_EXTR_CMAP_A_EN(cmd_header);
	bool cmap_b_en = FHARDDOOM_USER_DRAW_COLUMNS_HEADER_EXTR_CMAP_B_EN(cmd_header);
	bool trans_en = FHARDDOOM_USER_DRAW_COLUMNS_HEADER_EXTR_TRANS_EN(cmd_header);
	uint32_t num = FHARDDOOM_USER_DRAW_COLUMNS_HEADER_EXTR_NUM_COLS(cmd_header);
	uint32_t dst_pitch = slot_pitch[slot_dst];
	swrcmd_dst_pitch(dst_pitch);
	if (cmap_a_en || trans_en) {
		uint32_t w1 = *CMD_FETCH_ARG;
		if (cmap_a_en) {
			uint32_t slot_cmap_a = validate_slot(FHARDDOOM_USER_DRAW_COLUMNS_W1_EXTR_SLOT_CMAP_A(w1));
			uint32_t cmap_a_idx = FHARDDOOM_USER_DRAW_COLUMNS_W1_EXTR_CMAP_A_IDX(w1);
			srdcmd_src_slot(slot_cmap_a);
			srdcmd_src_ptr(cmap_a_idx << 8);
			srdcmd_src_pitch(FHARDDOOM_BLOCK_SIZE);
			srdcmd_read_col(4);
			colcmd_load_cmap_a();
		}
		if (trans_en) {
			uint32_t slot_transmap = validate_slot(FHARDDOOM_USER_DRAW_COLUMNS_W1_EXTR_SLOT_TRANSMAP(w1));
			uint32_t transmap_idx = FHARDDOOM_USER_DRAW_COLUMNS_W1_EXTR_TRANSMAP_IDX(w1);
			swrcmd_transmap_va(FHARDDOOM_VA(transmap_idx << 16, slot_transmap));
		}
	}
	uint32_t xlast = 0;
	uint32_t xl = 0, yl = 0;
	colcmd_col_src_pitch(1);
	while (num--) {
		STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_COLUMNS_COL] = 1;
		uint32_t wr0 = *CMD_FETCH_ARG;
		uint32_t x = FHARDDOOM_USER_DRAW_COLUMNS_WR0_EXTR_X(wr0);
		if ((x & ~FHARDDOOM_BLOCK_MASK) != xlast) {
			draw_columns_flush(dst_pitch, cmap_a_en, cmap_b_en, trans_en);
			xlast = x & ~FHARDDOOM_BLOCK_MASK;
		}
		uint32_t wr1 = *CMD_FETCH_ARG;
		uint32_t y0 = FHARDDOOM_USER_DRAW_COLUMNS_WR1_EXTR_Y0(wr1);
		uint32_t y1 = FHARDDOOM_USER_DRAW_COLUMNS_WR1_EXTR_Y1(wr1);
		if (y0 > y1)
			error(FHARDDOOM_CMD_ERROR_CODE_DRAW_COLUMNS_Y_REV, wr1);

		if (x < xl || (x == xl && y0 < yl))
			draw_columns_flush(dst_pitch, cmap_a_en, cmap_b_en, trans_en);

		uint32_t idx = dc_mem_idx++;
		dc_mem_wr0[idx] = wr0;
		uint32_t wr2 = *CMD_FETCH_ARG;
		uint32_t slot_tex = validate_slot(FHARDDOOM_USER_DRAW_COLUMNS_WR2_EXTR_SLOT_TEX(wr2));
		uint32_t tex_ptr = FHARDDOOM_USER_DRAW_COLUMNS_WR2_EXTR_TEX_PTR(wr2);
		dc_mem_tex_va[idx] = FHARDDOOM_VA(tex_ptr, slot_tex);
		dc_mem_ustart[idx] = *CMD_FETCH_ARG;
		dc_mem_ustep[idx] = *CMD_FETCH_ARG;
		if (cmap_b_en) {
			uint32_t wr5 = *CMD_FETCH_ARG;
			uint32_t slot_cmap_b = validate_slot(FHARDDOOM_USER_DRAW_COLUMNS_WR5_EXTR_SLOT_CMAP_B(wr5));
			uint32_t cmap_b_idx = FHARDDOOM_USER_DRAW_COLUMNS_WR5_EXTR_CMAP_B_IDX(wr5);
			dc_mem_cmap_b_va[idx] = FHARDDOOM_VA(cmap_b_idx << 8, slot_cmap_b);
		}

		heap_put(DC_OP(DC_OP_START, idx, y0));
		heap_put(DC_OP(DC_OP_STOP, idx, y1 + 1));

		xl = x;
		yl = y1 + 1;

		if (dc_mem_idx == DC_MEM_SIZE)
			draw_columns_flush(dst_pitch, cmap_a_en, cmap_b_en, trans_en);
	}
	draw_columns_flush(dst_pitch, cmap_a_en, cmap_b_en, trans_en);
}

static void draw_fuzz_flush(uint32_t dst_pitch, uint32_t fuzzstart, uint32_t fuzzend) {
	if (heap_empty())
		return;
	STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_FUZZ_BATCH] = 1;
	uint32_t dst_ptr = FHARDDOOM_USER_DRAW_COLUMNS_WR0_EXTR_X(dc_mem_wr0[0]) & ~FHARDDOOM_BLOCK_MASK;
	uint32_t active = 0;
	uint32_t ylast = 0;
	srdsem();
	while (!heap_empty()) {
		uint32_t opw = heap_get();
		uint32_t opy = opw >> 12;
		uint32_t op = opw >> 10 & 1;
		uint32_t opi = opw & 0x1ff;
		if (opy != ylast && active) {
			uint32_t num = opy - ylast;
			srdcmd_read_fx(num);
			fxcmd_draw_fuzz(num);
			swrcmd_draw_fx(num, false);
			STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_FUZZ_SEG] = 1;
		}
		ylast = opy;
		uint32_t x = FHARDDOOM_USER_DRAW_FUZZ_WR0_EXTR_X(dc_mem_wr0[opi]) & FHARDDOOM_BLOCK_MASK;
		if (op == DC_OP_START) {
			if (!active) {
				if (opy == fuzzstart) {
					srdcmd_src_ptr(dst_ptr + opy * dst_pitch);
					srdcmd_read_fx(1);
					srdcmd_src_ptr(dst_ptr + opy * dst_pitch);
				} else {
					srdcmd_src_ptr(dst_ptr + (opy - 1) * dst_pitch);
					srdcmd_read_fx(1);
				}
				fxcmd_load_fuzz();
				swrcmd_dst_ptr(dst_ptr + opy * dst_pitch);
			}
			active++;
			uint32_t fuzzpos = FHARDDOOM_USER_DRAW_FUZZ_WR0_EXTR_FUZZPOS(dc_mem_wr0[opi]);
			fxcmd_col_enable(x, fuzzpos);
		} else {
			fxcmd_col_disable(x);
			assert(active != 0);
			active--;
			if (!active) {
				if (opy == fuzzend + 1)
					srdcmd_src_ptr(dst_ptr + fuzzend * dst_pitch);
				srdcmd_read_fx(1);
			}
		}
	}
	assert(active == 0);
	dc_mem_idx = 0;
}

static void cmd_draw_fuzz(uint32_t cmd_header) {
	STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_FUZZ] = 1;
	uint32_t slot_dst = validate_slot_dst(FHARDDOOM_USER_DRAW_FUZZ_HEADER_EXTR_SLOT_DST(cmd_header));
	swrcmd_dst_slot(slot_dst);
	uint32_t num = FHARDDOOM_USER_DRAW_FUZZ_HEADER_EXTR_NUM_COLS(cmd_header);
	uint32_t dst_pitch = slot_pitch[slot_dst];
	swrcmd_dst_pitch(dst_pitch);
	uint32_t w1 = *CMD_FETCH_ARG;
	uint32_t fuzzstart = FHARDDOOM_USER_DRAW_FUZZ_W1_EXTR_FUZZSTART(w1);
	uint32_t fuzzend = FHARDDOOM_USER_DRAW_FUZZ_W1_EXTR_FUZZEND(w1);
	uint32_t w2 = *CMD_FETCH_ARG;
	uint32_t slot_cmap = validate_slot(FHARDDOOM_USER_DRAW_FUZZ_W2_EXTR_SLOT_CMAP(w2));
	uint32_t cmap_idx = FHARDDOOM_USER_DRAW_FUZZ_W2_EXTR_CMAP_IDX(w2);
	srdcmd_src_slot(slot_cmap);
	srdcmd_src_ptr(cmap_idx << 8);
	srdcmd_src_pitch(FHARDDOOM_BLOCK_SIZE);
	srdcmd_read_fx(4);
	fxcmd_load_cmap_a();
	fxcmd_skip(0, 0, false);
	srdcmd_src_slot(slot_dst);
	srdcmd_src_pitch(dst_pitch);

	uint32_t xlast = 0;
	uint32_t xl = 0, yl = 0;
	while (num--) {
		STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_FUZZ_COL] = 1;
		uint32_t wr0 = *CMD_FETCH_ARG;
		uint32_t x = FHARDDOOM_USER_DRAW_FUZZ_WR0_EXTR_X(wr0);
		if ((x & ~FHARDDOOM_BLOCK_MASK) != xlast) {
			draw_fuzz_flush(dst_pitch, fuzzstart, fuzzend);
			xlast = x & ~FHARDDOOM_BLOCK_MASK;
		}
		uint32_t wr1 = *CMD_FETCH_ARG;
		uint32_t y0 = FHARDDOOM_USER_DRAW_FUZZ_WR1_EXTR_Y0(wr1);
		uint32_t y1 = FHARDDOOM_USER_DRAW_FUZZ_WR1_EXTR_Y1(wr1);
		if (y0 > y1)
			error(FHARDDOOM_CMD_ERROR_CODE_DRAW_COLUMNS_Y_REV, wr1);

		if (x < xl || (x == xl && y0 < yl))
			draw_fuzz_flush(dst_pitch, fuzzstart, fuzzend);

		uint32_t idx = dc_mem_idx++;
		dc_mem_wr0[idx] = wr0;

		heap_put(DC_OP(DC_OP_START, idx, y0));
		heap_put(DC_OP(DC_OP_STOP, idx, y1 + 1));

		xl = x;
		yl = y1 + 1;

		if (dc_mem_idx == DC_MEM_SIZE)
			draw_fuzz_flush(dst_pitch, fuzzstart, fuzzend);
	}
	draw_fuzz_flush(dst_pitch, fuzzstart, fuzzend);
}

static void cmd_draw_spans(uint32_t cmd_header) {
	STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_SPANS] = 1;
	uint32_t slot_dst = validate_slot_dst(FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_SLOT_DST(cmd_header));
	swrcmd_dst_slot(slot_dst);
	bool cmap_a_en = FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_CMAP_A_EN(cmd_header);
	bool cmap_b_en = FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_CMAP_B_EN(cmd_header);
	bool trans_en = FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_TRANS_EN(cmd_header);
	uint32_t slot_src = validate_slot(FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_SLOT_SRC(cmd_header));
	spancmd_src_slot(slot_src);
	spancmd_src_pitch(slot_pitch[slot_src]);
	uint32_t ulog = FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_ULOG(cmd_header);
	uint32_t vlog = FHARDDOOM_USER_DRAW_SPANS_HEADER_EXTR_VLOG(cmd_header);
	uint32_t dst_pitch = slot_pitch[slot_dst];
	spancmd_uvmask(ulog, vlog);
	if (cmap_a_en || trans_en) {
		uint32_t w1 = *CMD_FETCH_ARG;
		if (cmap_a_en) {
			uint32_t slot_cmap_a = validate_slot(FHARDDOOM_USER_DRAW_SPANS_W1_EXTR_SLOT_CMAP_A(w1));
			uint32_t cmap_a_idx = FHARDDOOM_USER_DRAW_SPANS_W1_EXTR_CMAP_A_IDX(w1);
			srdcmd_src_slot(slot_cmap_a);
			srdcmd_src_ptr(cmap_a_idx << 8);
			srdcmd_src_pitch(FHARDDOOM_BLOCK_SIZE);
			srdcmd_read_fx(4);
			fxcmd_load_cmap_a();
		}
		if (trans_en) {
			uint32_t slot_transmap = validate_slot(FHARDDOOM_USER_DRAW_SPANS_W1_EXTR_SLOT_TRANSMAP(w1));
			uint32_t transmap_idx = FHARDDOOM_USER_DRAW_SPANS_W1_EXTR_TRANSMAP_IDX(w1);
			swrcmd_transmap_va(FHARDDOOM_VA(transmap_idx << 16, slot_transmap));
		}
	}
	uint32_t w2 = *CMD_FETCH_ARG;
	uint32_t y0 = FHARDDOOM_USER_DRAW_SPANS_W2_EXTR_Y0(w2);
	uint32_t y1 = FHARDDOOM_USER_DRAW_SPANS_W2_EXTR_Y1(w2);
	uint32_t dst_ptr = y0 * dst_pitch;
	uint32_t num;
	if (y1 > y0) {
		num = y1 - y0 + 1;
	} else {
		num = y0 - y1 + 1;
		dst_pitch = -dst_pitch;
	}
	swrcmd_dst_pitch(FHARDDOOM_BLOCK_SIZE);
	if (cmap_b_en)
		srdcmd_src_pitch(FHARDDOOM_BLOCK_SIZE);
	/* 1 is unaligned and thus invalid.  */
	uint32_t last_cmap_va = 1;
	while (num--) {
		STAT_BUMP[FHARDDOOM_STAT_FW_DRAW_SPANS_SPAN] = 1;
		uint32_t wr0 = *CMD_FETCH_ARG;
		uint32_t x0 = FHARDDOOM_USER_DRAW_SPANS_WR0_EXTR_X0(wr0);
		uint32_t x1 = FHARDDOOM_USER_DRAW_SPANS_WR0_EXTR_X1(wr0);
		if (x1 < x0)
			error(FHARDDOOM_CMD_ERROR_CODE_DRAW_SPANS_X_REV, wr0);
		spancmd_ustart(*CMD_FETCH_ARG);
		spancmd_vstart(*CMD_FETCH_ARG);
		spancmd_ustep(*CMD_FETCH_ARG);
		spancmd_vstep(*CMD_FETCH_ARG);
		if (cmap_b_en) {
			uint32_t wr5 = *CMD_FETCH_ARG;
			uint32_t slot_cmap_b = validate_slot(FHARDDOOM_USER_DRAW_SPANS_WR5_EXTR_SLOT_CMAP_B(wr5));
			uint32_t cmap_b_idx = FHARDDOOM_USER_DRAW_SPANS_WR5_EXTR_CMAP_B_IDX(wr5);
			uint32_t cmap_va = FHARDDOOM_VA(cmap_b_idx << 8, slot_cmap_b);
			if (cmap_va != last_cmap_va) {
				srdcmd_src_slot(slot_cmap_b);
				srdcmd_src_ptr(cmap_b_idx << 8);
				srdcmd_read_fx(4);
				fxcmd_load_cmap_b();
				last_cmap_va = cmap_va;
			}
		}
		uint32_t off = x0 & ~FHARDDOOM_BLOCK_MASK;
		x0 -= off;
		x1 -= off;
		uint32_t skip_end = ~x1 & FHARDDOOM_BLOCK_MASK;
		uint32_t blocks = (x1 + 1 + skip_end) >> FHARDDOOM_BLOCK_SHIFT;
		spancmd_draw(x1 - x0 + 1, x0);
		fxcmd_skip(x0, skip_end, false);
		fxcmd_draw_span(blocks, cmap_a_en, cmap_b_en);
		swrcmd_dst_ptr(dst_ptr + off);
		swrcmd_draw_fx(blocks, false);
		dst_ptr += dst_pitch;
	}
}

void validate_kernel_cmd(void) {
	uint32_t info = *CMD_INFO;
	if (info & FHARDDOOM_CMD_INFO_SUB)
		error(FHARDDOOM_CMD_ERROR_CODE_PRIV_COMMAND, 0);
}

void cmd_bind_slot(uint32_t cmd_header) {
	uint32_t slot = FHARDDOOM_USER_BIND_SLOT_HEADER_EXTR_SLOT(cmd_header);
	slot_pitch[slot] = FHARDDOOM_USER_BIND_SLOT_HEADER_EXTR_PITCH(cmd_header);
	uint32_t data = *CMD_FETCH_ARG;
	slot_data[slot] = data;
	MMU_BIND[slot] = data;
	if (data & FHARDDOOM_USER_BIND_SLOT_DATA_USER)
		STAT_BUMP[FHARDDOOM_STAT_FW_BIND_SLOT_USER] = 1;
	else
		STAT_BUMP[FHARDDOOM_STAT_FW_BIND_SLOT_KERNEL] = 1;
}

void cmd_clear_slots(uint32_t cmd_header) {
	STAT_BUMP[FHARDDOOM_STAT_FW_CLEAR_SLOTS] = 1;
	uint32_t mask_a = *CMD_FETCH_ARG;
	uint32_t mask_b = *CMD_FETCH_ARG;
	for (uint32_t i = 0; i < 32; i++)
		if (mask_a >> i & 1) {
			MMU_BIND[i] = 0;
			slot_data[i] = 0;
		}
	for (uint32_t i = 32; i < 64; i++)
		if (mask_b >> (i - 32) & 1) {
			MMU_BIND[i] = 0;
			slot_data[i] = 0;
		}
}

void cmd_call(uint32_t cmd_header) {
	STAT_BUMP[FHARDDOOM_STAT_FW_CALL] = 1;
	uint32_t w1 = *CMD_FETCH_ARG;
	*CMD_CALL_SLOT = validate_slot_kernel(FHARDDOOM_USER_CALL_HEADER_EXTR_SLOT(cmd_header));
	*CMD_CALL_ADDR = FHARDDOOM_USER_CALL_HEADER_EXTR_ADDR(cmd_header);
	*CMD_CALL_LEN = w1;
}

void cmd_fence(uint32_t cmd_header) {
	STAT_BUMP[FHARDDOOM_STAT_FW_FENCE] = 1;
	srdsem();
	fesem();
	*CMD_FLUSH_CACHES = 0xf;
	*CMD_FENCE = FHARDDOOM_USER_FENCE_HEADER_EXTR_VAL(cmd_header);
}

noreturn void main() {
	while (1) {
		uint32_t cmd_header = *CMD_FETCH_HEADER;
		STAT_BUMP[FHARDDOOM_STAT_FW_CMD] = 1;
		switch (FHARDDOOM_USER_CMD_HEADER_EXTR_TYPE(cmd_header)) {
			case FHARDDOOM_USER_CMD_TYPE_NOP:
				break;
			case FHARDDOOM_USER_CMD_TYPE_FILL_RECT:
				cmd_fill_rect(cmd_header);
				break;
			case FHARDDOOM_USER_CMD_TYPE_DRAW_LINE:
				cmd_draw_line(cmd_header);
				break;
			case FHARDDOOM_USER_CMD_TYPE_BLIT:
				cmd_blit(cmd_header);
				break;
			case FHARDDOOM_USER_CMD_TYPE_WIPE:
				cmd_wipe(cmd_header);
				break;
			case FHARDDOOM_USER_CMD_TYPE_DRAW_COLUMNS:
				cmd_draw_columns(cmd_header);
				break;
			case FHARDDOOM_USER_CMD_TYPE_DRAW_FUZZ:
				cmd_draw_fuzz(cmd_header);
				break;
			case FHARDDOOM_USER_CMD_TYPE_DRAW_SPANS:
				cmd_draw_spans(cmd_header);
				break;
			case FHARDDOOM_USER_CMD_TYPE_BIND_SLOT:
				validate_kernel_cmd();
				cmd_bind_slot(cmd_header);
				break;
			case FHARDDOOM_USER_CMD_TYPE_CLEAR_SLOTS:
				validate_kernel_cmd();
				cmd_clear_slots(cmd_header);
				break;
			case FHARDDOOM_USER_CMD_TYPE_CALL:
				validate_kernel_cmd();
				cmd_call(cmd_header);
				break;
			case FHARDDOOM_USER_CMD_TYPE_FENCE:
				validate_kernel_cmd();
				cmd_fence(cmd_header);
				break;
			default:
				error(FHARDDOOM_CMD_ERROR_CODE_UNK_COMMAND, 0);
		}
	}
}
