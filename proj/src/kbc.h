#ifndef __KBC_H
#define __KBC_H
/**
 * @brief  
**/
int (write_command)(uint8_t command);
/**
 * @brief  
**/
int (write_command_byte)(uint8_t command_byte);
/**
 * @brief  
**/
void (empty_input_buffer)();
/**
 * @brief  
**/
int check_data_error(uint8_t reg_status);
/**
 * @brief  
**/
int checkStatusRegister(uint8_t *reg_status);
/**
 * @brief  
**/
int read_keyboard_data(uint8_t *kb_data);
/**
 * @brief  
**/
int read_mouse_data(uint8_t *mouse_data);
#endif
