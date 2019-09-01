#define CMD_MAX_BUF_SIZE 20
uint8_t cmd_rcv_buffer[CMD_MAX_BUF_SIZE + 1];
uint8_t cmd_rcv_buffer_len = 0;

void cmd_init()
{
}


void cmd_put(Stream& io)
{
	char ch = io.read();

	// end of command
	if (ch < ' ' || ch > 'z')
	{
		if (cmd_rcv_buffer_len)
		{
			cmd_rcv_buffer[cmd_rcv_buffer_len] = 0;
			bool ok = cmd_parse(io);
			cmd_rcv_buffer_len = 0;
			io.println(ok ? F("ok") : F("err"));
		}
		return;
	}

	// new char
	if (cmd_rcv_buffer_len < CMD_MAX_BUF_SIZE)
	{
		cmd_rcv_buffer[cmd_rcv_buffer_len++] = ch;
		return;
	}

	io.println(F("bad"));
}



bool cmd_parse(Stream& io)
{
    int cmd = cmd_rcv_buffer[0];
    int sub = cmd_rcv_buffer[1];

	switch(cmd)
    {
	case 'h':
		return head_cmd(sub, io);
	case 'm':
		return motor_cmd(sub, io);
	case 'i':
		return ir_dist_cmd(sub, io);
	default:
		io.println(F("I don't know cmd("));
		io.print(cmd);
		io.println(F(")"));
		return false;
	}
}


#if 0
    switch(cmd)
    {
      case 's':
        motor_stop();
        break;
      case 'f':
        motor_forward();
        delay(1 * 1000);
        motor_stop();
        break;
      case 'b':
        motor_backward();
        delay(1 * 1000);
        motor_stop();
        break;
      case '1':
        motor_speed(128);
        break;
      case '2':
        motor_speed(192);
        break;
      case '3':
        motor_speed(255);
        break;
      case 'l':
        motor_left();
        break;
      case 'r':
        motor_right();
        break;
      case 'd':
        motor_demo();
        break;
      case 't':
        motor_test();
        break;
      default:
        Serial.print("I don't know (");
        break;
    }
    Serial.println("--done");
  }
#endif
