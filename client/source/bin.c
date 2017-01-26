#include "../header/bin.h"

int process_command(struct main_struct *m_s)
{
	if(my_strcmp(m_s->cmd, "exit\n")) {
		m_s->cn = SOCKET_ERROR;
	} else if(my_strcmp(m_s->cmd, "cd\n")){
		cd(m_s->cmd);
		sender(m_s->s, " ", 2, &m_s->cn);
	}
	return 0;
}

int exe(struct main_struct *m_s)// execute commands that are unknown to the
{						// shell.
	FILE *cmd_f = _popen(m_s->cmd, "r");	// open the terminal for commands
	char ch;
	char *temp;
	int i, size, done;

	done = 0;
	ch = 1;
	size = 1024;
	if(cmd_f == NULL) {
		m_s->buf = malloc((23+strlen(m_s->cmd)) * sizeof(char));	// create a string of size 23
		sprintf(m_s->buf, "[Err]Command %s : not found!\n", m_s->cmd);	// to tell that <- command not found
	} else {
		i = 0;
		do {
			m_s->buf = malloc(size * sizeof(char));		// alocate a string of size size
			if (size > 1024 && m_s->buf != NULL){			// if its the first loop it will be 1024 so skip those line on first loop
				strcat(m_s->buf, temp);								// save temp into m_s->buf
				free(temp);													// free temp to be realocated after
			}
			temp = malloc(size * sizeof(char));		// allocate temporary (only used if
			if(m_s->buf == NULL || temp == NULL)	// output bigger than 1024)
				return -1;											// check errors
			while(((ch = fgetc(cmd_f)) != EOF) && i <= (size-1)) {
				m_s->buf[i] = ch;						// read from file and save to buf
				i++;												// increment i;
			}		
			if((ch != EOF) && i <= size){		// if reading the output is not at eof
				strcpy(temp, m_s->buf);				// save it to the temp
				size *= 2;										// add more size
				free(m_s->buf);								// free the buffer
			} else {
				m_s->buf[i] = '\0';						// end buff
				done = 1;											
			}
		} while (!done);
	}
	_pclose(cmd_f);							// close terminal
	return 0;
}
