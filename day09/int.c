/* ���荞�݊֌W */

#include "bootpack.h"
#include <stdio.h>

void init_pic(void)
/* PIC�̏����� */
{
	io_out8(PIC0_IMR,  0xff  ); /* 禁止所有中断*/
	io_out8(PIC1_IMR,  0xff  ); /* 禁止所有中断 */

	io_out8(PIC0_ICW1, 0x11  ); /* 边沿触发模式（edge trigger mode） */
	io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7由INT20-27接收 */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1由IRQ2连接 */
	io_out8(PIC0_ICW4, 0x01  ); /* 无缓冲区模式 */

	io_out8(PIC1_ICW1, 0x11  ); /* 边沿触发模式（edge trigger mode） */
	io_out8(PIC1_ICW2, 0x28  ); /* /* IRQ8-15由INT28-2f接收 */
	io_out8(PIC1_ICW3, 2     ); /* PIC1由IRQ2连接 */
	io_out8(PIC1_ICW4, 0x01  ); /* 无缓冲区模式 */

	io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1以外全部禁止*/
	io_out8(PIC1_IMR,  0xff  ); /* 11111111 禁止所有中断 */

	return;
}

#define PORT_KEYDAT		0x0060
struct FIFO8 keyfifo;

void inthandler21(int *esp)
{
	unsigned char data;
	io_out8(PIC0_OCW2, 0x61);	/* IRQ-01��t������PIC�ɒʒm */
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&keyfifo, data);
	return;
}

struct FIFO8 mousefifo;

void inthandler2c(int *esp)
/* 来自PS/2鼠标的中断 */
{
	unsigned char data;
	io_out8(PIC1_OCW2, 0x64);	/* 通知PIC1 IRQ-12的受理已经完成 */
	io_out8(PIC0_OCW2, 0x62);	/* 通知PIC0 IRQ-02的受理已经完成 */
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&mousefifo, data);
	return;
}

void inthandler27(int *esp)
/* PIC0����̕s���S���荞�ݑ΍� */
/* Athlon64X2�@�Ȃǂł̓`�b�v�Z�b�g�̓s���ɂ��PIC�̏��������ɂ��̊��荞�݂�1�x���������� */
/* ���̊��荞�ݏ����֐��́A���̊��荞�݂ɑ΂��ĉ������Ȃ��ł��߂��� */
/* �Ȃ��������Ȃ��Ă����́H
	��  ���̊��荞�݂�PIC���������̓d�C�I�ȃm�C�Y�ɂ���Ĕ����������̂Ȃ̂ŁA
		�܂��߂ɉ����������Ă��K�v���Ȃ��B									*/
{
	io_out8(PIC0_OCW2, 0x67); /* IRQ-07��t������PIC�ɒʒm(7-1�Q��) */
	return;
}