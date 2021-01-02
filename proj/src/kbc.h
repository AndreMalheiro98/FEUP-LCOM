#ifndef __KBC_H
#define __KBC_H

  #include "i8042.h"
  int (write_command)(uint8_t command);
  int (write_command_byte)(uint8_t command_byte);
  void (empty_input_buffer)();
  int check_data_error(uint8_t reg_status);
  int checkStatusRegister(uint8_t *reg_status);
  int read_keyboard_data(uint8_t *kb_data);
  int read_mouse_data(uint8_t *mouse_data);
#endif
