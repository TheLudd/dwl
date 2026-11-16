/* Ctrl+m prefix chord: the next key that is not a modifier runs its binding
 * as if MODKEY were held with it; Escape cancels. */
static int prefixed;

void
prefix(const Arg *arg)
{
	prefixed = 1;
}

static int
ismodifier(xkb_keysym_t sym)
{
	return (sym >= XKB_KEY_Shift_L && sym <= XKB_KEY_Hyper_R)
		|| sym == XKB_KEY_ISO_Level3_Shift || sym == XKB_KEY_Mode_switch;
}

/* Returns 1 when the key press was taken by a pending prefix. */
int
prefixkey(uint32_t mods, const xkb_keysym_t syms[2])
{
	const Key *key;
	int i;

	if (!prefixed || ismodifier(syms[0]))
		return 0;
	prefixed = 0;
	if (syms[0] == XKB_KEY_Escape)
		return 1;
	for (i = 0; i < 2; i++) {
		if ((key = keybinding(mods | MODKEY, syms[i]))) {
			key->func(&key->arg);
			break;
		}
	}
	return 1;
}
