 /**
 * @file kbc.h
 * KBC
 */
#ifndef __KBC_H
#define __KBC_H
/** @defgroup kbc KBC
 * @{
 *
 * Functions for using the KBC
 */
/**
 * @brief  Writes command       
**/
int (write_command)(uint8_t command);
/**
 * @brief Writes command byte
**/
int (write_command_byte)(uint8_t command_byte);
/**
 * @brief empties out the input buffer 
**/
void (empty_input_buffer)();
/**
 * @brief checks data error
**/
int check_data_error(uint8_t reg_status);
/**
 * @brief Checks status register
**/
int checkStatusRegister(uint8_t *reg_status);
/**
 * @brief Reads keyboard data
**/
int read_keyboard_data(uint8_t *kb_data);
/**
 * @brief  Reads mouse data
**/
int read_mouse_data(uint8_t *mouse_data);
#endif
