

for (;;) {
	symb = getc(input);

	switch (symb) {
	case '=':
		err = read_arg(&arg1);
		if (err) {
			...
		}

		err = read_op(&op);
		if (err) {
			...
		}

		err = read_arg(&arg2);
		if (err) {
			...
		}
		break;
	default:
		while (symb != ',' && symb != '\n' && symb != EOF) {
			symb = getc(input);
		}
	}

	if (symb == EOF) {
		break;
	}

	switch (symb) {
	case ',':
		++j;
		break;
	case '\n':
		++i;
		j = 0;
		break;
	default:
		// format error
	}
}
