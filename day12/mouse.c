#include "bootpack.h"

#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4


void enable_mouse(struct MOUSE_DEC *mdec)
{
	/* 鼠标有效 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	/* 成功返回ACK(0xfa) */
	mdec->phase = 0; /* 等待0xfa的阶段 */
	return;
}

int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
{
	if (mdec->phase == 0) {
		/* 等待0xfa */
		if (dat == 0xfa) {
			mdec->phase = 1;
		}
		return 0;
	}
	if (mdec->phase == 1) {
		/* 等待鼠标第一字节 */
		if((dat & 0xc8) == 0x08){
			// 如果第一字节正确
			mdec->buf[0] = dat;
			mdec->phase = 2;
		}
		return 0;
	}
	if (mdec->phase == 2) {
		/* 等待鼠标第二字节 */
		mdec->buf[1] = dat;
		mdec->phase = 3;
		return 0;
	}
	if (mdec->phase == 3) {
		/* 等待鼠标第三字节 */
		mdec->buf[2] = dat;
		mdec->phase = 1;
		mdec->btn = mdec->buf[0] & 0x07;
		mdec->x = mdec->buf[1];
		mdec->y = mdec->buf[2];
		if((mdec->buf[0] & 0x10)!=0){
			mdec->x |= 0xffffff00;
		}
		if((mdec->buf[0] & 0x20)!=0){
			mdec->y |= 0xffffff00;
		}
		mdec->y = -mdec->y;//y的符号与画面符号相反
		return 1;
	}
	return -1; /* 出现错误 */
}