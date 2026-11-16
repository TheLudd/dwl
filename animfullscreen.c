/* Animates a floating client to fill its monitor and then makes it
 * fullscreen, or animates a fullscreen client back to the geometry it had
 * before. arg->v is the app_id of the client. */
#define ANIM_FRAMES   24
#define ANIM_FRAME_MS 15

static struct {
	Client *c;
	struct wlr_box from, to, small;
	int frame;
	int tofullscreen;
	struct wl_event_source *timer;
} anim;

static Client *
clientbyappid(const char *id)
{
	Client *c;
	wl_list_for_each(c, &clients, link)
		if (!strcmp(client_get_appid(c), id))
			return c;
	return NULL;
}

static int
clientexists(Client *target)
{
	Client *c;
	wl_list_for_each(c, &clients, link)
		if (c == target)
			return 1;
	return 0;
}

/* ease-out cubic between a and b, t in [0, 1] */
static struct wlr_box
easebox(struct wlr_box a, struct wlr_box b, double t)
{
	double u = 1 - t, e = 1 - u * u * u;
	return (struct wlr_box){
		.x = a.x + (int)((b.x - a.x) * e),
		.y = a.y + (int)((b.y - a.y) * e),
		.width = a.width + (int)((b.width - a.width) * e),
		.height = a.height + (int)((b.height - a.height) * e),
	};
}

static int
animframe(void *data)
{
	if (!anim.c || !clientexists(anim.c)) {
		anim.c = NULL;
		return 0;
	}
	anim.frame++;
	resize(anim.c, easebox(anim.from, anim.to, (double)anim.frame / ANIM_FRAMES), 0);
	if (anim.frame < ANIM_FRAMES) {
		wl_event_source_timer_update(anim.timer, ANIM_FRAME_MS);
		return 0;
	}
	if (anim.tofullscreen) {
		setfullscreen(anim.c, 1);
		anim.c->prev = anim.small;
	}
	anim.c = NULL;
	return 0;
}

void
animfullscreen(const Arg *arg)
{
	Client *c = clientbyappid(arg->v);

	if (anim.c || !c || !c->mon || (!c->isfloating && !c->isfullscreen))
		return;
	if (!anim.timer)
		anim.timer = wl_event_loop_add_timer(event_loop, animframe, NULL);

	anim.c = c;
	anim.frame = 0;
	anim.tofullscreen = !c->isfullscreen;
	if (anim.tofullscreen) {
		anim.small = anim.from = c->geom;
		anim.to = c->mon->m;
	} else {
		anim.to = c->prev;
		anim.from = c->mon->m;
		setfullscreen(c, 0);
		resize(c, anim.from, 0);
	}
	wl_event_source_timer_update(anim.timer, ANIM_FRAME_MS);
}
