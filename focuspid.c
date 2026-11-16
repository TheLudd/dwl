/* Writes the PID of the focused client to $XDG_RUNTIME_DIR/dwl-focused-pid,
 * or 0 when nothing is focused, so a process can tell whether the window it
 * runs in has focus (the Wayland counterpart of comparing $WINDOWID with X's
 * active window). */
void
writefocuspid(Client *c)
{
	static char path[PATH_MAX];
	pid_t pid = 0;
	FILE *f;

	if (!*path) {
		const char *dir = getenv("XDG_RUNTIME_DIR");
		if (!dir)
			return;
		snprintf(path, sizeof(path), "%s/dwl-focused-pid", dir);
	}
	if (c && client_surface(c) && client_surface(c)->resource)
		wl_client_get_credentials(wl_resource_get_client(client_surface(c)->resource),
				&pid, NULL, NULL);
	if (!(f = fopen(path, "w")))
		return;
	fprintf(f, "%d\n", (int)pid);
	fclose(f);
}
