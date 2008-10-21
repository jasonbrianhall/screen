/* Copyright (c) 2008
 *      Juergen Weigert (jnweiger@immd4.informatik.uni-erlangen.de)
 *      Michael Schroeder (mlschroe@immd4.informatik.uni-erlangen.de)
 *      Micah Cowan (micah@cowan.name)
 *      Sadrul Habib Chowdhury (sadrul@users.sourceforge.net)
 * Copyright (c) 1993-2002, 2003, 2005, 2006, 2007
 *      Juergen Weigert (jnweiger@immd4.informatik.uni-erlangen.de)
 *      Michael Schroeder (mlschroe@immd4.informatik.uni-erlangen.de)
 * Copyright (c) 1987 Oliver Laumann
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file COPYING); if not, see
 * http://www.gnu.org/licenses/, or contact Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 *
 ****************************************************************
 * $Id$ FAU
 */

#ifdef MAPKEYS

#define KMAP_KEYS (T_OCAPS-T_CAPS)
#define KMAP_AKEYS (T_OCAPS-T_CURSOR)

#define KMAP_NOTIMEOUT 0x4000

#define MAXLAY 10

struct kmap_ext
{
  char *str;
  int fl;
  struct action um;
  struct action dm;
  struct action mm;
};

#else

#define KMAP_KEYS 0

#endif

struct win;			/* forward declaration */

#define SLICE_UNKN 0
#define SLICE_VERT (1 << 0)
#define SLICE_HORI (1 << 1)

#define SLICE_THIS (1 << 2)	/* used in equal test */
#define SLICE_GLOBAL (1 << 3)

struct canvas
{
  struct canvas   *c_next;	/* next canvas on display */
  struct display  *c_display;	/* back pointer to display */

  struct canvas   *c_slnext;	/* next canvas in display slice */
  struct canvas   *c_slprev;	/* prev canvas in display slice */
  struct canvas   *c_slperp;	/* perpendicular slice */
  struct canvas   *c_slback;	/* perpendicular slice back pointer */
  int              c_slorient;  /* our slice orientation */
  int              c_slweight;	/* size ratio */

  struct viewport *c_vplist;
  struct layer    *c_layer;	/* layer on this canvas */
  struct canvas   *c_lnext;	/* next canvas that displays layer */
  struct layer     c_blank;	/* bottom layer, always blank */
  int              c_xoff;	/* canvas x offset on display */
  int              c_yoff;	/* canvas y offset on display */
  int              c_xs;
  int              c_xe;
  int              c_ys;
  int              c_ye;
  struct event     c_captev;	/* caption changed event */
};

struct layout
{
  struct layout   *lay_next;
  char            *lay_title;
  int              lay_number;
  struct canvas    lay_canvas;
  struct canvas   *lay_forecv;
  struct canvas   *lay_cvlist;
  int              lay_autosave;
};

struct viewport
{
  struct viewport *v_next;	/* next vp on canvas */
  struct canvas   *v_canvas;	/* back pointer to canvas */
  int              v_xoff;	/* layer x offset on display */
  int              v_yoff;	/* layer y offset on display */
  int              v_xs;	/* vp upper left */
  int              v_xe;	/* vp upper right */
  int              v_ys;	/* vp lower left */
  int              v_ye;	/* vp lower right */
};

struct display
{
  struct display *d_next;	/* linked list */
  struct acluser *d_user;	/* user who owns that display */
  struct canvas d_canvas;	/* our canvas slice */
  struct canvas *d_cvlist;	/* the canvases of this display */
  struct canvas *d_forecv;	/* current input focus */
  struct layout *d_layout;	/* layout we're using */
  void (*d_processinput) __P((char *, int));
  char *d_processinputdata;	/* data for processinput */
  int d_vpxmin, d_vpxmax;	/* min/max used position on display */
  struct win *d_fore;		/* pointer to fore window */
  struct win *d_other;		/* pointer to other window */
  int   d_nonblock;		/* -1 don't block if obufmax reached */
				/* >0: block after nonblock secs */
  char  d_termname[20 + 1];	/* $TERM */
  char	*d_tentry;		/* buffer for tgetstr */
  char	d_tcinited;		/* termcap inited flag */
  int	d_width, d_height;	/* width/height of the screen */
  int	d_defwidth, d_defheight;	/* default width/height of windows */
  int	d_top, d_bot;		/* scrollregion start/end */
  int	d_x, d_y;		/* cursor position */
  struct mchar d_rend;		/* current rendition */
  int   d_col16change;		/* the 16col bits changed in attr */
  char	d_atyp;			/* current attribute types */
#ifdef DW_CHARS
  int   d_mbcs;			/* saved char for multibytes charset */
#endif
#ifdef ENCODINGS
  int   d_encoding;		/* what encoding type the display is */
  int   d_decodestate;		/* state of our decoder */
  int   d_realfont;		/* real font of terminal */
#endif
  int	d_insert;		/* insert mode flag */
  int	d_keypad;		/* application keypad flag */
  int	d_cursorkeys;		/* application cursorkeys flag */
  int	d_revvid;		/* reverse video */
  int	d_curvis;		/* cursor visibility */
  int   d_has_hstatus;		/* display has hardstatus line */
  int	d_hstatus;		/* hardstatus used */
  int	d_lp_missing;		/* last character on bot line missing */
  int   d_mouse;		/* mouse mode */
#ifdef RXVT_OSC
  int   d_xtermosc[4];		/* osc used */
#endif
  struct mchar d_lpchar;	/* missing char */
  struct timeval d_status_time;	/* time of status display */
  int   d_status;		/* is status displayed? */
  char	d_status_bell;		/* is it only a vbell? */
  int	d_status_len;		/* length of status line */
  char *d_status_lastmsg;	/* last displayed message */
  int   d_status_buflen;	/* last message buffer len */
  int	d_status_lastx;		/* position of the cursor */
  int	d_status_lasty;		/*   before status was displayed */
  int   d_status_obuflen;	/* saved obuflen */ 
  int   d_status_obuffree;	/* saved obuffree */ 
  struct event d_statusev;	/* timeout event */
  struct event d_hstatusev;	/* hstatus changed event */
  int	d_kaablamm;		/* display kaablamm msg */
  struct action *d_ESCseen;	/* Was the last char an ESC (^a) */
  int	d_userpid;		/* pid of attacher */
  char	d_usertty[MAXPATHLEN];	/* tty we are attached to */
  int	d_userfd;		/* fd of the tty */
  struct event d_readev;	/* userfd read event */
  struct event d_writeev;	/* userfd write event */
  struct event d_blockedev;	/* blocked timeout */
  struct mode d_OldMode;	/* tty mode when screen was started */
  struct mode d_NewMode;	/* New tty mode */
  int	d_flow;			/* tty's flow control on/off flag*/
  int   d_intrc;		/* current intr when flow is on */
  char *d_obuf;			/* output buffer */
  int   d_obuflen;		/* len of buffer */
  int	d_obufmax;		/* len where we are blocking the pty */
  int	d_obuflenmax;		/* len - max */
  char *d_obufp;		/* pointer in buffer */
  int   d_obuffree;		/* free bytes in buffer */
#ifdef AUTO_NUKE
  int	d_auto_nuke;		/* autonuke flag */
#endif
#ifdef MAPKEYS
  int	d_nseqs;		/* number of valid mappings */
  int	d_aseqs;		/* number of allocated mappings */
  unsigned char  *d_kmaps;	/* keymaps */
  unsigned char *d_seqp;	/* pointer into keymap array */
  int	d_seql;			/* number of parsed chars */
  unsigned char *d_seqh;	/* last hit */
  struct event d_mapev;		/* timeout event */
  int	d_dontmap;		/* do not map next */
  int	d_mapdefault;		/* do map next to default */
#endif
  union	tcu d_tcs[T_N];		/* terminal capabilities */
  char *d_attrtab[NATTR];	/* attrib emulation table */
  char  d_attrtyp[NATTR];	/* attrib group table */
  int   d_hascolor;		/* do we support color */
  short	d_dospeed;		/* baudrate of tty */
#ifdef FONT
  char	d_c0_tab[256];		/* conversion for C0 */
  char ***d_xtable;		/* char translation table */
#endif
  int	d_UPcost, d_DOcost, d_LEcost, d_NDcost;
  int	d_CRcost, d_IMcost, d_EIcost, d_NLcost;
  int   d_printfd;		/* fd for vt100 print sequence */
#ifdef UTMPOK
  slot_t d_loginslot;		/* offset, where utmp_logintty belongs */
  struct utmp d_utmp_logintty;	/* here the original utmp structure is stored */
  int   d_loginttymode;
# ifdef _SEQUENT_
  char	d_loginhost[100+1];
# endif /* _SEQUENT_ */
#endif
  int   d_blocked;
  int   d_blocked_fuzz;
  struct event d_idleev;	/* screen blanker */
#ifdef BLANKER_PRG
  int   d_blankerpid;
  struct event d_blankerev;
#endif
};

#ifdef MULTI
# define DISPLAY(x) display->x
#else
extern struct display TheDisplay;
# define DISPLAY(x) TheDisplay.x
#endif

#define D_user		DISPLAY(d_user)
#define D_username	(DISPLAY(d_user) ? DISPLAY(d_user)->u_name : 0)
#define D_canvas	DISPLAY(d_canvas)
#define D_cvlist	DISPLAY(d_cvlist)
#define D_layout	DISPLAY(d_layout)
#define D_forecv	DISPLAY(d_forecv)
#define D_processinput	DISPLAY(d_processinput)
#define D_processinputdata	DISPLAY(d_processinputdata)
#define D_vpxmin	DISPLAY(d_vpxmin)
#define D_vpxmax	DISPLAY(d_vpxmax)
#define D_fore		DISPLAY(d_fore)
#define D_other		DISPLAY(d_other)
#define D_nonblock      DISPLAY(d_nonblock)
#define D_termname	DISPLAY(d_termname)
#define D_tentry	DISPLAY(d_tentry)
#define D_tcinited	DISPLAY(d_tcinited)
#define D_width		DISPLAY(d_width)
#define D_height	DISPLAY(d_height)
#define D_defwidth	DISPLAY(d_defwidth)
#define D_defheight	DISPLAY(d_defheight)
#define D_top		DISPLAY(d_top)
#define D_bot		DISPLAY(d_bot)
#define D_x		DISPLAY(d_x)
#define D_y		DISPLAY(d_y)
#define D_rend		DISPLAY(d_rend)
#define D_col16change	DISPLAY(d_col16change)
#define D_atyp		DISPLAY(d_atyp)
#define D_mbcs		DISPLAY(d_mbcs)
#define D_encoding	DISPLAY(d_encoding)
#define D_decodestate	DISPLAY(d_decodestate)
#define D_realfont	DISPLAY(d_realfont)
#define D_insert	DISPLAY(d_insert)
#define D_keypad	DISPLAY(d_keypad)
#define D_cursorkeys	DISPLAY(d_cursorkeys)
#define D_revvid	DISPLAY(d_revvid)
#define D_curvis	DISPLAY(d_curvis)
#define D_has_hstatus	DISPLAY(d_has_hstatus)
#define D_hstatus	DISPLAY(d_hstatus)
#define D_lp_missing	DISPLAY(d_lp_missing)
#define D_mouse		DISPLAY(d_mouse)
#define D_xtermosc	DISPLAY(d_xtermosc)
#define D_lpchar	DISPLAY(d_lpchar)
#define D_status	DISPLAY(d_status)
#define D_status_time	DISPLAY(d_status_time)
#define D_status_bell	DISPLAY(d_status_bell)
#define D_status_len	DISPLAY(d_status_len)
#define D_status_lastmsg	DISPLAY(d_status_lastmsg)
#define D_status_buflen	DISPLAY(d_status_buflen)
#define D_status_lastx	DISPLAY(d_status_lastx)
#define D_status_lasty	DISPLAY(d_status_lasty)
#define D_status_obuflen	DISPLAY(d_status_obuflen)
#define D_status_obuffree	DISPLAY(d_status_obuffree)
#define D_statusev	DISPLAY(d_statusev)
#define D_hstatusev	DISPLAY(d_hstatusev)
#define D_kaablamm	DISPLAY(d_kaablamm)
#define D_ESCseen	DISPLAY(d_ESCseen)
#define D_userpid	DISPLAY(d_userpid)
#define D_usertty	DISPLAY(d_usertty)
#define D_userfd	DISPLAY(d_userfd)
#define D_OldMode	DISPLAY(d_OldMode)
#define D_NewMode	DISPLAY(d_NewMode)
#define D_flow		DISPLAY(d_flow)
#define D_intr		DISPLAY(d_intr)
#define D_obuf		DISPLAY(d_obuf)
#define D_obuflen	DISPLAY(d_obuflen)
#define D_obufmax	DISPLAY(d_obufmax)
#define D_obuflenmax	DISPLAY(d_obuflenmax)
#define D_obufp		DISPLAY(d_obufp)
#define D_obuffree	DISPLAY(d_obuffree)
#define D_auto_nuke	DISPLAY(d_auto_nuke)
#define D_nseqs		DISPLAY(d_nseqs)
#define D_aseqs		DISPLAY(d_aseqs)
#define D_seqp		DISPLAY(d_seqp)
#define D_seql		DISPLAY(d_seql)
#define D_seqh		DISPLAY(d_seqh)
#define D_dontmap	DISPLAY(d_dontmap)
#define D_mapdefault	DISPLAY(d_mapdefault)
#define D_kmaps		DISPLAY(d_kmaps)
#define D_tcs		DISPLAY(d_tcs)
#define D_attrtab	DISPLAY(d_attrtab)
#define D_attrtyp	DISPLAY(d_attrtyp)
#define D_hascolor	DISPLAY(d_hascolor)
#define D_dospeed	DISPLAY(d_dospeed)
#define D_c0_tab	DISPLAY(d_c0_tab)
#define D_xtable	DISPLAY(d_xtable)
#define D_UPcost	DISPLAY(d_UPcost)
#define D_DOcost	DISPLAY(d_DOcost)
#define D_LEcost	DISPLAY(d_LEcost)
#define D_NDcost	DISPLAY(d_NDcost)
#define D_CRcost	DISPLAY(d_CRcost)
#define D_IMcost	DISPLAY(d_IMcost)
#define D_EIcost	DISPLAY(d_EIcost)
#define D_NLcost	DISPLAY(d_NLcost)
#define D_printfd	DISPLAY(d_printfd)
#define D_loginslot	DISPLAY(d_loginslot)
#define D_utmp_logintty	DISPLAY(d_utmp_logintty)
#define D_loginttymode	DISPLAY(d_loginttymode)
#define D_loginhost	DISPLAY(d_loginhost)
#define D_readev	DISPLAY(d_readev)
#define D_writeev	DISPLAY(d_writeev)
#define D_blockedev	DISPLAY(d_blockedev)
#define D_mapev		DISPLAY(d_mapev)
#define D_blocked	DISPLAY(d_blocked)
#define D_blocked_fuzz	DISPLAY(d_blocked_fuzz)
#define D_idleev	DISPLAY(d_idleev)
#define D_blankerev	DISPLAY(d_blankerev)
#define D_blankerpid	DISPLAY(d_blankerpid)


#define GRAIN 4096	/* Allocation grain size for output buffer */
#define OBUF_MAX 256	/* default for obuflimit */

#define OUTPUT_BLOCK_SIZE 256  /* Block size of output to tty */

#define AddChar(c)		\
do				\
  {				\
    if (--D_obuffree <= 0)	\
      Resize_obuf();		\
    *D_obufp++ = (c);		\
  }				\
while (0)

#define CV_CALL(cv, cmd)			\
{						\
  struct display *olddisplay = display;		\
  struct layer *oldflayer = flayer;		\
  struct layer *l = cv->c_layer;		\
  struct canvas *cvlist = l->l_cvlist;		\
  struct canvas *cvlnext = cv->c_lnext;		\
  flayer = l;					\
  l->l_cvlist = cv;				\
  cv->c_lnext = 0;				\
  cmd;						\
  flayer = oldflayer;				\
  l->l_cvlist = cvlist;				\
  cv->c_lnext = cvlnext;			\
  display = olddisplay;				\
}

#define STATUS_OFF	0
#define STATUS_ON_WIN	1
#define STATUS_ON_HS	2

#define HSTATUS_IGNORE		0
#define HSTATUS_LASTLINE	1
#define HSTATUS_MESSAGE		2
#define HSTATUS_HS		3
#define HSTATUS_ALWAYS		(1<<2)
