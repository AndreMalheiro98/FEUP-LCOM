#ifndef __KBC_H
#define __KBC_H

  int (kbc_subscribe_interrupts)();
  int (kbc_unsubsribe_interrupts)();
  void (kbc_ih)();
  int (write_command)(uint8_t command);
  int (write_command_byte)(uint8_t command_byte);
  int (read_from_output_buffer)(uint8_t *read_value);
#endif
