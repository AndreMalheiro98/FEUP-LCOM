#ifndef __I8042_H
#define __I8042_H

//#define BIT(n) (0x01<<(n))

#define KBD_IRQ	1  /**< @brief Keyboard IRQ line */
#define KBD_BIT 4 /* BIT SELECTOR FOR OUR INTERRUPTS */

#define ESC_KEY 0x81

#define DELAY_US	20000
#define KBD_OUT_BUF		0x60
#define KBD_IN_BUF 	0x60
#define KBD_STAT_REG 	0x64

#define	KBD_CMD_ED 0xED
#define BREAKCODE	BIT(7)
#define KB_ACK 0xFA

#define KB_TWO_BYTES_SCANCODE_FIRST_BYTE 0xE0

#define KBD_OBF BIT(0)
#define KBD_IBF BIT(1)
#define KBD_AUX BIT(5)
#define KBD_TO_ERR BIT(6)
#define KBD_PAR_ERR  BIT(7)

#endif /* __I8042_H */
