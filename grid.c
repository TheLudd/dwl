/* Arranges the tiled clients in a grid, filling columns top to bottom; the
 * last row and column take what the division leaves over. */
static void
grid(Monitor *m)
{
	unsigned int i, n = 0, rows, cols, cw, ch, aw, ah;
	Client *c;

	wl_list_for_each(c, &clients, link)
		if (VISIBLEON(c, m) && !c->isfloating && !c->isfullscreen)
			n++;
	if (n == 0)
		return;

	for (rows = 0; rows <= n / 2; rows++)
		if (rows * rows >= n)
			break;
	cols = (rows - 1) * rows >= n ? rows - 1 : rows;
	ch = m->w.height / rows;
	cw = m->w.width / cols;

	i = 0;
	wl_list_for_each(c, &clients, link) {
		if (!VISIBLEON(c, m) || c->isfloating || c->isfullscreen)
			continue;
		ah = (i + 1) % rows == 0 ? m->w.height - ch * rows : 0;
		aw = i >= rows * (cols - 1) ? m->w.width - cw * cols : 0;
		resize(c, (struct wlr_box){
			.x = m->w.x + (int)((i / rows) * cw),
			.y = m->w.y + (int)((i % rows) * ch),
			.width = (int)(cw + aw),
			.height = (int)(ch + ah),
		}, 0);
		i++;
	}
}
