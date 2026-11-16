/* Takes SchemeSel from the row for the current user in usercolors
 * (user-colors.h, from the machine's USERCOLORS file), as dwm does. */
static uint32_t
hexcolor(const char *hex)
{
	return (uint32_t)strtoul(hex + (*hex == '#'), NULL, 16) << 8 | 0xff;
}

void
setusercolors(void)
{
	const struct passwd *pw = getpwuid(getuid());
	const UserColors *uc;
	int i;

	if (!pw)
		return;
	for (uc = usercolors; uc->user; uc++) {
		if (strcmp(uc->user, pw->pw_name))
			continue;
		for (i = 0; i < 3; i++)
			colors[SchemeSel][i] = hexcolor(uc->colors[i]);
		return;
	}
}
