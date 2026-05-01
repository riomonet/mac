# Building 3270-style Fieldmap UIs on the Web

A discussion of how to translate framebuffer / character-grid / 3270 fieldmap thinking into HTML and CSS, without using `<canvas>`, without fighting the cascade, and without giving up the mental model that the screen is a grid you compute into.

---

## The premise

CSS's flow model — block boxes, line-height, margin collapse, the cascade reaching across the document and changing things behind your back — is the part that feels like the browser is making decisions you didn't authorize. None of it is mandatory.

If you strip CSS down to the parts that match a framebuffer mental model, you've basically got two viable styles:

1. A root element with a fixed grid (CSS Grid with explicit `grid-template-columns` / `grid-template-rows`, sized in `ch` if you want literal terminal cells), and children placed by coordinates: `grid-column: 3 / 7; grid-row: 2`. That's an (x, y, w, h) write into a 2D buffer.

2. More imperative: one container with `position: relative`, every child `position: absolute` with explicit `top/left/width/height`. The browser becomes a blitter — you compute coordinates, you place rectangles, done.

The pipeline maps cleanly: state object → render function that produces a DOM tree with explicit positions → diff and flush. Same shape as `compute world → write to backbuffer → swap`.

The cascade is the one CSS feature that fights this style, and you opt out: hard reset, scoped class names (or inline styles), no descendant selectors, no `inherit` you didn't ask for. Each element is a self-contained rectangle with its own complete style.

A minimum example of the discipline:

```html
<div id="screen">
  <div class="cell" style="--x:0; --y:0; --w:20; --h:3;">header</div>
  <div class="cell" style="--x:0; --y:3; --w:20; --h:10;">body</div>
</div>

<style>
  #screen { position: relative; width: 80ch; height: 24em; }
  .cell {
    position: absolute;
    left:  calc(var(--x) * 1ch);
    top:   calc(var(--y) * 1em);
    width: calc(var(--w) * 1ch);
    height:calc(var(--h) * 1em);
  }
</style>
```

That's a framebuffer in HTML. A cell is identified by (x, y, w, h), the grid is a `ch × em` lattice, rendering is "write some divs with these coordinates."

---

## Granularity: char-by-char vs rectangles

You've got three honest options:

**Cell-per-char is real, and it's what xterm.js's DOM renderer does.** One `<span>` per character, absolutely positioned or grid-placed. ~2000 spans for an 80×24 terminal. The DOM handles it fine — browsers are absurdly good at this now — but you do not hand-write 2000 spans. You write a render function: `state → array of {x, y, ch, fg, bg}` → spans. You have routines that draw into the buffer: `drawBox(x, y, w, h)`, `drawText(x, y, str)`, `fill(x, y, w, h, ch)`. Those routines emit spans. You never touch a span directly.

**Rectangle granularity** is the move I'd actually recommend, and it's what terminal-UI libraries (ncurses, blessed, ratatui) effectively do at the layout level. A "widget" is a rectangle. Inside the rectangle, you let the browser render text normally — `white-space: pre`, monospace font, no wrapping. The widget is positioned by (x, y, w, h) in cell units; its *contents* are just a string with `\n`s.

```html
<div class="cell" style="--x:0;--y:0;--w:40;--h:10;">
┌─ Status ──────────────┐
│ CPU: 12%              │
│ MEM: 4.2G / 16G       │
└───────────────────────┘
</div>
```

That's one DOM node, placed like a sprite, contents are a preformatted string you built in code.

**Metaprogramming the CSS** is the third option. CSS variables already cover most of what you'd want — the `--x`, `--y`, `--w`, `--h` pattern pushes the layout math into CSS so your markup just declares coordinates. Beyond that, you stop feeling like a framebuffer.

---

## The 3270 fieldmap model

A 3270 screen is a 24×80 grid where you define an ordered list of fields. Each field has a start position, an attribute byte (protected/unprotected, numeric, intensified, hidden, MDT), and a length that runs until the next field attribute. The screen is the field list.

Map that to the web directly:

```html
<div id="screen">
  <div class="field protected"     style="--r:1;  --c:2;  --w:20;">CUSTOMER INQUIRY</div>
  <div class="field protected"     style="--r:3;  --c:2;  --w:10;">Account:</div>
  <div class="field unprotected"   style="--r:3;  --c:13; --w:8;"  contenteditable></div>
  <div class="field protected"     style="--r:4;  --c:2;  --w:10;">Name:</div>
  <div class="field unprotected"   style="--r:4;  --c:13; --w:30;" contenteditable></div>
  <div class="field protected hi"  style="--r:22; --c:2;  --w:40;">PF3=Exit  PF7=Back  PF8=Fwd</div>
</div>

<style>
  #screen {
    position: relative;
    width: 80ch; height: 24em;
    font-family: 'IBM Plex Mono', monospace;
    background: #000; color: #0a0;
    line-height: 1em;
  }
  .field {
    position: absolute;
    left:   calc((var(--c) - 1) * 1ch);
    top:    calc((var(--r) - 1) * 1em);
    width:  calc(var(--w) * 1ch);
    height: 1em;
    white-space: pre;
    overflow: hidden;
  }
  .protected   { color: #0a0; }            /* green, read-only */
  .unprotected { color: #fff; outline: none; } /* white, input */
  .hi          { color: #ff0; }            /* yellow, intensified */
  .hidden      { color: transparent; }     /* nondisplay */
</style>
```

That's a fieldmap. Each `.field` is a 3270 field: row, column, length, attribute (encoded as classes), and content. `contenteditable` gives you the unprotected behavior almost for free.

The pieces that matter:

- The (row, column, length) triple goes into CSS variables. If you want multi-row fields, add `--h` and you're back to rectangles.
- Attributes become classes. `protected`, `unprotected`, `numeric`, `intensified`, `hidden`, `mdt`. You can compose them. This is the one place CSS earns its keep.
- The field list is your source of truth. Don't hand-write the HTML; write a JS array of field descriptors and a renderer:

```js
const fields = [
  { r: 1, c: 2,  w: 20, attr: 'protected hi', text: 'CUSTOMER INQUIRY' },
  { r: 3, c: 2,  w: 10, attr: 'protected',    text: 'Account:' },
  { r: 3, c: 13, w: 8,  attr: 'unprotected',  name: 'acct' },
];

screen.innerHTML = fields.map(f => `
  <div class="field ${f.attr}"
       style="--r:${f.r};--c:${f.c};--w:${f.w};"
       ${f.attr.includes('unprotected') ? 'contenteditable' : ''}
       data-name="${f.name ?? ''}">${f.text ?? ''}</div>
`).join('');
```

To read the screen back (the AID-key "submit" moment), walk the unprotected fields and collect `[name, textContent]` pairs. Tab order falls out for free if you add `tabindex` to unprotected fields in declaration order. PF keys: a global `keydown` listener that maps F1–F12 to AID codes and triggers your "transaction." You're literally implementing the AID-key dispatch loop.

---

## Server-side rendering

A fieldmap is basically designed to be generated server-side. The server owns a map definition (a list of field descriptors), merges in data, emits the HTML, sends it back. The browser does no logic beyond rendering and capturing input. Closest to the 3270 transaction model.

Python sketch:

```python
CUSTINQ = [
    Field(r=1,  c=2,  w=20, attr='protected hi', text='CUSTOMER INQUIRY'),
    Field(r=3,  c=2,  w=10, attr='protected',    text='Account:'),
    Field(r=3,  c=13, w=8,  attr='unprotected',  name='acct'),
    Field(r=4,  c=2,  w=10, attr='protected',    text='Name:'),
    Field(r=4,  c=13, w=30, attr='unprotected',  name='name'),
    Field(r=22, c=2,  w=40, attr='protected hi', text='PF3=Exit PF7=Back PF8=Fwd'),
]

def render(fields, data=None):
    data = data or {}
    out = ['<form method="post" id="screen">']
    for f in fields:
        text = data.get(f.name, f.text or '')
        editable = 'contenteditable' if 'unprotected' in f.attr else ''
        name = f'name="{f.name}"' if f.name else ''
        out.append(
            f'<div class="field {f.attr}" '
            f'style="--r:{f.r};--c:{f.c};--w:{f.w};" '
            f'{editable} {name}>{text}</div>'
        )
    out.append('</form>')
    return '\n'.join(out)
```

---

## Infinite scrolling — the elephant

The 3270 model is bounded (24×80, fixed). You page through data with PF7/PF8. The web's native model is the opposite: an unbounded vertical document. Three honest choices:

**1. Don't.** Page it like a 3270. PF7/PF8, or "Prev 50 / Next 50" buttons, or `?offset=200`. The screen stays bounded, the server stays in charge, the field model stays intact. For data-entry, ops consoles, admin tools, anything keyboard-driven — this is *better* than infinite scroll, not a compromise.

**2. Bounded viewport, virtualized rows.** Keep your fieldmap fixed-size — say 40 rows visible — but let the *data* be larger than the screen and slide rows through the viewport. The DOM only ever holds the 40 visible rows; as the user moves, you swap their contents. This is what AG Grid, Bloomberg terminal, and every serious data grid does. The library term is "windowing" or "virtualization" (react-window, TanStack Virtual). You can write it yourself in ~100 lines.

**3. Actual infinite scroll.** Page is unbounded, new content appends as you approach the bottom. The cleanest implementation is still windowing under the hood: virtual list grows, DOM only renders the visible window plus a small buffer. An `IntersectionObserver` on a sentinel near the bottom triggers `fetch(nextPage)`.

Options 2 and 3 are the same machinery. A bounded viewport sliding over a buffer of rows. The framebuffer discipline survives.

The thing infinite scroll *actually* breaks is the transactional model. 3270 was pseudo-conversational: each screen was a complete transaction, the server was stateless between them, the URL was the whole intent. Infinite scroll bakes ephemeral session state into the scroll position, which is why deep-linking into an infinite feed is always broken.

---

## On xterm.js (a correction)

xterm.js has multiple renderers:

- **DOM renderer** — one span per cell. The original. Still in the codebase, used as the accessible fallback.
- **Canvas renderer** — was the default for a while. Each cell rendered as a glyph blitted to a 2D canvas.
- **WebGL renderer** — the current default for modern setups. Same idea but GPU-accelerated, with the glyph atlas as a texture.

WebGL is itself a rendering context obtained from a `<canvas>` element, so xterm.js's fast renderers are *both* canvas-based. The DOM renderer is the only non-canvas option, and it's the fallback.

Better examples of pure-DOM cell-per-span rendering:
- xterm.js DOM renderer specifically (still in the codebase).
- Hyperterm/Hyper in early versions.
- Browser-based roguelikes; rot.js has a DOM backend.

---

## Rendering a single title

```html
<!DOCTYPE html>
<html>
<head>
<style>
  #screen {
    position: relative;
    width: 80ch;
    height: 24em;
    font-family: 'Courier New', monospace;
    font-size: 16px;
    line-height: 1em;
    background: #000;
    color: #0a0;
  }
  .field {
    position: absolute;
    left:   calc((var(--c) - 1) * 1ch);
    top:    calc((var(--r) - 1) * 1em);
    width:  calc(var(--w) * 1ch);
    height: 1em;
    white-space: pre;
    overflow: hidden;
  }
</style>
</head>
<body>
  <div id="screen">
    <div class="field" style="--r:5; --c:25; --w:17;">Hello and Welcome</div>
  </div>
</body>
</html>
```

`#screen` is your terminal. `width: 80ch` is 80 character-widths wide; `height: 24em` is 24 line-heights tall. Because the font is monospace, `1ch` is exactly one character, so the screen is a true 80×24 cell grid. `position: relative` makes it the coordinate origin.

The `.field` class converts `--r/--c/--w` into pixel positions: `left = (col - 1) * 1ch`, `top = (row - 1) * 1em`. The `- 1` is because 3270 coordinates are 1-indexed. `white-space: pre` stops space collapsing and wrapping. `overflow: hidden` clips overflow.

---

## All UI elements use this same pattern

**Static label:**
```html
<div class="field" style="--r:5; --c:25; --w:17;">Hello and Welcome</div>
```

**Input field** — real `<input>`, positioned identically:
```html
<input class="field" style="--r:7; --c:13; --w:30;" name="customer">
```

```css
input.field {
  border: none; background: transparent; color: #fff;
  font: inherit; padding: 0; outline: none;
}
input.field:focus { background: #003; }
```

**Button:**
```html
<button class="field" style="--r:22; --c:2; --w:10;">[ OK ]</button>
```

**Multi-line block** — add `--h`:
```css
.field { height: calc(var(--h, 1) * 1em); }
```

```html
<div class="field" style="--r:10; --c:5; --w:40; --h:5; white-space:pre;">
+--------------------------------------+
|  Account 12345 — John Smith          |
|  Balance: $4,231.07                  |
|  Status: ACTIVE                      |
+--------------------------------------+
</div>
```

**Table / list** — one multi-row field with preformatted text:
```html
<div class="field" style="--r:8; --c:2; --w:50; --h:6; white-space:pre;">
ID    NAME              BALANCE
----  ----------------  ----------
0001  John Smith        $4,231.07
0002  Jane Doe          $  890.50
0003  Bob Jones         $9,144.00
0004  Alice Lee         $   12.34
</div>
```

**Highlights / colors** — additional classes layered on. The class system is your attribute byte:
```html
<div class="field protected hi" style="--r:1;--c:30;--w:20;">CUSTOMER INQUIRY</div>
<div class="field err"          style="--r:23;--c:2;--w:40;">INVALID ACCOUNT NUMBER</div>
```

The mental shift: there are no "components." There's one primitive — *positioned cell-rectangle with text and attribute classes* — and what you call it (label, input, button, panel, status bar) is just convention.

What this approach can't do well:
- Anything that needs to grow with content. Pick a max width and clip, or compute width server-side.
- Anything that needs to reflow on window resize. Your screen is 80×24 cells; it doesn't rearrange itself.

---

## On `white-space: pre`

`white-space: pre` tells the browser: don't touch the whitespace, don't wrap, don't collapse runs of spaces, don't ignore newlines. Render every character including spaces and `\n`s exactly as written.

Default HTML behavior is the opposite — it eats whitespace aggressively. `pre` turns all of that off. Spaces are spaces, newlines are line breaks, the source is the output.

Gotcha: leading/trailing newlines inside the tag are preserved. Either put content on the same line as the tag, or account for the leading `\n`.

`white-space: pre-wrap` is the same except wraps at the field's width. For strict 3270 fidelity use plain `pre` — content beyond `--w` gets clipped.

---

## Images

Three honest options:

**ASCII/ANSI art.** No raster images at all. Box-drawing characters (`─ │ ┌ ┐ └ ┘ ├ ┤ ┬ ┴ ┼ ═ ║ ╔ ╗ ╚ ╝`), block characters (`█ ▓ ▒ ░ ▀ ▄ ▌ ▐`), shading. Tools like `jp2a`, `chafa`, `ascii-image-converter` will turn a real PNG into this kind of text on the server side. For data viz specifically — bar charts, sparklines (`▁▂▃▄▅▆▇█`) — this is genuinely good.

**Unicode symbols and emoji.** Single characters that happen to render as little pictures. `▶ ◆ ● ○ ★ ☆ ⚠ ✓ ✗ ⚙`. They sit in fields like text. Caveat: emoji width is unreliable across fonts; non-emoji symbols are reliably 1ch.

**Real raster images, placed as fields.** An `<img>` is just another element you can position with the same `--r/--c/--w/--h` machinery:

```css
img.field {
  object-fit: contain;
  display: block;
}
```

```html
<img class="field" src="/logo.png" style="--r:2;--c:60;--w:18;--h:4;">
```

Mental model: the image is a "graphic field" — a rectangle on the grid whose contents happen to be raster pixels. Constrained to the grid; doesn't push other fields around. IBM's later graphics-capable 3270 terminals worked roughly this way.

**SVG, same treatment.** Inline `<svg>` element positioned as a field. For server-rendered charts SVG is usually right over raster — your server emits SVG strings, browser draws crisp vectors:

```html
<svg class="field" style="--r:8;--c:10;--w:30;--h:10;" viewBox="0 0 300 100">
  <rect x="0"   y="50" width="20" height="50" fill="currentColor"/>
  <rect x="30"  y="20" width="20" height="80" fill="currentColor"/>
  <rect x="60"  y="40" width="20" height="60" fill="currentColor"/>
</svg>
```

A field is a positioned rectangle on the screen. Its contents can be text, raster image, SVG, or input. The grid is what *text fields* line up to; non-text fields are still rectangles at grid coordinates, their internal contents just don't tile.

---

## Aligning images to cells

Two layers: the *outer rectangle* (the field box) and the *inner image* (the bitmap inside).

**Outer rectangle: free.** Same `--r/--c/--w/--h` system; the field's edges land on cell boundaries by construction.

**Inner image:** four ways, in order of how much you care:

1. **Don't worry about it.** `object-fit: contain`, accept that the image is a free-form raster in a cell-aligned rectangle. For logos, photos, illustrations — fine.

2. **Pick image dimensions that are exact multiples of your cell size.** If `1ch = 9.6px` and `1em = 16px`, and your field is `--w:20; --h:5;`, the box is `192 × 80` pixels. Make your PNG exactly 192×80. Use `object-fit: fill` and `image-rendering: pixelated`.

3. **Lock the cell size to round pixels.** Define your cell explicitly:
```css
:root {
  --cell-w: 10px;
  --cell-h: 20px;
}
.field {
  left:   calc((var(--c) - 1) * var(--cell-w));
  top:    calc((var(--r) - 1) * var(--cell-h));
  width:  calc(var(--w) * var(--cell-w));
  height: calc(var(--h) * var(--cell-h));
}
```

4. **Render the image at the exact pixel size, no scaling.** `object-fit: none` displays it 1:1. Closest to "blit this sprite at (x, y)." Watch for high-DPI displays.

Practical recipe:
```css
#screen {
  font-family: 'Courier New', monospace;
  font-size: 16px;
  line-height: 1em;
}
img.field, svg.field {
  position: absolute;
  left:   calc((var(--c) - 1) * 1ch);
  top:    calc((var(--r) - 1) * 1em);
  width:  calc(var(--w) * 1ch);
  height: calc(var(--h) * 1em);
  object-fit: contain;
  image-rendering: pixelated;
}
```

For pixel-art / tile-based games where each tile is exactly one cell, use locked cell size + `object-fit: none` + `image-rendering: pixelated` + exact-size source. That's a sprite system on top of the DOM.

---

## C on the server

The rendering model is so simple it barely needs a framework. A field struct and a render function:

```c
typedef struct {
    int  r, c, w, h;        // grid coordinates; h=0 means single row
    const char *attr;       // "protected", "unprotected hi", etc.
    const char *name;       // for inputs; NULL for static
    const char *text;       // initial/static content
} Field;

void render_field(FILE *out, const Field *f, const char *value) {
    int h = f->h ? f->h : 1;
    const char *content = value ? value : (f->text ? f->text : "");
    int is_input = f->name && strstr(f->attr, "unprotected");

    if (is_input) {
        fprintf(out,
            "<input class=\"field %s\" "
            "style=\"--r:%d;--c:%d;--w:%d;--h:%d;\" "
            "name=\"%s\" value=\"%s\" maxlength=\"%d\">\n",
            f->attr, f->r, f->c, f->w, h, f->name,
            content, f->w);
    } else {
        fprintf(out,
            "<div class=\"field %s\" "
            "style=\"--r:%d;--c:%d;--w:%d;--h:%d;\">%s</div>\n",
            f->attr, f->r, f->c, f->w, h, content);
    }
}

void render_screen(FILE *out, const Field *fields, int n,
                   const char *(*lookup)(const char *name)) {
    fputs("<!DOCTYPE html><html><head>"
          "<link rel=\"stylesheet\" href=\"/screen.css\">"
          "</head><body><form method=\"post\"><div id=\"screen\">\n", out);
    for (int i = 0; i < n; i++) {
        const char *val = (fields[i].name && lookup)
                          ? lookup(fields[i].name) : NULL;
        render_field(out, &fields[i], val);
    }
    fputs("</div></form></body></html>\n", out);
}
```

A map definition is just an array — your BMS map, lives in the binary, no allocation:

```c
static const Field cust_inquiry[] = {
    { 1,  30, 20, 0, "protected hi",  NULL,  "CUSTOMER INQUIRY" },
    { 3,  2,  10, 0, "protected",     NULL,  "Account:"         },
    { 3,  13, 8,  0, "unprotected",   "acct", ""                },
    { 4,  2,  10, 0, "protected",     NULL,  "Name:"            },
    { 4,  13, 30, 0, "unprotected",   "name", ""                },
    { 22, 2,  76, 0, "protected hi",  NULL,  "F3=Exit F8=Next"  },
};
#define CUST_INQUIRY_N (sizeof(cust_inquiry)/sizeof(cust_inquiry[0]))
```

**HTTP layer options:**

- **CGI** — your program reads `QUERY_STRING` / stdin, writes HTML to stdout, exits. Apache or nginx (via fcgiwrap) handles the socket. Closest to pseudo-conversational CICS: stateless, transactional. ~50 lines.
- **FastCGI** (libfcgi) keeps the process alive between requests. Same code, wrapped in `while (FCGI_Accept() >= 0) { ... }`.
- **Embedded HTTP**: mongoose (single-file, MIT-licensed) or libmicrohttpd (GNU, more featureful). Register URL handlers as C functions:

```c
static int handle_cust_inquiry(struct mg_connection *c, struct mg_http_message *hm) {
    const char *acct = NULL;
    if (hm->method.len == 4 && !strncmp(hm->method.ptr, "POST", 4)) {
        acct = parse_form_field(hm->body.ptr, hm->body.len, "acct");
        // ... do the lookup, switch to a different screen ...
    }
    char buf[16384];
    FILE *out = fmemopen(buf, sizeof(buf), "w");
    render_screen(out, cust_inquiry, CUST_INQUIRY_N, NULL);
    fflush(out);
    mg_http_reply(c, 200, "Content-Type: text/html\r\n", "%s", buf);
    fclose(out);
    return 0;
}
```

**Form parsing** is the annoying bit. Body is `application/x-www-form-urlencoded`: `acct=12345&name=John+Smith`. Split on `&`, split each pair on `=`, URL-decode (`%20` → space, `+` → space). ~40 lines, or use the library helpers (mongoose has `mg_http_get_var()`).

**HTML escaping** — must not forget. When you `fprintf` user-supplied data into a field's contents, escape `<`, `>`, `&`, `"`:

```c
void html_escape(FILE *out, const char *s) {
    for (; *s; s++) {
        switch (*s) {
            case '<':  fputs("&lt;",   out); break;
            case '>':  fputs("&gt;",   out); break;
            case '&':  fputs("&amp;",  out); break;
            case '"':  fputs("&quot;", out); break;
            default:   fputc(*s, out);
        }
    }
}
```

**State between screens.** Stateless 3270 model: each request carries everything the server needs. Hidden form fields for state that needs to persist (current account, page offset). For real sessions, set a cookie with a session ID and look up in a hashtable / dbm / sqlite. For a fieldmap-style app, most state is naturally on-screen anyway.

**Build:**
```
cc -O2 -Wall -o app app.c mongoose.c
./app
```

C compiler, one or two source files, no node_modules, no Docker, no build pipeline. Drop the binary on a server, run it behind a TLS terminator (nginx, caddy, stunnel), done.

The mismatch people usually hit when writing web stuff in C is "I need to assemble nested templates with conditionals and loops and partials," and you don't — you have a flat array of field structs, you iterate, you print. Same shape as terminal rendering: a list of things to draw, drawn in order.

Concrete starting point: grab `mongoose.c` and `mongoose.h` from cesanta/mongoose on GitHub, write the `Field` struct and `render_screen` above, define one map, register one handler — working 3270-style screen on `localhost:8000` in maybe 200 lines.
